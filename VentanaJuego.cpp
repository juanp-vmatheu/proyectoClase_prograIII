#include "VentanaJuego.h"
#include "VistaJuego.h"

VentanaJuego::VentanaJuego(QWidget *parent)
    : QMainWindow(parent),
    m_mapa(FILAS, COLUMNAS),
    m_vista(new VistaJuego(this))
{
    ParametrosTerreno p = { 30, 10, 2, 2, 2, 5 };
    m_mapa.generarNivel(p);
    m_mapa.imprimir();

    m_vista->cargarMapa(&m_mapa, 1, 1);

    setCentralWidget(m_vista);
    setWindowTitle("Bomberman - Programacion III");
    adjustSize();
    setFixedSize(sizeHint());
    m_vista->setFocus();
}

VentanaJuego::~VentanaJuego()=default;

QGraphicsScene *VentanaJuego::escena() const
{
    return m_vista->scene();
}
