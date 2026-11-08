/*
 * Archivo: objeto.h
 * Autor: Pablo Serón
 * Descripción: Definición del tipo Objeto (armas, armaduras, pociones, etc.).
 */

#ifndef OBJETO_H
#define OBJETO_H

#define LARGO_NOMBRE_OBJETO 50

/**
 * @brief 
 * Categorias para objetos
 */
typedef enum {
    TIPO_ARMA,
    TIPO_ARMADURA,
    TIPO_POCION,
    TIPO_OTRO
} TipoObjeto;

/**
 * @brief 
 * id: identificador (> 0)
 * nombre: nombre del objeto
 * tipo: categoría del objeto
 * valor: efecto numérico (+ataque, +defensa, +vida... etc)
 */
typedef struct _Objeto {
    int id;
    char nombre[LARGO_NOMBRE_OBJETO];
    TipoObjeto tipo;
    int valor;
} Objeto;

/**
 * @brief Rellena un objeto con los datos entregados.
 * 
 * @param objeto Objeto destino (no debe ser NULL).
 * @param id Identificador único (> 0).
 * @param nombre Nombre no vacío; si es muy largo se trunca.
 * @param tipo Categoría del objeto.
 * @param valor Efecto numérico del objeto.
 * @return 0 si todo fue correcto, -1 si algún parámetro es inválido.
 */
int objeto_inicializar(Objeto *objeto, int id, const char *nombre,TipoObjeto tipo, int valor);

/**
 * @brief Convierte un tipo de objeto a texto.
 * @param tipo Tipo de objeto a convertir.
 * @return Cadena estática ("arma", "armadura", "poción", "otro") o
 *         "desconocido" si el tipo no es válido. No debe liberarse.
 */
const char *objeto_tipo_a_texto(TipoObjeto tipo);

/**
 * @brief Imprime un objeto por terminal con el formato: [id] nombre (tipo, valor).
 * 
 * @param objeto Objeto a imprimir. Si es NULL, no hace nada.
 */
void objeto_imprimir(const Objeto *objeto);

#endif /* OBJETO_H */