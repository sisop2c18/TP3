#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <pwd.h>

///////////////////////////////////////
typedef struct{
    char name[100];
    char owner[100];
    //int owner;
    long tamArch;
}t_datoL;

typedef struct s_nodoL{
    t_datoL dato;
    struct s_nodoL *sig;
}t_nodoL;


typedef t_nodoL* t_listaL;

typedef struct{
    char ext[100];
    int cantArch;
    long tamMax;
    long tamMin;
    char name[100];
    char owner[100];
    //int owner;
    long tamArch;
    t_listaL list;
}t_dato;

typedef struct s_nodo{
    t_dato dato;
    struct s_nodo *sig;
}t_nodo;

typedef t_nodo* t_lista;

typedef int (*t_cmp)(const void *e1,const void *e2);
typedef int (*t_cmpL)(const void *e1,const void *e2);
///////////////////////////////////////
void    crearLista(t_lista*);
int     insertarOrdenado(t_lista*,const t_dato*,t_cmp cmp);
int		cargarArchivosEnLista(t_lista* p, const char *name);
void    mostrarLista(t_lista*);
void    vaciarLista(t_lista*);
int     cmp(const void*,const void*);
int     eliminarPorPosicion(t_lista*, int);
int     size(t_lista*);

void    crearListaL(t_listaL*);
int     insertarOrdenadoL(t_listaL*,const t_datoL*,t_cmpL cmpL);
void    mostrarListaL(t_listaL*);
void    vaciarListaL(t_listaL*);
int     cmpL(const void*,const void*);
int     cmpA(const void*,const void*);
int     eliminarPorPosicionL(t_listaL*, int);
int     sizeL(t_listaL*);
void    ordenarListaL(t_listaL*);


const char *get_ext(const char *);
void buscarMayorMenorOwner(t_dato *);
//void buscarMostOwner(t_dato *);