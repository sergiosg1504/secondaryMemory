// SERGIO SANCHEZ GARCIA  sergiosg@usal.es  70961594Q  GRUPO B3
#include "indice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(){
	char ficheroDatos[]="alumnos.dat";
	char ficheroIndice[]="alumnos.idx";
	char ficheroHash[]="alumnos.hash";
	char dni[9], provincia[11], salida;
	int numReg, numRegDes, retorno, nCubos, ncuboDes, posReg;
	tipoAlumno alumno;
	tipoAlumno *al;
	FILE *fp;
	
    numReg=leeSecuencial(ficheroDatos);
    printf("Numero de Registros: %d \n\n\n",numReg);
	
	// Prueba EJERCICIO 1
	printf("Generacion de fichero indice ...");
	if((retorno = generaIndice(ficheroDatos, ficheroIndice)) < 0)
		return retorno;	
	printf("COMPLEATADA\n\n");
	do{
		printf("\n\nDNI del alumno que desea buscar : ");
		scanf("%s", dni);
		if((retorno = busquedaIndice(dni, ficheroDatos, ficheroIndice, &alumno)) < 0)
			return retorno;
		printf("DATOS DEL ALUMNO\nNombre: %s\nPrimer apellido: %s\nSegundo apellido : %s\nDNI: %s\nProvincia: %s\n\n",
					alumno.nombre, alumno.ape1, alumno.ape2, alumno.dni, alumno.provincia);
		fflush(stdin);
		printf("Si desea buscar otro alumno pulse Y, sino cualquier otra tecla : ");
		salida = getchar();
	} while (salida == 'Y' || salida == 'y');
	
	// Prueba EJERCICIO 2
	printf("\n\n\n\nGeneracion de fichero indice ...");
    numRegDes=creaHash(ficheroDatos,ficheroHash);
	printf("COMPLEATADA\n\n");
    printf("%d Registros DESBORDADOS\n\n",numRegDes);
	retorno = leeHash(ficheroHash);
	if(NULL == (fp = fopen(ficheroHash,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero hash\n");
		return -1;
	}
	do{
		printf("\n\nDNI del alumno que desea buscar : ");
		scanf("%s", dni);
		al = busquedaHash(fp, dni, &nCubos, &ncuboDes, &posReg, &retorno);
		if(retorno < 0)
			return retorno;
		printf("El registro del alumno con DNI %s  se encuentra en el cubo %d, desborde %d, posicion de registro en cubo %d\n\n", dni, nCubos, ncuboDes, posReg);
		fflush(stdin);
		printf("Si desea buscar la localizacion de otro alumno pulse Y, sino cualquier otra tecla : ");
		salida = getchar();
	} while (salida == 'Y' || salida == 'y');
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero hash\n");
		return -1;
	}
	do{
		fflush(stdin);
		printf("\n\nDNI del alumno que desea modificar su provincia : ");
		scanf("%s", dni);
		printf("\n\nNueva provincia : ");
		scanf("%s", provincia);
		if((retorno = modificarReg(ficheroHash, dni, provincia)) < 0)
			return retorno;
		printf("\nCambio efectuado en el Cubo %d\n\n", retorno);
		fflush(stdin);
		printf("Si desea hacer otro cambio pulse Y, sino cualquier otra tecla : ");
		salida = getchar();
	} while (salida == 'Y' || salida == 'y');
	
	retorno = leeHash(ficheroHash);
	
	free(al);
	al = NULL;
	return 0;
}