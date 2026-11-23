#include "VentanaJuego.h"

#include "MapaTemporal.h"
#include "VistaJuego.h"

VentanaJuego::VentanaJuego(QWidget *parent)
    : QMainWindow(parent),
      m_vista(new VistaJuego(this))
{
    // TEMPORAL: hasta que exista MapaBomberman::generarNivel
    m_mapa = MapaTemporal::crear(FILAS, COLUMNAS, 30);
    MapaTemporal::imprimir(m_mapa, FILAS, COLUMNAS);

    m_vista->cargarMapa(m_mapa, FILAS, COLUMNAS, 1, 1);

    setCentralWidget(m_vista);
    setWindowTitle("Bomberman - Programacion III");
    adjustSize();
    setFixedSize(sizeHint());
    m_vista->setFocus();
}

VentanaJuego::~VentanaJuego()
{
    // m_vista tiene a "this" como padre: Qt la libera sola.
    // La vista solo lee la matriz durante el juego, no al destruirse.
    MapaTemporal::liberar(m_mapa, FILAS);
}

QGraphicsScene *VentanaJuego::escena() const
{
    return m_vista->scene();
}
