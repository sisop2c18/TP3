#include "lista.h"
/////////////////////////////////////////
void crearLista(t_lista *p){
    *p = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int insertarOrdenado(t_lista *p,const t_dato *d,t_cmp cmp){
    t_nodo *nue;
    t_datoL dat;
    while(*p && (cmp(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    if(*p && (cmp(&((*p)->dato),d)==0)){
        ((*p)->dato.cantArch)++;
    }else{ // Extension diferente
        nue = (t_nodo*) malloc(sizeof(t_nodo));
        if(!nue){
            return 0;
        }
        nue->sig=*p;
        nue->dato= *d;
        *p=nue;
        crearListaL(&((*p)->dato.list));
    }
    strcpy(dat.name,d->name);
    dat.owner = d->owner;
    dat.tamArch = d->tamArch;
    insertarOrdenadoL(&((*p)->dato.list),&dat,cmpL);
    return 1;
 }
//////////////////////////////////////////////////////////////////////////////////
 int cmp(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    return strcmp(((t_dato*)e1)->ext,((t_dato*)e2)->ext);
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
    t_nodo* ant;
    ant = *p;
    printf("****************************************************************************************\n");
    while(*p){
        printf("Extension: %s\t", ((*p)->dato).ext);
        printf("Cant Archivos: %d\t", ((*p)->dato).cantArch);
        printf("tam Max: %ld\t", ((*p)->dato).tamMax);
        printf("tam Min: %ld\n", ((*p)->dato).tamMin);
        mostrarListaL(&((*p)->dato).list);
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
// Devuelve la extension del archivo
const char *get_ext(const char *filename){
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int cargarArchivosEnLista(t_lista* p, const char *name){
    t_dato d;
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    int file_count = 0;
    char ext[100];
    char pathArch[500];

    if (!(dir = opendir(name)))
      return 0;

    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_type == DT_DIR) { // Si es directorio
          char path[1024];
          if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
              continue;
          snprintf(path, sizeof(path), "%s%s/", name, entry->d_name);
          file_count += cargarArchivosEnLista(p, path);
      } else if(entry->d_type == DT_REG){ // Si es archivo regular
          strcpy(ext,get_ext(entry->d_name));
          if(strcmp(ext,"")!=0){ //TIENE EXTENSION
            file_count++;

            strcpy(pathArch,name);
            strcat(pathArch,entry->d_name);
            stat(pathArch, &st);

            // copy data
            strcpy(d.ext,ext);
            d.cantArch=1;
            strcpy(d.name,entry->d_name);
            d.owner = st.st_uid;
            d.tamArch = st.st_size;
            d.tamMax = d.tamArch;
            d.tamMin = d.tamArch;

            insertarOrdenado(p, &d, cmp);
          }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void crearListaL(t_listaL *p){
    *p = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int insertarOrdenadoL(t_listaL *p,const t_datoL *d,t_cmpL cmpL){
    t_nodoL *nue;
    
    while(*p && (cmpL(&((*p)->dato),d)<0)){
        p= &(*p)->sig;
    }
    /*if(*p && (cmpL(&((*p)->dato),d)==0)){
        return 0;
    }*/
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
    return strcmp(((t_datoL*)e1)->name,((t_datoL*)e2)->name);
}
//////////////////////////////////////////////////////////////////////////////////
 int cmpA(const void *e1,const void *e2){
    //e1 es la clave de la lista
    //e2 es la clave de la nueva info
    return ((t_datoL*)e1)->owner - ((t_datoL*)e2)->owner;
}
//////////////////////////////////////////////////////////////////////////////////
void vaciarListaL(t_listaL *p){
    t_nodoL *aux;
    while(*p){
        aux = *p;
        p = &(*p)->sig;
        free(aux);
    }
}
//////////////////////////////////////////////////////////////////////////////////
void mostrarListaL(t_listaL *p){
    t_nodoL* ant;
    ant = *p;
    while(*p){  
        printf("Name: %s\t", ((*p)->dato).name);
        printf("Owner: %d\t", ((*p)->dato).owner);
        printf("Tam: %ld\n", ((*p)->dato).tamArch);
        p = &(*p)->sig;
    }
    printf("****************************************************************************************\n");
}
//////////////////////////////////////////////////////////////////////////////////
int eliminarPorPosicionL(t_listaL *p, int poss){
    t_nodoL *aux;
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
int sizeL(t_listaL *p){
    int size = 0;
    while(*p){
        size++;
        p = &(*p)->sig;
    }
    return size;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ordenarListaL(t_listaL *p){
    t_nodoL* inicio;
    t_nodoL* current;
    int aux;
    char auxN[100];

    inicio = *p;
    current = (*p)->sig;

    while(inicio){
        while(current){
            if((inicio->dato).owner > ((current)->dato).owner){
                // INTERCAMBIO owner
                aux = ((current)->dato).owner;
                ((current)->dato).owner = (inicio->dato).owner;
                (inicio->dato).owner = aux;
                
                // INTERCAMBIO NOMBRE ARCHIVO
                strcpy(auxN, ((current)->dato).name);
                strcpy(((current)->dato).name, (inicio->dato).name);
                strcpy((inicio->dato).name, auxN);

                // INTERCAMBIO tam arch
                aux = ((current)->dato).tamArch;
                ((current)->dato).tamArch = (inicio->dato).tamArch;
                (inicio->dato).tamArch = aux;
            }else{
                current = current->sig;
            }
        }
        // Si no se termino la lista, cambio los punteros.
        if(inicio->sig){
            inicio = inicio->sig;
            current = inicio->sig;
        }else{
            inicio = inicio->sig;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void buscarMayorMenor(t_dato *d){
    t_listaL l = d->list;
    while(l){
        if(d->tamMax < l->dato.tamArch){
            d->tamMax = l->dato.tamArch;
        }
        if(d->tamMin > l->dato.tamArch){
            d->tamMin = l->dato.tamArch;
        }
        l = l->sig;
    }
    printf("%s - Max %ld - Min %ld\n", d->ext, d->tamMax, d->tamMin);
}
*/

void buscarMayorMenorOwner(t_dato *d){
    t_listaL l = d->list;
    while(l){
        if(d->tamMax < l->dato.tamArch){
            d->tamMax = l->dato.tamArch;
        }
        if(d->tamMin > l->dato.tamArch){
            d->tamMin = l->dato.tamArch;
        }
        l = l->sig;
    }

    ordenarListaL(&(d->list));
    l = d->list;

    t_nodoL* ant;
    ant = l;

    t_datoL d1;
    t_listaL l1;
    t_listaL l2;

    crearListaL(&l1);
    
    d1.owner = l->dato.owner;
    d1.tamArch = 1;
    d->tamArch = 1;

    insertarOrdenadoL(&l1,&d1,cmpA);
    l = l->sig;
    l2 = l1;

    while(l){
        if(((l->dato).owner == (l2->dato).owner)){
            l2->dato.tamArch++;   
        }else{
            d1.owner = l->dato.owner;
            d1.tamArch = 1;
            insertarOrdenadoL(&l1,&d1,cmpA);
            l2 = l2->sig;
        }
        ant = l;
        l = l->sig;
    }

    d->owner = l1->dato.owner;
    d->tamArch = l1->dato.tamArch;
    l1 = l1->sig;

    while(l1){
        if(d->tamArch < (l1->dato).tamArch){
            d->owner = l1->dato.owner;
            d->tamArch = l1->dato.tamArch;
        }
        l1 = l1->sig;
    }
    //printf("%s - Max %ld - Min %ld - Owner most files: %d - Cant Files: %ld\n", d->ext, d->tamMax, d->tamMin, d->owner, d->tamArch);
}
/*
void buscarMostOwner(t_dato *d){
    ordenarListaL(&(d->list));
    t_listaL l = d->list;

    t_nodoL* ant;
    ant = l;

    t_datoL d1;
    t_listaL l1;
    t_listaL l2;

    crearListaL(&l1);
    
    d1.owner = l->dato.owner;
    d1.tamArch = 1;
    d->tamArch = 1;

    insertarOrdenadoL(&l1,&d1,cmpA);
    l = l->sig;
    l2 = l1;

    while(l){
        if(((l->dato).owner == (l2->dato).owner)){
            l2->dato.tamArch++;   
        }else{
            d1.owner = l->dato.owner;
            d1.tamArch = 1;
            insertarOrdenadoL(&l1,&d1,cmpA);
            l2 = l2->sig;
        }
        ant = l;
        l = l->sig;
    }

    d->owner = l1->dato.owner;
    d->tamArch = l1->dato.tamArch;
    l1 = l1->sig;

    while(l1){
        if(d->tamArch < (l1->dato).tamArch){
            d->owner = l1->dato.owner;
            d->tamArch = l1->dato.tamArch;
        }
        l1 = l1->sig;
    }

    printf("%s - Owner most files: %d - Cant Files: %ld\n", d->ext, d->owner, d->tamArch);
}*/