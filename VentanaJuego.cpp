#include "VentanaJuego.h"
#include "VistaJuego.h"
#include "ColaBombas.h"

#include <QDebug>
#include <QString>
#include <QTimer>

VentanaJuego::VentanaJuego(QWidget *parent)
    : QMainWindow(parent),
    m_mapa(FILAS, COLUMNAS),
    m_vista(new VistaJuego(this)),
    m_bombas(nullptr)
{
    ParametrosTerreno p = { 30, 10, 2, 2, 2, 5 };
    m_mapa.generarNivel(p);
    m_mapa.imprimir();

    m_vista->cargarMapa(&m_mapa, INICIO_JUGADOR.filas, INICIO_JUGADOR.columnas);
    m_vista->setVelocidad(m_jugador.velocidad());

    //las bombas modifican el mapa y avisan a la vista que celda redibujar
    m_bombas = new ColaBombas(&m_mapa);
    connect(m_bombas, &ColaBombas::celdaCambio, m_vista, &VistaJuego::actualizarCelda);
    connect(m_bombas, &ColaBombas::llamaEn, this, [this](int f, int c) {
        if (f == m_vista->filaJugador() && c == m_vista->columnaJugador()) {
            revisarGolpe(f, c);
        }
    });
    connect(m_vista, &VistaJuego::pedirBomba, this, [this](int f, int c) {
        ponerBomba(f, c);
    });
    connect(m_vista, &VistaJuego::jugadorLlegoA, this, [this](int f, int c) {
        revisarGolpe(f, c);
    });

    //cada 100 ms se descuenta el tiempo de inmunidad del jugador
    QTimer *reloj = new QTimer(this);
    connect(reloj, &QTimer::timeout, this, [this]() {
        m_jugador.actualizar(MS_RELOJ / 1000.0);
    });
    reloj->start(MS_RELOJ);

    setCentralWidget(m_vista);
    actualizarTitulo();
    adjustSize();
    setFixedSize(sizeHint());
    m_vista->setFocus();
}

//las bombas se borran antes que el mapa porque al limpiarse lo usan
VentanaJuego::~VentanaJuego()
{
    delete m_bombas;
    m_bombas = nullptr;
}

void VentanaJuego::ponerBomba(int f, int c)
{
    if (!m_jugador.puedeColocarBomba(m_bombas->bombasActivas())) {
        return;
    }
    m_bombas->colocar(f, c, m_jugador.alcanceLlama());
}

void VentanaJuego::revisarGolpe(int f, int c)
{
    if (!m_mapa.enLimites(f, c) || !m_mapa.celda(f, c).enLlamas) {
        return;
    }
    if (m_jugador.perderVidas()) {
        qDebug() << "[JUGADOR]: perdio una vida, quedan" << m_jugador.vidas();
        actualizarTitulo();
    }
}

void VentanaJuego::actualizarTitulo()
{
    QString titulo = "Bomberman - Programacion III  |  Vidas: " + QString::number(m_jugador.vidas());
    if (!m_jugador.estaVivo()) {
        titulo += "  |  GAME OVER";
    }
    setWindowTitle(titulo);
}

QGraphicsScene *VentanaJuego::escena() const
{
    return m_vista->scene();
}
