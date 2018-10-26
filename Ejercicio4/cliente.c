#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int puerto = 8080;
int Socket_Servidor;

int menu();
void cargar_nota();
void consultar_promedio_general();
void consultar_promedio_por_materia();

int main(int argc, char *argv[]){
    struct sockaddr_in server ;
    struct hostent *Host;
    if( argc != 2){
        printf("Error, debe pasar la Materia a la que pertenece y la direccion del servidor \n");
        exit(EXIT_FAILURE);
    }

    //puerto = atoi(argc[0])

    printf("Conectando con el servidor aguarde unos instantes...\n");
    server.sin_family=AF_INET;
    server.sin_port=htons(puerto);
    //cuando pase parametros lo descomento
    //server.sin_addr=net_addr(argv[0]);
    
    server.sin_addr.s_addr = ((struct in_addr *)(Host->h_addr))->s_addr;

    Socket_Servidor = socket (AF_INET, SOCK_STREAM, 0);
    
    if (Socket_Servidor == -1)
        return -1;


    if (connect(Socket_Servidor,(struct sockaddr*)&server, sizeof(server))==-1)
    {
        printf("error en la conexion al servidor");
        exit(-1);
    }

    printf("Bienvenido \n");

    menu();

}

 int menu(){
    int opcion;
    do
    {
        printf( "\n   1. Cargar nota. " );
        printf( "\n   2. Consultar promedio de notas general. ");
        printf( "\n   3. Consultar promedio de notas por materia. ");
        printf( "\n   4. Salir." );
        printf( "\n\n   Introduzca opcion (1-4): ");

        scanf( "%d", &opcion );

        switch ( opcion )
        {
            case 1: cargar_nota();
                    break;

            case 2: consultar_promedio_general();
                    break;

            case 3: consultar_promedio_por_materia();
                    break;
         }

    } while ( opcion != 4 );

    return 0;
}


void cargar_nota(){
    printf("ingresaste a cargar notas");
}

void consultar_promedio_general(){
    printf("ingresaste a promedio general");
}

void consultar_promedio_por_materia(){
    printf("ingresaste a promedio por materia");
}