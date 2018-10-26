#include<stdlib.h> 
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h> 
#include <stdio.h>
#include <signal.h>

int PORT = 8080;

int socket_desc;

void sigInt(int dummy){
    close(socket_desc);
    socket_desc=0;
}

int main(int argc , char *argv[])
{
    int client_sock , c;
    struct sockaddr_in server , client;

    signal(SIGINT, sigInt);


    //creamos socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Error al crear socket");
        exit(1);
    }
     
    server.sin_family = AF_INET;
    //asigna el puerto 8080
    server.sin_port = htons( PORT );
    //IP de cualquier maquina
    server.sin_addr.s_addr = INADDR_ANY;
     
    //asigno un nombre local al socket
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("No se pudo abrir el socket");
        exit(1);
    }

    if (listen(socket_desc, 3) == -1)
    {
        printf("Error en listenn" );
        exit(-1);
    }
     
    printf("Esperando conexiones...\n");
    c = sizeof(struct sockaddr_in);
	
    //aca irian las configuraciones del cliente CREO


    //esta activo esperando a multiples clientes
    while( socket_desc ){
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        printf("Nueva conexion\n");
        if (client_sock < 0){
            printf("Error al aceptar conexion\n");
            continue;
        }

    }
     
    return 0;
}