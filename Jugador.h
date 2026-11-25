#ifndef JUGADOR_H
#define JUGADOR_H
#include "Tipos.h"

//Explicacion: Esta clase se encarga


class Jugador{
public:
    Jugador();

    void reiniciar();
    void prepaarNivel();
    void actualiza();

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
    int m_vidas;
    int m_monedas;
    int m_bombasMaximas;
    int m_alcanceLlama;
    double m_velocidad;
    double m_tiempoInvulnerable;
}

#endif // JUGADOR_H
