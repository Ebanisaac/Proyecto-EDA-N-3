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

/**
 * @brief Nodo de la lista enlazada de habitaciones secretas pendientes.
 */
typedef struct NodoSecreta {
    Habitacion *habitacion;
    int *vecinas_ids;
    int num_vecinas;
    struct NodoSecreta *siguiente;
} NodoSecreta;

/**
 * @brief Lista enlazada de habitaciones secretas aún no descubiertas.
 */
typedef struct {
    NodoSecreta *cabeza;
    int cantidad;
} ListaSecretas;

/**
 * @brief Estructura auxiliar para guardar las vecinas pendientes entre la pasada 1 y 2.
 */
typedef struct {
    int id_origen;
    int num_linea;
    int *vecinas;
    int num_vecinas;
} RegistroPasillos;

ListaSecretas* crear_lista_secretas(void);
void agregar_secreta_pendiente(ListaSecretas *lista, Habitacion *h, int *vecinas_ids, int num_vecinas);
void liberar_lista_secretas(ListaSecretas *lista);
int cargar_mazmorra(const char *ruta_archivo, NodoArbol **raiz_arbol, grafo_t *grafo, ListaSecretas *secretas, int *id_inicio);
int revelar_pasadizos_secretos(ListaSecretas *secretas, int id_actual, NodoArbol **raiz_arbol, grafo_t *grafo);

#endif