#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
//////////////////////////////////////////////////////////////////////////////////////
void* server_run(void *args){
    char buffer[256];
    int leido = 0;
    t_dato *dato = (t_dato*)args; // CREO QUE NO VA
    double prom;

    ordenarListaL(&bd);

    while(leido != -1){
        // LEER SHM t_comando 
        sem_wait(mutexServer);
        
        pthread_mutex_lock(&mutex);

        switch(mensaje->comando){
            case CARGAR :

                decision.comando = CARGAR;

                if(addLista(&bd,mensaje,cmpL)){
                    FILE * fp;
                    fp = fopen(BD, "a");

                    if (fp == NULL) {
                        printf("No se encontro la base de datos.\n");
                        exit(1);
                    }

                    fprintf(fp, "%d,%s,%s,%d\n", mensaje->dni, mensaje->materia, mensaje->instancia, mensaje->nota);

                    fclose(fp);

                    ordenarListaL(&bd);

                    decision.decision = GOOD;
                    printf("[+] Se ha cargado en la DB correctamente.\n");
                }else{
                    // EXISTE MANDAR MSJ ERROR
                    decision.decision = BAD;
                    printf("[X] Ya se encuentra cargado un registro para esa instancia de examen.\n");
                }
                // V DEL MUTEX
                pthread_mutex_unlock(&write_mutex);
                break;

            case GENERAL :
                // do promedio general
                decision.comando = GENERAL;

                if(existInDB(&bd,mensaje,cmpG)){

                    prom = devolverGeneral(&bd,mensaje,cmpG);
                    
                    decision.prom = prom;
                    decision.dni = mensaje->dni;
                    decision.decision = GOOD;
                    printf("[?] Se ha consultado el promedio general de %d.\n", mensaje->dni);
                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    decision.dni = mensaje->dni;
                    decision.decision = BAD;
                    printf("[X] No existe persona con dni = %d.\n", mensaje->dni);
                }
                // V DEL MUTEX
                pthread_mutex_unlock(&write_mutex);

                break;

            case MATERIA :
                // do promedio materia
                decision.comando = MATERIA;

                if(existInDB(&bd,mensaje,cmpG)){
                    crearPromedio(&promedios);
                    
                    devolverMateria(&bd,&promedios,mensaje,cmpG);

                    printf("[?] Se ha consultado el promedio por materia de %d.\n", mensaje->dni);

                    decision.dni = mensaje->dni;
                    decision.decision = GOOD;

                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    decision.dni = mensaje->dni;
                    decision.decision = BAD;
                    printf("[X] No existe persona con dni = %d.\n", mensaje->dni);
                }
                // V DEL MUTEX
                pthread_mutex_unlock(&write_mutex);
                break;

            case QUIT :
                decision.comando = QUIT;
                // sacar de la lista al user
                pthread_mutex_unlock(&write_mutex);
                pthread_mutex_lock(&quit_mutex);
                eliminarUser(&clientes,dato,cmp);
                // pongo esto para que termine el hilo
                leido=-1;
                printf("[-] Se ha desconectado un cliente.\n");
                break;

            default:
                printf("OPCION DESCONOCIDA SERVER RUN!\n");
                exit(1);
            break;
        }
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
void* server_write(void *args){

    decision.comando = 0;

    while(decision.comando != -1){

        pthread_mutex_lock(&write_mutex);

        switch(decision.comando){
            case CARGAR :
                
                if(decision.decision == GOOD){
                    strcpy(mensaje->buffer,"Se ha cargado en la DB correctamente.");
                }else{
                    strcpy(mensaje->buffer,"Ya se encuentra cargado un registro para esa instancia de examen.");
                }
                sem_post(mutexClient);

                break;

            case GENERAL :
                if(decision.decision == GOOD){     
                    sprintf(mensaje->buffer,"El promedio general de %d es %.2f", decision.dni, decision.prom);
                }else{
                    sprintf(mensaje->buffer,"No existe persona con dni = %d", decision.dni);
                }
                sem_post(mutexClient);
                break;

            case MATERIA :
                if(decision.decision == GOOD){
                    t_listaP p;

                    p = promedios;

                    sprintf(mensaje->buffer,"******************************************************************");
                    sem_post(mutexClient);
                    sem_wait(mutexEspera);
                    sprintf(mensaje->buffer,"PROMEDIO POR MATERIA DE %d\n", decision.dni);
                    sem_post(mutexClient);
                    sem_wait(mutexEspera);
                    while(p){
                        sprintf(mensaje->buffer,"Materia: %s - Promedio: %.2f", (p->dato).materia, (p->dato).prom);
                        sem_post(mutexClient);
                        sem_wait(mutexEspera);
                        p = p->sig;
                    }
                    sprintf(mensaje->buffer,"******************************************************************\n");
                    sem_post(mutexClient);
                    sem_wait(mutexEspera);
                    deletePromedio(&promedios);
                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    sprintf(mensaje->buffer,"No existe persona con dni = %d", decision.dni);
                    sem_post(mutexClient);
                    sem_wait(mutexEspera);
                }
                sprintf(mensaje->buffer,"FIN");
                sem_post(mutexClient);
                break;

            case QUIT :
                sprintf(mensaje->buffer,"Hasta luego!");
                sem_post(mutexClient);
                pthread_mutex_unlock(&quit_mutex);
                break;

            default:
                printf("OPCION DESCONOCIDA SERVER WRITE!\n");
                exit(1);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
/*
#define CARGAR 1 //CARGAR NOTAS
#define GENERAL 2 //PROMEDIO GENERAL
#define MATERIA 3 //PROMEDIO MATERIA
#define QUIT 4 //CLIENTE SE DESCONECTO
*/
int menu(){
    int opcion;
    int go=1;
    blocked = 1;

    do{
        pthread_mutex_lock(&cliente_mutex);
        printf( "\n   1. Cargar nota. " );
        printf( "\n   2. Consultar promedio de notas general. ");
        printf( "\n   3. Consultar promedio de notas por materia. ");
        printf( "\n   4. Salir." );
        printf( "\n\n   Introduzca opcion (1-4): ");

        scanf( "%d", &opcion );

        switch(opcion){
            case 1: cargar_nota();
                    pthread_mutex_unlock(&cread_mutex);
                    break;

            case 2: consultar_promedio_general();
                    pthread_mutex_unlock(&cread_mutex);
                    break;

            case 3: consultar_promedio_por_materia();
                    pthread_mutex_unlock(&cread_mutex);
                    break;

            case 4: salir();
                    pthread_mutex_unlock(&cread_mutex);
                    go=0;
                    while(blocked){
                    }
                    break;

            default:cls();
                    printf("Opcion incorrecta, re ingrese.\n");
                    pthread_mutex_unlock(&cliente_mutex);
                    break;
         }
    }while(go);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
void cargar_nota(){
    int opcion;
    int go = 1;
    char buffer[256];

    cls();
    printf("Ingresaste a cargar notas.\n\n");
    mensaje->comando=CARGAR;
    strcpy(mensaje->materia,materia);

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&(mensaje->dni));

    do{
        printf("Ingrese nota.\n");
        scanf("%d",&(mensaje->nota));
    }while(mensaje->nota < 1 || mensaje->nota > 10);

    printf("Seleccione Instancia.\n");

    do{
        printf( "\n   1. Primer Parcial. " );
        printf( "\n   2. Segundo Parcial. ");
        printf( "\n   3. Recuperatorio. ");
        printf( "\n\n   Introduzca opcion (1-3): ");

        scanf( "%d", &opcion );

        switch(opcion){
            case 1: // cargo a la struct primer parcial
                    strcpy(mensaje->instancia,PP);
                    go=0;
                    break;

            case 2: // cargo a la struct segundo parcial
                    strcpy(mensaje->instancia,SP);
                    go=0;
                    break;

            case 3: // cargo a la struct recuperatorio parcial
                    strcpy(mensaje->instancia,REC);
                    go=0;
                    break;

            default:cls();
                    printf("Opcion incorrecta, re ingrese.\n");
                    break;
         }

    }while(go);

    sem_post(mutexServer);
    cl_dec.comando = CARGAR;

    return;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_general(){

    cls();
    printf("Ingresaste a promedio general.\n\n");

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&(mensaje->dni));
    mensaje->comando=GENERAL;

    sem_post(mutexServer);
    cl_dec.comando = GENERAL;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_por_materia(){

    cls();
    printf("Ingresaste a promedio por materia.\n\n");
    
    printf("Ingrese documento del alumno.\n");
    scanf("%d",&(mensaje->dni));
    mensaje->comando=MATERIA;

    sem_post(mutexServer);
    cl_dec.comando = MATERIA;
}
//////////////////////////////////////////////////////////////////////////////////////
void salir(){

    mensaje->comando=QUIT;

    sem_post(mutexServer);
    cl_dec.comando = QUIT;
}
///
void salir2(){

    mensaje->comando=QUIT;

    sem_post(mutexServer);
    cl_dec.comando = QUIT;
    pthread_mutex_unlock(&cread_mutex);
}
//////////////////////////////////////////////////////////////////////////////////////
void* client_read(void *args){

    cl_dec.comando = 0;

    while(cl_dec.comando != -1){

        pthread_mutex_lock(&cread_mutex);

        switch(cl_dec.comando){
            case CARGAR :
                sem_wait(mutexClient);
                cls();
                printf("%s\n\n", mensaje->buffer);
                break;

            case GENERAL :
                sem_wait(mutexClient);
                cls();
                printf("%s\n\n", mensaje->buffer);
                break;

            case MATERIA :
                cls();

                sem_wait(mutexClient);
                while(strcmp(mensaje->buffer,"FIN") != 0){
                    printf("%s\n", mensaje->buffer);
                    sem_post(mutexEspera);
                    sem_wait(mutexClient);
                }
                break;

            case QUIT :
                sem_wait(mutexClient);
                cls();
                printf("%s\n", mensaje->buffer);
                cl_dec.comando = -1;
                blocked = 0;
                break;

            default:
                printf("OPCION DESCONOCIDA!\n");
                exit(1);
            break;
        }
        pthread_mutex_unlock(&cliente_mutex);
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
int normalizar(char* cad){

    while(*cad != '\0'){
        if(ES_LETRA(*cad)){
            *cad = toupper(*cad);
            cad++;
        }else{
            return 0;
        }
        while(((*cad) != ' ') && ES_LETRA(*cad)){
            *cad = tolower(*cad);
            cad++;
        }
        cad++;
    }

    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////