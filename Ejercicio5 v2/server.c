#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h>
#include <string.h>
#include "funciones.h"

int locked = 1;
int leave = 1;
int fd;
pthread_t threadWrite;

void sigInt(int dummy){
    // cancelar todos los threads
    printf("\nCerrando todo...\n");
    locked = 0;
    leave = 0;
    sem_post(mutexConexion); //desbloqueo el sem_wait del while de conexiones
    t_lista first = clientes;
    while(first){
        pthread_cancel(first->dato.threadId);
        //pthread_join(first->dato.threadWrite, NULL);   
        first = first->sig;
    }
    printf("TERMINE THREADS\n");
    pthread_cancel(threadWrite);  
    printf("TERMINE THREAD WRITE\n");
    vaciarLista(&clientes);
    printf("TERMINE VACIAR CLIENTES\n");
    deleteDB(&bd);
    printf("DESTROY BD\n");
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_mutex);
    pthread_mutex_destroy(&quit_mutex);
    printf("DESTROY MUTEX\n");
    sem_close(mutexServer);
    sem_unlink(SMUTEX);
    sem_close(mutexClient);
    sem_unlink(CMUTEX);
    sem_close(mutexEspera);
    sem_unlink(WMUTEX);
    printf("DESTROY MUTEX SV CLIENTE\n");
    close(fd);
    munmap(mensaje, sizeof(t_comando));
    shm_unlink(SHMFILE);
    printf("BYE BYE\n");
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

            d.id=id++;
            addUsuario(&copiaClientes,&d,cmp);

            if(size(&copiaClientes) > 1){
                copiaClientes = copiaClientes->sig;
            }

            pthread_create( &(d.threadId), NULL, server_run, (void*) &(copiaClientes->dato));

            addUsuario(&clientes,&d,cmp);
        }
    }

    sem_close(mutexConexion);
    sem_unlink(EMUTEX);

    return 0;
}
