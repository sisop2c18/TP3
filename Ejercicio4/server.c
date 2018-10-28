#include <stdlib.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "funciones.h"

int PORT = 5000;

int client_sock;
int socket_desc;
struct sockaddr_in server;
//t_listaL bd;
//t_lista clientes;

void sigInt(int dummy){
    close(client_sock);
    close(socket_desc);
    socket_desc=0;
}

int main(int argc , char *argv[]){
    int id=1;
    t_dato d;
    socklen_t cl=sizeof(struct sockaddr_in);
	struct sockaddr_in client;

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
        printf("No se pudo abrir el socket");
        exit(1);
    }

    if(listen(socket_desc, 3) == -1){
        printf("Error en listen.");
        exit(1);
    }

    crearBD(&bd);
    printf("Cargando DB...\n\n");
    cargarDB(&bd);

    mostrarDB(&bd);

    // testing promedios
    t_comando dato;
    double prom;
    dato.dni = 22222222;
    prom = devolverGeneral(&bd,&dato,cmpG);
    printf("Prom General de %d = %.2f\n", dato.dni, prom);
    dato.dni = 11111111;
    strcpy(dato.materia,"Base De Datos");
    prom = devolverMateria(&bd,&dato,cmpM);
    printf("Prom Materia (%s) de %d = %.2f\n", dato.materia, dato.dni, prom);
    crearLista(&clientes);
    printf("Esperando conexiones...\n\n");
	//

    //aca irian las configuraciones del cliente CREO

    //esta activo esperando a multiples clientes
    while( socket_desc ){
        client_sock = accept(socket_desc, (struct sockaddr *)&client,&cl);
        if(client_sock < 0){
            printf("Error al aceptar conexion.\n");
            continue;
        }else{
            printf("Nueva conexion\n");
            d.id=id++;
            d.socket=client_sock;
            addUsuario(&clientes,&d,cmp);
            mostrarClientes(&clientes);
        }
    }

    return 0;
}
