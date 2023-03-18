/// @file
/// @defgroup costanti Costanti
/// @{

#ifndef PADRE_H
#define PADRE_H

/**
 * @brief Wrapper processo padre
 * @param sorgente File sorgente per l'elaborazione
 * @param destinazione File destinazione contente le chiavi trovate
 */
void padre(char *sorgente, char *destinazione);

/**
 * @brief Libera le risorse, a seguito di un errore
 */
void svuotarisorse();

/**
 * @brief Crea segmento memoria condivisa
 * @param numero Identificativo della memoria condivisa
 * @param dimensione Dimensione della memoria condivisa
 * @return Puntatore all'inizio dell'area di memoria
 */
char * attach_segments(int numero, int dimensione);

/**
 * @brief Elimina segmento di memoria condivisa
 * @param indirizzo Indirizzo del segmento condiviso
 * @param chiave Chiave dello spazio di memoria
 * @param dimensione Dimensione dello spazio di memoria
 */
void detach_segments(char *indirizzo, int chiave, int dimensione);

/**
  * @brief Conta le righe del file
  * @param File descriptor del file da legge
  */
int ottienilinee(int fd);

/**
 * @brief Carica il file nella memoria condivisa
 * @param fd File descriptor del file
 * @param s1 Puntatore alla memoria condivisa (salta la struttura Status)
 * @return Numero di righe del file
 */
void load_file(int fd, char *s1);

/**
 * @brief Salva le chiavi sul file di output
 * @param nomefile Posizione del file dove salvare le chiavi
 * @param s2 Segmento di memoria che contiene le chiavi in unsigned int
 * @param numerochiavi Numero totali di chiavi
 */
void save_keys(char *nomefile, unsigned int *s2, int numerochiavi);

/**
 * @brief Controlla che le chiavi siano corrette per tutta la lunghezza delle stringhe
 * @param nrighe Numero delle righe
 * @param s1 Segmento di memoria condivisa che contiene il file di input
 * @param s2 Segmento di memoria condivisa che contiene le chiavi (in unsigned int)
 * @return puntatore alla linea interessata
 */
int check_keys(int nrighe, char *s1, unsigned int *s2);

/**
 * @brief Controlla che le chiavi siano corrette per tutta la lunghezza delle stringhe
 * @param s2 Segmento di memoria condivisa che contiene le chiavi
 * @param originale Parte riga originale
 * @param encoded Parte codificata con xor
 * @return Risultato del controllo
 */
int controllacoppia(unsigned int s2, char *originale, char *encoded);

#endif

/// @}
