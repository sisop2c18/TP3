#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "funciones_Lista.h"

#define PP "Primer Parcial"
#define SP "Segundo Parcial"
#define REC "Recuperatorio"
#define CARGAR 1 //CARGAR NOTAS
#define GENERAL 2 //PROMEDIO GENERAL
#define MATERIA 3 //PROMEDIO MATERIA
#define QUIT 4 //CLIENTE SE DESCONECTO
#define cls() printf("\033[H\033[J") // FUNCION PARA LIMPIAR PANTALLA
#define ES_LETRA(x) ((x > 64 && x < 91) || (x > 96 && x < 123)) ? 1 : 0
#define ES_NUMERO(x) ((x > 47 && x < 58)) ? 1 : 0

t_listaL bd;
t_lista clientes;

int menu();
void cargar_nota();
void consultar_promedio_general();
void consultar_promedio_por_materia();
void salir();
int normalizar(char*);
int leer_socket (void *, int lon, t_dato*);
int escribir_socket (void *, int lon , t_dato*);