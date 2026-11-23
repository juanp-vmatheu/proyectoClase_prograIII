#include "VentanaJuego.h"

#include "MapaTemporal.h"
#include "VistaJuego.h"

VentanaJuego::VentanaJuego(QWidget *parent)
    : QMainWindow(parent),
      m_vista(new VistaJuego(this))
{
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
    MapaTemporal::liberar(m_mapa, FILAS);
}

QGraphicsScene *VentanaJuego::escena() const
{
    return m_vista->scene();
}
