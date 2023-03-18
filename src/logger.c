#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "types.h"
#include "costanti.h"

#include "utilita.h"

int idcoda;

void logger() {
    creacoda();

    while(polling_receive(idcoda) != 1)
    {
        alarm(1);
        pause();
        alarm(0);
    }

    eliminacoda();

    exit(0);
}

void creacoda()
{
    if((idcoda = msgget(NUMEROCODA, (IPC_CREAT | 0666))) == -1)
    {
        stampaerrore("Impossibile creare la coda");
    }
}

void eliminacoda()
{
    if (msgctl(idcoda, IPC_RMID, NULL) == -1)
    {
        stampaerrore("Impossibile cancellare la coda");
    }
}

int polling_receive() {
    int result = 0;
    int status = 0;
    struct Message message;

    while (result != 1 && (status = msgrcv(idcoda, &message, sizeof(struct Message), 0, 0)) >= 0)
    {
        if (message.mtype == 1) {
            result = 1;
        }

        if (message.text[127] != '\0') {
            // Sostituisce l'ultimo carattere con il carattere di terminazione di stringa
            message.text[127] = '\0';
        }

        stampa(message.text);
    };

    if (status == -1)
    {
        stampaerrore("Impossibile leggere dalla coda");
    }

    return result;
}
