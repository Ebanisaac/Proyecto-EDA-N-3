#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interfaz.h"
#include "personaje.h"
#include "inventario.h"
#include "uso_objeto.h"
#include "guardado.h"
#include "aventura.h"
#include "historial.h"

/**
 * @brief Limpia el buffer de entrada para evitar bucles infinitos con scanf.
 */

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Solicita un número entero al usuario de forma segura.
 */

static int leer_entero(const char* mensaje) {
    int valor;
    int leidos;
    do {
        printf("%s", mensaje);
        leidos = scanf("%d", &valor);
        limpiar_buffer();
        if (leidos != 1) {
            printf("Error: Entrada inválida. Por favor, ingresa un número.\n");
        }
    } while (leidos != 1);
    return valor;
}

/**
 * @brief Solicita una cadena de texto al usuario sin espacios (para guardados y nombres).
 */

static void leer_cadena(const char* mensaje, char* buffer, int limite) {
    printf("%s", mensaje);
    scanf("%s", buffer);
    limpiar_buffer();
    buffer[limite - 1] = '\0';
}

/* ========================================================================== */
/*                              LÓGICA DEL MENÚ                               */
/* ========================================================================== */

static Personaje* menu_creacion_personaje(Inventario* inventario) {
    Personaje* p = NULL;
    int opcion;
    char nombre[50];
    char ruta[256];
    ResultadoGuardado res;

    printf("=== BIENVENIDO AL RPG ===\n");
    printf("1. Nuevo Personaje\n");
    printf("2. Cargar Partida\n");
    
    do {
        opcion = leer_entero("Elige una opción (1-2): ");
        if (opcion == 1) {
            leer_cadena("Ingresa el nombre de tu personaje (sin espacios): ", nombre, 50);
            p = crear_personaje(nombre, 100, 15, 5); // Valores base iniciales
            if (p != NULL) {
                printf("¡Personaje %s creado con éxito!\n", p->nombre);
            }
        } else if (opcion == 2) {
            leer_cadena("Ingresa el nombre del archivo a cargar (ej. partida.txt): ", ruta, 256);
            p = crear_personaje("Temp", 1, 1, 1); // Personaje temporal para sobreescribir
            res = cargar_partida(ruta, p, inventario); // Carga directa al inventario y personaje
            
            if (res == GUARDADO_OK) {
                printf("Partida cargada correctamente.\n");
            } else {
                printf("Error al cargar la partida. Código: %d\n", res);
                free(p);
                p = NULL;
            }
        } else {
            printf("Opción no válida.\n");
        }
    } while (p == NULL);

    return p;
}

void iniciar_juego(void) {
    Personaje* jugador = NULL;
    Inventario inventario;
    PilaHistorial* historial = NULL;
    int salir = 0;
    int opcion;

    // Inicialización de sistemas core
    inventario_inicializar(&inventario);
    historial = historial_crear();

    jugador = menu_creacion_personaje(&inventario);
    if (jugador == NULL) {
        printf("Error fatal al inicializar el jugador.\n");
        return;
    }

    // Bucle principal
    while (!salir && jugador->puntos_vida > 0) {
        printf("\n===================================\n");
        mostrar_estado(jugador);
        printf("===================================\n");
        printf("1. Explorar Zona\n");
        printf("2. Ver Inventario\n");
        printf("3. Usar / Equipar Objeto\n");
        printf("4. Buscar Objeto por Nombre\n");
        printf("5. Descartar Objeto\n");
        printf("6. Ver Historial de Acciones\n");
        printf("7. Deshacer última acción\n");
        printf("8. Guardar Partida\n");
        printf("9. Salir del Juego\n");
        printf("===================================\n");
        
        opcion = leer_entero("¿Qué deseas hacer?: ");

        switch (opcion) {
            case 1:
                iniciar_zona(&inventario, jugador);
                historial_registrar(historial, ACCION_EXPLORACION, "Exploró una zona", NULL);
                break;

            case 2:
                inventario_listar(&inventario);
                break;

            case 3:
                inventario_listar(&inventario);
                if (inventario.cantidad > 0) {
                    int id_obj = leer_entero("Ingresa el ID del objeto a usar (0 para cancelar): ");
                    if (id_obj != 0) {
                        Objeto* obj = inventario_buscar_por_id(&inventario, id_obj);
                        if (obj != NULL) {
                            Objeto copia_obj = *obj; // Copia para el historial antes de consumirse
                            ResultadoUso res = usar_objeto(&inventario, id_obj, jugador, 0);
                            if (res == USO_OK || res == USO_OK_ENEMIGO_DISTRAIDO) {
                                historial_registrar(historial, ACCION_USAR_OBJETO, "Se usó/equipó un objeto", &copia_obj);
                            } else {
                                printf("No se pudo usar el objeto (Código de error: %d).\n", res);
                            }
                        } else {
                            printf("Objeto no encontrado en el inventario.\n");
                        }
                    }
                }
                break;

            case 4:
            {
                char nom_buscar[50];
                leer_cadena("Ingresa el nombre del objeto a buscar: ", nom_buscar, 50);
                Objeto* obj = inventario_buscar_por_nombre(&inventario, nom_buscar);
                if (obj != NULL) {
                    printf("¡Objeto encontrado! ID: %d | Nombre: %s | Valor: %d\n", obj->id, obj->nombre, obj->valor);
                } else {
                    printf("El objeto '%s' no se encuentra en tu inventario.\n", nom_buscar);
                }
                break;
            }

            case 5:
                inventario_listar(&inventario);
                if (inventario.cantidad > 0) {
                    int id_desc = leer_entero("Ingresa el ID del objeto a descartar (0 para cancelar): ");
                    if (id_desc != 0) {
                        Objeto copia;
                        // Pasamos &copia como tercer parámetro para que la función guarde ahí el objeto eliminado
                        if (inventario_eliminar_por_id(&inventario, id_desc, &copia) == INVENTARIO_OK) {
                            printf("Descartaste: %s\n", copia.nombre);
                            historial_registrar(historial, ACCION_DESCARTAR_OBJETO, "Se descartó un objeto", &copia);
                        } else {
                            printf("Objeto no encontrado.\n");
                        }
                    }
                }
                break;

            case 6:
                historial_mostrar(historial);
                break;

            case 7:
            {
                const Accion *ult = historial_cima(historial);
                if (ult == NULL) {
                    printf("No hay acciones que deshacer.\n");
                } else if (ult->tipo != ACCION_DESCARTAR_OBJETO) {
                    printf("La última acción no se puede deshacer.\n");
                } else {
                    Objeto o = ult->objeto_involucrado;
                    if (inventario_agregar(&inventario, &o) == INVENTARIO_OK) {
                        historial_desapilar(historial);
                        printf("Se restauró %s.\n", o.nombre);
                    } else {
                        printf("No se pudo restaurar el objeto.\n");
                    }
                }
                break;
            }

            case 8:
            {
                char ruta_guardado[256];
                leer_cadena("Ingresa el nombre del archivo para guardar (ej. partida.txt): ", ruta_guardado, 256);
                if (guardar_partida(ruta_guardado, jugador, &inventario) == GUARDADO_OK) {
                    printf("¡Partida guardada con éxito!\n");
                    historial_registrar(historial, ACCION_VACIA, "Guardó la partida", NULL);
                } else {
                    printf("Ocurrió un error al guardar la partida.\n");
                }
                break;
            }
            case 9:
                printf("Saliendo del juego...\n");
                salir = 1;
                break;

            default:
                printf("Opción inválida. Intenta nuevamente.\n");
                break;
        }

        if (jugador->puntos_vida <= 0) {
            printf("\n¡Has caído en combate! Fin de la partida.\n");
        }
    }

    // Limpieza de memoria general
    printf("Limpiando memoria...\n");
    inventario_liberar(&inventario);
    historial_liberar(historial);
    if (jugador != NULL) {
        free(jugador);
    }
    printf("gracias por jugaaarrr\n");
}