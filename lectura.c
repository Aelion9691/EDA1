#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablahash.h"
#include <locale.h>


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

int comparador_palabras(void* dato1, void* dato2)
{

	char* palabra1 = (char*) dato1;
	char* palabra2 = (char*) dato2;

	return palabra1 == palabra2;
}


void creacion_abb(char* nombreArchivo, TablaHash* th)
{
	
	char* palabra = NULL;
	FILE *fp = fopen(nombreArchivo, "r");
	size_t j = 0;
	size_t final;
	int contador = 0;

	while(!(feof(fp)))
	{		
		final = getline(&palabra, &j, fp);
		palabra[final-1] = '\0';
		contador += tablahash_insertar(th, palabra, palabra, comparador_palabras);

		
	}	
	free(palabra);
	fclose(fp);
	printf("%d\n", contador);
	
}


int main()
{
	TablaHash* th = tablahash_crear(121129, RSHash, comparador_palabras);
	creacion_abb("listado-general.txt", th);	



	return 0;
}