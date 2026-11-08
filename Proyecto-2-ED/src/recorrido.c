#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recorrido.h"

// Nota: Asegúrate de incluir el archivo de cabecera del grafo de Pablo 
// para conocer la estructura exacta de Mapa, Vertice y NodoAdyacencia.
// #include "grafo.h" 

// --- Estructuras internas para simular el grafo si aún no tienes el include definitivo ---
// (Estas coinciden con el diseño estándar de listas de adyacencia del proyecto)
typedef struct NodoAdyacencia {
    int id_destino;
    struct NodoAdyacencia *siguiente;
} NodoAdyacencia;

typedef struct Vertice {
    int id;
    int visitado; 
    NodoAdyacencia *lista_pasillos; 
    struct Vertice *siguiente;      
} Vertice;

struct Mapa {
    Vertice *cabeza_vertice; 
    int num_habitaciones;    
};
// ---------------------------------------------------------------------------------------

/**
 * @brief Función auxiliar interna para buscar un vértice en el mapa por su ID.
 */
static Vertice* buscar_vertice(Mapa *mapa, int id) {
    if (mapa == NULL) return NULL;
    Vertice *actual = mapa->cabeza_vertice; // Asumiendo Vertice*
    while (actual != NULL) {
        if (actual->id == id) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

/**
 * @brief Función auxiliar para resetear los flags de 'visitado' en todos los vértices del mapa.
 */
static void reiniciar_visitados(Mapa *mapa) {
    if (mapa == NULL) return;
    Vertice *actual = mapa->cabeza_vertice;
    while (actual != NULL) {
        actual->visitado = 0;
        actual = actual->siguiente;
    }
}

// ==========================================
// 1. BÚSQUEDA EN ANCHURA (BFS) - Ruta más corta (Opción 4)
// ==========================================
int recorrido_bfs_ruta_corta(Mapa *mapa, int origen, int destino) {
    if (mapa == NULL) return 0;

    reiniciar_visitados(mapa);

    // Verificamos que origen y destino existan
    Vertice *v_origen = buscar_vertice(mapa, origen);
    Vertice *v_destino = buscar_vertice(mapa, destino);
    if (v_origen == NULL || v_destino == NULL) {
        printf("Error: La habitación de origen o destino no existe en el mapa.\n");
        return 0;
    }

    // Implementación de una cola simple basada en arreglo para BFS
    int cola[200];
    int inicio_cola = 0, fin_cola = 0;

    // Arreglo para reconstruir la ruta guardando el "padre" de cada nodo
    // (Guardamos el ID previo para trazar el camino de vuelta)
    int padre[1000]; 
    for (int i = 0; i < 1000; i++) padre[i] = -1;

    // Encolar origen
    cola[fin_cola++] = origen;
    v_origen->visitado = 1;

    int encontrado = 0;

    while (inicio_cola < fin_cola) {
        int actual_id = cola[inicio_cola++];

        if (actual_id == destino) {
            encontrado = 1;
            break;
        }

        Vertice *v_actual = buscar_vertice(mapa, actual_id);
        if (v_actual != NULL) {
            NodoAdyacencia *pasillo = v_actual->lista_pasillos;
            while (pasillo != NULL) {
                Vertice *v_vecino = buscar_vertice(mapa, pasillo->id_destino);
                // Si el vecino existe y no ha sido visitado
                if (v_vecino != NULL && !v_vecino->visitado) {
                    v_vecino->visitado = 1;
                    padre[pasillo->id_destino] = actual_id;
                    cola[fin_cola++] = pasillo->id_destino;
                }
                pasillo = pasillo->siguiente;
            }
        }
    }

    if (!encontrado) {
        printf("No existe una ruta desde la habitación %d hasta la habitación %d (mapa desconectado).\n", origen, destino);
        return 0;
    }

    // Reconstruir la ruta desde el destino hacia el origen usando una pila temporal o arreglo invertido
    int ruta[200];
    int tam_ruta = 0;
    int curr = destino;

    while (curr != -1) {
        ruta[tam_ruta++] = curr;
        curr = padre[curr];
    }

    // Imprimir la ruta en orden correcto (de origen a destino)
    printf("Ruta encontrada: ");
    for (int i = tam_ruta - 1; i >= 0; i--) {
        printf("%d", ruta[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\nCantidad de pasillos: %d\n", tam_ruta - 1);

    return 1;
}

// ==========================================
// 2. BÚSQUEDA EN PROFUNDIDAD (DFS) - Habitaciones alcanzables (Opción 5)
// ==========================================
static void dfs_recursivo(Mapa *mapa, int id_actual) {
    Vertice *v = buscar_vertice(mapa, id_actual);
    if (v == NULL) return;

    v->visitado = 1;
    printf("%d ", v->id);

    NodoAdyacencia *pasillo = v->lista_pasillos;
    while (pasillo != NULL) {
        Vertice *v_vecino = buscar_vertice(mapa, pasillo->id_destino);
        if (v_vecino != NULL && !v_vecino->visitado) {
            dfs_recursivo(mapa, v_vecino->id);
        }
        pasillo = pasillo->siguiente;
    }
}

void recorrido_dfs_alcanzables(Mapa *mapa, int inicio) {
    if (mapa == NULL) return;

    reiniciar_visitados(mapa);
    printf("Habitaciones alcanzables desde la habitación %d: ", inicio);
    dfs_recursivo(mapa, inicio);
    printf("\n");
}

// ==========================================
// 3. HABITACIONES A K PASILLOS (Opción 7)
// ==========================================
void recorrido_mostrar_k_pasillos(Mapa *mapa, int origen, int k) {
    if (mapa == NULL || k < 0) return;

    reiniciar_visitados(mapa);

    // Usaremos una cola de BFS pero registrando la distancia (niveles) de cada nodo
    int cola[200];
    int distancia[1000]; // Mapeo de distancia por ID
    for(int i=0; i<1000; i++) distancia[i] = -1;

    int inicio_cola = 0, fin_cola = 0;

    Vertice *v_origen = buscar_vertice(mapa, origen);
    if (v_origen == NULL) return;

    cola[fin_cola++] = origen;
    v_origen->visitado = 1;
    distancia[origen] = 0;

    printf("Habitaciones a %d pasillos de la habitación %d: ", k, origen);
    int encontradas = 0;

    while (inicio_cola < fin_cola) {
        int actual_id = cola[inicio_cola++];
        int d_actual = distancia[actual_id];

        if (d_actual == k) {
            printf("%d ", actual_id);
            encontradas = 1;
        }

        // Si ya superamos la distancia k, no necesitamos seguir explorando profundidad extra
        if (d_actual < k) {
            Vertice *v_actual = buscar_vertice(mapa, actual_id);
            if (v_actual != NULL) {
                NodoAdyacencia *pasillo = v_actual->lista_pasillos;
                while (pasillo != NULL) {
                    Vertice *v_vecino = buscar_vertice(mapa, pasillo->id_destino);
                    if (v_vecino != NULL && !v_vecino->visitado) {
                        v_vecino->visitado = 1;
                        distancia[pasillo->id_destino] = d_actual + 1;
                        cola[fin_cola++] = pasillo->id_destino;
                    }
                    pasillo = pasillo->siguiente;
                }
            }
        }
    }

    if (!encontradas) {
        printf("Ninguna.");
    }
    printf("\n");
}