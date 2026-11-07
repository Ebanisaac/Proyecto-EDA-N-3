#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaje.h"

// Hecho por: Carlos Cofré

Personaje* crear_personaje(const char* nombre, int vida, int ataque, int defensa) {
    Personaje* nuevo = (Personaje*)malloc(sizeof(Personaje));

    if (nuevo == NULL) {
        fprintf(stderr, "Error: Memoria insuficiente para crear personaje.\n");
        return NULL;
    }

    strncpy(nuevo->nombre, nombre, LARGO_NOMBRE_PERSONAJE - 1);
    nuevo->nombre[LARGO_NOMBRE_PERSONAJE - 1] = '\0';

    nuevo->puntos_vida = vida;
    nuevo->vida_maxima = vida;
    nuevo->ataque = ataque;
    nuevo->bono_ataque = 0;
    nuevo->defensa = defensa;
    nuevo->bono_defensa = 0;
    
    return nuevo;

}

Personaje* cargar_personaje(const char* ruta_archivo) {
    // Implementar lectura de archivo

    FILE* archivo = fopen(ruta_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s.\n", ruta_archivo);
        return NULL;
    }
    
    char nombre[LARGO_NOMBRE_PERSONAJE];
    int vida, ataque, defensa;
    
    // asumo que el archivo de texto tendra el formato: Nombre Vida Ataque Defensa
    if (fscanf(archivo, "%49s %d %d %d", nombre, &vida, &ataque, &defensa) != 4) {
        fprintf(stderr, "Error: Formato de archivo incorrecto o corrupto.\n");
        fclose(archivo);
        return NULL;
    }
    
    fclose(archivo);
    return crear_personaje(nombre, vida, ataque, defensa);

}

void mostrar_estado(Personaje* p) {
    if (p == NULL) {
        printf("Error: Personaje no inicializado.\n");
        return;
    }

    printf("%s: %d/%d PV | Ataque %d (+%d) | Defensa %d (+%d)\n", p->nombre, p->puntos_vida, p->vida_maxima, p->ataque, p->bono_ataque, p->defensa, p->bono_defensa);
}