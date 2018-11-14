#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "funciones.h"

pthread_t cliente_lee;
int fd;

void sigInt(int dummy){
    // cancelar todos los threads
    printf("\nCerrando todo...\n");
    
    salir2();

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);
    pthread_cancel(cliente_lee);

    sem_close(mutexServer);
    sem_close(mutexClient);
    sem_close(mutexConexion);
    sem_close(mutexEspera);

    close(fd);
    munmap(mensaje, sizeof(t_comando));
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

    if(argc != 2){
        printf("Error, debe pasar la Materia a la que pertenece y la direccion del servidor \n");
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

    sem_post(mutexConexion);

    printf("Bienvenido, Profesor de %s\n", materia);

    pthread_create(&(cliente_lee), NULL, client_read, NULL); 

    menu(); 

    pthread_mutex_destroy(&cliente_mutex);
    pthread_mutex_destroy(&cread_mutex);
    pthread_join(cliente_lee, NULL);

    return 0;
}
