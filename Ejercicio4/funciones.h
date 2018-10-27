#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
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

int menu();
void cargar_nota();
void consultar_promedio_general();
void consultar_promedio_por_materia();
void salir();
int leer_socket (void *, int lon, t_dato);
int write_socket (void *, int lon , t_dato);