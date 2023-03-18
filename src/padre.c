#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

#include "logger.h"
#include "nipote.h"
#include "figlio.h"
#include "types.h"
#include "utilita.h"
#include "costanti.h"

#include "padre.h"

void padre(char * sorgente, char * destinazione)
{
    //Gestione degli errori
    signal(SIGINT, svuotarisorse);
    signal(SIGTERM, svuotarisorse);
    signal(SIGQUIT, svuotarisorse);
    signal(SIGSEGV, svuotarisorse);
    signal(SIGTSTP, svuotarisorse);

    int fd = open(sorgente, O_RDONLY);
    if (fd < 0)
    {
        stampaerrore("Impossibile aprire il file");
        svuotarisorse();
    }

    int nrighe = ottienilinee(fd);

    void * s1 = attach_segments(S1, nrighe * (1 + 512 + 3 + 512 + 1 + 1));
    void * s2 = attach_segments(S2, nrighe * (2 + 8 + 1));
    struct Status * t = (struct Status *)s1;
    t->grandson = 0;
    t->id_string = 0;
    load_file(fd, (char *)(s1 + sizeof(struct Status)));

    if (close(fd) < 0)
    {
        stampaerrore("Impossibile chiudere il file");
        svuotarisorse();
    }

    int pid = fork();
    if (pid == -1)
    {
        //Errore
        stampaerrore("Impossibile fork");
    }
    else if (pid == 0)
    {
        //Figlio
        figlio(s1, s2, nrighe);
    }
    else
    {
        int pid2 = fork();
        if (pid2 == -1)
        {
            //Errore
            stampaerrore("Impossibile fork");
        }
        else if (pid2 == 0)
        {
            logger();
        }
        else
        {
            int returnStatus;
            waitpid(pid, &returnStatus, 0);
            waitpid(pid2, &returnStatus, 0);
            
            if (check_keys(nrighe, (char *)(s1 + sizeof(struct Status)), (unsigned int *)s2))
            {
                save_keys(destinazione, (unsigned int *)s2, nrighe);
            }
            else
            {
                stampaerrore("La chiave è sbagliata");
            }

            detach_segments(s1, S1, nrighe * (1 + 512 + 3 + 512 + 1 + 1));
            detach_segments(s2, S2, nrighe * (2 + 8 + 1));
            return;
        }
    }
}

void svuotarisorse()
{
    system(concatena("ipcrm -M ", numerotostringa(S1)));
    system(concatena("ipcrm -M ", numerotostringa(S2)));
    system(concatena("ipcrm -Q ", numerotostringa(NUMEROCODA)));
    system(concatena("ipcrm -S ", numerotostringa(SEMAFORO1)));

    exit(1);
}

char * attach_segments(int numero, int dimensione)
{
    int shmid;
    key_t key;

    key = numero;

    if ((shmid = shmget(key, dimensione, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        stampaerrore("Impossibile creare il segmento");
        svuotarisorse();
    }

    char * s1;
    if ((s1 = shmat(shmid, NULL, 0)) == (char *) -1)
    {
        stampaerrore("Impossibile collegare il segmento");
        svuotarisorse();
    }

    return s1;
}

void detach_segments(char * indirizzo, int chiave, int dimensione)
{
    shmdt(indirizzo);

    int shm_id;
    if((shm_id =  shmget(chiave, dimensione, 0666)) < 0){
        stampaerrore("Impossibile recuperare il segmento");
        svuotarisorse();
    }

    struct shmid_ds shmid_struct;
    if(shmctl(shm_id, IPC_RMID, &shmid_struct) == -1){
        stampaerrore("Impossibile deallocare il segmento");
        svuotarisorse();
    }
}

int ottienilinee(int fd)
{
    int numerorighe = 0;
    int primo = 0;
    int contacaratteri = 0;
    int timer = 0;

    int dimensioneletto = 0;
    char buffer[DIMMAX];
    do {
        dimensioneletto = read(fd, buffer, DIMMAX);

        int i = 0;
        for (i = 0; i < dimensioneletto; i++)
        {
            // Prima parte testo
            if (buffer[i] == '<' && primo == 0 && timer == 0)
            {
                primo = 1;
                contacaratteri = 0;
            }
            else if (buffer[i] == '>' && primo == 1 && timer == 0)
            {
                //Fine parte testo
                numerorighe++;
                timer = 3 + contacaratteri + 1;
                primo = 0;
            }
            else if (primo == 1 && timer == 0)
            {
                contacaratteri++;
            }
            else
            {
                timer--;
            }
        }
    } while (dimensioneletto > 0);

    return numerorighe;
}

void load_file(int fd, char * s1)
{
    lseek(fd, 0L, SEEK_SET);

    char * tmp = s1;

    int dimensioneletto = 0;
    char buffer[DIMMAX];
    do {
        dimensioneletto = read(fd, buffer, DIMMAX);

        int i = 0;
        for (i = 0; i < dimensioneletto; i++)
        {
            // Copia nel buffer
            *tmp++ = buffer[i];
        }
    } while (dimensioneletto > 0);
    tmp = '\0';
}

void save_keys(char * nomefile, unsigned int * s2, int numerochiavi)
{
    // Crea e apre il file di output
    int filechiavi;
    if((filechiavi = creat(nomefile, O_RDWR ^ 0644)) == -1)
    {
        stampaerrore("Impossibile creare il file di output");
    }

    int i;
    for (i = 0; i < numerochiavi; i++)
    {
        char * dascrivere = inttohex(s2[i]);
        dascrivere[10] = '\n';
        write(filechiavi, dascrivere, 11);
    }

    // Chiude il file di output
    if(close(filechiavi) == -1)
    {
        stampaerrore("Impossibile chiudere il file di output");
    }
}

int check_keys(int nrighe, char * s1, unsigned int * s2)
{
    int i = 0;
    for (i = 0; i < nrighe; i++)
    {
        struct DecAndCod * tmp = load_string(s1, i);

        if (controllacoppia(s2[i], tmp->originale, tmp->encoded) == 0)
        {
            return 0;
        }
    }

    return 1;
}

int controllacoppia(unsigned int s2, char * originale, char * encoded)
{
    int dimensione = lunghezza(originale);

    int i = 0;
    for (i = 0; i < dimensione; i += 4)
    {
        char * parteoriginale = partecharlen(originale, i, 4);
        char * parteencode = partecharlen(encoded, i, 4);

        unsigned int inttesto = (unsigned int) convertchar(parteoriginale);
        unsigned int intencoded = (unsigned int) convertchar(parteencode);

        unsigned int chiavetrovata = (inttesto ^ intencoded);

        if (chiavetrovata != s2)
        {
            return 0;
        }
    }

    return 1;
}
