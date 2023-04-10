// SERGIO SANCHEZ GARCIA  sergiosg@usal.es  70961594Q  GRUPO B3
#include "indice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>



int generaIndice(char *ficheroDatos, char *ficheroIndice){
	FILE *fDatos, *fIndice;
	int nrr=0;
	tipoAlumno alumno;
	tipoIndice indice;
	
	if(NULL == ficheroDatos){
		fprintf(stderr, "ERROR: Parametro invalido, puntero a fichero datos es NULL\n");
		return -2;
	}
	if(NULL == (fDatos = fopen(ficheroDatos,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero datos\n");
		return -2;
	}
	if(NULL == ficheroIndice){
		fprintf(stderr, "ERROR: Parametro invalido, puntero a fichero indice es NULL\n");
		return -3;
	}
	if(NULL == (fIndice = fopen(ficheroIndice,"w+b"))){
		fprintf(stderr, "ERROR: Apertura de fichero indice\n");
		return -3;
	}
	if(fread(&alumno, sizeof(alumno), 1, fDatos) != 1){
		fprintf(stderr, "ERROR: Lectura en fichero datos\n");
		return -4;
	}
	while(!feof(fDatos)){
		strcpy(indice.dni, alumno.dni);
		indice.NRR = nrr;
		fwrite(&indice, sizeof(indice), 1, fIndice);
		nrr++;
		fread(&alumno, sizeof(alumno), 1, fDatos);
	}
	if(fclose(fDatos) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero datos\n");
		return -2;
	}
	if(fclose(fIndice) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero indice\n");
		return -3;
	}
	return nrr;
}

int busquedaIndice(char *dni, char *ficheroDatos, char *ficheroIndice, tipoAlumno *alumno){
	FILE *fp;
	tipoIndice indice;
	int nrr=-1;
	
	if(NULL == ficheroIndice){
		fprintf(stderr, "ERROR: Parametro invalido, puntero a fichero indice es NULL\n");
		return -3;
	}
	if(NULL == (fp = fopen(ficheroIndice,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero indice\n");
		return -3;
	}
	if(fread(&indice, sizeof(indice), 1, fp) != 1){
		fprintf(stderr, "ERROR: Lectura en fichero datos\n");
		return -4;
	}
	while(!feof(fp)){
		if(!strcmp(indice.dni, dni)){
			nrr = indice.NRR;
			break;
		}
		fread(&indice, sizeof(indice), 1, fp);
	}
	if(nrr == -1){
		fprintf(stderr, "ERROR: Registro no existente\n");
		return -1;
	}
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero indice\n");
		return -3;
	}
	if(NULL == ficheroDatos){
		fprintf(stderr, "ERROR: Parametro invalido, puntero a fichero datos es NULL\n");
		return -2;
	}
	if(NULL == (fp = fopen(ficheroDatos,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero datos\n");
		return -2;
	}
	if(fseek(fp, nrr*(sizeof(*alumno)), SEEK_SET) != 0){
		fprintf(stderr, "ERROR: Movimiento del puntero en fichero datos\n");
		return -4;
	}
	if(fread(alumno, sizeof(*alumno), 1, fp) != 1){
		fprintf(stderr, "ERROR: Lectura en fichero datos\n");
		return -4;
	}
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero datos\n");
		return -2;
	}
	return nrr;
}

/*-------------------------------------------------------------------------------------------------------*/

int leeSecuencial(char *fichero){
	FILE *fp;
	tipoAlumno reg;
	int nrr = 0;
	
	if(NULL == fichero){
		fprintf(stderr, "ERROR: Parametro invalido, fichero es NULL\n");
		return -1;
	}
	if(NULL == (fp = fopen(fichero,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero\n");
		return -1;
	}
	fread(&reg, sizeof(reg), 1, fp);
	while(!feof(fp)){
	    printf("\t %d %s %s %s %s %s\n\n", nrr, reg.dni, reg.nombre, reg.ape1, reg.ape2, reg.provincia);
		nrr++;
		fread(&reg, sizeof(reg), 1, fp);
	}
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero\n");
		return -1;
	}
	return nrr;
} 