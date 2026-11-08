#ifndef PERSONAJE_H
#define PERSONAJE_H

#define LARGO_NOMBRE_PERSONAJE 50

typedef struct _personaje {
    char nombre[LARGO_NOMBRE_PERSONAJE];
    int puntos_vida;
    int vida_maxima;
    int ataque;
    int bono_ataque;
    int defensa;
    int bono_defensa;
    
    
    int habitacion_actual;              
    int habitaciones_visitadas[100];    
    int num_visitadas;                  
} Personaje;

/**
 * @brief Crea un personaje reservando memoria dinámica.
 * @param nombre Nombre del personaje.
 * @param vida Puntos de vida iniciales y máximos.
 * @param ataque Puntos de daño base.
 * @param defensa Puntos de reducción de daño.
 * @return Puntero al nuevo Personaje, o NULL si falla la memoria.
 */
Personaje* crear_personaje(const char* nombre, int vida, int ataque, int defensa);

/**
 * @brief Carga un personaje desde un archivo de texto.
 * @param ruta_archivo Ruta al archivo que contiene los datos.
 * @return Puntero al Personaje cargado, o NULL si hay error.
 */
Personaje* cargar_personaje(const char* ruta_archivo);

/**
 * @brief Muestra el estado del personaje en la terminal.
 * @param p Puntero al personaje.
 */
void mostrar_estado(Personaje* p);

/**
 * @brief Registra una habitación en el historial de visitadas si no ha sido registrada antes.
 * @param p Puntero al personaje.
 * @param id_habitacion ID de la habitación a registrar.
 */
void personaje_registrar_visita(Personaje* p, int id_habitacion);

/**
 * @brief Muestra por pantalla la lista de todas las habitaciones visitadas (Opción 10).
 * @param p Puntero al personaje.
 */
void personaje_mostrar_visitadas(const Personaje* p);

#endif /* PERSONAJE_H */