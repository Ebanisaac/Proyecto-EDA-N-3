/**
 * @file lector.h
 * @author Benjamin Hernandez
 * @brief Declaración de funciones para la lectura y parsing de mazmorra.txt
 * @date 2026-11-07
 */
#ifndef LECTOR_H
#define LECTOR_H

#include "arbol.h"
#include "grafo.h"
#include "habitacion.h"

typedef struct{
    Habitacion *habitacion;
    struct NodoSecreta *siguiente;
} NodoSecreta;

typedef struct{
    NodoSecreta *cabeza;
    int cantidad;
} ListaSecretas;

ListaSecretas* crear_lista_secretas();
void agregar_secreta_pendiente(ListaSecretas *lista, Habitacion *h);
void liberar_lista_secretas(ListaSecretas *lista);
int cargar_mazmorra(const char *ruta_archivo, NodoArbol **raiz_arbol, grafo_t *grafo, ListaSecretas *secretas);

#endif