prueba: prueba.c indice.o indice.h dispersion.o dispersion.h
	gcc -g prueba.c indice.o dispersion.o -o prueba
	
indice.o: indice.c indice.h secuencial.h dispersion.h
	gcc -c indice.c -o indice.o
	
dispersion.o: dispersion.c dispersion.h
	gcc -c dispersion.c -o dispersion.o
	
limpiar:
	rm *.o