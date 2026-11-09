/**
 * @file habitacion.h
 * @author Benjamin Hernandez
 * @brief Definición de la estructura Habitacion y sus funciones asociadas
 * @date 2026-11-07
 */
#ifndef HABITACION_H
#define HABITACION_H

#define MAX_NOMBRE 60
#define MAX_DESCRIPCION 200
#define MAX_TIPO 30

#include "enemigo.h"
#include "objeto.h"

typedef struct
{
    int id;
    char nombre[MAX_NOMBRE];   
    char descripcion[MAX_DESCRIPCION]; 
    char tipo[MAX_TIPO];                
    Enemigo **enemigo;
    int cant_enemigos;
    Objeto **objeto;
    int cant_objetos; 
} Habitacion;

// Prototipos de funciones
Habitacion* crear_habitacion(int id, const char *nombre, const char *desc, const char *tipo);
void agregar_enemigo(Habitacion *h, Enemigo *enemigo);
void agregar_objeto(Habitacion *h, Objeto *o);
void mostrar_habitacion(Habitacion *h);
void liberar_habitacion(Habitacion *h);

#endif