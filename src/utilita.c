#include <malloc.h>
#include <unistd.h>

#include "costanti.h"

#include "utilita.h"

void copiastringa(char *destinazione, char *origine)
{
    int i = 0;
    int dimensione = lunghezza(origine);
    for (i = 0; i < dimensione + 1; i++)
    {
        destinazione[i] = origine[i];
    }
}

void stampaerrore(char *stringa)
{
    stampa("Errore: ");
    stampa(stringa);
}

void stampa(char *stringa)
{
    int dimensione = lunghezza(stringa);
    // \0 + \n
    char *valore = (char *) malloc(sizeof(char) * (dimensione + 1 + 1));

    int i;
    for (i = 0; i < dimensione; i++)
    {
        valore[i] = stringa[i];
    }
    valore[i] = '\n';
    valore[i + 1] = '\0';

    write(STDOUT, valore, dimensione + 1 + 1);
}

char *substring(char *stringa, int inizio, int fine)
{
    char *tmp = (char *) malloc(sizeof(char) * (fine - inizio) + 1 + 1);
    int i = 0;
    for (i = 0; i < (fine - inizio); i++)
    {
        tmp[i] = stringa[inizio + i];
    }
    tmp[i] = '\0';

    return tmp;
}

// 0 2
// >01<2345678
// 123456789
char *partecharlen(char *stringa, int inizio, int lunghezza)
{
    char *tmp = substring(stringa, inizio, inizio + lunghezza);
    return tmp;
}

char *concatena(char *uno, char *due)
{
    int len1 = lunghezza(uno);
    int len2 = lunghezza(due);
    int dimtmp = 0;
    char *result = (char *)malloc(sizeof(char) * len1 + len2 + 1);

    int i = 0;
    for (i = 0; i < len1; ) {
        result[dimtmp++] = uno[i++];
    }

    for (i = 0; i < len2; ) {
        result[dimtmp++] = due[i++];
    }

    result[len1 + len2 + 1] = '\0';
    return result;
}

int lunghezza(char *stringa)
{
    int i;
    for (i = 0; stringa[i] != '\0'; i++);
    return i;
}

char *numerotostringa(int numero)
{
    int tmp = numero;
    int digits = 0;
    do
    {
        digits++;
    } while((tmp /= 10) != 0);

    char *result = (char *)malloc((sizeof(char) * digits) + 1);
    int i;
    for (i = digits - 1; i >= 0; i--)
    {
        int digit = (numero % 10);
        result[i] = (char)(digit + '0');
        numero /= 10;
    }

    result[digits] = '\0';
    return result;
}

char *inttohex(unsigned int numero)
{
    unsigned int n = numero;
    int digits = 0;
    while (n != 0)
    {
        n = n / 16;
        digits++;
    }

    char *new_string = (char *) malloc(sizeof(char) * 8);

    int i = 0;
    for(i = digits - 1; i >= 0; i--)
    {
        int valore = (numero % 16);

        if((valore) <= 9)
        {
            new_string[i] = (char) (valore) + '0';
        }
        else
        {
            new_string[i] = (char) ((valore) - 10) + 'A';
        }

        numero = numero / 16;
    }

    return concatena("0x", new_string);
}

unsigned int convertchar(char quattro[])
{
    return (unsigned char)quattro[3] * 256 * 256 * 256 +
            (unsigned char)quattro[2] * 256 * 256 +
            (unsigned char)quattro[1] * 256 +
            (unsigned char)quattro[0] * 1;
}

char converti(int numero)
{
    if (numero >= 0 && numero <= 10)
    {
        return '0' + numero;
    }
    else
    {
        return 'A' + numero - 10;
    }
}
