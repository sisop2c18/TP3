#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include "funciones_Lista.h"

#define PP "Primer Parcial"
#define SP "Segundo Parcial"
#define REC "Recuperatorio"
#define BAD 0 // error
#define GOOD 1 // todo ok
#define CARGAR 1 //CARGAR NOTAS
#define GENERAL 2 //PROMEDIO GENERAL
#define MATERIA 3 //PROMEDIO MATERIA
#define QUIT 4 //CLIENTE SE DESCONECTO
#define cls() printf("\033[H\033[J") // FUNCION PARA LIMPIAR PANTALLA
#define ES_LETRA(x) ((x > 64 && x < 91) || (x > 96 && x < 123)) ? 1 : 0
#define ES_NUMERO(x) ((x > 47 && x < 58)) ? 1 : 0

typedef struct{
    int comando;
    int decision;
    int dni;
    double prom;
}t_decision;

t_listaL bd;
t_lista clientes;
t_listaP promedios;
char materia[50];
pthread_mutex_t mutex;
pthread_mutex_t write_mutex;
pthread_mutex_t quit_mutex;
pthread_mutex_t cliente_mutex;
pthread_mutex_t cread_mutex;
t_decision decision;
t_decision cl_dec;
int blocked; // espera a que se muestre por pantalla el saludo de despedida

void* server_run(void *args);
void* server_write(void *args);
void* client_read(void *args);

int menu(t_dato *);
void cargar_nota(t_dato *);
void consultar_promedio_general(t_dato *);
void consultar_promedio_por_materia(t_dato *);
void salir(t_dato *);
int normalizar(char*);
int leer_socket (void *, int lon, t_dato*);
int escribir_socket (void *, int lon , t_dato*);

void quitAll(t_lista *);
// new try
t_dato fd;
void salir2(t_dato *);
void clearStdin();