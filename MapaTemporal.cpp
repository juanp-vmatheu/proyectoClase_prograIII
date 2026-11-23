#include "MapaTemporal.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QString>

namespace MapaTemporal {

static bool esZonaAparicion(int f, int c)
{
    // El jugador aparece en (1,1); dejamos libres sus dos vecinos para que
    // pueda poner la primera bomba y escapar.
    return (f == 1 && c == 1) || (f == 1 && c == 2) || (f == 2 && c == 1);
}

CeldaMapa **crear(int filas, int columnas, int porcentajeDestructibles)
{
    CeldaMapa **mapa = new CeldaMapa *[filas];
    for (int f = 0; f < filas; ++f)
        mapa[f] = new CeldaMapa[columnas];

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            bool borde = (f == 0 || c == 0 || f == filas - 1 || c == columnas - 1);
            bool pilar = (f % 2 == 0 && c % 2 == 0);
            if (borde || pilar) {
                mapa[f][c].tipo = TipoCelda::MuroFijo;
            } else if (!esZonaAparicion(f, c)
                       && QRandomGenerator::global()->bounded(100) < porcentajeDestructibles) {
                mapa[f][c].tipo = TipoCelda::Destructible;
                mapa[f][c].resistencia = 1;
            }
        }
    }
    return mapa;
}

void liberar(CeldaMapa **&mapa, int filas)
{
    if (!mapa)
        return;
    for (int f = 0; f < filas; ++f)
        delete[] mapa[f];
    delete[] mapa;
    mapa = nullptr;
}

void imprimir(CeldaMapa **mapa, int filas, int columnas)
{
    for (int f = 0; f < filas; ++f) {
        QString linea;
        for (int c = 0; c < columnas; ++c) {
            switch (mapa[f][c].tipo) {
            case TipoCelda::MuroFijo:     linea += '#'; break;
            case TipoCelda::Destructible: linea += '+'; break;
            case TipoCelda::Reforzado:    linea += 'R'; break;
            case TipoCelda::Vacio:        linea += '.'; break;
            }
        }
        qDebug().noquote() << linea;
    }
}

}
