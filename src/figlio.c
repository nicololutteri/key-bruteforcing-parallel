#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "utilita.h"
#include "nipote.h"
#include "types.h"
#include "logger.h"
#include "costanti.h"
#include "padre.h"

#include "figlio.h"

#ifdef THREAD
    #include <pthread.h>
#else

#endif

struct Status *status;
int semid;
int pipe_descriptors[2];

#ifdef THREAD
struct Params {
    void *s1;
    void *s2;
    int nrighe;
    int numero;
    int semid;
    int idcoda;
    int pipe;
};

void *nipotethread(void *params)
{
    struct Params *prm = (struct Params*) params;
    nipote((char *)(prm->s1 + (sizeof(struct Status))), (unsigned int *) prm->s2, (struct Status *) prm->s1, prm->nrighe, prm->numero, prm->semid, prm->idcoda, prm->pipe);

    pthread_exit(0);
}
#endif

void figlio(void *s1, void *s2, int nrighe)
{
    signal(SIGUSR1, status_updated);

    status = (struct Status *)(s1);

    creasemaforo(SEMAFORO1);

    int idcoda = 0;
    if((idcoda = msgget(NUMEROCODA, (IPC_CREAT | 0666))) == -1)
    {
        stampaerrore("Impossibile creare la coda");
        svuotarisorse();
    }


    // creo la pipe per la comunicazione tra figlio e nipoti/threads
    if(pipe(pipe_descriptors) == -1){
        stampaerrore("Creazione della pipe");
        svuotarisorse();
    }

    // imposto il non bloccaggio in fase di lettura e scrittura nella pipe
    fcntl(pipe_descriptors[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_descriptors[1], F_SETFL, O_NONBLOCK);

#ifdef THREAD

    pthread_t lista[NUMEROTHREAD];
    struct Params param[NUMEROTHREAD];
        
    int retcode;

    int i;
    for (i = 0; i < NUMEROTHREAD; i++)
    {
        param[i].s1 = s1;
        param[i].s2 = s2;
        param[i].nrighe = nrighe;
        param[i].numero = (i + 1);
        param[i].semid = semid;
        param[i].idcoda = idcoda;
        param[i].pipe = pipe_descriptors[1];

        if (pthread_create(&lista[i], NULL, nipotethread, (void *) &param[i]) < 0)
        {
            stampaerrore("Errore nella creazione del thread");
            svuotarisorse();
        }
    }

    for (i = 0; i < NUMEROTHREAD; i++)
    {
        retcode = pthread_join(lista[i], NULL);

        if (retcode != 0)
	{
	    stampaerrore("Join fallito");
	}
        else
	{
		
	}
    }

#else
    int pid = fork();
    if (pid == -1)
    {
        //Errore
        stampaerrore("Impossibile fare fork");
        svuotarisorse();
    }
    else if (pid == 0)
    {
        //Figlio
        nipote((char *)(s1 + (sizeof(struct Status))), (unsigned int *)s2, ((struct Status *)s1), nrighe, 1, semid, idcoda, pipe_descriptors[1]);
        return;
    }
    else
    {
        //Nipote

        int pid2 = fork();
        if (pid2 == -1)
        {
            //Errore
            stampaerrore("Impossibile fare fork");
            svuotarisorse();
        }
        else if (pid2 == 0)
        {
            //Figlio
            nipote((char *)(s1 + (sizeof(struct Status))), (unsigned int *)s2, ((struct Status *)s1), nrighe, 2, semid, idcoda, pipe_descriptors[1]);
            return;
        }
        else
        {
            //Nipote

            int returnStatus;
            waitpid(pid, &returnStatus, 0);
            waitpid(pid2, &returnStatus, 0);
        }
    }
#endif

    send_terminate(idcoda);
    cancellasemaforo();
}

void creasemaforo()
{
    // Crea il semafori
    if ((semid = semget(SEMAFORO1, 2, IPC_CREAT | 0666)) < 0)
    {
        stampaerrore("Impossibile creare i semafori");
    }

    struct sembuf *opzioni = (struct sembuf *)malloc(sizeof(struct sembuf));
    opzioni->sem_num = 0;
    opzioni->sem_op = 1;
    opzioni->sem_flg = 0;
    if (semop(semid, opzioni, 1) == -1) {
        stampaerrore("Impossibile impostare il semaforo a 1");
    }
}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

void cancellasemaforo()
{
    union semun sem_arg;

    sem_arg.val = 0;
    if(semctl(semid, 0, IPC_RMID, sem_arg) == -1){
        stampaerrore("Rimozione del semaforo");
        svuotarisorse();
    }
}

void status_updated(int numero){
    if(numero == SIGUSR1){
        char buffer[DIMMAX];
        int dimensioneletta;

        while((dimensioneletta = read(pipe_descriptors[0], buffer, DIMMAX)) > 0) {
            write(1, buffer, dimensioneletta);
        }
    }
}

void send_terminate(int idcoda)
{
    struct Message messaggio;
    messaggio.mtype = 1;
    copiastringa(messaggio.text, "ricerca conclusa");

    if (msgsnd(idcoda, &messaggio, sizeof(struct Message), 0) == -1)
    {
        stampaerrore("Impossibile inviare messaggio di terminazione");
    }
}
