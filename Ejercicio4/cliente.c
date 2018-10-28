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

int PORT = 5000;
int Socket_Servidor;
struct sockaddr_in server;

void set(const char *y, int z){
	bzero(&(server.sin_zero), 8);
	server.sin_family=AF_INET;
	server.sin_port=htons(z);
	server.sin_addr.s_addr = inet_addr(y);
}

int main(int argc, char *argv[]){
    char materia[50];

    struct hostent *Host;

    if(argc < 2 || argc > 3){
        printf("Error, debe pasar la Materia a la que pertenece y la direccion del servidor \n");
        exit(1);
    }
    if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
        printf("La sintaxis es ./Client [Materia][IP]\nFormato: ./Client [string][string]\nEjemplo: ./Client Algebra 127.0.0.1\n");
        exit(1);
    }else if(argc==2){
        printf("La sintaxis es ./Client [Materia][IP]\nFormato: ./Client [string][string]\nEjemplo: ./Client Algebra 127.0.0.1\n");
        exit(1);
    }

    strcpy(materia,argv[1]);
    normalizar(materia);
    
    printf("Conectando con el servidor aguarde unos instantes...\n\n");

    Socket_Servidor = socket (AF_INET, SOCK_STREAM, 0);
    if(Socket_Servidor == -1){
        printf("ERROR SOCKET SERVIDOR.\n");
        exit(1);
    }

    set(argv[2],PORT);

    if(connect(Socket_Servidor,(struct sockaddr*)&server, sizeof(server))==-1){
        printf("Error en la conexion al servidor.\n");
        close(Socket_Servidor);
        exit(1);
    }

    // CLIENTE SE CONECTO

    printf("Bienvenido, Profesor de %s\n", materia);

    menu();

    close(Socket_Servidor);

    return 0;
}
