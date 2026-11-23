#include "VentanaJuego.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsView>

VentanaJuego::VentanaJuego(QWidget *parent)
    : QMainWindow(parent),
      m_escena(new QGraphicsScene(this)),
      m_vista(new QGraphicsView(this))
{
    configurarEscena();
    configurarVista();

    setCentralWidget(m_vista);
    setWindowTitle("Bomberman - Programacion III");
}

VentanaJuego::~VentanaJuego()
{
    // m_escena y m_vista tienen a "this" como padre: Qt los libera solo.
}

void VentanaJuego::configurarEscena()
{
    const int ancho = COLUMNAS * TAM_CASILLA;
    const int alto = FILAS * TAM_CASILLA;

    // El origen (0,0) es la esquina superior izquierda del tablero
    m_escena->setSceneRect(0, 0, ancho, alto);
    m_escena->setBackgroundBrush(QBrush(QColor(34, 139, 34))); // pasto verde
}

void VentanaJuego::configurarVista()
{
    m_vista->setScene(m_escena);
    m_vista->setRenderHint(QPainter::Antialiasing, false);
    m_vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vista->setFrameShape(QFrame::NoFrame);

    // La vista no debe robar el teclado: las teclas las maneja la ventana
    m_vista->setFocusPolicy(Qt::NoFocus);

    // Ventana del tamaño exacto del tablero
    m_vista->setFixedSize(static_cast<int>(m_escena->width()),
                          static_cast<int>(m_escena->height()));
    adjustSize();
    setFixedSize(sizeHint());
}
