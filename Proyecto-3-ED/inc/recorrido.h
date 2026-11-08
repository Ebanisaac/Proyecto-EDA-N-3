#ifndef RECORRIDO_H
#define RECORRIDO_H

// Declaración adelantada de la estructura Mapa (definida en el grafo de Pablo)
typedef struct Mapa Mapa;

/**
 * @brief Búsqueda en Anchura (BFS) para encontrar la ruta más corta entre dos habitaciones (Opción 4).
 * @param mapa Puntero al grafo de la mazmorra.
 * @param origen ID de la habitación de partida.
 * @param destino ID de la habitación de destino.
 * @return 1 si se encontró ruta e imprime el camino, 0 si el destino no es alcanzable.
 */
int recorrido_bfs_ruta_corta(Mapa *mapa, int origen, int destino);

/**
 * @brief Búsqueda en Profundidad (DFS) para explorar y listar las habitaciones alcanzables (Opción 5).
 * @param mapa Puntero al grafo de la mazmorra.
 * @param inicio ID de la habitación actual desde donde se explora.
 */
void recorrido_dfs_alcanzables(Mapa *mapa, int inicio);

/**
 * @brief Calcula y muestra las habitaciones que se encuentran exactamente a 'k' pasillos de distancia (Opción 7).
 * @param mapa Puntero al grafo de la mazmorra.
 * @param origen ID de la habitación actual.
 * @param k Número exacto de pasillos.
 */
void recorrido_mostrar_k_pasillos(Mapa *mapa, int origen, int k);

#endif /* RECORRIDO_H */