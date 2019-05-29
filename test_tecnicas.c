#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "hash.h"
#include "hash.c"


typedef struct _Word
{
	char* palabra;
	int linea;
}Word;

void* copiar_palabra(void* dato)
{
	int size = strlen((char*) dato);
	char* copia = malloc(sizeof(char) * (size+1));
	strcpy(copia, (char* ) dato);
	return copia;

}

int comparador_palabras(void* dato1, void* dato2)
{

	char* palabra1 = (char*) dato1;
	char* palabra2 = (char*) dato2;

	return !(strcmp(palabra1,palabra2));
}


Word** filtrado_palabras(Word** texto,TablaHash* th)
{
	int SIZE_MAX = 500;
	int i = 0;
	int j = 0;
	if (texto[0] == NULL)
	{
		return texto;
	}
	else
	{
		Word** textoFiltrado = malloc(sizeof(Word*)* SIZE_MAX);
		while(texto[i] != NULL)
		{
			if (tablahash_buscar(th, texto[i]->palabra,comparador_palabras))
			{
				textoFiltrado[j] = malloc(sizeof(Word));
				textoFiltrado[j]->palabra = malloc(strlen(texto[i]->palabra) * sizeof(char));
				strcpy(textoFiltrado[j]->palabra, texto[i]->palabra);
				if (j> SIZE_MAX-2)
				{
					textoFiltrado = realloc(textoFiltrado,sizeof(Word*)*(SIZE_MAX+100));
					SIZE_MAX +=100; 
				}
				j++;
			}	
			i++;
		}
		return textoFiltrado;		
	}
}




unsigned int RSHash(void* p)
{ 	
	unsigned int b = 378551;
 	unsigned int a = 63689;
 	unsigned int hash = 0;
	unsigned int i = 0;
	char* str = (char*) p;

 	for(i = 0; *str; str++, i++)
 	{ 
 		hash = hash * a + (*str);
 		a = a * b;
 	}
 return (hash);
}



Word** lectura_texto(char* nombreArchivo)
{
	int SIZE_MAX = 500;
	int SIZE_MAX_PALABRA  = 30;
	char* palabra = malloc(sizeof(char)* SIZE_MAX_PALABRA);
	Word** lista = malloc(sizeof(Word*)* SIZE_MAX);
	char caracter;
	int linea = 0;
	int pos = 0;	
	int j = 0;
	
	FILE *fp = fopen(nombreArchivo, "r");
	
	while((caracter = fgetc(fp)) != EOF)
	{		
		
		j = 0;
		SIZE_MAX_PALABRA = 30;
		while(caracter != '\n' && caracter != 32 )
		{
			if(caracter <=89 && caracter>=65)
				caracter+=32;
			if (j < SIZE_MAX_PALABRA)
			{
				palabra[j] = caracter;
			}
			else
			{
				SIZE_MAX_PALABRA += 10;
				palabra = realloc(palabra, sizeof(char) * SIZE_MAX_PALABRA);
				palabra[j] = fgetc(fp);
			}
			j++;	
			caracter = fgetc(fp);
		}

		palabra[j] ='\0';
		Word *dato = malloc(sizeof(Word));
		dato->palabra = malloc(SIZE_MAX_PALABRA *sizeof(char));
		dato->linea = linea;
		strcpy(dato->palabra, palabra);
		if (caracter == '\n')
		{
			linea++;
		}

		if (pos < SIZE_MAX-1)
		{
			lista[pos] = dato;
		}
		else
		{
			lista = realloc(lista,sizeof(Word*)*(SIZE_MAX+100));
			SIZE_MAX +=100;
		}
		pos++;
		
		
	}
	lista[SIZE_MAX-1] = NULL;
	
	
	free(palabra);
	fclose(fp);
	return lista;
}


void crear_diccionario(char* nombreArchivo, TablaHash* th)
{
	char* palabra = NULL;
	FILE *fp = fopen(nombreArchivo, "r");
	size_t j = 0;
	size_t final;
	FILE *salida = fopen("salida.txt", "w");
	
	while(!(feof(fp)))
	{		
		final = getline(&palabra, &j, fp);
		palabra[final-1] = '\0';		
		tablahash_insertar(th, palabra,palabra, copiar_palabra);

	}
	free(palabra);
	fclose(fp);
	fclose(salida);
	
}


char** intercambio(char* palabra, char** altern, int* l) {
	int len = strlen(palabra);
	if (len > 1) {
		char* temp = malloc(len * sizeof(char));
		for (int i = 0; i < (len - 1) && (*l) < 3; i++) {
			strcpy(temp, palabra);
			strncpy(temp+i,palabra+i+1,1);
			strncpy(temp+i+1,palabra+i,1);
			altern[*l] = malloc(sizeof(char) * (len + 1));
			strcpy(altern[*l], temp);
			(*l)++;
		}
		free(temp);
	}
	return altern;
}

char** insertar(char* palabra, char** altern, int* l, char* abc) {
	int len = strlen(palabra);
	char* temp = malloc(sizeof(char) * (len + 2));
	for (int i = 0; i <= len; i++) {
		for (int j = 0; j < strlen(abc) ; j++) {
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
		}
	}
	free(temp);
	return altern;
}

char** eliminar(char* palabra, char** altern, int* l) {
	int len = strlen(palabra);
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

char** reemplazar(char* palabra, char** altern, int* l, char* abc) {
	int len = strlen(palabra);
	if (len > 1) {
		for (int i = 0; i < len; i++) {
			for (int j = 0; j < strlen(abc); j++) {
				char* temp = malloc(sizeof(char) * (len + 1));
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

char** separar(char* palabra, char** altern, int* l) {
	int len = strlen(palabra);
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
	
	char* abc = malloc(sizeof(char) * 34);
   
	strcpy(abc,"abcdefgihjkilmnopqrstuvwxyz");
	
	
	TablaHash* th = tablahash_crear(15000, RSHash);
	
	crear_diccionario("listado-general.txt", th);

	Word** texto = lectura_texto("testeo.txt");

	int i = 0;
	while(texto[i] != NULL)
	{
		printf("%s\n",texto[i]->palabra );
		i++;
	}
	

	
	
	
	return 0;
}
