/// @file
/// @defgroup nipote Nipote
/// @{

#ifndef UTILITA_H
#define UTILITA_H

struct DecAndCod {
    char *originale;
    char *encoded;
};

/**
 * @brief Copia una stringa sorgente nella destinazione
 * @param destinazione Stringa di destinazione
 * @param origine Stringa di origine
 */
void copiastringa(char *destinazione, char *origine);

/**
 * @brief Stampa un errore
 * @param stringa Stringa da stampare
 */
void stampaerrore(char *stringa);

/**
 * @brief Stampa una stringa
 * @param stringa Stringa da stampare
 */
void stampa(char *stringa);

/**
 * @brief Preleva una sottostringa
 * @param stringa Stringa da elaborare
 * @param inizio Posizione compresa da dove iniziare
 * @param fine Posizione compresa da dove finire
 * @return Sottostringa
 */
char *substring(char *stringa, int inizio, int fine);

/**
 * @brief Preleva una sottostringa
 * @param stringa Stringa da elaborare
 * @param inizio Posizione compresa da dove iniziare
 * @param lunghezza Lunghezza dei caratteri da prelevare
 * @return Sottostringa
 */
char *partecharlen(char *stringa, int inizio, int fine);

/**
 * @brief Concatena due stringhe
 * @param uno Prima stringa da concatenare
 * @param due Seconda stringa da concatenare
 * @return (uno + due)
 */
char *concatena(char *uno, char *due);

/**
 * @brief Calcola la lunghezza di una stringa
 * @param stringa Stringa da calcolare
 * @return Lunghezza della stringa
 */
int lunghezza(char *stringa);

/**
 * @brief Trasforma un numero in una stringa
 * @param numero Numero da trasformare
 * @return Stringa finale
 */
char *numerotostringa(int numero);

/**
 * @brief Trasforma numero in hex
 * @param numero Numero da trasformare
 * @return Hex da 8 cifre
 */
char *inttohex(unsigned int numero);

/**
 * @brief Converti array da 4 char in un unsigned int
 * @param quattro Array da convertire
 * @return Numero che rappresenta il l'array
 */
unsigned int convertichar(char quattro[]);

/**
 * @brief Converti int in un numero
 * @param numero Numero da convertire
 * @return Char convertito in base esadecimale
 */
char converti(int numero);

#endif
