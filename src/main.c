#include <stdio.h>
#include <stdlib.h>

#include "utilita.h"

#include "padre.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        stampa("Utilizzo: key_finder <input> <output>");
        exit(1);
    }

    padre(argv[1], argv[2]);
    return 1;
}
