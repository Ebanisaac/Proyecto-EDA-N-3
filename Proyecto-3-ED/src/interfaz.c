/**
 * @file interfaz.c
 * @author Carlos Cofré
 * @brief Menú principal y flujo de exploración de la mazmorra (Proyecto 3)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interfaz.h"
#include "personaje.h"
#include "habitacion.h"
#include "arbol.h"
#include "grafo.h"
#include "lector.h"
#include "recorrido.h"

/**
 * @brief Limpia los caracteres pendientes en el buffer de entrada estándar.
 */
static void limpiar_buffer(void){
    int c;
    while ((c = getchar())!= '\n' && c != EOF);
}

/**
 * @brief Solicita un número entero al usuario validando que la entrada sea numérica.
 *
 * @param mensaje Texto que se muestra al pedir el dato.
 * @return Número entero ingresado por el usuario.
 */
static int leer_entero(const char *mensaje){
    int valor;
    int leidos;
    do {
        printf("%s", mensaje);
        leidos = scanf("%d", &valor);
        limpiar_buffer();
        if (leidos != 1){
            printf("Error: Entrada inválida. Por favor, ingresa un número.\n");
        }
    } while (leidos != 1);
    return valor;
}

/**
 * @brief Muestra la habitación actual y todas sus habitaciones vecinas conectadas.
 *
 * @param grafo Puntero al grafo de la mazmorra.
 * @param raiz_arbol Raíz del ABB para consultar los nombres de cada habitación.
 * @param id_actual ID de la habitación actual del personaje.
 */
static void mostrar_vecinas(const grafo_t *grafo, NodoArbol *raiz_arbol, int id_actual){
    int cant = 0;
    int *vecinos = grafo_copiar_vecinos(grafo, id_actual, &cant);
    Habitacion *actual = buscar_nodo(raiz_arbol, id_actual);

    printf("Habitación actual: %s (ID %d)\n", actual ? actual->nombre : "Desconocida", id_actual);
    printf("Habitaciones vecinas:\n");
    if (vecinos == NULL || cant == 0){
        printf("  - No hay habitaciones vecinas conectadas.\n");
        free(vecinos);
        return;
    }

    for (int i = 0; i < cant; i++){
        Habitacion *v = buscar_nodo(raiz_arbol, vecinos[i]);
        if (v != NULL){
            printf("  - %s (ID %d)\n", v->nombre, v->id);
        } else {
            printf("  - Habitación ID %d\n", vecinos[i]);
        }
    }
    free(vecinos);
}

/**
 * @brief Traslada al personaje a una habitación de destino, registra la visita y gestiona enemigos.
 *
 * @param raiz_arbol Raíz del ABB para obtener los datos de la habitación.
 * @param jugador Puntero al personaje que explora la mazmorra.
 * @param destino ID de la habitación a la que entra el personaje.
 */
static void entrar_habitacion(NodoArbol *raiz_arbol, Personaje *jugador, int destino){
    Habitacion *hab = buscar_nodo(raiz_arbol, destino);
    if (hab == NULL){
        printf("Error: No se encontró la información de la habitación %d.\n", destino);
        return;
    }

    personaje_registrar_visita(jugador, destino);
    printf("Ahora estás en: %s (ID %d)\n", hab->nombre, hab->id);

    if (hab->cant_enemigos > 0 && hab->enemigo != NULL && hab->enemigo[0] != NULL){
        printf("¡Un %s te bloquea el paso!\n", hab->enemigo[0]->nombre);
        int accion;
        do {
            accion = leer_entero("1. Combatir  2. Escapar\n> ");
        } while (accion != 1 && accion != 2);

        if (accion == 1){
            printf("¡Enfrentas a %s y logras abrirte paso!\n", hab->enemigo[0]->nombre);
        } else {
            printf("Esquivas a %s sin combatir.\n", hab->enemigo[0]->nombre);
        }
    }
}

/**
 * @brief Valida y ejecuta el movimiento hacia una habitación vecina conectada.
 *
 * @param grafo Puntero al grafo de la mazmorra.
 * @param raiz_arbol Raíz del ABB de habitaciones.
 * @param jugador Puntero al personaje actual.
 */
static void moverse_habitacion(const grafo_t *grafo, NodoArbol *raiz_arbol, Personaje *jugador){
    int destino = leer_entero("Ingresa el ID de la habitación vecina: ");
    if (!grafo_son_vecinos(grafo, jugador->habitacion_actual, destino)){
        printf("No puedes moverte allí: la habitación %d no es vecina de tu posición actual.\n", destino);
        return;
    }
    entrar_habitacion(raiz_arbol, jugador, destino);
}

/**
 * @brief Quema una habitación vecina eliminándola tanto del grafo como del ABB.
 *
 * @param grafo Puntero al grafo de la mazmorra.
 * @param raiz_arbol Doble puntero a la raíz del ABB.
 * @param jugador Puntero al personaje actual.
 */
static void quemar_habitacion(grafo_t *grafo, NodoArbol **raiz_arbol, const Personaje *jugador){
    int id_quemar = leer_entero("Ingresa el ID de la habitación vecina a quemar: ");

    if (id_quemar == jugador->habitacion_actual){
        printf("No puedes quemar la habitación en la que te encuentras.\n");
        return;
    }

    if (!grafo_son_vecinos(grafo, jugador->habitacion_actual, id_quemar)){
        printf("Solo puedes quemar habitaciones vecinas de tu posición actual.\n");
        return;
    }

    Habitacion *hab = buscar_nodo(*raiz_arbol, id_quemar);
    if (hab == NULL){
        printf("La habitación indicada no existe.\n");
        return;
    }

    if (strcmp(hab->tipo, "INICIO")== 0){
        printf("No se puede quemar la habitación de tipo INICIO.\n");
        return;
    }

    char nombre_quemada[MAX_NOMBRE];
    strncpy(nombre_quemada, hab->nombre, MAX_NOMBRE - 1);
    nombre_quemada[MAX_NOMBRE - 1] = '\0';

    int cant_vec = 0;
    int *vecinos = grafo_copiar_vecinos(grafo, id_quemar, &cant_vec);

    grafo_eliminar_vertice(grafo, id_quemar);
    *raiz_arbol = eliminar_nodo(*raiz_arbol, id_quemar);

    printf("%s (ID %d)fue reducida a cenizas.\n", nombre_quemada, id_quemar);
    if (vecinos != NULL && cant_vec > 0){
        printf("Pasillos destruidos: ");
        for (int i = 0; i < cant_vec; i++){
            printf("%d <-> %d", id_quemar, vecinos[i]);
            if (i < cant_vec - 1)printf(", ");
        }
        printf("\n");
    }
    free(vecinos);
}

void iniciar_juego(void){
    grafo_t grafo;
    if (grafo_inicializar(&grafo)!= GRAFO_OK){
        printf("Error fatal al inicializar el grafo.\n");
        return;
    }

    NodoArbol *raiz_arbol = NULL;
    ListaSecretas *secretas = crear_lista_secretas();
    int id_inicio = -1;

    if (!cargar_mazmorra("mazmorra.txt", &raiz_arbol, &grafo, secretas, &id_inicio)){
        liberar_lista_secretas(secretas);
        liberar_arbol(raiz_arbol);
        grafo_liberar(&grafo);
        return;
    }

    Personaje *jugador = crear_personaje("Aventurero", 100, 15, 5);
    if (jugador == NULL){
        liberar_lista_secretas(secretas);
        liberar_arbol(raiz_arbol);
        grafo_liberar(&grafo);
        return;
    }
    personaje_registrar_visita(jugador, id_inicio);

    int salir = 0;
    while (!salir){
        Habitacion *actual = buscar_nodo(raiz_arbol, jugador->habitacion_actual);
        printf("\n===== MENÚ PRINCIPAL =====\n");
        printf("Habitación actual: %s (ID %d)\n",
               actual ? actual->nombre : "Desconocida", jugador->habitacion_actual);
        printf("1. Consultar habitación\n");
        printf("2. Mostrar habitaciones vecinas\n");
        printf("3. Moverse a una habitación vecina\n");
        printf("4. Buscar ruta hacia otra habitación\n");
        printf("5. Explorar habitaciones alcanzables\n");
        printf("6. Mostrar todas las habitaciones\n");
        printf("7. Mostrar habitaciones a k pasillos\n");
        printf("8. Buscar pasadizos secretos\n");
        printf("9. Quemar una habitación\n");
        printf("10. Mostrar habitaciones visitadas\n");
        printf("11. Salir\n");

        int opcion = leer_entero("> ");
        switch (opcion){
            case 1: {
                int id = leer_entero("Ingresa el ID de la habitación: ");
                Habitacion *h = buscar_nodo(raiz_arbol, id);
                if (h != NULL){
                    mostrar_habitacion(h);
                } else {
                    printf("La habitación con ID %d no existe o aún no ha sido descubierta.\n", id);
                }
                break;
            }
            case 2:
                mostrar_vecinas(&grafo, raiz_arbol, jugador->habitacion_actual);
                break;
            case 3:
                moverse_habitacion(&grafo, raiz_arbol, jugador);
                break;
            case 4: {
                int destino = leer_entero("Ingresa el ID de la habitación de destino: ");
                if (recorrido_bfs_ruta_corta(&grafo, jugador->habitacion_actual, destino)){
                    char resp;
                    printf("¿Deseas recorrer esta ruta? (S/N): ");
                    scanf(" %c", &resp);
                    limpiar_buffer();
                    if (resp == 'S' || resp == 's'){
                        entrar_habitacion(raiz_arbol, jugador, destino);
                    }
                }
                break;
            }
            case 5:
                recorrido_dfs_alcanzables(&grafo, jugador->habitacion_actual);
                break;
            case 6:
                recorrer_inorden(raiz_arbol);
                break;
            case 7: {
                int k = leer_entero("Ingresa k: ");
                recorrido_mostrar_k_pasillos(&grafo, jugador->habitacion_actual, k);
                break;
            }
            case 8:
                revelar_pasadizos_secretos(secretas, jugador->habitacion_actual, &raiz_arbol, &grafo);
                break;
            case 9:
                quemar_habitacion(&grafo, &raiz_arbol, jugador);
                break;
            case 10:
                personaje_mostrar_visitadas(jugador);
                break;
            case 11:
                printf("Saliendo de la mazmorra...\n");
                salir = 1;
                break;
            default:
                printf("Opción inválida.\n");
                break;
        }
    }

    liberar_lista_secretas(secretas);
    liberar_arbol(raiz_arbol);
    grafo_liberar(&grafo);
    free(jugador);
}