#ifndef TIPOS_H
#define TIPOS_H

// Contratos compartidos entre Persona A y Persona B (punto 3 del plan).
// Convencion de coordenadas: siempre (fila, columna).
// La conversion a pixeles ocurre en un solo lugar: aPixel(columna) / aPixel(fila).

constexpr int TAM_CELDA = 48;   // lado de cada celda en pixeles

inline int aPixel(int indice) { return indice * TAM_CELDA; }

enum class TipoCelda {
    Vacio,
    MuroFijo,       // indestructible (bordes y pilares)
    Destructible,   // se rompe con una explosion
    Reforzado       // necesita varias explosiones (usa resistencia)
};

enum class Contenido {
    Nada,
    PowerUpFlama,
    PowerUpBomba,
    PowerUpVelocidad,
    Moneda,
    Portal
};

struct CeldaMapa {
    TipoCelda tipo = TipoCelda::Vacio;
    Contenido contenido = Contenido::Nada;
    int resistencia = 0;
    bool tieneBomba = false;
    bool enLlamas = false;
};

#endif // TIPOS_H
