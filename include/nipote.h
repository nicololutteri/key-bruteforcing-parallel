/// @file
/// @defgroup costanti Costanti
/// @{

#ifndef NIPOTE_H
#define NIPOTE_H

#include <sys/types.h>

#include "types.h"
#include "utilita.h"

/**
 * @brief Wrapper del processo nipote
 * @param s1 Segmento di memoria che contiene il file di input
 * @param s2 Segmento di memoria che contiene il file delle chiavi
 * @param t Parte iniziale del segmento s1 che contiene la struttura
 * @param righemax Numero di righe del file
 * @param numero Numero identificativo del nipote
 * @param semid Id del semaforo
 * @param idcoda Id della coda
 * @param pipe Id del pipe
 */
void nipote(char *s1, unsigned int *s2, struct Status *t, int righemax, int numero, int semid, int idcoda, int pipe);

/**
 * @brief Legge la stringa del segmento S1
 * @param s1 Segmento di memoria condivisa del file
 * @param numeroriga Numero di riga da prelevare
 */
struct DecAndCode * load_string(char *s1, int numeroriga);

/**
 * @brief Blocca accesso esclusivo regione critica
 * @param semid Id del semaforo
 */
void lock(int semid);

/**
 * @brief Sblocca accesso esclusivo alla regione critica
 * @param semid Id del semaforo
 */
void unlock(int semid);

/**
 * @brief Trova la chiave
 * @param originale Parte della riga originale
 * @param encoded Parte della riga codificata
 * @return Chiave trovata
 */
unsigned int find_key(char *originale, char *encode);

/**
 * @brief Deposita il messaggio "chiave trovata/secondi" nella coda di messaggi del processo logger
 * @param time Tempo trascorso per cercare la chiave
 * @param idcoda Numero della coda
 */
void send_timeelapsed(long secondi, int idcoda);

/**
 * @brief Salva la chiave nel segmento S2
 * @param s2 Segmento di memoria che contiene le chiavi
 * @param chiave Chiave da salvare
 * @param numeroriga Numero di riga della chiave
 */
void save_key(unsigned int *s2, unsigned int chiave, int numeroriga);

#endif

/// @}
