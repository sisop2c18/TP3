#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "funciones.h"

int main(int argc, char *argv[]){
    if(argc < 2 || argc > 3){
        printf("Error, debe pasar la Materia a la que pertenece y la operacion que desea realizar. \n");
        exit(1);
    }

    if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
        printf("La sintaxis es ./Client [Materia]\nFormato: ./Client [string][string]\nEjemplo: ./Client Algebra\n");
        exit(1);
    }else if(argc==2){
        printf("La sintaxis es ./Client [Materia]\nFormato: ./Client [string][string]\nEjemplo: ./Client Algebra\n");
        exit(1);
    }

    strcpy(materia,argv[1]);
    normalizar(materia);
    
    // CLIENTE SE CONECTO

    printf("Bienvenido, Profesor de %s\n", materia);

    menu(); 

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);
    pthread_join(cliente_lee, NULL);
    close(Socket_Servidor);

    return 0;
}