/**
 * @file aventura.h
 * @author Daniela Soto
 * @brief creacion de las zonas
 */
#include "aventura.h"

/**
 * @brief funcion para recoger un objeto
 * 
 * @param inventario 
 * @param obj 
 * @param nombre_objeto 
 */
void recoger_objeto(Inventario* inventario, Objeto* obj, const char* nombre_objeto){
    objeto_imprimir(obj);
    char respuesta;
    printf("¿Deseas recogerlo? (s/n): \n");
    scanf(" %c", &respuesta);
    while(getchar() != '\n');
    
    if (respuesta == 's' || respuesta == 'S'){
        ResultadoInventario resu_inven = inventario_agregar(inventario, obj);
        if (resu_inven == INVENTARIO_OK){
            printf("%s agregado al inventario. \n", nombre_objeto);
        } else {
            printf("No se pudo agregar el objeto (posible duplicado o inventario lleno). \n");
        }
    } else {
        printf("Dejas el objeto en el suelo. \n");
    }
}

/**
 * @brief organiza la exploracion, recoleccion de objetos combate de la zona
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_catacumba(Inventario* inventario, Personaje* personaje){
    printf("Entras a la Catacumba... \n");
    printf("Caminas entre las paredes de pierda. No encuentras nada \n");
    printf("Entras en un pasillo oscuro. No ves nada \n");
    printf("Empiezas a tocar las paredes. ¡Encuentras un objeto! \n");
    
    Objeto antiseptico;
    objeto_inicializar(&antiseptico, 1, "antiseptico",  TIPO_POCION, 7);
    recoger_objeto(inventario, &antiseptico, "Antiseptico");

    printf("Termina el pasillo oscuro. Entras a un cuarto \n");
    printf("¡Hay un enemigo esperandote en el cuarto! \n");
    printf("Enemigos:\n");
    printf("1. Furtivo (30 PV)\n");
    printf("Siguiente enemigo: Furtivo\n");

    ColaEnemigo cola = cola_enemigo_create();
    if (cola == NULL) {
        printf("Error al crear la cola de enemigos.\n");
        return;
    }

    Enemigo furtivo = crear_enemigo_tipo1(1, "Furtivo", 30, 5, 30);
    cola_enemigo_enqueue(furtivo, cola);

    combate(personaje, cola, inventario);

    // si el personaje murio en el combate, termina la exploracion
    if (personaje->puntos_vida <= 0){
        cola_enemigo_delete(cola);
        return;
    }

    char res_exp;
    printf("¿Seguir explorando? (s/n): \n");
    scanf(" %c", &res_exp);
    while (getchar() != '\n');
    if (res_exp != 's' && res_exp != 'S'){
        printf("Decides regresar. \n");
        cola_enemigo_delete(cola);
        return;
    }
    
    printf("Entras a otro cuarto. \n");
    printf("¡Encuentras un objeto en el suelo! \n");

    Objeto machete;
    objeto_inicializar(&machete, 4, "machete",  TIPO_ARMA, 10);
    recoger_objeto(inventario, &machete, "Machete");

    printf("Avanzas con cautela. Te encuentras en un pasillo estrecho \n");
    printf("Un grupo de enemigos bloquea el paso. \n");

    Enemigo furtivo2 = crear_enemigo_tipo1(2, "Furtivo", 30, 5, 30);
    Enemigo mastin = crear_enemigo_tipo2(3, "Mastin", 38, 7, 38);

    cola_enemigo_enqueue(furtivo2, cola);
    cola_enemigo_enqueue(mastin, cola);

    combate(personaje, cola, inventario);

    // si el personaje murio en el combate, termina la exploracion
    if (personaje->puntos_vida <= 0){
        cola_enemigo_delete(cola);
        return;
    }

    cola_enemigo_delete(cola);

    printf("Sigues por el pasillo. \n");
    printf("¡Encuentras la salida de la catacumba! \n");
    printf("Felicidades :) \n");
}

/**
 * @brief organiza la exploracion, recoleccion de objetos combate de la zona
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_abismo(Inventario* inventario, Personaje* personaje){
    printf("Entras al Abismo... \n");
    printf("El suelo es rocoso y humedo. Te resbalas un poco. \n");
    printf("Avanzas por una grieta oscura. \n");
    printf("En una esquina humeda, ¡encuentras una prenda colgada! \n");
    
    Objeto impermeable;
    objeto_inicializar(&impermeable, 2, "impermeable", TIPO_ARMADURA, 5);
    recoger_objeto(inventario, &impermeable, "Impermeable");

    printf("Cruzas la grieta y llegas a una caverna amplia. \n");
    printf("¡Un par de criaturas resguardan el camino! \n");
    printf("Enemigos:\n");
    printf("1. Mastin (38 PV)\n");
    printf("2. Vandalo (45 PV)\n");

    ColaEnemigo cola = cola_enemigo_create();
    if (cola == NULL) {
        printf("Error al crear la cola de enemigos.\n");
        return;
    }

    Enemigo mastin = crear_enemigo_tipo2(2, "Mastin", 38, 7, 38);
    Enemigo vandalo = crear_enemigo_tipo3(3, "Vandalo", 45, 10, 45);

    cola_enemigo_enqueue(mastin, cola);
    cola_enemigo_enqueue(vandalo, cola);

    combate(personaje, cola, inventario);

    // si el personaje murio en el combate, termina la exploracion
    if (personaje->puntos_vida <= 0){
        cola_enemigo_delete(cola);
        return;
    }

    char res_exp;
    printf("¿Seguir explorando? (s/n): \n");
    scanf(" %c", &res_exp);
    while (getchar() != '\n');
    if (res_exp != 's' && res_exp != 'S'){
        printf("Decides regresar. \n");
        cola_enemigo_delete(cola);
        return;
    }

    printf("Avanzas mas profundo en la oscuridad. \n");
    printf("¡Encuentras un pequeño maletin en el suelo! \n");

    Objeto adrenalina;
    objeto_inicializar(&adrenalina, 5, "adrenalina", TIPO_POCION, 15);
    recoger_objeto(inventario, &adrenalina, "Adrenalina");

    printf("¡Has superado los peligros del Abismo y encuentras el ascenso! \n");
    printf("Felicidades :) \n");

    cola_enemigo_delete(cola);
}

/**
 * @brief organiza la exploracion, recoleccion de objetos combate de la zona
 * 
 * @param inventario 
 * @param personaje 
 */
void zona_paramo(Inventario* inventario, Personaje* personaje){
    printf("Entras al Paramo... \n");
    printf("Vientos helados y ceniza cubren el paisaje desolado. \n");
    printf("Entre los restos de un campamento abandonado, ¡encuentras una caja fuerte abierta! \n");
    
    Objeto escopeta;
    objeto_inicializar(&escopeta, 3, "escopeta", TIPO_ARMA, 15);
    recoger_objeto(inventario, &escopeta, "Escopeta");

    printf("A pocos metros, colgado de una estaca, ves equipo militar. \n");
    Objeto antidisturbio;
    objeto_inicializar(&antidisturbio, 6, "antidisturbio", TIPO_ARMADURA, 10);
    recoger_objeto(inventario, &antidisturbio, "Antidisturbio");

    printf("El camino principal esta bloqueado por una horda final de enemigos. \n");
    printf("Antes de avanzar, revisas una caja metalica abandonada y ¡encuentras una bengala! \n");

    Objeto bengala;
    objeto_inicializar(&bengala, 7, "bengala", TIPO_OTRO, 0);
    recoger_objeto(inventario, &bengala, "Bengala");

    printf("Enemigos:\n");
    printf("1. Furtivo (30 PV)\n");
    printf("2. Mastin (38 PV)\n");
    printf("3. Vandalo (45 PV)\n");

    ColaEnemigo cola = cola_enemigo_create();
    if (cola == NULL) {
        printf("Error al crear la cola de enemigos.\n");
        return;
    }

    Enemigo furtivo = crear_enemigo_tipo1(1, "Furtivo", 30, 5, 30);
    Enemigo mastin = crear_enemigo_tipo2(2, "Mastin", 38, 7, 38);
    Enemigo vandalo = crear_enemigo_tipo3(3, "Vandalo", 45, 10, 45);

    cola_enemigo_enqueue(furtivo, cola);
    cola_enemigo_enqueue(mastin, cola);
    cola_enemigo_enqueue(vandalo, cola);

    combate(personaje, cola, inventario);

    // si el personaje murio en el combate, termina la exploracion
    if (personaje->puntos_vida <= 0){
        cola_enemigo_delete(cola);
        return;
    }

    printf("¡Has derrotado a los jefes y conquistado el Paramo! \n");
    printf("Felicidades :) \n");

    cola_enemigo_delete(cola);
}

/**
 * @brief menu para seleccionas la zona de juego
 * 
 * @param inventario 
 * @param personaje 
 */
void iniciar_zona(Inventario* inventario, Personaje* personaje){
    if (inventario == NULL || personaje == NULL){
        printf("Personaje o inventario no inicializados. \n");
        return;
    }

    int opcion_zona;

    printf("\n========================================\n");
    printf("         ZONA DE EXPLORACION            \n");
    printf("========================================\n");
    printf("1.- Catacumba (dificultad baja) \n");
    printf("2.- Abismo (dificultad media) \n");
    printf("3.- Paramo (dificultad alta) \n");
    printf("4.- Salir de la exploración \n");
    printf("Seleccione una zona: \n");

    if (scanf("%d", &opcion_zona) != 1) {
        printf("Error. Solo debe ingresar un numero.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    switch (opcion_zona){
    case 1:
        zona_catacumba(inventario, personaje);
        break;
    case 2:
        zona_abismo(inventario, personaje);
        break;
    case 3:
        zona_paramo(inventario, personaje);
        break;
    case 4:
        printf("Volviendo al menu principal. \n");
        //Volver al main
        break;
    default:
        printf("Opcion invalida. \n");
        break;
    }

}