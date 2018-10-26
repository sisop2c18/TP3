#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

 int menu(){
    int opcion;
    do
    {
        printf( "\n   1. Cargar nota. " );
        printf( "\n   2. Consultar promedio de notas general. ");
        printf( "\n   3. Consultar promedio de notas por materia. ");
        printf( "\n   4. Salir." );
        printf( "\n\n   Introduzca opcion (1-4): ");

        scanf( "%d", &opcion );

        switch ( opcion )
        {
            case 1: cargar_nota();
                    break;

            case 2: consultar_promedio_general();
                    break;

            case 3: consultar_promedio_por_materia();
                    break;
         }

    } while ( opcion != 4 );

    return 0;
}


void cargar_nota(){
    printf("ingresaste a cargar notas");
}

void consultar_promedio_general(){
    printf("ingresaste a promedio general");
}

void consultar_promedio_por_materia(){
    printf("ingresaste a promedio por materia");
}