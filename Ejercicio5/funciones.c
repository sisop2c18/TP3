#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

//////////////////////////////////////////////////////////////////////////////////////
int leer_socket (void *data, int lon, t_dato *d){
    int read = 0;
    int aux = 0;

    if ((d->socket == -1) || (data == NULL) || (lon < 1)){
        return -1;
    }
    while (read < lon){
        aux = recv(d->socket, data + read, lon - read,MSG_WAITALL);
        if (aux > 0){
            read = read + aux;
        }else{
            if (aux == 0){
                return read;
            }
            if (aux == -1){
                switch (errno) {
                    case EINTR:
                    case EAGAIN:
                        usleep (100);
                        break;
                    default:
                        return -1;
                }
            }
        }
    }
    return read;
}

//////////////////////////////////////////////////////////////////////////////////////
int escribir_socket (void * data, int lon , t_dato *d){
    int sent = 0;
    int aux = 0;

    if ((d->socket == -1) || (data == NULL) || (lon < 1)){
        return -1;
    }
    while (sent < lon){
        aux = send(d->socket, data + sent, lon - sent,0);
        if (aux > 0){
            sent = sent + aux;
        }else{
            if(aux == 0){
                return sent;
            }else{
                return -1;
            }
        }
    }
    return sent;
}


void* server_write(void *args){
    char buffer[256];

    decision.comando = 0;

    while(decision.comando != -1){

        pthread_mutex_lock(&write_mutex);

        switch(decision.comando){
            case CARGAR :
                
                if(decision.decision == GOOD){
                    strcpy(buffer,"Se ha cargado en la DB correctamente.");
                }else{
                    strcpy(buffer,"Ya se encuentra cargado un registro para esa instancia de examen.");
                }
                //escribir_socket(buffer, 256, &fd);
                break;

            case GENERAL :
                if(decision.decision == GOOD){     
                    sprintf(buffer,"El promedio general de %d es %.2f", decision.dni, decision.prom);
                }else{
                    sprintf(buffer,"No existe persona con dni = %d", decision.dni);
                }
                //escribir_socket(buffer, 256, &fd);
                break;

            case MATERIA :
                if(decision.decision == GOOD){
                    t_listaP p;

                    p = promedios;

                    sprintf(buffer,"******************************************************************");
                   // escribir_socket(buffer, 256, &fd);
                    sprintf(buffer,"PROMEDIO POR MATERIA DE %d\n", decision.dni);
                   // escribir_socket(buffer, 256, &fd);
                    while(p){
                        sprintf(buffer,"Materia: %s - Promedio: %.2f", (p->dato).materia, (p->dato).prom);
                       // escribir_socket(buffer, 256, &fd);
                        p = p->sig;
                    }
                    sprintf(buffer,"******************************************************************\n");
                   // escribir_socket(buffer, 256, &fd);
                    deletePromedio(&promedios);
                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    sprintf(buffer,"No existe persona con dni = %d", decision.dni);
                   // escribir_socket(buffer, 256, &fd);
                }
                sprintf(buffer,"FIN");
                escribir_socket(buffer, 256, &fd);
                break;

            case QUIT :
                sprintf(buffer,"Hasta luego!");
               // escribir_socket(buffer, 256, &fd);
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

            case 3: consultar_promedio_por_materia(sv);
                    pthread_mutex_unlock(&cread_mutex);
                    break;

            case 4: salir(sv);
                    pthread_mutex_unlock(&cread_mutex);
                    go=0;
                    while(blocked){
                    }
                    break;

            default:cls();
                    printf("Opcion incorrecta, re ingrese.\n");
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
    t_comando dat;

    cls();
    printf("Ingresaste a cargar notas.\n\n");
    dat.comando=CARGAR;
    strcpy(dat.materia,materia);

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    do{
        printf("Ingrese nota.\n");
        scanf("%d",&dat.nota);
    }while(dat.nota < 1 || dat.nota > 10);

    printf("Seleccione Instancia.\n");

    do{
        printf( "\n   1. Primer Parcial. " );
        printf( "\n   2. Segundo Parcial. ");
        printf( "\n   3. Recuperatorio. ");
        printf( "\n\n   Introduzca opcion (1-3): ");

        scanf( "%d", &opcion );

        switch(opcion){
            case 1: // cargo a la struct primer parcial
                    strcpy(dat.instancia,PP);
                    go=0;
                    break;

            case 2: // cargo a la struct segundo parcial
                    strcpy(dat.instancia,SP);
                    go=0;
                    break;

            case 3: // cargo a la struct recuperatorio parcial
                    strcpy(dat.instancia,REC);
                    go=0;
                    break;

            default:cls();
                    printf("Opcion incorrecta, re ingrese.\n");
                    break;
         }

    }while(go);

    message.operation = CARGAR;
    message.comando = dat;

    return;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_general(){
    t_comando dat;
    char buffer[256];

    cls();
    printf("Ingresaste a promedio general.\n\n");
    dat.comando=GENERAL;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    cl_dec.comando = GENERAL;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_por_materia(){
    t_comando dat;
    char buffer[256];

    cls();
    printf("Ingresaste a promedio por materia.\n\n");
    dat.comando=MATERIA;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    cl_dec.comando = MATERIA;
}
//////////////////////////////////////////////////////////////////////////////////////
void salir(){
    t_comando dat;
    char buffer[256];

    dat.comando=QUIT;
    cl_dec.comando = QUIT;
}
///
void salir2(){
    t_comando dat;
    char buffer[256];

    dat.comando=QUIT;

    cl_dec.comando = QUIT;
    pthread_mutex_unlock(&cread_mutex);
}
//////////////////////////////////////////////////////////////////////////////////////
void* client_read(void *args){
    char buffer[256];
    t_dato *sv = args;

    cl_dec.comando = 0;

    while(cl_dec.comando != -1){

        pthread_mutex_lock(&cread_mutex);

        switch(cl_dec.comando){
            case CARGAR :
                leer_socket(buffer, 256, sv);
                cls();
                printf("%s\n\n", buffer);
                break;

            case GENERAL :
                leer_socket(buffer, 256, sv);
                cls();
                printf("%s\n\n", buffer);
                break;

            case MATERIA :
                cls();

                leer_socket(buffer, 256, sv);
                while(strcmp(buffer,"FIN") != 0){
                    printf("%s\n", buffer);
                    leer_socket(buffer, 256, sv);
                }
                break;

            case QUIT :
                leer_socket(buffer, 256, sv);
                cls();
                printf("%s\n", buffer);
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