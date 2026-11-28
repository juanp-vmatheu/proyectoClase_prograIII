#include "Jugador.h"

Jugador::Jugador(){
    reiniciar();
}

void Jugador::reiniciar(){
    m_vidas=VIDAS_INICIALES;
    m_monedas=0;
    m_bombasMaximas=BOMBAS_INICIALES;
    m_alcanceLlama=ALCANCE_INICIAL;
    m_velocidad=VELOCIDAD_INICIAL;
    m_tiempoInvulnerable=0.0;
}

void Jugador::prepararNivel(){
    m_tiempoInvulnerable=0.0;
    //Bella: esto see usa porque al pasar de nivel en el modo
    //de avance continuo se conservan los powerups obtenidos
    //pero la inmunidad se reinicia
}

//Se llama en cada tick del juego con los segundos que pasaron
//desde el tick anterior. Solo descuenta el tiempo de inmunidad.
void Jugador::actualizar(double tiempoPasado){
    if(m_tiempoInvulnerable>0.0){
        m_tiempoInvulnerable-=tiempoPasado;
        if(m_tiempoInvulnerable<0.0){
            m_tiempoInvulnerable=0.0;
        }
    }
}

//Devuelve true si el golpe si le quito una vida.
//Si esta invulnerable o ya esta muerto no pasa nada.
bool Jugador::perderVidas(){
    if(esInvulnerable() || !estaVivo()){
        return false;
    }
    m_vidas--;
    m_tiempoInvulnerable=TIEMPO_INMUNIDAD;
    return true;
}

bool Jugador::esInvulnerable() const{
    return m_tiempoInvulnerable>0.0;
}

double Jugador::tiempoInvulnerable() const{
    return m_tiempoInvulnerable;
}

bool Jugador::puedeColocarBomba(int bombasPuestas) const{
    return estaVivo() && bombasPuestas<m_bombasMaximas;
}

//Aplica el efecto del objeto que se recogio en el mapa.
//Cada powerUp tiene un tope para que no crezca sin limite.
void Jugador::recogerObjeto(Contenido c){
    switch(c){
    case Contenido::PowerUpFlama:
        if(m_alcanceLlama<ALCANCE_TOPE){
            m_alcanceLlama++;
        }
        break;
    case Contenido::PowerUpBomba:
        if(m_bombasMaximas<BOMBAS_TOPE){
            m_bombasMaximas++;
        }
        break;
    case Contenido::PowerUpVelocidad:
        m_velocidad+=INCREMENTO_VELOCIDAD;
        if(m_velocidad>VELOCIDAD_TOPE){
            m_velocidad=VELOCIDAD_TOPE;
        }
        break;
    case Contenido::Moneda:
        m_monedas++;
        break;
    case Contenido::Portal:
    case Contenido::Nada:
        //el portal lo maneja el motor del juego (pasar de nivel)
        break;
    }
}

int Jugador::vidas() const{
    return m_vidas;
}

int Jugador::monedas() const{
    return m_monedas;
}

int Jugador::bombasMaximas() const{
    return m_bombasMaximas;
}

int Jugador::alcanceLlama() const{
    return m_alcanceLlama;
}

double Jugador::velocidad() const{
    return m_velocidad;
}

//Guarda los datos del jugador para pasarlos al siguiente nivel
EstadoJugador Jugador::estado() const{
    EstadoJugador e;
    e.vidas=m_vidas;
    e.monedas=m_monedas;
    e.bombaMaximas=m_bombasMaximas;
    e.alcanceLlamas=m_alcanceLlama;
    e.velocidad=m_velocidad;
    return e;
}

//Carga un estado guardado, respetando los topes
void Jugador::restaurar(const EstadoJugador& e){
    m_vidas=e.vidas;
    if(m_vidas>VIDAS_MAXIMAS){
        m_vidas=VIDAS_MAXIMAS;
    }
    m_monedas=e.monedas;
    m_bombasMaximas=e.bombaMaximas;
    if(m_bombasMaximas>BOMBAS_TOPE){
        m_bombasMaximas=BOMBAS_TOPE;
    }
    m_alcanceLlama=e.alcanceLlamas;
    if(m_alcanceLlama>ALCANCE_TOPE){
        m_alcanceLlama=ALCANCE_TOPE;
    }
    m_velocidad=e.velocidad;
    if(m_velocidad>VELOCIDAD_TOPE){
        m_velocidad=VELOCIDAD_TOPE;
    }
    m_tiempoInvulnerable=0.0;
}
