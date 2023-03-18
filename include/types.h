#ifndef TYPES_H
#define TYPES_H

/**
 * @brief Struttura che controlla il punto dove sono arrivato con l'elaborazione
 */
struct Status {
    int grandson;
    int id_string;
};

/**
 * @brief Messaggio per la coda
 */
struct Message {
    long mtype;
    char text[128];
};

#endif
