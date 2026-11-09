/**
 * @file arbol.h
 * @author Benjamin Hernandez
 * @brief Declaración de funciones y estructura para el ABB de Habitaciones
 * @date 2026-11-07
 */
#ifndef ARBOL_H
#define ARBOL_H

#include <stdio.h>
#include <stdlib.h>
#include "habitacion.h"

/**
 * @brief  Estructura de cada nodo del ABB
 */
typedef struct NodoArbol {
    Habitacion *habitacion;   
    struct NodoArbol *izq;    
    struct NodoArbol *der;
} NodoArbol;

NodoArbol* crear_nodo(Habitacion *hab);
NodoArbol* insertar_nodo(NodoArbol *raiz, Habitacion *hab);
Habitacion* buscar_nodo(NodoArbol *raiz, int id);
NodoArbol* obtener_minimo(NodoArbol *nodo);
NodoArbol* eliminar_nodo(NodoArbol *raiz, int id);
void recorrer_inorden(NodoArbol *raiz);
void recorrer_preorden(NodoArbol *raiz);
void recorrer_postorden(NodoArbol *raiz);
void liberar_arbol(NodoArbol *raiz);

#endif