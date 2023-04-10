#include <stdio.h>

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

int leeSecuencial(char *fichero);
