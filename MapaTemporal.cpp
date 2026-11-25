#include "MapaTemporal.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QString>

MapaBomberman::MapaBomberman(int nFilas, int nCol)
    :mapa(nullptr),filas(0), columnas(0){

    if (nFilas <= 0 || nCol <= 0) return;

    filas = nFilas;
    columnas = nCol;

    try {
        mapa = new CeldaMapa *[filas];

        for (int i = 0; i < filas; ++i){
            mapa[i] = nullptr;}

        for (int i = 0; i < filas; ++i) {
            mapa[i] = new CeldaMapa[columnas];
        }
    } catch (...) {
        liberar();
        throw;
    }
    vaciarCelda();
}

MapaBomberman::~MapaBomberman()
{
    liberar();
}

void MapaBomberman::liberar(){
    if (mapa == nullptr) return;
    for (int i = 0; i < filas; ++i) delete[] mapa[i];
    delete[] mapa;
    mapa = nullptr;
    filas = 0;
    columnas = 0;
}


////---Funcinees ee geneacion//

void MapaBomberman::generarNivel(const ParametrosTerreno &p)
{
    if (mapa == nullptr){
        return;}

    vaciarCelda();
    colocarCeldasFijas();
    reservarZonaAparicion();

    int destructibles = contarCeldasDisponibles() * p.porcentajeDestructibles / 100;
    colocarBloquesDestructibles(destructibles);
    marcarCeldasReforzadas(destructibles * p.porcentajeReforzado / 100);

    colocarContenido(Contenido::Portal, 1);
    colocarContenido(Contenido::PowerUpFlama, p.powerUpsFlama);
    colocarContenido(Contenido::PowerUpBomba, p.powerUpsBomba);
    colocarContenido(Contenido::PowerUpVelocidad, p.powerUpsVelocidad);
    colocarContenido(Contenido::Moneda, p.monedas);

}
void MapaBomberman::vaciarCelda(){//coloca ceeldas con valores por defecto
    if(mapa==nullptr){
        return;
    }
    for (int i = 0; i < filas; ++i){
        for (int j = 0; j < columnas; ++j){
            mapa[i][j] = CeldaMapa();
        }
    }

}

void MapaBomberman::colocarCeldasFijas(){
    if (mapa == nullptr){
        return;
    }
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            bool borde = (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1);
            bool pilar = (i % 2 == 0 && j % 2 == 0);
            if (borde || pilar){
                mapa[i][j].tipo = TipoCelda::MuroFijo;
            }
        }
    }
}

void MapaBomberman::reservarZonaAparicion(){
    if(mapa==nullptr){
        return;
    }
    for(int i=0; i<filas;++i){
        for(int j=0; j<columnas;++j){
            if (esZonaAparicion(i, j)) {
                mapa[i][j].tipo = TipoCelda::Vacio;
            }
        }
    }
}

int MapaBomberman::contarCeldasDisponibles()const{
    int total=0;
    for(int i=0;i<filas;++i){
        for(int j=0;j<columnas;++j){
            if(mapa[i][j].tipo==TipoCelda::Vacio && !esZonaAparicion(i,j)){
                ++total;
            }
        }
    }
    return total;
}


//Esta funcion es auxiliar para ColocarBloquesDestructibles y marcarCeldasReforzadas
//Sorte una celda que cumpla una condicion
Posicion MapaBomberman::elegirCelda(TipoCelda tipoRequerido, bool soloSinContenido) const
{
    int candidatas=0;
    for(int i=0;i<filas;++i){
        for(int j=0; j<columnas; ++j){
            const CeldaMapa &c=mapa[i][j];
            if(c.tipo!=tipoRequerido||esZonaAparicion(i,j)){
                continue;
            }
            if(soloSinContenido && c.contenido!=Contenido::Nada){
                continue;
            }
            ++candidatas;
        }
    }

        Posicion resultado={-1,-1};
        if(candidatas==0){
            return resultado;
        }
        int objetivo= QRandomGenerator::global()->bounded(candidatas);
        for(int i=0; i<filas;++i){
            for(int j=0; j<columnas;++j){
                const CeldaMapa &c=mapa[i][j];
                if(c.tipo!=tipoRequerido||esZonaAparicion(i,j)){
                    continue;
                }
                 if (soloSinContenido && c.contenido != Contenido::Nada){
                    continue;
                 }
                 if(objetivo==0){
                     resultado.filas=i;
                     resultado.columnas=j;
                     return resultado;
                 }
                 --objetivo;
            }

        }
        return resultado;

}

//Funcion para colocar LOS BLOQUES DESTRUCTIBLES y colocar resistencia
void MapaBomberman::colocarBloquesDestructibles(int cantidad){
    for(int i=0; i<cantidad;++i){
        Posicion p=elegirCelda(TipoCelda::Vacio,false);
        if(p.filas<0){
            break;
        }
        mapa[p.filas][p.columnas].tipo=TipoCelda::Destructible;
        mapa[p.filas][p.columnas].resistencia=1;
    }
}

void MapaBomberman::marcarCeldasReforzadas(int cantidad)
{
    for (int k = 0; k < cantidad; ++k) {
        Posicion p = elegirCelda(TipoCelda::Destructible, false);
        if (p.filas < 0) break;
        mapa[p.filas][p.columnas].tipo = TipoCelda::Reforzado;
        mapa[p.filas][p.columnas].resistencia = 2;
    }
}

//Colocar Contenido
void MapaBomberman::colocarContenido(Contenido tipo, int cantidad)
{
    for (int k = 0; k < cantidad; ++k) {
        Posicion p = elegirCelda(TipoCelda::Destructible, true);
        if (p.filas < 0){
            p = elegirCelda(TipoCelda::Reforzado, true);
        }
        if (p.filas < 0){
            break;
        }
        mapa[p.filas][p.columnas].contenido = tipo;
    }
}


//---Funciones de consultas: comunicacion entre logica y visual---
//Lo visual pregunta al terreno no modifica memooria y representacion
//logica

bool MapaBomberman::enLimites(int f, int c) const
{
    return mapa != nullptr && f >= 0 && f < filas && c >= 0 && c < columnas;
}

bool MapaBomberman::esTransitable(int f, int c) const
{
    if (!enLimites(f, c)){
        return false;
    }
    return mapa[f][c].tipo == TipoCelda::Vacio && !mapa[f][c].tieneBomba;
}
//Funcion especial para enemigos espctrales: que atraviesan bloques

bool MapaBomberman::esTransitableEspectral(int f, int c) const
{
    return enLimites(f, c) && mapa[f][c].tipo != TipoCelda::MuroFijo;
}


bool MapaBomberman::esMuroFijo(int f, int c) const
{
    return enLimites(f, c) && mapa[f][c].tipo == TipoCelda::MuroFijo;
}

bool MapaBomberman::esDestructible(int f, int c) const
{
    if (!enLimites(f, c)) return false;
    TipoCelda t = mapa[f][c].tipo;
    return t == TipoCelda::Destructible || t == TipoCelda::Reforzado;
}

Posicion MapaBomberman::posicionAparicion() const
{
    Posicion p = {1, 1};
    return p;
}

bool MapaBomberman::esZonaAparicion(int f, int c) const
{
    Posicion a = posicionAparicion();
    return (f == a.filas && c == a.columnas)
           || (f == a.filas && c == a.columnas + 1)
           || (f == a.filas + 1 && c == a.columnas);
}

const CeldaMapa &MapaBomberman::celda(int f, int c) const
{
    return mapa[f][c];
}

Posicion MapaBomberman::celdaLibreAleatoria() const
{
    return elegirCelda(TipoCelda::Vacio, false);
}

//----Getters
int MapaBomberman::getFilas() const {
    return filas; }
int MapaBomberman::getColumnas() const {
    return columnas; }


//-Funciones de modificacion---:
ResultadoDaño MapaBomberman::dañarBloque(int f, int c){
    if (!esDestructible(f, c)){
        return ResultadoDaño::noDañable;
    }
    CeldaMapa &cel = mapa[f][c];
    --cel.resistencia;
    if (cel.resistencia > 0) {
        return ResultadoDaño::Dañado;
    }
    cel.tipo = TipoCelda::Vacio;
    cel.resistencia = 0;
    return ResultadoDaño::Destruido;
}

bool MapaBomberman::ponerBomba(int f, int c)
{
    if (!enLimites(f, c)){
        return false;
    }
    if (mapa[f][c].tipo != TipoCelda::Vacio || mapa[f][c].tieneBomba){
        return false;
    }
    mapa[f][c].tieneBomba = true;
    return true;
}

bool MapaBomberman::quitarBomba(int f, int c)
{
    if (!enLimites(f, c) || !mapa[f][c].tieneBomba){
        return false;
    }
    mapa[f][c].tieneBomba = false;
    return true;
}

bool MapaBomberman::ponerLlamas(int f, int c)
{
    if (!enLimites(f, c) || mapa[f][c].tipo != TipoCelda::Vacio){
        return false;
    }
    mapa[f][c].enLlamas = true;
    return true;
}

void MapaBomberman::quitarLlama(int f, int c)
{
    if (enLimites(f, c)){
        mapa[f][c].enLlamas = false;
    }
}

Contenido MapaBomberman::recogerContenido(int f, int c)
{
    if (!enLimites(f, c) || mapa[f][c].tipo != TipoCelda::Vacio) {
        return Contenido::Nada;
    }

    Contenido recogido = mapa[f][c].contenido;
    if(recogido==Contenido::Portal){
        return recogido;
    }

    mapa[f][c].contenido = Contenido::Nada;
    return recogido;
}

void MapaBomberman::imprimir()
{
    if (mapa == nullptr) return;
    for (int f = 0; f < filas; ++f) {
        QString linea;
        for (int c = 0; c < columnas; ++c) {
            switch (mapa[f][c].tipo) {
            case TipoCelda::MuroFijo:    {
                linea += '#';
                break;
            }
            case TipoCelda::Destructible:{
                linea += '+';
                break;
            }
            case TipoCelda::Reforzado:   {
                linea += 'R';
                break;
            }
            case TipoCelda::Vacio: {
                linea += '.';
                break;
            }
            }
        }
        qDebug().noquote() << linea;
    }
}
