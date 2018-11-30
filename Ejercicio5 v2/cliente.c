#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "funciones.h"

pthread_t cliente_lee;
pthread_t t_menu;
int fd;

void sigInt(int dummy){
    cls();
    printf("Cerrando todo...\n");

    salir2();

    pthread_join(cliente_lee, NULL);
    //pthread_cancel(cliente_lee);
    pthread_cancel(t_menu);

    sem_close(mutexConexion);
    sem_close(mutexEspera);
    sem_close(mutexClient);
    sem_close(mutexServer);   

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);

    close(fd);
    munmap(mensaje, sizeof(t_comando));
    exit(1);
}

void sigUsr1(int dummy){
    cls();
    printf("El servidor se ha cerrado...\n");
    /*
    salir2();
    printf("\nSalio salir2()...\n");

    pthread_join(cliente_lee, NULL);*/
    pthread_cancel(cliente_lee);
    pthread_cancel(t_menu);

    sem_close(mutexServer);
    sem_close(mutexClient);
    sem_close(mutexConexion);
    sem_close(mutexEspera);

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);

    close(fd);
    munmap(mensaje, sizeof(t_comando));
    printf("\nCerrado correctamente.\n");
    exit(1);
}

int main(int argc, char *argv[]){

    mutexServer = sem_open(SMUTEX, 0);
    mutexClient = sem_open(CMUTEX, 0);
    mutexConexion = sem_open(EMUTEX, 0);
    mutexEspera = sem_open(WMUTEX, 0);

    fd = shm_open(SHMFILE, O_CREAT | O_RDWR, 0666);

    mensaje = mmap(NULL, sizeof(t_comando), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pthread_mutex_init(&cliente_mutex, NULL);
    pthread_mutex_init(&cread_mutex, NULL);
    pthread_mutex_lock(&cread_mutex);

    signal(SIGINT, sigInt);
    signal(SIGUSR1, sigUsr1);

    if(argc != 2){
        printf("Error, debe pasar la Materia a la que pertenece.\n");
        exit(1);
    }
    if(argc == 2 && ((strcmp(argv[1],"-help")==0) || (strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"-?") == 0))){
        printf("La sintaxis es ./Client [Materia]\nFormato: ./Client [string]\nEjemplo: ./Client Algebra\n");
        exit(1);
    }

    strcpy(materia,argv[1]);
    normalizar(materia);
    
    cls();
    printf("Conectando con el servidor aguarde unos instantes...\n\n");

    mensaje->dni=getpid(); // uso dni para no agregar mas cosas a la estructura

    sem_post(mutexConexion);

    printf("Bienvenido, Profesor de %s\n", materia);

    pthread_create(&(cliente_lee), NULL, client_read, NULL); 

    pthread_create(&(t_menu), NULL, menu, NULL);
    
    
    pthread_join(cliente_lee, NULL);

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);
    pthread_join(t_menu, NULL);

    return 0;
}
