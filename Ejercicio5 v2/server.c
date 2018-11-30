#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h>
#include <string.h>
#include "funciones.h"

/*#################################################
#             Sistemas Operativos                 #
#       Trabajo Práctico 3 - Ejercicio 5          #
#       Nombre del Script: server.c           	  #
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

int locked = 1;
int leave = 1;
int fd;
pthread_t threadWrite;

void sigInt(int dummy){

    cls();
    printf("Cerrando todo...\n");
    locked = 0;
    //leave = 0;
    killThemAll();
    printf("Se ha desconectado a todos los clientes.\n");
    sem_post(mutexConexion); //desbloqueo el sem_wait del while de conexiones
    t_lista first = clientes;
    while(first){
        pthread_cancel(first->dato.threadId);
        //pthread_join(first->dato.threadWrite, NULL);   
        first = first->sig;
    }
    sleep(2);
    printf("Se cancelaron los threads de lectura.\n");
    sleep(2);
    pthread_cancel(threadWrite);
    printf("Se cancelaron los threads de escritura.\n");
    sleep(2);
    vaciarLista(&clientes);
    deleteDB(&bd);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);
    pthread_mutex_destroy(&quit_mutex);

    sem_close(mutexConexion);
    sem_close(mutexEspera);            
    sem_close(mutexClient);             
    sem_close(mutexServer);

    sem_unlink(EMUTEX);
    sem_unlink(WMUTEX);                 
    sem_unlink(CMUTEX); 
    sem_unlink(SMUTEX);     

    close(fd);                          
    munmap(mensaje, sizeof(t_comando));
    shm_unlink(SHMFILE);
    
    printf("Servidor cerrado correctamente.\n");
    exit(1);
}

int main(int argc , char *argv[]){

    mutexServer = sem_open(SMUTEX, O_CREAT | O_EXCL, 0666, 0);
    mutexClient = sem_open(CMUTEX, O_CREAT | O_EXCL, 0666, 0);
    mutexConexion = sem_open(EMUTEX, O_CREAT | O_EXCL, 0666, 0);
    mutexEspera = sem_open(WMUTEX, O_CREAT | O_EXCL, 0666, 0);

    fd = shm_open(SHMFILE, O_CREAT | O_RDWR, 0666);

    ftruncate(fd, sizeof(t_comando));

    mensaje = mmap(NULL, sizeof(t_comando), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    pthread_mutex_lock(&write_mutex);
    pthread_mutex_init(&quit_mutex, NULL);
    pthread_mutex_lock(&quit_mutex);

    t_lista copiaClientes;
    int id = 1;
    t_dato d;

    crearLista(&clientes);
    crearLista(&copiaClientes);

    signal(SIGINT, sigInt);

    crearDB(&bd);
    printf("Cargando DB...\n");
    cargarDB(&bd);
    printf("DB lista.\n\n");

    printf("Esperando conexiones...\n\n");
    //esta activo esperando a multiples clientes

    pthread_create(&threadWrite, NULL, server_write, NULL);

    while(leave){

        sem_wait(mutexConexion);

        if(locked){    
            printf("[+] Se ha conectado un cliente.\n");

            d.pid=mensaje->dni;
            d.id=id++;
            addUsuario(&copiaClientes,&d,cmp);

            if(size(&copiaClientes) > 1){
                copiaClientes = copiaClientes->sig;
            }

            pthread_create( &(d.threadId), NULL, server_run, (void*) &(copiaClientes->dato));

            addUsuario(&clientes,&d,cmp);
        }
    }

    return 0;
}
