/// @file
/// @defgroup costanti Costanti
/// @{

#ifndef FIGLIO_H
#define FIGLIO_H

/**
 * @brief Wrapper del processo figlio
 */
void figlio();

/**
 * @brief Crea il semaforo per i nipoti/thread
 * @param numero Numero della risorsa
 * @return Id del semaforo
 */
void creasemaforo();

/**
 * @brief Cancella il semaforo
 */
void cancellasemaforo();

/**
 * @brief signal_handler per notifica dei nipoti/thread
 */
void status_updated();

/**
 * @brief Deposita il messaggio di terminazione nella coda di messaggi del processo logger
 * @param idcoda Numero della coda generato da msgget
 */
void send_terminate(int idcoda);

#endif

/// @}
