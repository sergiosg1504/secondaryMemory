#include <stdio.h>
#define C 5	                // capacidad del cubo
#define CUBOS 20           // Número de cubos en el area prima
#define CUBOSDESBORDE  4   // Número de cubos area de desborde

#ifndef _STRUCT_TIPO_ALUMNO
#define _STRUCT_TIPO_ALUMNO
typedef struct {
	char dni[9];
	char nombre[19];
	char ape1[19];
	char ape2[19];
	char provincia[11];
	} tipoAlumno;
#endif

typedef struct {
	tipoAlumno reg[C];
	int cuboDES;
	int numRegAsignados;
	}tipoCubo;

void creaHvacio(char *fichHash);
int leeHash(char *fichHash);
int creaHash(char *fichEntrada,char *fichHash);
int insertarReg(FILE *f, tipoAlumno *reg);

// Ejercicio 2
tipoAlumno * busquedaHash(FILE *f, char *dni, int *nCubo, int *ncuboDes, int *posReg, int *error);
int modificarReg(char *fichero, char *dni, char *provincia);


