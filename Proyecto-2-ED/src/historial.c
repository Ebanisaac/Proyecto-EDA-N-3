//Autor: Carlos Cofré

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historial.h"

PilaHistorial *historial_crear(void) {
    PilaHistorial *pila = malloc(sizeof(PilaHistorial));
    if (pila == NULL) {
        fprintf(stderr, "Error: Memoria insuficiente al crear el historial.\n");
        return NULL;
    }
    pila->cima = NULL;
    return pila;
}

void historial_registrar(PilaHistorial *historial, TipoAccion tipo, const char *descripcion, const Objeto *obj) {
    NodoHistorial *nuevo_nodo;

    if (historial == NULL || descripcion == NULL) {
        return;
    }

    nuevo_nodo = malloc(sizeof(NodoHistorial));
    if (nuevo_nodo == NULL) {
        fprintf(stderr, "Error: Memoria insuficiente para registrar la acción.\n");
        return;
    }

    nuevo_nodo->accion.tipo = tipo;
    snprintf(nuevo_nodo->accion.descripcion, LARGO_DESC_ACCION, "%s", descripcion);

    if (obj != NULL) {
        nuevo_nodo->accion.objeto_involucrado = *obj;
    } else {
        nuevo_nodo->accion.objeto_involucrado.id = 0;
        nuevo_nodo->accion.objeto_involucrado.nombre[0] = '\0';
        nuevo_nodo->accion.objeto_involucrado.tipo = TIPO_OTRO;
        nuevo_nodo->accion.objeto_involucrado.valor = 0;
    }

    nuevo_nodo->siguiente = historial->cima;
    historial->cima = nuevo_nodo;
}

Accion historial_desapilar(PilaHistorial *historial) {
    Accion accion_extraida;
    NodoHistorial *nodo_a_eliminar;

    if (historial == NULL || historial->cima == NULL) {
        accion_extraida.tipo = ACCION_VACIA;
        accion_extraida.descripcion[0] = '\0';
        accion_extraida.objeto_involucrado.id = 0;
        return accion_extraida;
    }

    nodo_a_eliminar = historial->cima;
    accion_extraida = nodo_a_eliminar->accion;

    historial->cima = nodo_a_eliminar->siguiente;
    free(nodo_a_eliminar);

    return accion_extraida;
}

void historial_mostrar(const PilaHistorial *historial) {
    NodoHistorial *actual;

    if (historial == NULL || historial->cima == NULL) {
        printf("El historial está vacío.\n");
        return;
    }

    printf("\n=== HISTORIAL DE ACCIONES ===\n");
    actual = historial->cima;
    while (actual != NULL) {
        printf("- %s\n", actual->accion.descripcion);
        actual = actual->siguiente;
    }
    printf("=============================\n");
}

void historial_liberar(PilaHistorial *historial) {
    NodoHistorial *actual;
    NodoHistorial *siguiente;

    if (historial == NULL) {
        return;
    }

    actual = historial->cima;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    free(historial);
}

int historial_esta_vacio(const PilaHistorial *h) {
    return (h == NULL || h->cima == NULL);
}

const Accion *historial_cima(const PilaHistorial *h) {
    if (historial_esta_vacio(h)) {
        return NULL;
    }
    return &(h->cima->accion);
}