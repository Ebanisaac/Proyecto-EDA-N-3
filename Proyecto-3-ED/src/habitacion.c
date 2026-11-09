/**
 * @file habitacion.c
 * @author Benjamin Hernandez
 * @brief Implementación de las funciones de gestión de Habitacion
 * @date 2026-11-07
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "habitacion.h"

// Inicialización básica al crear la habitación
/**
 * @brief 
 * 
 * @param id 
 * @param nombre 
 * @param desc 
 * @param tipo 
 * @return Habitacion* 
 */
Habitacion* crear_habitacion(int id, const char *nombre, const char *desc, const char *tipo) {
    Habitacion *h = malloc(sizeof(Habitacion));
    if(h == NULL){
        return NULL;
    }
    h->id = id;
    strncpy(h->nombre, nombre, MAX_NOMBRE - 1);
    h->nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(h->descripcion, desc, MAX_DESCRIPCION - 1);
    h->descripcion[MAX_DESCRIPCION - 1] = '\0';
    strncpy(h->tipo, tipo, MAX_TIPO - 1);
    h->tipo[MAX_TIPO - 1] = '\0';
    h->enemigo = NULL;
    h->cant_enemigos = 0;
    h->objeto = NULL;
    h->cant_objetos = 0;
    return h;
}

/**
 * @brief 
 * 
 * @param h 
 * @param enemigo 
 */
// Agregar un enemigo dinámicamente
void agregar_enemigo(Habitacion *h, Enemigo *enemigo){
    if(h == NULL || enemigo == NULL){
        return;
    }
    Enemigo **temp = realloc(h->enemigo, sizeof(Enemigo*) * (h->cant_enemigos + 1));
    if(temp == NULL){
        return;
    }

    h->enemigo = temp;
    h->enemigo[h->cant_enemigos] = enemigo;
    h->cant_enemigos++;
}

/**
 * @brief 
 * 
 * @param h 
 * @param o 
 */
// Agregar un objeto dinámicamente
void agregar_objeto(Habitacion *h, Objeto *o){
    if(h == NULL || o == NULL){
        return;
    }
    Objeto **temp = realloc(h->objeto, sizeof(Objeto*) * (h->cant_objetos + 1));
    if(temp == NULL){
        return;
    }

    h->objeto = temp;
    h->objeto[h->cant_objetos] = o;
    h->cant_objetos++;
}

/**
 * @brief 
 * 
 * @param h 
 */
// Muestra la información completa de la habitación
void mostrar_habitacion(Habitacion *h) {
    if(h == NULL){
        return;
    }

    printf("=== HABITACIÓN [%d]: %s ===\n", h->id, h->nombre);
    printf("Tipo: %s\n", h->tipo);
    printf("Descripción: %s\n", h->descripcion);

    // Iteración e impresión de Objetos
    printf("Objetos (%d):\n", h->cant_objetos);
    if(h->cant_objetos == 0 || h->objeto == NULL){
        printf("  - Ninguno\n");
    }else{
        for(int i = 0; i < h->cant_objetos; i++){
            if(h->objeto[i] != NULL){
                printf("  - %s (Valor: %d)\n", h->objeto[i]->nombre, h->objeto[i]->valor);
            }
        }
    }

    // Iteración e impresión de Enemigos
    printf("Enemigos (%d):\n", h->cant_enemigos);
    if(h->cant_enemigos == 0 || h->enemigo == NULL){
        printf("  - Ninguno\n");
    }else{
        for(int i = 0; i < h->cant_enemigos; i++){
            if(h->enemigo[i] != NULL){
                printf("  - %s (Vida: %d | Ataque: %d)\n",h->enemigo[i]->nombre, h->enemigo[i]->pv, h->enemigo[i]->ataque);
            }
        }
    }
    printf("-----------------------------------\n");
}

/**
 * @brief 
 * 
 * @param h 
 */
// Liberación limpia de la memoria asignada a la habitación
void liberar_habitacion(Habitacion *h){
    if(h == NULL){
        return;
    }
    if(h->objeto != NULL){
        for(int i = 0; i < h->cant_objetos; i++){
            if(h->objeto[i] != NULL){
                free(h->objeto[i]);
            }
        }
        free(h->objeto);
    }

    if(h->enemigo != NULL){
        for(int i = 0; i < h->cant_enemigos; i++){
            if(h->enemigo[i] != NULL){
                free(h->enemigo[i]);
            }
        }
        free(h->enemigo);
    }
    free(h);
}