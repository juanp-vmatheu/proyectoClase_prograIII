#ifndef JUGADOR_H
#define JUGADOR_H
#include "Tipos.h"

//Explicacion: Esta clase se encarga del manejo de datos
//y habilidades del jugador.


class Jugador{
public:
    Jugador();

    void reiniciar();
    void prepaarNivel();
    void actualizar();

    bool perderVidas();
    bool estaVivo() const{
        return m_vidas>0;
    }
    bool esInvulnerable() const;
    double tiempoInvulnerable() const;

    bool puedeColocarBomba(int bombasPuestas) const;
    void recogerObjeto(Contenido c);

    int vidas() const;
    int monedas() const;
    int bombasMaximas() const;
    int alcanceLlama() const;
    double velocidad() const;

    EstadoJugador estado()const;
    void restaurar(const EstadoJugador& e);

private:
    //cnstantes con las que se controlaran los valores
    //y alcances de los powerUps

    constexpr int VIDAS_INICIALES = 3;
    constexpr int VIDAS_MAXIMAS = VIDAS_INICIALES;
    constexpr int BOMBAS_INICIALES = 1;
    constexpr int BOMBAS_TOPE = 8;
    constexpr int ALCANCE_INICIAL = 1;
    constexpr int ALCANCE_TOPE = 8;
    constexpr double VELOCIDAD_INICIAL = 4.0;
    constexpr double VELOCIDAD_TOPE = 8.0;
    constexpr double INCREMENTO_VELOCIDAD = 1.0;
    constexpr double TIEMPO_INMUNIDAD = 2.0;


    int m_vidas;
    int m_monedas;
    int m_bombasMaximas;
    int m_alcanceLlama;
    double m_velocidad;
    double m_tiempoInvulnerable;
}

#endif // JUGADOR_H
