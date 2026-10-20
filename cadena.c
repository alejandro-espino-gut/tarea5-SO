#include <stdio.h>
#include <stdlib.h>

static int parsear_addrs(const char *path, long **addrs_out, int *cont_out) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("No se pudo abrir el archivo");
        return 0;
    }

    long *addrs = NULL;
    int cont = 0;
    int capacidad = 0;
    long valor;

    while (fscanf(file, "%ld", &valor) == 1) {
        if (cont == capacidad) {
            int dos_capacidad = (capacidad == 0) ? 32 : capacidad * 2;
            long *tmp = realloc(addrs, dos_capacidad * sizeof(long));
            if (tmp == NULL) {
                fprintf(stderr, "Error: no hay memoria suficiente.\n");
                free(addrs);
                fclose(file);
                return 0;
            }
            addrs = tmp;
            capacidad = dos_capacidad;
        }
        addrs[cont++] = valor;
    }

    fclose(file);
    *addrs_out = addrs;
    *cont_out = cont;
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Uso: <tamaño_pagina> <archivo_direcciones>\n");
        return 1;
    }

    long tam_pag = strtol(argv[1], NULL, 10);
    long *addrs = NULL;
    int cont = 0;
    if (!parsear_addrs(argv[2], &addrs, &cont)) {
        return 1;
    }

    if (cont == 0) {
        fprintf(stderr, "Error: el archivo no tiene un formato valido\n");
        free(addrs);
        return 1;
    }

    printf("Tamaño de pagina: %ld bytes\n", tam_pag);
    printf("Secuencia de referencias de pagina: ");
    for (int i = 0; i < cont; ++i) {
        long page = addrs[i] / tam_pag;
        printf("%s%ld", (i == 0 ? "" : " "), page);
    }
    printf("\n");

    free(addrs);
    return 0;
}
