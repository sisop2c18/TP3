all : Server Client

Server: server.c funciones.o funciones_Lista.o
	gcc -o Server server.c  funciones.o funciones_Lista.o -lpthread

Client: cliente.c funciones.o funciones_Lista.o
	gcc -o Client cliente.c funciones.o funciones_Lista.o -lpthread

funciones.o: funciones.h funciones.c funciones_Lista.h
	gcc -c funciones.c

funciones_Lista.o: funciones_Lista.h funciones_Lista.c
	gcc -c funciones_Lista.c
	
clean :
	rm *.o Client Server