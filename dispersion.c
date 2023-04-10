// SERGIO SANCHEZ GARCIA  sergiosg@usal.es  70961594Q  GRUPO B3
#include "dispersion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

void creaHvacio(char *fichHash)
{ FILE *fHash;
  tipoCubo cubo;
  int j;
  int numCubos =CUBOS+CUBOSDESBORDE;

  memset(&cubo,0,sizeof(cubo));

  fHash = fopen(fichHash,"wb");
  for (j=0;j<numCubos;j++) fwrite(&cubo,sizeof(cubo),1,fHash);
	  
  fclose(fHash);
}

int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  int j,i=0;
  size_t numLee;

   f = fopen(fichHash,"rb");
   rewind(f);
   fread(&cubo,sizeof(cubo),1,f);
   while (!feof(f)){
	for (j=0;j<C;j++) {
        if (j==0)    	printf("Cubo %2d (%2d reg. ASIGNADOS)",i,cubo.numRegAsignados);
        else  	printf("\t\t\t");
		//printf("no desborde %lu",i * sizeof(tipoCubo) + j * sizeof(tipoAlumno));
	if (j < cubo.numRegAsignados) 
		    printf("\t%s %s %s %s %s\n",
	    		cubo.reg[j].dni,
				cubo.reg[j].nombre,
				cubo.reg[j].ape1,
		  		cubo.reg[j].ape2,
  	     	   	cubo.reg[j].provincia);
	else printf ("\n");
        }
       i++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   return i;
}

int creaHash(char *fichEntrada,char *fichHash){
	FILE *fp, *fhash;
	tipoAlumno reg;
	int numDesbordes = 0;
	
	creaHvacio(fichHash);
	if(NULL == fichEntrada){
		fprintf(stderr, "ERROR: Parametro invalido, fichero entrada es NULL\n");
		return -1;
	}
	if(NULL == (fp = fopen(fichEntrada,"rb"))){
		fprintf(stderr, "ERROR: Apertura de fichero entrada\n");
		return -1;
	}
	if(NULL == fichHash){
		fprintf(stderr, "ERROR: Parametro invalido, fichero hash es NULL\n");
		return -1;
	}
	if(NULL == (fhash = fopen(fichHash,"r+b"))){
		fprintf(stderr, "ERROR: Apertura de fichero hash\n");
		return -1;
	}
	
	fread(&reg, sizeof(reg), 1, fp);
	while(!feof(fp)){
		if(insertarReg(fhash, &reg) >= CUBOS)
			numDesbordes++;
		fread(&reg, sizeof(reg), 1, fp);
	}
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero entrada\n");
		return -1;
	}
	if(fclose(fhash) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero hash\n");
		return -1;
	}
	return numDesbordes;
}

int insertarReg(FILE *f, tipoAlumno *reg){
	int i;
	int desp;
	tipoCubo cubo;
	
	desp = atoi(reg->dni) % CUBOS;
	fseek(f, desp * sizeof(cubo), SEEK_SET);
	fread(&cubo, sizeof(cubo), 1, f);
	if(cubo.numRegAsignados < C){ // NO esta desbordado
		fseek(f, desp * sizeof(cubo), SEEK_SET);
		cubo.reg[cubo.numRegAsignados] = *reg;
		cubo.numRegAsignados++;
		fwrite(&cubo, sizeof(cubo), 1, f);
		return desp;
	}
	else{ // esta desbordado
		fseek(f, desp * sizeof(cubo), SEEK_SET);
		cubo.numRegAsignados++;
		fwrite(&cubo, sizeof(cubo), 1, f);
		fseek(f, CUBOS * sizeof(cubo), SEEK_SET); // vamos a los cubos de desborde
		i = 0;
		fread(&cubo, sizeof(cubo), 1, f);
		while(!feof(f)){
			if(cubo.numRegAsignados < C){ // hay hueco en el cubo de desborde
				cubo.reg[cubo.numRegAsignados] = *reg;
				cubo.numRegAsignados++;
				fseek(f, (CUBOS+i) * sizeof(cubo), SEEK_SET);
				fwrite(&cubo, sizeof(cubo), 1, f);
				return CUBOS + i;
			}
			else{
				cubo.numRegAsignados++;
				fseek(f, (CUBOS+i) * sizeof(cubo), SEEK_SET);
				fwrite(&cubo, sizeof(cubo), 1, f);
			}
			i++;
			fread(&cubo, sizeof(cubo), 1, f);
		}
		return -1;
	}
}

tipoAlumno * busquedaHash(FILE *f, char *dni, int *nCubo, int *ncuboDes, int *posReg, int *error){
	int clave, i, j;
	tipoAlumno *alumno;
	tipoCubo cubo;
	
	clave = atoi(dni) % CUBOS;
	fseek(f, clave * (sizeof(cubo)), SEEK_SET);
	if(fread(&cubo, sizeof(cubo), 1, f) != 1){
		fprintf(stderr, "ERROR: Lectura en fichero hash\n");
		*error = -4;
		return NULL;
	}
	for(i=0 ; (i < C) && (i < cubo.numRegAsignados) ; i++){
		if(!strcmp(cubo.reg[i].dni , dni)){ // se encuentra el registro en cubo no desbordado
			*nCubo = clave;
			*ncuboDes = -1;
			*posReg = i;
			if(NULL == (alumno = malloc(sizeof(tipoAlumno)))){
				fprintf(stderr, "ERROR: Reserva de memoria\n");
				*error = -4;
				return NULL;
			}
			*error = 0;
			*alumno = cubo.reg[i];
			return alumno;
		}
	}
	if(cubo.numRegAsignados < C){ // el registro no se encuentra
		*error = -1;
		return NULL;
	}
	// sino comprobar si esta en algun cubo de desborde
	fseek(f, CUBOS * sizeof(cubo), SEEK_SET); // vamos a los cubos de desborde
	fread(&cubo, sizeof(cubo), 1, f);
	j=0;
	while(!feof(f)){
		for(i = 0 ; (i < C) && (i < cubo.numRegAsignados) ; i++){
			if(!strcmp(cubo.reg[i].dni, dni)){ // se encuentra el registro en el cubo j desbordado
				*nCubo = clave;
				*ncuboDes = CUBOS + j;
				*posReg = i;
				if(NULL == (alumno = malloc(sizeof(tipoAlumno)))){
					fprintf(stderr, "ERROR: Reserva de memoria\n");
					*error = -4;
					return NULL;
				}
				*error = 0;
				*alumno = cubo.reg[i];
				return alumno;
			}
		}
		fread(&cubo, sizeof(cubo), 1, f);
		j++;
	}
	// el registro no se ha encontrado en ningun cubo
	*error = -1;
	return NULL;
}

int modificarReg(char *fichero, char *dni, char *provincia){
	FILE *fp;
	int nCubo, ncuboDes, posReg, error;
	tipoAlumno *alumno;
	tipoCubo cubo;
	
	if(NULL == fichero){
		fprintf(stderr, "ERROR: Parametro invalido, puntero a fichero hash es NULL\n");
		return -2;
	}
	if(NULL == (fp = fopen(fichero,"r+b"))){
		fprintf(stderr, "ERROR: Apertura de fichero hash\n");
		return -2;
	}
	
	alumno = busquedaHash(fp, dni, &nCubo, &ncuboDes, &posReg, &error);
	if(error != 0)
		return error;
	
	if(ncuboDes == -1){ // Sobreescribir en nCubo
		fseek(fp, nCubo * sizeof(cubo), SEEK_SET);
		if(fread(&cubo, sizeof(cubo), 1, fp) != 1){
			fprintf(stderr, "ERROR: Lectura en fichero hash\n");
			return -4;
		}
		strcpy(cubo.reg[posReg].provincia, provincia);
		fseek(fp, nCubo * sizeof(tipoCubo), SEEK_SET);
		if(fwrite(&cubo, sizeof(cubo), 1, fp) != 1){
			fprintf(stderr, "ERROR: Escritura en fichero hash\n");
			return -4;
		}
	}
	else{ // Sobreescribir en cubo de desborde
		fseek(fp, ncuboDes * sizeof(cubo), SEEK_SET);
		if(fread(&cubo, sizeof(cubo), 1, fp) != 1){
			fprintf(stderr, "ERROR: Lectura en fichero hash\n");
			return -4;
		}
		strcpy(cubo.reg[posReg].provincia, provincia);
		fseek(fp, ncuboDes * sizeof(tipoCubo), SEEK_SET);
		if(fwrite(&cubo, sizeof(cubo), 1, fp) != 1){
			fprintf(stderr, "ERROR: Escritura en fichero hash\n");
			return-4;
		}
	}
	
	if(fclose(fp) == EOF){
		fprintf(stderr, "ERROR: Cierre de fichero datos\n");
		return -2;
	}
	if(ncuboDes == -1)
		return nCubo;
	else
		return ncuboDes;
}