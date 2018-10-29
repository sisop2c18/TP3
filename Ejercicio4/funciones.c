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
//////////////////////////////////////////////////////////////////////////////////////
void* server_run(void *args){
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    char buffer[256];
    int leido = 0;
    t_listaP promedios;
    t_dato *dato = args;
    t_comando c;
    double prom;

    ordenarListaL(&bd);
    while(leido != -1){
        leido = leer_socket(&c, sizeof(t_comando),dato);
        
        if(leido == 0){
            usleep (100);
            continue;
        }

        switch(c.comando){
            case CARGAR :
                // do cargar

                // P DEL MUTEX creo que tendria 2 mutex uno para general y otro para materia, asi puedo consultar los dos promedios al mismo tiempo
                pthread_mutex_lock(&mutex);
                if(addLista(&bd,&c,cmpL)){
                    FILE * fp;
                    fp = fopen(BD, "a");

                    if (fp == NULL) {
                        printf("No se encontro la base de datos.\n");
                        exit(1);
                    }

                    fprintf(fp, "%d,%s,%s,%d\n", c.dni, c.materia, c.instancia, c.nota);

                    fclose(fp);

                    strcpy(buffer,"Se ha cargado en la DB correctamente.");
                    printf("[+] Se ha cargado en la DB correctamente.\n");
                    ordenarListaL(&bd);
                }else{
                    // EXISTE MANDAR MSJ ERROR
                    strcpy(buffer,"Ya se encuentra cargado un registro para esa instancia de examen.");
                    printf("[X] Ya se encuentra cargado un registro para esa instancia de examen.\n");
                }
                escribir_socket(buffer, 256, dato);
                pthread_mutex_unlock(&mutex);
                // V DEL MUTEX

                break;

            case GENERAL :
                // do promedio general
                // P DEL MUTEX
                pthread_mutex_lock(&mutex);
                if(existInDB(&bd,&c,cmpG)){

                    prom = devolverGeneral(&bd,&c,cmpG);
                    
                    sprintf(buffer,"El promedio general de %d es %.2f", c.dni, prom);
                    printf("[?] Se ha consultado el promedio general de %d.\n", c.dni);
                    // MANDAR EL PROMEDIO
                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    sprintf(buffer,"No existe persona con dni = %d", c.dni);
                    printf("[X] No existe persona con dni = %d.\n", c.dni);
                }
                escribir_socket(buffer, 256, dato);
                // V DEL MUTEX
                pthread_mutex_unlock(&mutex);
                break;

            case MATERIA :
                // do promedio materia
                // P DEL MUTEX
                pthread_mutex_lock(&mutex);
                if(existInDB(&bd,&c,cmpG)){
                    crearPromedio(&promedios);
                    t_listaP p;
                    
                    devolverMateria(&bd,&promedios,&c,cmpG);

                    printf("[?] Se ha consultado el promedio por materia de %d.\n", c.dni);

                    p = promedios;

                    sprintf(buffer,"******************************************************************");
                    escribir_socket(buffer, 256, dato);
                    sprintf(buffer,"PROMEDIO POR MATERIA DE %d\n", c.dni);
                    escribir_socket(buffer, 256, dato);
                    while(p){
                        sprintf(buffer,"Materia: %s - Promedio: %.2f", (p->dato).materia, (p->dato).prom);
                        escribir_socket(buffer, 256, dato);
                        p = p->sig;
                    }
                    sprintf(buffer,"******************************************************************\n");
                    escribir_socket(buffer, 256, dato);
                    deletePromedio(&promedios);
                }else{
                    // NO EXISTE MANDAR MSJ ERROR
                    sprintf(buffer,"No existe persona con dni = %d", c.dni);
                    escribir_socket(buffer, 256, dato);
                    printf("[X] No existe persona con dni = %d.\n", c.dni);
                }
                sprintf(buffer,"FIN");
                escribir_socket(buffer, 256, dato);
                // V DEL MUTEX
                pthread_mutex_unlock(&mutex);
                break;

            case QUIT :
                // sacar de la lista al user
                eliminarUser(&clientes,dato,cmp);
                // pongo esto para que termine el hilo
                leido=-1;
                printf("[-] Se ha desconectado un cliente.\n");
                sprintf(buffer,"Hasta luego!");
                escribir_socket(buffer, 256, dato);
                break;

            default:
                printf("OPCION DESCONOCIDA!\n");
                exit(1);
            break;
        }
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
int menu(t_dato *sv){
    int opcion;
    int go=1;

    do{
        printf( "\n   1. Cargar nota. " );
        printf( "\n   2. Consultar promedio de notas general. ");
        printf( "\n   3. Consultar promedio de notas por materia. ");
        printf( "\n   4. Salir." );
        printf( "\n\n   Introduzca opcion (1-4): ");

        scanf( "%d", &opcion );

        switch(opcion){
            case 1: cargar_nota(sv);
                    break;

            case 2: consultar_promedio_general(sv);
                    break;

            case 3: consultar_promedio_por_materia(sv);
                    break;

            case 4: salir(sv);
                    go=0;
                    break;

            default:cls();
                    printf("Opcion incorrecta, re ingrese.\n");
                    break;
         }

    }while(go);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
void cargar_nota(t_dato *sv){
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

    escribir_socket(&dat, sizeof(t_comando), sv);
    leer_socket(buffer, 256, sv);
    cls();
    printf("%s\n\n", buffer);
    return;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_general(t_dato *sv){
    t_comando dat;
    char buffer[256];

    cls();
    printf("Ingresaste a promedio general.\n\n");
    dat.comando=GENERAL;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    escribir_socket(&dat, sizeof(t_comando), sv);

    leer_socket(buffer, 256, sv);
    cls();
    printf("%s\n\n", buffer);
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_por_materia(t_dato *sv){
    t_comando dat;
    char buffer[256];

    cls();
    printf("Ingresaste a promedio por materia.\n\n");
    dat.comando=MATERIA;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    escribir_socket(&dat, sizeof(t_comando), sv);

    cls();

    leer_socket(buffer, 256, sv);
    while(strcmp(buffer,"FIN") !=0 ){
        printf("%s\n", buffer);
        leer_socket(buffer, 256, sv);
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void salir(t_dato *sv){
    t_comando dat;
    char buffer[256];

    dat.comando=QUIT;

    escribir_socket(&dat, sizeof(t_comando), sv);
    
    leer_socket(buffer, 256, sv);
    cls();
    printf("%s\n", buffer);
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