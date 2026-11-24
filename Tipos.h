#ifndef TIPOS_H
#define TIPOS_H

constexpr int TAM_CELDA = 48;

inline int aPixel(int indice) {
    return indice * TAM_CELDA; }

enum class ResultadoDaño { noDañable,
                            Dañado,
                            Destruido };


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

struct Posicion {
    int filas;
    int columnas;
};

//struct necesario para cnfigNvel
struct ParametrosTerreno {
    int porcentajeDestructibles;
    int porcentajeReforzado;
    int powerUpsFlama;
    int powerUpsBomba;
    int powerUpsVelocidad;
    int monedas;
};

struct CeldaMapa {
    TipoCelda tipo = TipoCelda::Vacio;
    Contenido contenido = Contenido::Nada;
    int resistencia = 0;
    bool tieneBomba = false;
    bool enLlamas = false;
};

#endif
