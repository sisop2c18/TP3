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
void server_run(void *args){
    int leido = 0;
    t_dato *dato = args;
    t_comando c;
    double prom;

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

                addLista(&bd,&c,cmpL);

                // V DEL MUTEX

                // agregar al archivo bd.txt SI ADDLISTA DEVUELVE 1
                //sent info ok o no
                break;
            case GENERAL :
                // do promedio general

                // P DEL MUTEX

                prom = devolverGeneral(&bd,&c,cmpG);

                // V DEL MUTEX

                //sent info ok o no
                break;
            case MATERIA :
                // do promedio materia

                // P DEL MUTEX

                prom = devolverMateria(&bd,&c,cmpM);

                // V DEL MUTEX
                
                //sent info ok o no
                break;
            case QUIT :
                // sacar de la lista al user
                eliminarUser(&clientes,dato,cmp);
                break;
            default:
                printf("OPCION DESCONOCIDA!\n");
                exit(1);
            break;
        }
    }

    free(args);

    return;
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

    do{
        printf( "\n   1. Cargar nota. " );
        printf( "\n   2. Consultar promedio de notas general. ");
        printf( "\n   3. Consultar promedio de notas por materia. ");
        printf( "\n   4. Salir." );
        printf( "\n\n   Introduzca opcion (1-4): ");

        scanf( "%d", &opcion );

        switch(opcion){
            case 1: cargar_nota();
                    break;

            case 2: consultar_promedio_general();
                    break;

            case 3: consultar_promedio_por_materia();
                    break;

            case 4: salir();
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
void cargar_nota(){
    int opcion;
    int go=1;
    t_comando dat;

    cls();
    printf("Ingresaste a cargar notas.\n\n");
    dat.comando=CARGAR;

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

    printf("Usted ha ingresado: %d - %d - %s - %d\n", dat.comando, dat.dni, dat.instancia, dat.nota);

    return;
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_general(){
    t_comando dat;

    cls();
    printf("Ingresaste a promedio general.\n\n");
    dat.comando=GENERAL;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    printf("Usted ha ingresado: %d - %d\n", dat.comando, dat.dni);
}
//////////////////////////////////////////////////////////////////////////////////////
void consultar_promedio_por_materia(){
    t_comando dat;

    cls();
    printf("Ingresaste a promedio por materia.\n\n");
    dat.comando=MATERIA;

    printf("Ingrese documento del alumno.\n");
    scanf("%d",&dat.dni);

    printf("Usted ha ingresado: %d - %d\n", dat.comando, dat.dni);
}
//////////////////////////////////////////////////////////////////////////////////////
void salir(){
    t_comando dat;

    cls();
    dat.comando=QUIT;

    printf("Usted ha ingresado: %d\n", dat.comando);

    printf("Bye bye...\n");
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