#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>

#include "types.h"
#include "logger.h"
#include "utilita.h"

#include "nipote.h"

void nipote(char *s1, unsigned int *s2, struct Status *t, int righemax, int numero, int semid, int idcoda, int pipe)
{
    while (1)
    {
        lock(semid);

        t->grandson = numero;
        int rigadaleggere = t->id_string;
        if (rigadaleggere == righemax)
        {
            unlock(semid);
            break;
        }

        struct DecAndCod *lavoro = (struct DecAndCod *) load_string(s1, rigadaleggere);

#ifdef THREAD
       char *messaggio = concatena("Il thread ", numerotostringa(numero));
#else
       char *messaggio = concatena("Il nipote ", numerotostringa(numero));
#endif

        messaggio = concatena(messaggio, " sta analizzando la ");
        messaggio = concatena(messaggio, numerotostringa((t->id_string + 1)));
        messaggio = concatena(messaggio, "-esima stringa.\n");

        write(pipe, messaggio, lunghezza(messaggio));

#ifdef THREAD
	kill(getpid(), SIGUSR1);
#else
	kill(getppid(), SIGUSR1);
#endif

        t->id_string++;
        unlock(semid);

        struct timeval  tv;
        gettimeofday(&tv, NULL);
        int inizio = tv.tv_sec;

        unsigned int keyfinal = find_key(lavoro->originale, lavoro->encoded);

        struct timeval tv2;
        gettimeofday(&tv2, NULL);
        int fine = tv2.tv_sec;

        int bench_time = fine - inizio;

        lock(semid);
        save_key(s2, keyfinal, rigadaleggere);
        unlock(semid);

        send_timeelapsed(bench_time, idcoda);
    }

    return;
}

struct DecAndCode *load_string(char *s1, int numeroriga)
{
    char *tmp = s1;

    // Numero linea che si sta controllando
    int linea = 0;
    // Inizio della riga da controllare "<1234"
    int posizionegenerale = 0;

    while(1)
    {
        // Numero di caratteri
        int i = 0;

        // 0123456789ABCDEF
        // <ABCD>;<FFFF>\n<ASDF>;<DFTGG>\n
        // i = 1 salta il '<'
        for (i = 1; s1[posizionegenerale + i] != '>'; i++) { }
        i--;

        if (linea == numeroriga)
        {
            struct DecAndCod *array = (struct DecAndCod *) malloc(sizeof(struct DecAndCod));
            array->originale = partecharlen(tmp, posizionegenerale + 1, i);
            array->encoded = partecharlen(tmp, posizionegenerale + i + 3 + 1, i);

            return array;
        }
        else
        {
            linea++;
            posizionegenerale += 1 + i + 3 + i + 1 + 1;
        }
    }
}

void lock(int semid)
{
    struct sembuf *opzioni = (struct sembuf *)malloc(sizeof(struct sembuf));
    opzioni->sem_num = 0;
    opzioni->sem_op = -1;
    opzioni->sem_flg = 0;

    if (semop(semid, opzioni, 1) == -1)
    {
        stampaerrore("Impossibile bloccare il semaforo");
    }

    free(opzioni);
}

void unlock(int semid)
{
    struct sembuf *opzioni = (struct sembuf *)malloc(sizeof(struct sembuf));
    opzioni->sem_num = 0;
    opzioni->sem_op = 1;
    opzioni->sem_flg = 0;

    if (semop(semid, opzioni, 1) == -1)
    {
        stampaerrore("Impossibile sbloccare il semaforo");
    }

    free(opzioni);
}

unsigned int find_key(char *originale, char *encoded)
{
    char *bufferoriginal = substring(originale, 0, 4);
    char *bufferencoded = substring(encoded, 0, 4);

    unsigned int inttesto = (unsigned int) convertchar(bufferoriginal);
    unsigned int intencoded = (unsigned int) convertchar(bufferencoded);
    unsigned int chiave = 0;

    while ((chiave ^ intencoded) != inttesto)
    {
        chiave++;
    }

    return chiave;
}

void send_timeelapsed(long secondi, int idcoda)
{
    struct Message messaggiodainviare;
    messaggiodainviare.mtype = 2;
    copiastringa(messaggiodainviare.text, concatena("chiave trovata in ", numerotostringa(secondi)));

    if (msgsnd(idcoda, &messaggiodainviare, sizeof(struct Message), 0) == -1)
    {
        stampaerrore("Impossibile inviare messaggio");
    }
}

void save_key(unsigned int *s2, unsigned int chiave, int numeroriga)
{
    s2[numeroriga] = chiave;
}
