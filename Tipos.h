#ifndef TIPOS_H
#define TIPOS_H
#include "Tipos.h"

constexpr int TAM_CELDA = 48;
inline int aPixel(int indice) {
    return indice * TAM_CELDA; }

enum class ResultadoDaño { noDañable,
                           Dañado,
                           Destruido };

struct EstadoJugador{
    int vidas;
    int monedas;
    int bombaMaximas;
    int alcanceLlamas;
    double velocidad;
};

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
//Constantes solicitadas en varias clases:




constexpr int RESISTENCIA_DESTRUCTIBLE = 1;
constexpr int RESISTENCIA_REFORZADO = 2;
constexpr Posicion INICIO_JUGADOR={1,1};

// Jugador
constexpr int VIDAS_INICIALES = 3;
constexpr double VELOCIDAD_INICIAL = 4.0;
constexpr double VELOCIDAD_TOPE = 8.0;

#endif
