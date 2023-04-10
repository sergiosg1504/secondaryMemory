// SERGIO SANCHEZ GARCIA  sergiosg@usal.es  70961594Q  GRUPO B3

#ifndef __INDICE_H
#define __INDICE_H
#include <stdio.h>
#include "secuencial.h"
#include "dispersion.h"
typedef struct {
	char dni[9];
	int  NRR;
} tipoIndice;

// Funciones a entregar
// Ejercicio 1
int generaIndice(char *ficheroDatos, char *ficheroIndice);
int busquedaIndice(char *dni, char *ficheroDatos, char *ficheroIndice, tipoAlumno *alumno);

#endif
