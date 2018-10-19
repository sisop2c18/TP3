#include "lista.h"
/////////////////////////////////////////
void crearLista(t_lista *p){
    *p = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int insertarOrdenado(t_lista *p,const t_dato *d,t_cmp cmp){
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
    return strcmp(((t_dato*)e1)->patente,((t_dato*)e2)->patente);
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
void mostrarLista(t_lista *p){
    while(*p){
        printf("Patente: %s - Hora: %d:%d:%d\n", ((*p)->dato).patente, ((*p)->dato).hora, ((*p)->dato).min, ((*p)->dato).seg);
        p = &(*p)->sig;
    }
}
//////////////////////////////////////////////////////////////////////////////////
int eliminarPorPosicion(t_lista *p, int poss){
    t_nodo *aux;
    int cont = 1;
    if(poss<=0){
        return 0;
    }
    while((cont<poss) && (*p)){
        p = &(*p)->sig;
        cont++;
    }
    aux = *p;
    *p = aux->sig;
    free(aux);
    return 1;
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
void cargarArchivosEnLista(t_lista* p, char pIn[]){
    t_dato d;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(pIn);

    if(!dirp){
        printf("No se pudo abrir el directorio.\n");
        exit(1);
    }

    entry = readdir(dirp);

    while(entry != NULL) {
        if(entry->d_type == DT_REG){ // Si es archivo regular
            //strcpy(d.patente, entry->d_patente);
            //d.hora = 0;
            insertarOrdenado(p,&d,cmp);
        }
        entry = readdir(dirp);
    }
    closedir(dirp);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// 0 = NO EXISTE
// 1 = EXISTE

int exist(t_lista *p,const t_dato *d,t_cmp cmp){
    t_nodo *nue;
    while(*p && (cmp(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    if(*p && (cmp(&((*p)->dato),d)==0)){
        return 1;
    }
    return 0;
 }