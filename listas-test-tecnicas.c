#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "hash.h"
#include "cola.h"


typedef struct _Word
{
	char* palabra;
	int linea;
}Word;

typedef int (*FuncionIgualdad)(void *, void *);

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
int comparador_palabras_no(void* dato1, void* dato2)
{
	int flag = 1;
	char* palabra1 = (char*) dato1;
	char* palabra2 = (char*) dato2;

	if (strcmp(palabra1, palabra2) != 0)
	{
		flag = 0;
	}
	return (flag);
}


Word** filtrado_palabras(Word** texto,TablaHash* th, FuncionIgualdad igual)
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
		for (int i = 0; i < SIZE_MAX; ++i)
		{
			textoFiltrado[i] =NULL;
		}
		while(texto[i] != NULL)
		{
			if (tablahash_buscar(th, texto[i]->palabra,igual))
			{
				textoFiltrado[j] = malloc(sizeof(Word));
				textoFiltrado[j]->palabra = malloc(strlen(texto[i]->palabra) * sizeof(char));
				strcpy(textoFiltrado[j]->palabra, texto[i]->palabra);
				if (j> SIZE_MAX-2)
				{
					textoFiltrado = realloc(textoFiltrado,sizeof(Word*)*(SIZE_MAX+100));
					SIZE_MAX +=100; 
					for (int i = SIZE_MAX - 100; i < SIZE_MAX; ++i)
					{	
						textoFiltrado[i] =NULL;
					}
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
	for (int i = 0; i < SIZE_MAX; ++i)
	{
		lista[i] =NULL;//anula todas la entradas.
	}
	char caracter;
	int linea = 0;
	int pos = 0;	
	int j = 0;
	
	FILE *fp = fopen(nombreArchivo, "r");
	
	while((caracter = fgetc(fp)) != EOF)
	{		
		
		j = 0;
		SIZE_MAX_PALABRA = 30;
		while(caracter != '\n' && caracter != 32 && caracter != EOF )
		{
			if(caracter <=90 && caracter>=65)
				caracter+=32;// convierte a todas las letras en minusculas.
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
		char ultimoCaracter = palabra[strlen(palabra)-1];
		if(ultimoCaracter <= 64 || ultimoCaracter >= 166) // elimina caracteres no deseados al final de la palabra
			palabra[strlen(palabra)-1] = '\0';

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
			for (int i = SIZE_MAX - 100; i < SIZE_MAX; ++i)
			{	
				lista[i] =NULL;//anula todas las entradas
			}
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
// modifique las funciones para que trabajen con colas.

void intercambio(char* palabra, Cola* c) {
	int len = strlen(palabra);
	if (len > 1) {
		char* temp = malloc(len * sizeof(char));
		for (int i = 0; i < (len - 1); i++) {
			strcpy(temp, palabra);
			strncpy(temp+i,palabra+i+1,1);
			strncpy(temp+i+1,palabra+i,1);
			cola_agregar(c,copiar_palabra(temp));
		}
		free(temp);
	}
}

void insertar(char* palabra, Cola* c, char* abc) {
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
			cola_agregar(c,copiar_palabra(temp));
		}
	}
	free(temp);	
}

void eliminar(char* palabra, Cola* c) {
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
			cola_agregar(c,copiar_palabra(temp));
			free(temp);
		}
	}
}

void reemplazar(char* palabra, Cola* c, char* abc) {
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
				cola_agregar(c,copiar_palabra(temp));	
				free(temp);
			}
		}
	}
}

void separar(char* palabra, Cola* c) 
{
	int len = strlen(palabra);
	for (int i = 1; i < len; i++)
	{
		char* temp = malloc(sizeof(char) * (len + 2));
		strncpy(temp, palabra, i);
		int k = i;
		temp[k] = ' ';
		k++;
		strcpy(temp + k, palabra + i);
		temp[len + 1] = '\0';
		cola_agregar(c,copiar_palabra(temp));
		free(temp);
	}
	
}

//~ char** corrector(char* palabra, char** altern, BSTree diccionario) {
	//~ altern = intercambio(char* palabra, char** altern, BSTree diccionario);


void buscador_opciones(Cola* c, char* abc)
{
	int inicio = c->primero;
	int fin = c->ultimo; // esto es para que solo utilice las palabras
	//con las que ingreso, de otra forma crea un bucle infinito.
	for (int i = inicio; i < fin+1; ++i)
	{
		intercambio(c->array[i], c);
		insertar(c->array[i], c, abc);
		eliminar(c->array[i], c);
		reemplazar(c->array[i], c, abc);
		separar(c->array[i], c);
		free(c->array[i]);
		cola_pop(c);
	}

}
// cuenta la cantidad de palabras bien escritas de la cola
int contador_opciones_correctas(Cola* c, TablaHash* th, FuncionIgualdad igual)
{
	int cont = 0;

	for (int i = c->primero; i < c->ultimo + 1 ; ++i)
		cont += tablahash_buscar(th, c->array[i], igual);

	return cont;
}

// esta deberia ser la ultima funcion del programa, imprime las palabras
void salida(Word** texto, TablaHash* th, char* abc)
{
	texto = filtrado_palabras(texto, th, comparador_palabras_no);	
	int i = 0;
	while(texto[i] != NULL)
	{
		printf("%s\n",texto[i]->palabra );
		i++;
	}

	Cola* queue = cola_crear(500);
	i = 0;
	int flag;
	int cont;
	while(texto[i] != NULL)
	{
		flag = 0;
		cont = 0;
		cola_agregar(queue, texto[i]->palabra);
		while(cont  < 5)
		{
			buscador_opciones(queue, abc);//pone en la cola las palabras generadas por
			//las 5 funciones que programaste vos.
			cont = contador_opciones_correctas(queue,th, comparador_palabras);
			//se encarga de decirnos cuantas de las palabras de cola estan en el diccionario.
			// a revisar si no nos conviene crear una pila alternativa que las contenga, y sie es 
			//menor a 5 liberarla.
		}

//------------------	//IMPORTANTE!!!!---------------------------
//
//		//!!!falta filtrar las palabras de la cola que no estan en el diccionario.
//		//!!! falta solucionar los caracteres especiales....!!!!
//------------------FIN---------------!!
		printf("En la linea %d, La palabra: %s no se encuentra en nuestro diccionario. Quizas quiso decir:\n",texto[i]->linea, texto[i]->palabra );

		for (int j= 0; j < queue->cant ; ++j)
		{
			printf("%s\n,",(char*)queue->array[j] );
			
		}
		printf("\n");
		queue->primero = 0;
		queue->ultimo = -1;
	}


}

int main() {
	setlocale(LC_CTYPE, "C.UTF8");
	
	char* abc = malloc(sizeof(char) * 34);
   
	strcpy(abc,"abcdefgihjkilmnopqrstuvwxyz");
	
	
	TablaHash* th = tablahash_crear(15000, RSHash);
	
	crear_diccionario("listado-general.txt", th);

	Word** texto = lectura_texto("testeo.txt");
	int i = 0;
	while(texto[i] != NULL)// solucione el problema que no encontraba la terminacion.
	{
		printf("%s\n",texto[i]->palabra );
		i++;
	}
	printf(".-----------------.\n");
	texto = filtrado_palabras(texto, th, comparador_palabras);	
	 i = 0;
	 while(texto[i] != NULL)
	{
		printf("%s\n",texto[i]->palabra );
		i++;
	}

	return 0;
}