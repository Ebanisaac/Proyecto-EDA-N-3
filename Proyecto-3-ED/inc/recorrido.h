#ifndef RECORRIDO_H
#define RECORRIDO_H

#include "grafo.h"

/**
 * @brief Encuentra y muestra la ruta más corta (BFS) entre dos habitaciones.
 * @return 1 si encontró ruta, 0 si no existe.
 */
int recorrido_bfs_ruta_corta(grafo_t *grafo, int origen, int destino);

/**
 * @brief Explora mediante DFS todas las habitaciones alcanzables desde una inicial.
 */
void recorrido_dfs_alcanzables(grafo_t *grafo, int inicio);

/**
 * @brief Muestra todas las habitaciones que se encuentran a exactamente k pasillos de distancia.
 */
void recorrido_mostrar_k_pasillos(grafo_t *grafo, int origen, int k);

#endif // RECORRIDO_H