/*
 * Archivo: test_pablo.c
 * Autor: Pablo Serón
 * Descripción: Pruebas de los módulos de Pablo: objeto, inventario
 *              (lista enlazada con centinela), uso_objeto y guardado.
 *              Se enfoca en los casos límite: estructura vacía, un solo
 *              elemento, eliminación (primero, medio, último), entradas
 *              inválidas y archivos de guardado corruptos.
 */

#include <stdio.h>
#include <string.h>
#include "objeto.h"
#include "inventario.h"
#include "personaje.h"
#include "uso_objeto.h"
#include "guardado.h"

// Archivo temporal para las pruebas de guardado (se borra al terminar)
#define RUTA_PRUEBA "prueba_guardado_tmp.txt"

// Contadores del resumen final. Son privados de este archivo de pruebas
static int pruebas_ejecutadas = 0;
static int pruebas_fallidas = 0;

/* ========================================================================== */
/*                          FUNCIONES AUXILIARES                              */
/* ========================================================================== */

/**
 * @brief Registra el resultado de una prueba. Si la condición es falsa, la
 *        marca como fallida e imprime su descripción.
 *
 * @param condicion Resultado de la comprobación (distinto de 0 = correcto).
 * @param descripcion Qué se estaba comprobando.
 */
static void verificar(int condicion, const char *descripcion)
{
    pruebas_ejecutadas++;
    if (!condicion)
    {
        pruebas_fallidas++;
        printf("  [FALLO] %s\n", descripcion);
    }
}

/**
 * @brief Rellena un personaje en la pila (sin usar personaje.c).
 */
static void preparar_personaje(Personaje *p, const char *nombre, int vida,
                               int ataque, int defensa)
{
    snprintf(p->nombre, LARGO_NOMBRE_PERSONAJE, "%s", nombre);
    p->puntos_vida = vida;
    p->vida_maxima = vida;
    p->ataque = ataque;
    p->bono_ataque = 0;
    p->defensa = defensa;
    p->bono_defensa = 0;
}

/**
 * @brief Compara todos los campos de dos personajes.
 *
 * @return 1 si son iguales, 0 si alguno difiere.
 */
static int personajes_iguales(const Personaje *a, const Personaje *b)
{
    return strcmp(a->nombre, b->nombre) == 0 &&
           a->puntos_vida == b->puntos_vida &&
           a->vida_maxima == b->vida_maxima &&
           a->ataque == b->ataque &&
           a->bono_ataque == b->bono_ataque &&
           a->defensa == b->defensa &&
           a->bono_defensa == b->bono_defensa;
}

/**
 * @brief Crea un objeto y lo agrega al inventario (atajo para preparar datos).
 *
 * @return 1 si se agregó, 0 si algo falló.
 */
static int agregar_objeto(Inventario *inventario, int id, const char *nombre,
                          TipoObjeto tipo, int valor)
{
    Objeto objeto;

    if (objeto_inicializar(&objeto, id, nombre, tipo, valor) != 0)
    {
        return 0;
    }
    return inventario_agregar(inventario, &objeto) == INVENTARIO_OK;
}

/**
 * @brief Escribe un texto completo en un archivo (para armar archivos de
 *        guardado a mano, incluso corruptos).
 *
 * @return 1 si se escribió, 0 si no se pudo abrir el archivo.
 */
static int escribir_archivo(const char *ruta, const char *contenido)
{
    FILE *archivo = fopen(ruta, "w");

    if (archivo == NULL)
    {
        return 0;
    }
    fputs(contenido, archivo);
    fclose(archivo);
    return 1;
}

/**
 * @brief Indica si un archivo existe y se puede abrir.
 */
static int archivo_existe(const char *ruta)
{
    FILE *archivo = fopen(ruta, "r");

    if (archivo == NULL)
    {
        return 0;
    }
    fclose(archivo);
    return 1;
}

/* ========================================================================== */
/*                               MÓDULO OBJETO                                */
/* ========================================================================== */

static void probar_objeto(void)
{
    Objeto objeto;
    char nombre_largo[100];
    int i;

    printf("\n--- Objeto ---\n");

    // Caso válido
    verificar(objeto_inicializar(&objeto, 3, "Espada corta", TIPO_ARMA, 5) == 0,
              "un objeto valido se inicializa");
    verificar(objeto.id == 3 && objeto.tipo == TIPO_ARMA && objeto.valor == 5 &&
              strcmp(objeto.nombre, "Espada corta") == 0,
              "los campos del objeto quedan correctos");

    // Parámetros inválidos: la función devuelve -1
    verificar(objeto_inicializar(NULL, 1, "X", TIPO_OTRO, 0) == -1,
              "objeto NULL rechazado");
    verificar(objeto_inicializar(&objeto, 1, NULL, TIPO_OTRO, 0) == -1,
              "nombre NULL rechazado");
    verificar(objeto_inicializar(&objeto, 1, "", TIPO_OTRO, 0) == -1,
              "nombre vacio rechazado");
    verificar(objeto_inicializar(&objeto, 0, "X", TIPO_OTRO, 0) == -1,
              "id 0 rechazado");
    verificar(objeto_inicializar(&objeto, -4, "X", TIPO_OTRO, 0) == -1,
              "id negativo rechazado");
    verificar(objeto_inicializar(&objeto, 1, "X", (TipoObjeto)99, 0) == -1,
              "tipo de objeto invalido rechazado");

    // Las pociones deben curar algo; los demás tipos pueden valer 0
    verificar(objeto_inicializar(&objeto, 1, "Pocion", TIPO_POCION, 0) == -1,
              "pocion con valor 0 rechazada");
    verificar(objeto_inicializar(&objeto, 1, "Pocion", TIPO_POCION, -5) == -1,
              "pocion con valor negativo rechazada");
    verificar(objeto_inicializar(&objeto, 1, "Bengala", TIPO_OTRO, 0) == 0,
              "objeto de tipo otro con valor 0 permitido");

    // Un nombre más largo que el arreglo se trunca sin desbordarlo
    for (i = 0; i < 99; i++)
    {
        nombre_largo[i] = 'a';
    }
    nombre_largo[99] = '\0';
    verificar(objeto_inicializar(&objeto, 1, nombre_largo, TIPO_OTRO, 0) == 0 &&
              strlen(objeto.nombre) == LARGO_NOMBRE_OBJETO - 1,
              "un nombre muy largo se trunca");

    // Conversión de tipo a texto
    verificar(strcmp(objeto_tipo_a_texto(TIPO_ARMA), "arma") == 0, "texto de arma");
    verificar(strcmp(objeto_tipo_a_texto(TIPO_ARMADURA), "armadura") == 0, "texto de armadura");
    verificar(strcmp(objeto_tipo_a_texto(TIPO_POCION), "poción") == 0, "texto de pocion");
    verificar(strcmp(objeto_tipo_a_texto(TIPO_OTRO), "otro") == 0, "texto de otro");
    verificar(strcmp(objeto_tipo_a_texto((TipoObjeto)99), "desconocido") == 0,
              "texto de un tipo invalido");

    // Imprimir un objeto NULL no debe caerse
    objeto_imprimir(NULL);
    verificar(1, "objeto_imprimir(NULL) no se cae");
}

/* ========================================================================== */
/*                          MÓDULO INVENTARIO (LISTA)                         */
/* ========================================================================== */

static void probar_inventario_vacio(void)
{
    Inventario inventario;
    Inventario sin_inicializar = {NULL, 0};
    Objeto objeto;
    Objeto eliminado;

    printf("\n--- Inventario: vacio y sin inicializar ---\n");

    verificar(inventario_inicializar(NULL) == INVENTARIO_ERROR_PARAMETRO,
              "inicializar(NULL) devuelve error de parametro");
    verificar(inventario_inicializar(&inventario) == INVENTARIO_OK,
              "inicializar un inventario");

    // Inventario recién creado: solo existe el centinela
    verificar(inventario.cantidad == 0, "inventario nuevo: cantidad 0");
    verificar(inventario.cabeza != NULL, "inventario nuevo: el centinela existe");
    verificar(inventario.cabeza->siguiente == NULL,
              "inventario nuevo: el centinela no apunta a nada");

    // Operaciones sobre la lista vacía
    verificar(inventario_buscar_por_id(&inventario, 1) == NULL,
              "buscar por id en vacio devuelve NULL");
    verificar(inventario_buscar_por_nombre(&inventario, "Espada") == NULL,
              "buscar por nombre en vacio devuelve NULL");
    verificar(inventario_eliminar_por_id(&inventario, 1, &eliminado) ==
              INVENTARIO_ERROR_NO_ENCONTRADO,
              "eliminar por id en vacio: no encontrado");
    verificar(inventario_eliminar_por_nombre(&inventario, "Espada", &eliminado) ==
              INVENTARIO_ERROR_NO_ENCONTRADO,
              "eliminar por nombre en vacio: no encontrado");
    inventario_listar(&inventario);
    verificar(inventario.cantidad == 0, "las operaciones en vacio no cambian la cantidad");

    inventario_liberar(&inventario);

    // Inventario que nunca se inicializó (cabeza NULL): ninguna función debe caerse
    objeto_inicializar(&objeto, 1, "Espada", TIPO_ARMA, 5);
    verificar(inventario_agregar(&sin_inicializar, &objeto) == INVENTARIO_ERROR_PARAMETRO,
              "agregar en inventario sin inicializar: error de parametro");
    verificar(inventario_buscar_por_id(&sin_inicializar, 1) == NULL,
              "buscar por id en inventario sin inicializar: NULL");
    verificar(inventario_buscar_por_nombre(&sin_inicializar, "Espada") == NULL,
              "buscar por nombre en inventario sin inicializar: NULL");
    verificar(inventario_eliminar_por_id(&sin_inicializar, 1, NULL) ==
              INVENTARIO_ERROR_PARAMETRO,
              "eliminar por id en inventario sin inicializar: error de parametro");
    verificar(inventario_eliminar_por_nombre(&sin_inicializar, "Espada", NULL) ==
              INVENTARIO_ERROR_PARAMETRO,
              "eliminar por nombre en inventario sin inicializar: error de parametro");
    inventario_listar(&sin_inicializar);
    inventario_listar(NULL);
    inventario_liberar(&sin_inicializar);
    inventario_liberar(NULL);
    verificar(1, "listar y liberar con inventarios nulos o sin inicializar no se caen");
}

static void probar_inventario_un_elemento(void)
{
    Inventario inventario;
    Objeto espada;
    Objeto eliminado;

    printf("\n--- Inventario: un solo elemento ---\n");

    inventario_inicializar(&inventario);
    objeto_inicializar(&espada, 3, "Espada_corta", TIPO_ARMA, 5);

    verificar(inventario_agregar(&inventario, &espada) == INVENTARIO_OK,
              "agregar el primer objeto");
    verificar(inventario.cantidad == 1, "cantidad 1 despues de agregar");
    verificar(inventario.cabeza->siguiente != NULL &&
              inventario.cabeza->siguiente->siguiente == NULL,
              "la lista tiene exactamente un nodo real");
    verificar(inventario_buscar_por_id(&inventario, 3) != NULL,
              "buscar por id el unico objeto");
    inventario_listar(&inventario);

    // Eliminar el único elemento deja la lista vacía
    verificar(inventario_eliminar_por_id(&inventario, 3, &eliminado) == INVENTARIO_OK,
              "eliminar el unico objeto");
    verificar(eliminado.id == 3 && strcmp(eliminado.nombre, "Espada_corta") == 0,
              "la copia del objeto eliminado es correcta");
    verificar(inventario.cantidad == 0 && inventario.cabeza->siguiente == NULL,
              "tras eliminar el unico objeto la lista queda vacia");

    // La lista sigue siendo usable después de quedar vacía
    verificar(inventario_agregar(&inventario, &espada) == INVENTARIO_OK &&
              inventario.cantidad == 1,
              "se puede volver a agregar despues de vaciar");

    inventario_liberar(&inventario);
}

static void probar_inventario_agregar_y_buscar(void)
{
    Inventario inventario;
    Objeto repetido;
    Objeto *encontrado;

    printf("\n--- Inventario: agregar y buscar ---\n");

    inventario_inicializar(&inventario);
    agregar_objeto(&inventario, 3, "Espada corta", TIPO_ARMA, 5);
    agregar_objeto(&inventario, 7, "Pocion menor", TIPO_POCION, 20);
    agregar_objeto(&inventario, 9, "Escudo", TIPO_ARMADURA, 3);

    verificar(inventario.cantidad == 3, "tres objetos agregados");

    // Los objetos quedan en el orden en que se agregaron (al final)
    verificar(inventario.cabeza->siguiente->objeto.id == 3 &&
              inventario.cabeza->siguiente->siguiente->objeto.id == 7 &&
              inventario.cabeza->siguiente->siguiente->siguiente->objeto.id == 9,
              "orden de insercion: 3, 7, 9");

    // ID duplicado: se rechaza y el inventario no cambia
    objeto_inicializar(&repetido, 7, "Otra cosa", TIPO_OTRO, 0);
    verificar(inventario_agregar(&inventario, &repetido) == INVENTARIO_ERROR_ID_DUPLICADO,
              "id duplicado rechazado");
    verificar(inventario.cantidad == 3, "un duplicado no cambia la cantidad");
    verificar(strcmp(inventario_buscar_por_id(&inventario, 7)->nombre, "Pocion menor") == 0,
              "un duplicado no modifica el objeto original");

    verificar(inventario_agregar(&inventario, NULL) == INVENTARIO_ERROR_PARAMETRO,
              "agregar un objeto NULL: error de parametro");
    verificar(inventario_agregar(NULL, &repetido) == INVENTARIO_ERROR_PARAMETRO,
              "agregar a un inventario NULL: error de parametro");

    // Búsqueda por id
    encontrado = inventario_buscar_por_id(&inventario, 9);
    verificar(encontrado != NULL && strcmp(encontrado->nombre, "Escudo") == 0,
              "buscar por id existente");
    verificar(inventario_buscar_por_id(&inventario, 99) == NULL,
              "buscar por id inexistente: NULL");
    verificar(inventario_buscar_por_id(NULL, 3) == NULL,
              "buscar por id en inventario NULL: NULL");

    // Búsqueda por nombre: exacta y distingue mayúsculas
    encontrado = inventario_buscar_por_nombre(&inventario, "Espada corta");
    verificar(encontrado != NULL && encontrado->id == 3,
              "buscar por nombre exacto");
    verificar(inventario_buscar_por_nombre(&inventario, "espada corta") == NULL,
              "la busqueda por nombre distingue mayusculas");
    verificar(inventario_buscar_por_nombre(&inventario, "Espada") == NULL,
              "un nombre parcial no coincide");
    verificar(inventario_buscar_por_nombre(&inventario, NULL) == NULL,
              "buscar un nombre NULL: NULL");

    // buscar devuelve un puntero DENTRO de la lista: modificarlo modifica el inventario
    encontrado = inventario_buscar_por_id(&inventario, 3);
    encontrado->valor = 50;
    verificar(inventario_buscar_por_id(&inventario, 3)->valor == 50,
              "buscar devuelve un puntero al objeto dentro de la lista");

    inventario_liberar(&inventario);
}

static void probar_inventario_eliminar(void)
{
    Inventario inventario;
    Objeto eliminado;

    printf("\n--- Inventario: eliminar ---\n");

    inventario_inicializar(&inventario);
    agregar_objeto(&inventario, 1, "A", TIPO_OTRO, 0);
    agregar_objeto(&inventario, 2, "B", TIPO_OTRO, 0);
    agregar_objeto(&inventario, 3, "C", TIPO_OTRO, 0);
    agregar_objeto(&inventario, 4, "D", TIPO_OTRO, 0);

    // Del medio, pidiendo la copia
    verificar(inventario_eliminar_por_id(&inventario, 2, &eliminado) == INVENTARIO_OK,
              "eliminar por id un objeto del medio");
    verificar(eliminado.id == 2 && strcmp(eliminado.nombre, "B") == 0,
              "copia del objeto eliminado (medio)");
    verificar(inventario.cantidad == 3 && inventario_buscar_por_id(&inventario, 2) == NULL,
              "el objeto del medio ya no esta");
    verificar(inventario.cabeza->siguiente->siguiente->objeto.id == 3,
              "la lista se reconecto (1 -> 3)");

    // El primero, sin pedir copia
    verificar(inventario_eliminar_por_nombre(&inventario, "A", NULL) == INVENTARIO_OK,
              "eliminar por nombre el primero");
    verificar(inventario.cabeza->siguiente->objeto.id == 3,
              "tras eliminar el primero, el centinela apunta al siguiente");

    // El último; después agregar debe seguir funcionando
    verificar(inventario_eliminar_por_id(&inventario, 4, NULL) == INVENTARIO_OK,
              "eliminar por id el ultimo");
    verificar(agregar_objeto(&inventario, 5, "E", TIPO_OTRO, 0) &&
              inventario.cabeza->siguiente->siguiente->objeto.id == 5,
              "agregar despues de eliminar el ultimo funciona");

    // Casos de error: el inventario no cambia
    verificar(inventario.cantidad == 2, "cantidad 2 antes de los casos de error");
    verificar(inventario_eliminar_por_id(&inventario, 99, NULL) ==
              INVENTARIO_ERROR_NO_ENCONTRADO,
              "eliminar un id inexistente");
    verificar(inventario_eliminar_por_nombre(&inventario, "Z", NULL) ==
              INVENTARIO_ERROR_NO_ENCONTRADO,
              "eliminar un nombre inexistente");
    verificar(inventario_eliminar_por_nombre(&inventario, "c", NULL) ==
              INVENTARIO_ERROR_NO_ENCONTRADO,
              "eliminar por nombre distingue mayusculas");
    verificar(inventario_eliminar_por_nombre(&inventario, NULL, NULL) ==
              INVENTARIO_ERROR_PARAMETRO,
              "eliminar un nombre NULL: error de parametro");
    verificar(inventario_eliminar_por_id(NULL, 1, NULL) == INVENTARIO_ERROR_PARAMETRO,
              "eliminar de un inventario NULL: error de parametro");
    verificar(inventario.cantidad == 2, "los errores no cambian la cantidad");

    inventario_liberar(&inventario);
}

static void probar_inventario_liberar(void)
{
    Inventario inventario;

    printf("\n--- Inventario: liberar ---\n");

    inventario_inicializar(&inventario);
    agregar_objeto(&inventario, 1, "A", TIPO_OTRO, 0);
    agregar_objeto(&inventario, 2, "B", TIPO_OTRO, 0);

    inventario_liberar(&inventario);
    verificar(inventario.cabeza == NULL && inventario.cantidad == 0,
              "tras liberar el inventario queda sin inicializar");

    // Liberar dos veces no debe hacer doble free
    inventario_liberar(&inventario);
    verificar(inventario.cabeza == NULL, "liberar dos veces es seguro");

    // Se puede reutilizar inicializándolo de nuevo
    verificar(inventario_inicializar(&inventario) == INVENTARIO_OK &&
              agregar_objeto(&inventario, 1, "A", TIPO_OTRO, 0) &&
              inventario.cantidad == 1,
              "un inventario liberado se puede inicializar y usar de nuevo");
    inventario_liberar(&inventario);
}

/* ========================================================================== */
/*                             MÓDULO USO_OBJETO                              */
/* ========================================================================== */

/**
 * @brief Arma un inventario con dos armas, una armadura, dos pociones y una
 *        bengala para las pruebas de uso.
 */
static void preparar_inventario_de_uso(Inventario *inventario)
{
    inventario_inicializar(inventario);
    agregar_objeto(inventario, 1, "Espada", TIPO_ARMA, 5);
    agregar_objeto(inventario, 2, "Hacha", TIPO_ARMA, 9);
    agregar_objeto(inventario, 3, "Escudo", TIPO_ARMADURA, 3);
    agregar_objeto(inventario, 4, "Pocion_grande", TIPO_POCION, 20);
    agregar_objeto(inventario, 5, "Pocion_chica", TIPO_POCION, 10);
    agregar_objeto(inventario, 6, "Bengala", TIPO_OTRO, 0);
}

static void probar_uso_parametros_y_errores(void)
{
    Inventario inventario;
    Personaje personaje;
    Personaje copia;

    printf("\n--- Uso de objetos: parametros y errores ---\n");

    preparar_inventario_de_uso(&inventario);
    preparar_personaje(&personaje, "Aria", 30, 8, 2);
    copia = personaje;

    verificar(usar_objeto(NULL, 1, &personaje, 0) == USO_ERROR_PARAMETRO,
              "usar con inventario NULL");
    verificar(usar_objeto(&inventario, 1, NULL, 0) == USO_ERROR_PARAMETRO,
              "usar con personaje NULL");
    verificar(usar_objeto(&inventario, 99, &personaje, 0) == USO_ERROR_NO_ENCONTRADO,
              "usar un id inexistente");
    verificar(personajes_iguales(&personaje, &copia),
              "los errores no modifican al personaje");
    verificar(inventario.cantidad == 6, "los errores no modifican el inventario");

    inventario_liberar(&inventario);
}

static void probar_uso_equipar(void)
{
    Inventario inventario;
    Personaje personaje;

    printf("\n--- Uso de objetos: equipar ---\n");

    preparar_inventario_de_uso(&inventario);
    preparar_personaje(&personaje, "Aria", 30, 8, 2);

    // Arma: asigna el bono (no lo suma)
    verificar(usar_objeto(&inventario, 1, &personaje, 0) == USO_OK,
              "equipar un arma");
    verificar(personaje.bono_ataque == 5 && personaje.ataque == 8,
              "el bono de ataque es el valor del arma y el ataque base no cambia");

    // La misma arma dos veces no acumula
    usar_objeto(&inventario, 1, &personaje, 0);
    verificar(personaje.bono_ataque == 5, "equipar la misma arma dos veces no acumula");

    // Cambiar de arma reemplaza el bono
    usar_objeto(&inventario, 2, &personaje, 0);
    verificar(personaje.bono_ataque == 9, "cambiar de arma reemplaza el bono");

    // Equipar no saca los objetos del inventario
    verificar(inventario.cantidad == 6 &&
              inventario_buscar_por_id(&inventario, 1) != NULL &&
              inventario_buscar_por_id(&inventario, 2) != NULL,
              "las armas equipadas siguen en el inventario");

    // Armadura
    verificar(usar_objeto(&inventario, 3, &personaje, 0) == USO_OK,
              "equipar una armadura");
    verificar(personaje.bono_defensa == 3 && personaje.defensa == 2,
              "el bono de defensa es el valor de la armadura");
    usar_objeto(&inventario, 3, &personaje, 0);
    verificar(personaje.bono_defensa == 3, "equipar la misma armadura dos veces no acumula");
    verificar(inventario_buscar_por_id(&inventario, 3) != NULL,
              "la armadura equipada sigue en el inventario");

    inventario_liberar(&inventario);
}

static void probar_uso_pociones(void)
{
    Inventario inventario;
    Personaje personaje;

    printf("\n--- Uso de objetos: pociones ---\n");

    preparar_inventario_de_uso(&inventario);
    preparar_personaje(&personaje, "Aria", 30, 8, 2);

    // La cura se recorta en la vida máxima: 22 + 20 -> 30
    personaje.puntos_vida = 22;
    verificar(usar_objeto(&inventario, 4, &personaje, 0) == USO_OK,
              "usar una pocion con la vida incompleta");
    verificar(personaje.puntos_vida == 30, "la cura no pasa de la vida maxima");
    verificar(inventario_buscar_por_id(&inventario, 4) == NULL && inventario.cantidad == 5,
              "la pocion consumida se elimina del inventario");

    // Con la vida completa la poción no se consume
    verificar(usar_objeto(&inventario, 5, &personaje, 0) == USO_ERROR_VIDA_COMPLETA,
              "usar una pocion con la vida completa");
    verificar(inventario_buscar_por_id(&inventario, 5) != NULL && inventario.cantidad == 5,
              "la pocion no se consume con la vida completa");
    verificar(personaje.puntos_vida == 30, "la vida no cambia");

    // Cura completa sin llegar al máximo: 5 + 10 = 15
    personaje.puntos_vida = 5;
    verificar(usar_objeto(&inventario, 5, &personaje, 0) == USO_OK &&
              personaje.puntos_vida == 15,
              "una cura que no llega al maximo suma el valor completo");
    verificar(inventario_buscar_por_id(&inventario, 5) == NULL,
              "la segunda pocion tambien se elimina");

    // Una poción ya consumida no existe
    verificar(usar_objeto(&inventario, 4, &personaje, 0) == USO_ERROR_NO_ENCONTRADO,
              "usar una pocion ya consumida");

    inventario_liberar(&inventario);
}

static void probar_uso_bengala(void)
{
    Inventario inventario;
    Personaje personaje;
    Personaje copia;

    printf("\n--- Uso de objetos: bengala (tipo otro) ---\n");

    preparar_inventario_de_uso(&inventario);
    preparar_personaje(&personaje, "Aria", 30, 8, 2);
    copia = personaje;

    // Fuera de combate no se puede usar y no se consume
    verificar(usar_objeto(&inventario, 6, &personaje, 0) == USO_ERROR_NO_USABLE,
              "la bengala no se puede usar fuera de combate");
    verificar(inventario_buscar_por_id(&inventario, 6) != NULL,
              "la bengala sigue en el inventario");

    // En combate se consume y distrae al enemigo
    verificar(usar_objeto(&inventario, 6, &personaje, 1) == USO_OK_ENEMIGO_DISTRAIDO,
              "la bengala se usa en combate");
    verificar(inventario_buscar_por_id(&inventario, 6) == NULL && inventario.cantidad == 5,
              "la bengala se elimina del inventario");
    verificar(personajes_iguales(&personaje, &copia),
              "la bengala no modifica al personaje");

    // Ya no existe
    verificar(usar_objeto(&inventario, 6, &personaje, 1) == USO_ERROR_NO_ENCONTRADO,
              "usar una bengala ya consumida");

    inventario_liberar(&inventario);
}

/* ========================================================================== */
/*                              MÓDULO GUARDADO                               */
/* ========================================================================== */

static void probar_guardado_ida_y_vuelta(void)
{
    Personaje original;
    Personaje cargado;
    Inventario inventario_original;
    Inventario inventario_cargado;
    NodoInventario *nodo;

    printf("\n--- Guardado: guardar y cargar ---\n");
    remove(RUTA_PRUEBA);

    preparar_personaje(&original, "Aria", 30, 8, 2);
    original.puntos_vida = 22;
    original.bono_ataque = 5;
    original.bono_defensa = 3;

    inventario_inicializar(&inventario_original);
    agregar_objeto(&inventario_original, 3, "Espada_corta", TIPO_ARMA, 5);
    agregar_objeto(&inventario_original, 7, "Pocion_menor", TIPO_POCION, 20);
    agregar_objeto(&inventario_original, 9, "Bengala", TIPO_OTRO, 0);

    verificar(guardar_partida(RUTA_PRUEBA, &original, &inventario_original) == GUARDADO_OK,
              "guardar una partida");
    verificar(archivo_existe(RUTA_PRUEBA), "el archivo de guardado existe");
    verificar(!archivo_existe(RUTA_PRUEBA ".tmp"),
              "no queda el archivo temporal despues de guardar");

    // Cargar en estructuras distintas y comparar
    preparar_personaje(&cargado, "Temporal", 1, 1, 1);
    inventario_inicializar(&inventario_cargado);
    verificar(cargar_partida(RUTA_PRUEBA, &cargado, &inventario_cargado) == GUARDADO_OK,
              "cargar la partida guardada");
    verificar(personajes_iguales(&original, &cargado),
              "el personaje cargado es igual al guardado");
    verificar(inventario_cargado.cantidad == 3, "se cargaron los 3 objetos");

    // Mismos objetos y en el mismo orden
    nodo = inventario_cargado.cabeza->siguiente;
    verificar(nodo != NULL && nodo->objeto.id == 3 && nodo->objeto.tipo == TIPO_ARMA &&
              nodo->objeto.valor == 5 && strcmp(nodo->objeto.nombre, "Espada_corta") == 0,
              "primer objeto cargado correcto");
    nodo = nodo != NULL ? nodo->siguiente : NULL;
    verificar(nodo != NULL && nodo->objeto.id == 7 && nodo->objeto.tipo == TIPO_POCION &&
              nodo->objeto.valor == 20 && strcmp(nodo->objeto.nombre, "Pocion_menor") == 0,
              "segundo objeto cargado correcto");
    nodo = nodo != NULL ? nodo->siguiente : NULL;
    verificar(nodo != NULL && nodo->objeto.id == 9 && nodo->objeto.tipo == TIPO_OTRO &&
              nodo->objeto.valor == 0 && strcmp(nodo->objeto.nombre, "Bengala") == 0,
              "tercer objeto cargado correcto");
    verificar(nodo != NULL && nodo->siguiente == NULL, "no sobran objetos al final");

    inventario_liberar(&inventario_original);
    inventario_liberar(&inventario_cargado);
    remove(RUTA_PRUEBA);
}

static void probar_guardado_inventario_vacio(void)
{
    Personaje original;
    Personaje cargado;
    Inventario inventario_original;
    Inventario inventario_cargado;

    printf("\n--- Guardado: inventario vacio ---\n");
    remove(RUTA_PRUEBA);

    preparar_personaje(&original, "Aria", 30, 8, 2);
    inventario_inicializar(&inventario_original);

    verificar(guardar_partida(RUTA_PRUEBA, &original, &inventario_original) == GUARDADO_OK,
              "guardar con el inventario vacio");

    preparar_personaje(&cargado, "Temporal", 1, 1, 1);
    inventario_inicializar(&inventario_cargado);
    verificar(cargar_partida(RUTA_PRUEBA, &cargado, &inventario_cargado) == GUARDADO_OK,
              "cargar un inventario vacio");
    verificar(personajes_iguales(&original, &cargado), "personaje correcto");
    verificar(inventario_cargado.cantidad == 0 && inventario_cargado.cabeza != NULL &&
              inventario_cargado.cabeza->siguiente == NULL,
              "el inventario cargado queda vacio y utilizable");

    inventario_liberar(&inventario_original);
    inventario_liberar(&inventario_cargado);
    remove(RUTA_PRUEBA);
}

static void probar_guardado_cargar_reemplaza(void)
{
    Personaje original;
    Personaje destino;
    Inventario inventario_original;
    Inventario inventario_destino;

    printf("\n--- Guardado: cargar reemplaza lo anterior ---\n");
    remove(RUTA_PRUEBA);

    preparar_personaje(&original, "Aria", 30, 8, 2);
    inventario_inicializar(&inventario_original);
    agregar_objeto(&inventario_original, 3, "Espada_corta", TIPO_ARMA, 5);
    guardar_partida(RUTA_PRUEBA, &original, &inventario_original);

    // El destino ya tiene un objeto distinto: debe desaparecer al cargar
    preparar_personaje(&destino, "Otro", 10, 1, 1);
    inventario_inicializar(&inventario_destino);
    agregar_objeto(&inventario_destino, 50, "Viejo", TIPO_OTRO, 0);

    verificar(cargar_partida(RUTA_PRUEBA, &destino, &inventario_destino) == GUARDADO_OK,
              "cargar sobre un inventario que ya tenia objetos");
    verificar(inventario_destino.cantidad == 1 &&
              inventario_buscar_por_id(&inventario_destino, 3) != NULL &&
              inventario_buscar_por_id(&inventario_destino, 50) == NULL,
              "el inventario anterior fue reemplazado por el del archivo");

    inventario_liberar(&inventario_original);
    inventario_liberar(&inventario_destino);
    remove(RUTA_PRUEBA);
}

static void probar_guardado_parametros(void)
{
    Personaje personaje;
    Personaje sin_nombre;
    Inventario inventario;
    Inventario sin_inicializar = {NULL, 0};
    char ruta_larga[400];
    int i;

    printf("\n--- Guardado: parametros invalidos ---\n");
    remove(RUTA_PRUEBA);

    preparar_personaje(&personaje, "Aria", 30, 8, 2);
    inventario_inicializar(&inventario);

    verificar(guardar_partida(NULL, &personaje, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar con ruta NULL");
    verificar(guardar_partida(RUTA_PRUEBA, NULL, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar con personaje NULL");
    verificar(guardar_partida(RUTA_PRUEBA, &personaje, NULL) == GUARDADO_ERROR_PARAMETRO,
              "guardar con inventario NULL");
    verificar(guardar_partida(RUTA_PRUEBA, &personaje, &sin_inicializar) ==
              GUARDADO_ERROR_PARAMETRO,
              "guardar con un inventario sin inicializar");
    verificar(cargar_partida(NULL, &personaje, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "cargar con ruta NULL");
    verificar(cargar_partida(RUTA_PRUEBA, NULL, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "cargar con personaje NULL");
    verificar(cargar_partida(RUTA_PRUEBA, &personaje, NULL) == GUARDADO_ERROR_PARAMETRO,
              "cargar con inventario NULL");

    // Los nombres con espacios no se podrían leer de vuelta: se rechazan
    // antes de escribir nada
    preparar_personaje(&sin_nombre, "Aria Soto", 30, 8, 2);
    verificar(guardar_partida(RUTA_PRUEBA, &sin_nombre, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar un personaje con espacios en el nombre");
    preparar_personaje(&sin_nombre, "", 30, 8, 2);
    verificar(guardar_partida(RUTA_PRUEBA, &sin_nombre, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar un personaje con nombre vacio");
    agregar_objeto(&inventario, 1, "Espada corta", TIPO_ARMA, 5);
    verificar(guardar_partida(RUTA_PRUEBA, &personaje, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar un objeto con espacios en el nombre");
    verificar(!archivo_existe(RUTA_PRUEBA),
              "ninguno de los rechazos dejo un archivo escrito");

    // Ruta demasiado larga para el archivo temporal
    for (i = 0; i < 399; i++)
    {
        ruta_larga[i] = 'a';
    }
    ruta_larga[399] = '\0';
    inventario_liberar(&inventario);
    inventario_inicializar(&inventario);
    verificar(guardar_partida(ruta_larga, &personaje, &inventario) == GUARDADO_ERROR_PARAMETRO,
              "guardar con una ruta demasiado larga");

    // Carpeta que no existe: no se puede abrir el archivo
    verificar(guardar_partida("carpeta_que_no_existe/partida.txt", &personaje, &inventario) ==
              GUARDADO_ERROR_ARCHIVO,
              "guardar en una carpeta que no existe");

    inventario_liberar(&inventario);
}

/**
 * @brief Un archivo de guardado con un defecto concreto.
 */
typedef struct _CasoArchivo
{
    const char *descripcion;
    const char *contenido;

} CasoArchivo;

static void probar_guardado_archivos_invalidos(void)
{
    // Cada caso tiene UN solo defecto respecto a un archivo válido:
    //   RPG_PARTIDA_V1 / PERSONAJE / <7 datos> / INVENTARIO / <cantidad> / <objetos> / FIN
    const CasoArchivo casos[] = {
        {"archivo vacio",
         ""},
        {"marca de archivo incorrecta",
         "OTRA_COSA\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n0\nFIN\n"},
        {"falta la marca PERSONAJE",
         "RPG_PARTIDA_V1\nAria 30 30 8 0 2 0\nINVENTARIO\n0\nFIN\n"},
        {"datos del personaje incompletos",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8\nINVENTARIO\n0\nFIN\n"},
        {"vida actual mayor que la maxima",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 50 30 8 0 2 0\nINVENTARIO\n0\nFIN\n"},
        {"vida maxima cero",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 0 0 8 0 2 0\nINVENTARIO\n0\nFIN\n"},
        {"ataque negativo",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 -8 0 2 0\nINVENTARIO\n0\nFIN\n"},
        {"falta la marca INVENTARIO",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\n0\nFIN\n"},
        {"cantidad de objetos negativa",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n-1\nFIN\n"},
        {"cantidad mayor que los objetos del archivo",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n2\n3 0 5 Espada\nFIN\n"},
        {"id de objeto duplicado",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n2\n3 0 5 Espada\n3 0 9 Hacha\nFIN\n"},
        {"tipo de objeto invalido",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n1\n3 9 5 Espada\nFIN\n"},
        {"id de objeto cero",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n1\n0 0 5 Espada\nFIN\n"},
        {"pocion con valor cero",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n1\n7 2 0 Pocion\nFIN\n"},
        {"falta la marca FIN",
         "RPG_PARTIDA_V1\nPERSONAJE\nAria 30 30 8 0 2 0\nINVENTARIO\n1\n3 0 5 Espada\n"}
    };
    const int cantidad_casos = (int)(sizeof(casos) / sizeof(casos[0]));
    Personaje destino;
    Personaje copia;
    Inventario inventario;
    char mensaje[120];
    int i;

    printf("\n--- Guardado: archivos invalidos ---\n");

    // Archivo que no existe
    remove(RUTA_PRUEBA);
    preparar_personaje(&destino, "Original", 30, 8, 2);
    inventario_inicializar(&inventario);
    verificar(cargar_partida(RUTA_PRUEBA, &destino, &inventario) == GUARDADO_ERROR_ARCHIVO,
              "cargar un archivo que no existe");
    inventario_liberar(&inventario);

    for (i = 0; i < cantidad_casos; i++)
    {
        // Destino con datos conocidos: si la carga falla NO debe tocarlos
        preparar_personaje(&destino, "Original", 30, 8, 2);
        copia = destino;
        inventario_inicializar(&inventario);
        agregar_objeto(&inventario, 50, "Objeto_original", TIPO_OTRO, 0);

        escribir_archivo(RUTA_PRUEBA, casos[i].contenido);

        snprintf(mensaje, sizeof(mensaje), "%s: devuelve error de formato", casos[i].descripcion);
        verificar(cargar_partida(RUTA_PRUEBA, &destino, &inventario) == GUARDADO_ERROR_FORMATO,
                  mensaje);

        snprintf(mensaje, sizeof(mensaje), "%s: el personaje no se modifica", casos[i].descripcion);
        verificar(personajes_iguales(&destino, &copia), mensaje);

        snprintf(mensaje, sizeof(mensaje), "%s: el inventario no se modifica", casos[i].descripcion);
        verificar(inventario.cantidad == 1 && inventario_buscar_por_id(&inventario, 50) != NULL,
                  mensaje);

        inventario_liberar(&inventario);
    }

    remove(RUTA_PRUEBA);
}

/* ========================================================================== */
/*                                  PRINCIPAL                                 */
/* ========================================================================== */

int main(void)
{
    probar_objeto();

    probar_inventario_vacio();
    probar_inventario_un_elemento();
    probar_inventario_agregar_y_buscar();
    probar_inventario_eliminar();
    probar_inventario_liberar();

    probar_uso_parametros_y_errores();
    probar_uso_equipar();
    probar_uso_pociones();
    probar_uso_bengala();

    probar_guardado_ida_y_vuelta();
    probar_guardado_inventario_vacio();
    probar_guardado_cargar_reemplaza();
    probar_guardado_parametros();
    probar_guardado_archivos_invalidos();

    printf("\n==============================\n");
    printf("Pruebas ejecutadas: %d\n", pruebas_ejecutadas);
    printf("Pruebas fallidas:   %d\n", pruebas_fallidas);
    if (pruebas_fallidas == 0)
    {
        printf("Resultado: TODAS LAS PRUEBAS PASARON\n");
    }
    else
    {
        printf("Resultado: HAY PRUEBAS FALLIDAS\n");
    }
    printf("==============================\n");

    return pruebas_fallidas == 0 ? 0 : 1;
}