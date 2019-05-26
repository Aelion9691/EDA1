#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glist.h"
#include <locale.h>
#include "tablahash.h"
#include "tablahash.c"

#define B 121139

int igual(void* a, void* b)
{
  return a == b;
}

unsigned int RSHash(void* str)
{ 	
	char* aux = (char*) str;
	unsigned int b = 378551;
 	unsigned int a = 63689;
 	unsigned int hash = 0;
	unsigned int i = 0;
 	for(i = 0; *aux; aux++, i++)
 	{ 
 		hash = hash * a + (*aux);
 		a = a * b;
 	}
 return (hash%B);
}


void creacion_abb(char* nombreArchivo, TablaHash* th)
{
	char* palabra = NULL;
	FILE *fp = fopen(nombreArchivo, "r");
	size_t j = 0;
	size_t final;
	

	while(!(feof(fp)))
	{		
		final = getline(&palabra, &j, fp);
		palabra[final-1] = '\0';
		final--;
		
		tablahash_insertar(th, palabra,palabra, igual);
	}
	free(palabra);
	fclose(fp);
	
}

char** intercambio(char* palabra, int len, char** altern, int* l) {
	if (len > 1) {
		for (int i = 0; i < (len - 1) && (*l) < 3; i++) {
			char* temp = malloc(sizeof(palabra));
			strcpy(temp, palabra);
			strncpy(temp+i,palabra+i+1,1);
			strncpy(temp+i+1,palabra+i,1);
			altern[*l] = malloc(sizeof(char) * (len + 1));
			strcpy(altern[*l], temp);
			(*l)++;
			free(temp);
		}
	}
	return altern;
}

char** insertar(char* palabra, int len, char** altern, int* l, char* abc) {
	for (int i = 0; i <= len; i++) {
		for (int j = 0; j < 33; j++) {
			char* temp = malloc(sizeof(char) * (len + 2));
			int k = 0;
			if (i>0) {
				strncpy(temp, palabra, i);
				k = k + i;
			}
			strncpy(temp + k, abc + j, 1);
			if (i < len ) {
				k++;
				strcpy(temp + k, palabra + i);
			}
			temp[len + 1] = '\0';
			altern[*l] = malloc(sizeof(char) * (len + 1));
			strcpy(altern[*l], temp);
			(*l)++;
			free(temp);
		}
	}
	return altern;
}

char** eliminar(char* palabra, int len, char** altern, int* l) {
	if (len > 1) {
		for (int i = 0; i < len; i++) {
			char* temp = malloc(sizeof(char) * len);
			int k = 0;
			if (i > 0) {
				strncpy(temp, palabra, i);
				k = k + i;
			}
			if (i < (len - 1)) {
				strcpy(temp + k, palabra + i + 1);
			}
			temp[len - 1] = '\0';
			altern[*l] = malloc(sizeof(char) * (len + 1));
			strcpy(altern[*l], temp);
			//printf("%s\n", altern[*l]);
			(*l)++;
			free(temp);
		}
	}
	return altern;
}

char** reemplazar(char* palabra, int len, char** altern, int* l, char* abc) {
	if (len > 1) {
		for (int i = 0; i < len; i++) {
			for (int j = 0; j < 33; j++) {
				char* temp = malloc(sizeof(palabra));
				int k = 0;
				if (i > 0) {
					strncpy(temp, palabra, i);
					k = k + i;
				}
				strncpy(temp + k, abc + j, 1);
				if (i < (len - 1)) {
					k++;
					strcpy(temp + k, palabra + i + 1);
				}
				temp[len] = '\0';
				altern[*l] = malloc(sizeof(char) * (len + 1));
				strcpy(altern[*l], temp);
				//printf("%s\n", altern[*l]);
				(*l)++;
				free(temp);
			}
		}
	}
	return altern;
}

char** separar(char* palabra, int len, char** altern, int* l) {
	for (int i = 1; i < len; i++) {
		char* temp = malloc(sizeof(char) * (len + 2));
		strncpy(temp, palabra, i);
		int k = i;
		temp[k] = ' ';
		k++;
		strcpy(temp + k, palabra + i);
		temp[len + 1] = '\0';
		altern[*l] = malloc(sizeof(char) * (len + 1));
		strcpy(altern[*l], temp);
		(*l)++;
		free(temp);
	}
	return altern;
}

//~ char** corrector(char* palabra, char** altern, BSTree diccionario) {
	//~ altern = intercambio(char* palabra, char** altern, BSTree diccionario);
	

int main() {
	setlocale(LC_CTYPE, "C.UTF8");
	char* palabra = malloc(sizeof(char) * 5);
	char* abc = malloc(sizeof(char) * 34);
	palabra = "hola";
	abc = "aáeéoósrniídlctuúümpbgvyqhfzjñcwk";
	int len = strlen(palabra);
	TablaHash* th = tablahash_crear(121129, RSHash, igual);
	creacion_abb("listado-general.txt",th);
	char** alternativas = malloc(sizeof(char*) * 307);
	int l = 0;
	alternativas = intercambio(palabra, len, alternativas, &l);
	alternativas = insertar(palabra, len, alternativas, &l, abc);
	alternativas = eliminar(palabra, len, alternativas, &l);
	alternativas = reemplazar(palabra, len, alternativas, &l, abc);
	alternativas = separar(palabra, len, alternativas, &l);
	for (int i = 0; i < l; i++) {
		printf("%s\n", alternativas[i]);
	}
	for (int i = 0; i < l; i++) {
		free(alternativas[i]);
	}
	free(alternativas);
	free(palabra);
	return 0;
}
	
