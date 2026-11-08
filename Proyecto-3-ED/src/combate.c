/**
 * @file combate.c
 * @author Daniela Soto
 * @brief logica del combate
 */
#include "combate.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief muestra por consola el inventario
 * 
 * @param inventario 
 */
void ver_inventario(const Inventario* inventario){
    printf("\n");
    inventario_listar(inventario);
}

/**
 * @brief ataque del jugador al enemigo actual y verifica si a sido derrotado
 * 
 * @param enemigo 
 * @param personaje 
 * @param cola 
 * @return int 1 derroto al enemigo, 0 no lo derroto.
 */
int ataque_jugador(Enemigo* enemigo, Personaje* personaje, ColaEnemigo cola){
    int ataque_jug = personaje->ataque + personaje->bono_ataque;
    enemigo->pv -=  ataque_jug;

    if (enemigo->pv < 0){
        enemigo->pv = 0;
    }

    printf("Atacas al %s: %d de danio | %d/%d pv \n", enemigo->nombre, ataque_jug, enemigo->pv, enemigo->vida_max);

    if (enemigo->pv == 0){
        printf("¡Derrotaste al enemigo %s! Se retira de la cola de combate \n", enemigo->nombre);
        cola_enemigo_dequeue(cola);
        return 1;
    }

    return 0;
}

/**
 * @brief ataque del enemigo actual al personaje. se calcula el daño recibido
 * teneindo en cuanta la defensa y si tiene algun bono de defennsa el personaje.
 * 
 * @param enemigo 
 * @param personaje 
 */
void ataque_enemigo(Enemigo* enemigo, Personaje* personaje){
    int defensa_jug = personaje->defensa + personaje->bono_defensa;
    int danio_rec = enemigo->ataque - defensa_jug;

    if (danio_rec <= 0){
        printf("¡Tu defensa bloqueo el ataque de %s! \n", enemigo->nombre);
        return;
    }

    if (defensa_jug == 0){
        printf("¡Estas sin defensa, recibes el golpe completo! \n");
    }

    personaje->puntos_vida -= danio_rec;
    if (personaje->puntos_vida < 0){
        personaje->puntos_vida = 0;
    }

    printf("El %s ataca: danio recibido %d | %d pv \n", enemigo->nombre, danio_rec, personaje->puntos_vida);
}

/**
 * @brief imprime en pantalla la interfaz del estado de combate
 * 
 * @param personaje 
 * @param enemigo 
 * @param cola 
 */
void estado_combate(Personaje* personaje, Enemigo* enemigo, ColaEnemigo cola){
    int enemigos = cola_enemigo_size(cola) -1;
    if (enemigos < 0){
        enemigos = 0;
    }

    printf("\n===== COMBATE =====\n");
    printf("%s: %d PV | Ataque: %d | Defensa: %d\n", personaje->nombre, personaje->puntos_vida, personaje->ataque, personaje->defensa);
    printf("Enemigo actual: %s (%d/%d PV)\n", enemigo->nombre, enemigo->pv, enemigo->vida_max);
    printf("Enemigos en espera: %d\n", enemigos);
    printf("1. Atacar\n");
    printf("2. Ver inventario\n");
    printf("3. Usar objeto\n");
}

/**
 * @brief bucle principal del combate
 * 
 * @param personaje 
 * @param cola 
 * @param inventario 
 */
void combate(Personaje* personaje, ColaEnemigo cola, Inventario* inventario){
    if (cola == NULL || inventario == NULL || personaje == NULL){
        return; // verificar que las estructuras existan
    }
    
    while (!cola_enemigo_is_empty(cola) && personaje->puntos_vida > 0){
        Enemigo* enemigo_actual = cola_enemigo_front(cola);
        if (enemigo_actual == NULL){
            break;
        }
    

        estado_combate(personaje, enemigo_actual, cola);

        int opcion;
        if (scanf("%d", &opcion) != 1){
            printf("Entrada invalida. INgrese un numero\n");
            while (getchar() != '\n'); //limpia el buffer
            continue;
        }
        while (getchar() != '\n'); //limpia el buffer

        switch (opcion){
        case 1:{
            int derrotado = ataque_jugador(enemigo_actual, personaje, cola);
            if (derrotado != 1){
                ataque_enemigo(enemigo_actual, personaje);
                if (personaje->puntos_vida == 0){
                    printf("Has sido derrotado :( \n");
                }
            }
        }
            break;
        case 2:{
            ver_inventario(inventario);
            break;
        }
        case 3:{
            int id_objeto;
            printf("Ingrese ID del objeto a usar: \n");
            if (scanf("%d", &id_objeto) != 1){
                printf("Entrada inválida.\n");
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');

            ResultadoUso objeto_uso = usar_objeto(inventario, id_objeto, personaje, 1);

            if (objeto_uso == USO_OK){
                // arma, armadura o pocion: el enemigo responde con su ataque
                if (personaje->puntos_vida > 0 && !cola_enemigo_is_empty(cola)){
                    ataque_enemigo(enemigo_actual, personaje);
                }
            }else if (objeto_uso == USO_OK_ENEMIGO_DISTRAIDO){
                // bengala: el enemigo pierde su ataque este turno
                printf("¡El %s queda desorientado y pierde su turno de ataque!\n", enemigo_actual->nombre);
                    
            } else if (objeto_uso == USO_ERROR_NO_ENCONTRADO){
                printf("No se encuentra un objeto con este ID \n");
            } else if (objeto_uso == USO_ERROR_VIDA_COMPLETA){
                    printf("Vida al maximo. No se utilizo pocion");
            }else {
                printf("Este objeto no se puede usar en combate");
            }
            
            break;
        }
        default:
            printf("Opcion invalida \n");
            break;
        }

        if (personaje->puntos_vida <= 0){
            printf("\nHas sido derrotado :( \n");
            break;
        }
    }

    if (cola_enemigo_is_empty(cola) && personaje->puntos_vida > 0){
        printf("\n¡Has derrotado a todos los enemigos. Victoria!\n");
    }
}