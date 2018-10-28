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
    pthread_t threadId;
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
typedef int (*t_cmpL)(const void *e1,const void *e2);
typedef int (*t_cmpG)(const void *e1,const void *e2);
typedef int (*t_cmpM)(const void *e1,const void *e2);
///////////////////////////////////////////////////////////////
void    crearBD(t_listaL *);
void    cargarDB(t_listaL*);
int     addDB(t_listaL*,const t_comando*);
int     addLista(t_listaL*,const t_comando*,t_cmpL cmpL);
void    mostrarDB(t_listaL*);
int     cmpL(const void*,const void*);
int     cmpG(const void*,const void*);
int     cmpM(const void*,const void*);
double  devolverGeneral(t_listaL *,const t_comando *, t_cmpG cmpG);
double  devolverMateria(t_listaL *,const t_comando *, t_cmpM cmpM);

void    crearLista(t_lista *);
void    vaciarLista(t_lista*);
int     addUsuario(t_lista*,const t_dato*,t_cmp cmp);
void    mostrarClientes(t_lista *);
int     cmp(const void*,const void*);
int     eliminarUser(t_lista *p, const t_dato *,t_cmp cmp);



int    size(t_lista*);
int    exist(t_lista *,const t_dato *,t_cmp cmp);
double   buscarHora(t_lista *,const t_dato *,t_cmp cmp);

void    imprimirListaL(t_listaL *, FILE*);