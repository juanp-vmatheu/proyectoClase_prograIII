#ifndef TIPOS_H
#define TIPOS_H

constexpr int TAM_CELDA = 48;

inline int aPixel(int indice) { return indice * TAM_CELDA; }

enum class TipoCelda {
    Vacio,
    MuroFijo,
    Destructible,
    Reforzado
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

#endif
