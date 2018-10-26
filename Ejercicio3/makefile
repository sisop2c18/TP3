
Ejercicio3: Ejercicio3.o funciones_Lista.o
	gcc -o Ejercicio3 Ejercicio3.c funciones_Lista.c

Ejericicio2.o: Ejercicio3.c parametros.h
	gcc -c Ejercicio3.c

funciones_Lista.o: funciones_Lista.c lista.h
	gcc -c funciones_Lista.c

clean:
	rm -f *.o
