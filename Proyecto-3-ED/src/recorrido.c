#include <stdio.h>
#include <stdlib.h>
#include "recorrido.h"
#include "grafo.h"

// ==========================================
// 1. BÚSQUEDA EN ANCHURA (BFS) - Ruta más corta
// ==========================================
int recorrido_bfs_ruta_corta(grafo_t *grafo, int origen, int destino) {
    if (grafo == NULL) return 0;

    if (!grafo_existe_vertice(grafo, origen) || !grafo_existe_vertice(grafo, destino)) {
        printf("Error: La habitación de origen o destino no existe en el mapa.\n");
        return 0;
    }

    // Estructuras auxiliares para BFS
    int cola[200];
    int inicio_cola = 0, fin_cola = 0;

    // Arreglo para rastrear el nodo padre de cada habitación y reconstruir el camino
    int padre[1000];
    int visitado[1000];
    for (int i = 0; i < 1000; i++) {
        padre[i] = -1;
        visitado[i] = 0;
    }

    // Encolar origen
    cola[fin_cola++] = origen;
    visitado[origen] = 1;

    int encontrado = 0;

    while (inicio_cola < fin_cola) {
        int actual_id = cola[inicio_cola++];

        if (actual_id == destino) {
            encontrado = 1;
            break;
        }

        // Obtener los vecinos usando la función oficial de grafo.h
        int num_vecinos = 0;
        int *vecinos = grafo_copiar_vecinos(grafo, actual_id, &num_vecinos);

        if (vecinos != NULL) {
            for (int i = 0; i < num_vecinos; i++) {
                int vecino_id = vecinos[i];
                if (!visitado[vecino_id]) {
                    visitado[vecino_id] = 1;
                    padre[vecino_id] = actual_id;
                    cola[fin_cola++] = vecino_id;
                }
            }
            free(vecinos); // ¡Importante liberar la memoria asignada por el grafo!
        }
    }

    if (!encontrado) {
        printf("No existe una ruta desde la habitación %d hasta la habitación %d.\n", origen, destino);
        return 0;
    }

    // Reconstruir la ruta desde el destino hacia el origen
    int ruta[200];
    int tam_ruta = 0;
    int curr = destino;

    while (curr != -1) {
        ruta[tam_ruta++] = curr;
        curr = padre[curr];
    }

    // Imprimir resultado en orden correcto
    printf("Ruta más corta: ");
    for (int i = tam_ruta - 1; i >= 0; i--) {
        printf("%d", ruta[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\nCantidad de pasillos: %d\n", tam_ruta - 1);

    return 1;
}

// ==========================================
// 2. BÚSQUEDA EN PROFUNDIDAD (DFS) - Habitaciones alcanzables
// ==========================================
static void dfs_recursivo(grafo_t *grafo, int id_actual, int visitado[]) {
    visitado[id_actual] = 1;
    printf("%d ", id_actual);

    int num_vecinos = 0;
    int *vecinos = grafo_copiar_vecinos(grafo, id_actual, &num_vecinos);

    if (vecinos != NULL) {
        for (int i = 0; i < num_vecinos; i++) {
            int vecino_id = vecinos[i];
            if (!visitado[vecino_id]) {
                dfs_recursivo(grafo, vecino_id, visitado);
            }
        }
        free(vecinos);
    }
}

void recorrido_dfs_alcanzables(grafo_t *grafo, int inicio) {
    if (grafo == NULL || !grafo_existe_vertice(grafo, inicio)) return;

    int visitado[1000];
    for (int i = 0; i < 1000; i++) visitado[i] = 0;

    printf("Habitaciones alcanzables desde la habitación %d: ", inicio);
    dfs_recursivo(grafo, inicio, visitado);
    printf("\n");
}

// ==========================================
// 3. HABITACIONES A K PASILLOS
// ==========================================
void recorrido_mostrar_k_pasillos(grafo_t *grafo, int origen, int k) {
    if (grafo == NULL || k < 0 || !grafo_existe_vertice(grafo, origen)) return;

    int cola[200];
    int distancia[1000];
    int visitado[1000];
    
    for (int i = 0; i < 1000; i++) {
        distancia[i] = -1;
        visitado[i] = 0;
    }

    int inicio_cola = 0, fin_cola = 0;

    cola[fin_cola++] = origen;
    visitado[origen] = 1;
    distancia[origen] = 0;

    printf("Habitaciones a exactamente %d pasillos de la habitación %d: ", k, origen);
    int encontradas = 0;

    while (inicio_cola < fin_cola) {
        int actual_id = cola[inicio_cola++];
        int d_actual = distancia[actual_id];

        if (d_actual == k) {
            printf("%d ", actual_id);
            encontradas = 1;
        }

        if (d_actual < k) {
            int num_vecinos = 0;
            int *vecinos = grafo_copiar_vecinos(grafo, actual_id, &num_vecinos);

            if (vecinos != NULL) {
                for (int i = 0; i < num_vecinos; i++) {
                    int vecino_id = vecinos[i];
                    if (!visitado[vecino_id]) {
                        visitado[vecino_id] = 1;
                        distancia[vecino_id] = d_actual + 1;
                        cola[fin_cola++] = vecino_id;
                    }
                }
                free(vecinos);
            }
        }
    }

    if (!encontradas) {
        printf("Ninguna.");
    }
    printf("\n");
}