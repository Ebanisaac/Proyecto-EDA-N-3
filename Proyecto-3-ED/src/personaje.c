#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaje.h"

// Hecho por: Carlos Cofré (Actualizado para el Proyecto 3)

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
    
    // --- Inicialización de los nuevos campos del Proyecto 3 ---
    nuevo->habitacion_actual = -1;  // Se inicializa sin habitación asignada
    nuevo->num_visitadas = 0;       // El historial comienza vacío
    for (int i = 0; i < 100; i++) {
        nuevo->habitaciones_visitadas[i] = 0;
    }
    
    return nuevo;
}

Personaje* cargar_personaje(const char* ruta_archivo) {
    FILE* archivo = fopen(ruta_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s.\n", ruta_archivo);
        return NULL;
    }
    
    char nombre[LARGO_NOMBRE_PERSONAJE];
    int vida, ataque, defensa;
    
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

    printf("%s: %d/%d PV | Ataque %d (+%d) | Defensa %d (+%d) | Habitación Actual: %d\n", 
           p->nombre, p->puntos_vida, p->vida_maxima, 
           p->ataque, p->bono_ataque, 
           p->defensa, p->bono_defensa, 
           p->habitacion_actual);
}

// --- Funciones para el control de exploración del personaje ---

void personaje_registrar_visita(Personaje* p, int id_habitacion) {
    if (p == NULL) return;

    p->habitacion_actual = id_habitacion;

    int ya_visitada = 0;
    for (int i = 0; i < p->num_visitadas; i++) {
        if (p->habitaciones_visitadas[i] == id_habitacion) {
            ya_visitada = 1;
            break;
        }
    }

    if (!ya_visitada && p->num_visitadas < 100) {
        p->habitaciones_visitadas[p->num_visitadas] = id_habitacion;
        p->num_visitadas++;
    }
}

void personaje_mostrar_visitadas(const Personaje* p) {
    if (p == NULL) return;

    printf("Habitaciones visitadas: ");
    if (p->num_visitadas == 0) {
        printf("Ninguna todavía.\n");
        return;
    }

    for (int i = 0; i < p->num_visitadas; i++) {
        printf("%d", p->habitaciones_visitadas[i]);
        if (i < p->num_visitadas - 1) {
            printf(", ");
        }
    }
    printf("\n");
}