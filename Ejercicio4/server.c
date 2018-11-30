#include <stdlib.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdio.h>
#include <string.h>
#include "funciones.h"

/*#################################################
#             Sistemas Operativos                 #
#       Trabajo Práctico 3 - Ejericio 4           #
#       Nombre del Script: server.c               #
#                                                 #
#       Integrantes:                              #
#         Miller, Lucas            39353278       #
#         Ramos, Micaela           39266928       #
#         Sapaya, Nicolás Martín   38319489       #
#         Schafer, Federico        39336856       #
#         Secchi, Lucas            39267345       #
#                                                 #
#       Instancia de Entrega: Re Entrega 1        #
#                                                 #
###################################################*/

int PORT = 5000;

int locked = 1;
pthread_t threadWrite;
int client_sock;
int socket_desc;
struct sockaddr_in server;
//t_listaL bd;
//t_lista clientes;

void sigInt(int dummy){
    // cancelar todos los threads
    printf("\nCerrando todo...\n");
    locked = 0;
    t_lista first = clientes;
    while(first){
        pthread_cancel(first->dato.threadId);
        //pthread_join(first->dato.threadWrite, NULL);   
        first = first->sig;
    }
    pthread_cancel(threadWrite);
    quitAll(&clientes); 
    vaciarLista(&clientes);
    deleteDB(&bd);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);
    pthread_mutex_destroy(&quit_mutex);
    close(client_sock);
    close(socket_desc);
    socket_desc=0;
}

int main(int argc , char *argv[]){
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_mutex_lock(&write_mutex);
    pthread_mutex_init(&quit_mutex, NULL);
    pthread_mutex_lock(&quit_mutex);

    t_lista copiaClientes;
    int id=1;
    t_dato d;
    socklen_t cl=sizeof(struct sockaddr_in);
	struct sockaddr_in client;

    crearLista(&clientes);
    crearLista(&copiaClientes);

    signal(SIGINT, sigInt);


    //creamos socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Error al crear socket");
        exit(1);
    }
    bzero((char*)&server,sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    //asigna el puerto 5000
    server.sin_port = htons( PORT );
    //IP de cualquier maquina
    server.sin_addr.s_addr = INADDR_ANY;
     
    //asigno un nombre local al socket
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0){
        printf("No se pudo abrir el socket\n");
        exit(1);
    }

    if(listen(socket_desc, 7) == -1){
        printf("Error en listen.\n");
        exit(1);
    }

    crearDB(&bd);
    printf("Cargando DB...\n");
    cargarDB(&bd);
    printf("DB lista.\n\n");
    //aca irian las configuraciones del cliente CREO
    printf("Esperando conexiones...\n\n");
    //esta activo esperando a multiples clientes

    pthread_create( &threadWrite, NULL , server_write , (void*) &d);

    while( socket_desc ){
        client_sock = accept(socket_desc, (struct sockaddr *)&client,&cl);
        if(client_sock < 0 && locked){
            printf("Error al aceptar conexion.\n");
            continue;
        }else if(locked){
            printf("[+] Se ha conectado un cliente.\n");
            /*
            d.id=id++;
            d.socket=client_sock;
            pthread_create( &(d.threadId), NULL , server_run , (void*) &d);
            pthread_create( &(d.threadWrite), NULL , server_write , (void*) &d);
            addUsuario(&clientes,&d,cmp);
            mostrarClientes(&clientes);
            */
            d.id=id++;
            d.socket=client_sock;
            addUsuario(&copiaClientes,&d,cmp);

            if(size(&copiaClientes) > 1){
                copiaClientes = copiaClientes->sig;
            }

            pthread_create( &(d.threadId), NULL , server_run , (void*) &(copiaClientes->dato));
            //pthread_create( &(d.threadWrite), NULL , server_write , (void*) &(copiaClientes->dato));
            addUsuario(&clientes,&d,cmp);
        }
    }

    return 0;
}
