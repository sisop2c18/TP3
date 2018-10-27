#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
///////////////////////////////////////
#define BD "bd.txt"

typedef struct{
    int id;
    int socket;
}t_dato;

typedef struct s_nodo{
    t_dato dato;
    struct s_nodo *sig;
}t_nodo;

typedef struct{
    int comando;
    int dni;
    char materia[50];
    char instancia[16];
    int nota;
}t_comando;

typedef struct s_nodoL{
    t_comando dato;
    struct s_nodoL *sig;
}t_nodoL;

typedef t_nodo* t_lista;
typedef t_nodoL* t_listaL;
typedef int (*t_cmp)(const void *e1,const void *e2);
///////////////////////////////////////
void    crearBD(t_listaL *);
void    cargarDB(t_listaL*);
int     addDB(t_listaL*,const t_comando*);
void    mostrarDB(t_listaL*);

int     addUsuario(t_lista*,const t_dato*,t_cmp cmp);
void    mostrarClientes(t_lista *);

void   crearLista(t_lista *);
void   vaciarLista(t_lista*);
int    cmp(const void*,const void*);
int    eliminarDato(t_lista *p, const t_dato *,t_cmp);
int    size(t_lista*);
int    exist(t_lista *,const t_dato *,t_cmp);
double   buscarHora(t_lista *,const t_dato *,t_cmp);



int     cmpL(const void*,const void*);
void    imprimirListaL(t_listaL *, FILE*);