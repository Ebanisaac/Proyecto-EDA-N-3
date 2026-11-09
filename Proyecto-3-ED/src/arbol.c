/**
 * @file arbol.c
 * @author Benjamin Hernandez
 * @brief Implementación completa del ABB de habitaciones
 * @date 2026-11-07
 */
#include <stdio.h>
#include <stdlib.h>
#include "arbol.h"

/**
 * @brief 
 * 
 * @param hab 
 * @return NodoArbol* 
 */
NodoArbol* crear_nodo(Habitacion *hab){
    if(hab == NULL){
        return NULL;
    }
    NodoArbol *nuevo = malloc(sizeof(NodoArbol));
    if(nuevo == NULL){
        printf("Error: No se pudo asignar memoria para el nodo del arbol.\n");
        return NULL;
    }
    nuevo->habitacion = hab;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

/**
 * @brief 
 * 
 * @param raiz 
 * @param hab 
 * @return NodoArbol* 
 */
NodoArbol* insertar_nodo(NodoArbol *raiz, Habitacion *hab){
    if(hab == NULL){
        return raiz;
    }
    if(raiz == NULL){
        return crear_nodo(hab);
    }
    if(hab->id < raiz->habitacion->id){
        raiz->izq = insertar_nodo(raiz->izq, hab);
    }else if(hab->id > raiz->habitacion->id){
        raiz->der = insertar_nodo(raiz->der, hab);
    }else{
        printf("Advertencia: La habitacion con ID %d ya existe en el arbol.\n", hab->id);
    }
    return raiz;
}

/**
 * @brief 
 * 
 * @param raiz 
 * @param id 
 * @return Habitacion* 
 */
Habitacion* buscar_nodo(NodoArbol *raiz, int id){
    if(raiz == NULL){
        return NULL;
    }
    if(id == raiz->habitacion->id){
        return raiz->habitacion;
    }
    if(id < raiz->habitacion->id){
        return buscar_nodo(raiz->izq, id);
    }else{
        return buscar_nodo(raiz->der, id);
    }
}

/**
 * @brief 
 * 
 * @param nodo 
 * @return NodoArbol* 
 */
NodoArbol* obtener_minimo(NodoArbol *nodo){
    NodoArbol *actual = nodo;
    while(actual && actual->izq != NULL){
        actual = actual->izq;
    }
    return actual;
}

/**
 * @brief 
 * 
 * @param raiz 
 * @param id 
 * @return NodoArbol* 
 */
NodoArbol* eliminar_nodo(NodoArbol *raiz, int id){
    if(raiz == NULL){
        return NULL;
    }

    if(id < raiz->habitacion->id){
        raiz->izq = eliminar_nodo(raiz->izq, id);
    }else if (id > raiz->habitacion->id){
        raiz->der = eliminar_nodo(raiz->der, id);
    }else{
        if(raiz->izq == NULL){
            NodoArbol *temp = raiz->der;
            liberar_habitacion(raiz->habitacion);
            free(raiz);
            return temp;
        }else if (raiz->der == NULL) {
            NodoArbol *temp = raiz->izq;
            liberar_habitacion(raiz->habitacion);
            free(raiz);
            return temp;
        }
        NodoArbol *temp = obtener_minimo(raiz->der);
        liberar_habitacion(raiz->habitacion);
        raiz->habitacion = temp->habitacion;
        raiz->der = eliminar_nodo(raiz->der, temp->habitacion->id);
    }
    return raiz;
}

/**
 * @brief 
 * 
 * @param raiz 
 */
void recorrer_inorden(NodoArbol *raiz) {
    if(raiz == NULL){
        return;
    }
    recorrer_inorden(raiz->izq);
    mostrar_habitacion(raiz->habitacion); 
    recorrer_inorden(raiz->der);
}

/**
 * @brief 
 * 
 * @param raiz 
 */
void recorrer_preorden(NodoArbol *raiz){
    if(raiz == NULL){
        return;
    }
    mostrar_habitacion(raiz->habitacion);
    recorrer_preorden(raiz->izq);
    recorrer_preorden(raiz->der);
}

/**
 * @brief 
 * 
 * @param raiz 
 */
void recorrer_postorden(NodoArbol *raiz){
    if(raiz == NULL){
        return;
    }
    recorrer_postorden(raiz->izq);
    recorrer_postorden(raiz->der);
    mostrar_habitacion(raiz->habitacion);
}

/**
 * @brief 
 * 
 * @param raiz 
 */
void liberar_arbol(NodoArbol *raiz){
    if(raiz == NULL){
        return;
    }
    liberar_arbol(raiz->izq);
    liberar_arbol(raiz->der);
    liberar_habitacion(raiz->habitacion);
    free(raiz);
}