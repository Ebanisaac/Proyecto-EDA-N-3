#ifndef PERSONAJE_H
#define PERSONAJE_H

#define LARGO_NOMBRE_PERSONAJE 50

typedef struct {
    char nombre[LARGO_NOMBRE_PERSONAJE];
    int puntos_vida;
    int vida_maxima;
    int ataque;
    int bono_ataque;
    int defensa;
    int bono_defensa;
    
    // --- Nuevos campos para la exploración en el Proyecto 3 ---
    int habitacion_actual;
    int habitaciones_visitadas[100];
    int num_visitadas;
} Personaje;

// Funciones principales del personaje
Personaje* crear_personaje(const char* nombre, int vida, int ataque, int defensa);
Personaje* cargar_personaje(const char* ruta_archivo);
void mostrar_estado(Personaje* p);

// Funciones de control de exploración y posición
void personaje_registrar_visita(Personaje* p, int id_habitacion);
void personaje_mostrar_visitadas(const Personaje* p);

#endif // PERSONAJE_H