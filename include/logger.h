/// @file
/// @defgroup costanti Costanti
/// @{

#ifndef LOGGER_H
#define LOGGER_H

extern int id_coda;

/**
 * @brief Wrapper del processo logger
 */
void logger();

/**
 * @brief Crea la coda
 */
void creacoda();

/**
 * @brief Chiude la coda
 */
void eliminacoda();

/**
 * @brief Legge i messaggi della coda
 * @return Restituisce Vero se devo chiudere tutto
 */
int polling_receive();

#endif

/// @}
