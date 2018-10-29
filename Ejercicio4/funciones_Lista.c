#include "funciones_Lista.h"
/////////////////////////////////////////
void crearLista(t_lista *p){
    *p = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int addUsuario(t_lista *p,const t_dato *d,t_cmp cmp){
    t_nodo *nue;
    while(*p && (cmp(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    if(*p && (cmp(&((*p)->dato),d)==0)){
        return 0;
    }
    nue = (t_nodo*) malloc(sizeof(t_nodo));
    if(!nue){
        return 0;
    }
    nue->sig=*p;
    nue->dato= *d;
    *p=nue;
    return 1;
 }
//////////////////////////////////////////////////////////////////////////////////
int cmp(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    return ((t_dato*)e1)->socket - ((t_dato*)e2)->socket;
}
//////////////////////////////////////////////////////////////////////////////////
void vaciarLista(t_lista *p){
    t_nodo *aux;
    while(*p){
        aux = *p;
        p = &(*p)->sig;
        free(aux);
    }
}
//////////////////////////////////////////////////////////////////////////////////

// 1 = ELimino
// 0 = no existe

int eliminarUser(t_lista *p, const t_dato *d,t_cmp cmp){
    t_nodo *aux;
    while(*p && (cmp(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    if(*p && (cmp(&((*p)->dato),d)==0)){
        aux = *p;
        *p = aux->sig;
        free(aux);
        return 1;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int size(t_lista *p){
    int size = 0;
    while(*p){
        size++;
        p = &(*p)->sig;
    }
    return size;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mostrarClientes(t_lista *p){
    if(*p == NULL){
        return;
    }
    while(*p){
        printf("ID: %d - Socket: %d IDTHREAD: %ld\n", ((*p)->dato).id, ((*p)->dato).socket, ((*p)->dato).threadId);
        p = &(*p)->sig;
    }
}
//////////////////////////////////////////////////////////////////////////////////

// 0 = NO EXISTE
// 1 = EXISTE

int existInDB(t_listaL *p,const t_comando *d,t_cmpG cmpG){
    t_nodoL *nue;
    while(*p && (cmpG(&((*p)->dato),d) != 0)){
        p= &(*p)->sig;
    }
    if(*p && (cmpG(&((*p)->dato),d) == 0)){
        return 1;
    }
    return 0;
 }
/////////////////////////////////////////
void crearDB(t_listaL *p){
    *p = NULL;
}
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cargarDB(t_listaL* p){
    t_comando d;
    FILE * fp;
    char * line = NULL;
    char * aux;
    size_t len = 0;

    fp = fopen(BD, "r");

    if (fp == NULL) {
        printf("No se encontro la base de datos.\n");
        exit(1);
    }

    while(getline(&line, &len, fp) != -1) {
        
        if((aux = strsep(&line,",")) != NULL) {
            d.dni = atoi(aux);
            if((aux = strsep(&line,",")) != NULL) {
                strcpy(d.materia,aux);
                if((aux = strsep(&line,",")) != NULL) {
                    strcpy(d.instancia,aux);
                    if((aux = strsep(&line,",")) != NULL) {
                        d.nota = atoi(aux);
                    }
                }
            }
        }
        addDB(p,&d);
    }
    fclose(fp);
}
//////////////////////////////////////////////////////////////////////////////////
int addDB(t_listaL *p,const t_comando *d){
    t_nodoL *nue;
    
    while(*p){
        p= &(*p)->sig;
    }
    nue = (t_nodoL*) malloc(sizeof(t_nodoL));
    if(!nue){
        return 0;
    }
    nue->sig=*p;
    nue->dato= *d;
    *p=nue;
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int addLista(t_listaL *p,const t_comando *d, t_cmpL cmpL){
    t_nodoL *nue;
    while(*p && (cmpL(&((*p)->dato),d) != 0)){
        p= &(*p)->sig;
    }
    if(*p && (cmpL(&((*p)->dato),d)==0)){
        return 0;
    }
    nue = (t_nodoL*) malloc(sizeof(t_nodoL));
    if(!nue){
        return 0;
    }
    nue->sig=*p;
    nue->dato= *d;
    *p=nue;
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int cmpL(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    /*
    int dni;
    char materia[50];
    char instancia[16];
    */
    int retorno = 0;

    retorno += strcmp(((t_comando*)e1)->instancia,((t_comando*)e2)->instancia);
    retorno += strcmp(((t_comando*)e1)->materia,((t_comando*)e2)->materia);
    retorno += ((t_comando*)e1)->dni - ((t_comando*)e2)->dni;

    return retorno;
}
//////////////////////////////////////////////////////////////////////////////////
void deleteDB(t_listaL *p){
    t_nodoL *aux;
    while(*p){
        aux = *p;
        p = &(*p)->sig;
        free(aux);
    }
}
//////////////////////////////////////////////////////////////////////////////////
void mostrarDB(t_listaL *p){
    if(*p == NULL){
        return;
    }
    while(*p){
        printf("DNI: %d - Nombre Materia: %s - Instancia: %s - Nota: %d\n", ((*p)->dato).dni, ((*p)->dato).materia, ((*p)->dato).instancia, ((*p)->dato).nota);
        p = &(*p)->sig;
    }
}
//////////////////////////////////////////////////////////////////////////////////
double devolverGeneral(t_listaL *p,const t_comando *d, t_cmpG cmpG){
    double general=0;
    double notas=0;
    int cantMateria=0;

    if(*p == NULL){
        return 0;
    }

    while(*p){
        if(cmpG(&((*p)->dato),d)==0){
            notas += (*p)->dato.nota;
            cantMateria++;
        }
        p = &(*p)->sig;
    }

    if(cantMateria != 0){
        general = notas / cantMateria;
    }

    return general;
}
/////////////////////////////////////////
void crearPromedio(t_listaP *p){
    *p = NULL;
}
/////////////////////////////////////////
void deletePromedio(t_listaP *p){
    t_nodoM *aux;
    while(*p){
        aux = *p;
        p = &(*p)->sig;
        free(aux);
    }
}
////////////////////////////////////////////////////////////////////////////////
int devolverMateria(t_listaL *p, t_listaP *c, const t_comando *d, t_cmpG cmpG){
    t_materia m;
    if(*p == NULL){
        return 0;
    }
    while(*p){
        if(cmpG(&((*p)->dato),d)==0){
            strcpy(m.materia,(*p)->dato.materia);
            m.nota = (*p)->dato.nota;
            m.cantMaterias = 1;
            insertProm(c,&m,cmpM);
        }
        p = &(*p)->sig;
    }
    while(*c){
        ((*c)->dato.prom) = ((*c)->dato.nota) / ((*c)->dato.cantMaterias);
        c = &(*c)->sig;
    }
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////
int insertProm(t_listaP *p, const t_materia *d, t_cmpM cmpM){
    t_nodoM *nue;
    while(*p && (cmpM(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    if(*p && (cmpM(&((*p)->dato),d)==0)){
        (*p)->dato.nota += d->nota;
        ((*p)->dato.cantMaterias)++;
        return 1;
    }
    nue = (t_nodoM*) malloc(sizeof(t_nodoM));
    if(!nue){
        return 0;
    }
    nue->sig=*p;
    nue->dato= *d;
    *p=nue;
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int cmpG(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    return ((t_comando*)e1)->dni - ((t_comando*)e2)->dni;
}
//////////////////////////////////////////////////////////////////////////////////
int cmpM(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    return strcmp(((t_materia*)e1)->materia,((t_materia*)e2)->materia);
}