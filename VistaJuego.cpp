#include "VistaJuego.h"

#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPen>
#include <QTimer>

static const int MS_POR_TICK = 16;

VistaJuego::VistaJuego(QWidget *parent)
    : QGraphicsView(parent),
      m_escena(new QGraphicsScene(this)),
      m_timer(new QTimer(this))
{
    setScene(m_escena);
    setBackgroundBrush(QColor(30, 30, 30));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing, false);
    setFocusPolicy(Qt::StrongFocus);

    m_timer->setInterval(MS_POR_TICK);
    connect(m_timer, &QTimer::timeout, this, &VistaJuego::tickMovimiento);
}

VistaJuego::~VistaJuego()
{
    m_timer->stop();
    liberarItems();
}

void VistaJuego::liberarItems()
{
    if (!m_items)
        return;
    for (int f = 0; f < m_filas; ++f)
        delete[] m_items[f];
    delete[] m_items;
    m_items = nullptr;
}

QColor VistaJuego::colorDe(const CeldaMapa &celda)
{
    switch (celda.tipo) {
    case TipoCelda::MuroFijo:     return QColor(90, 90, 100);
    case TipoCelda::Destructible: return QColor(170, 110, 60);
    case TipoCelda::Reforzado:    return QColor(120, 70, 40);
    case TipoCelda::Vacio:        break;
    }
    return QColor(60, 140, 70);
}

void VistaJuego::cargarMapa(CeldaMapa **mapa, int filas, int columnas,
                            int filaJugador, int colJugador)
{
    m_timer->stop();
    m_escena->clear();
    m_jugador = nullptr;
    liberarItems();

    m_mapa = mapa;
    m_filas = filas;
    m_columnas = columnas;
    m_escena->setSceneRect(0, 0, aPixel(columnas), aPixel(filas));

    m_items = new QGraphicsRectItem **[filas];
    for (int f = 0; f < filas; ++f) {
        m_items[f] = new QGraphicsRectItem *[columnas];
        for (int c = 0; c < columnas; ++c) {
            QGraphicsRectItem *r = m_escena->addRect(0, 0, TAM_CELDA, TAM_CELDA,
                                                     QPen(QColor(0, 0, 0, 60)));
            r->setPos(aPixel(c), aPixel(f));
            r->setBrush(colorDe(mapa[f][c]));
            r->setZValue(0);
            m_items[f][c] = r;
        }
    }

    const int margen = 6;
    m_jugador = m_escena->addRect(margen, margen,
                                  TAM_CELDA - 2 * margen, TAM_CELDA - 2 * margen,
                                  QPen(Qt::black, 2), QBrush(QColor(240, 240, 240)));
    m_jugador->setZValue(10);
    m_filaJugador = filaJugador;
    m_colJugador = colJugador;
    m_jugador->setPos(aPixel(colJugador), aPixel(filaJugador));

    m_teclaActual = m_pasoActual = Ninguna;
    m_progreso = 0.0;

    setFixedSize(aPixel(columnas) + 2 * frameWidth(), aPixel(filas) + 2 * frameWidth());
    setFocus();
}

void VistaJuego::actualizarCelda(int fila, int columna)
{
    if (!m_items || fila < 0 || columna < 0 || fila >= m_filas || columna >= m_columnas)
        return;
    m_items[fila][columna]->setBrush(colorDe(m_mapa[fila][columna]));
}

void VistaJuego::setVelocidad(double celdasPorSegundo)
{
    if (celdasPorSegundo > 0)
        m_velocidad = celdasPorSegundo;
}

bool VistaJuego::esTransitable(int fila, int columna) const
{
    if (!m_mapa || fila < 0 || columna < 0 || fila >= m_filas || columna >= m_columnas)
        return false;
    const CeldaMapa &celda = m_mapa[fila][columna];
    return celda.tipo == TipoCelda::Vacio && !celda.tieneBomba;
}

VistaJuego::Direccion VistaJuego::direccionDeTecla(int tecla)
{
    switch (tecla) {
    case Qt::Key_Up:    case Qt::Key_W: return Arriba;
    case Qt::Key_Down:  case Qt::Key_S: return Abajo;
    case Qt::Key_Left:  case Qt::Key_A: return Izquierda;
    case Qt::Key_Right: case Qt::Key_D: return Derecha;
    default: return Ninguna;
    }
}

void VistaJuego::keyPressEvent(QKeyEvent *event)
{
    Direccion d = direccionDeTecla(event->key());
    if (d == Ninguna) {
        QGraphicsView::keyPressEvent(event);
        return;
    }
    if (event->isAutoRepeat())
        return;
    m_teclaActual = d;
    if (m_pasoActual == Ninguna && intentarIniciarPaso()) {
        m_reloj.start();
        m_timer->start();
    }
}

void VistaJuego::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    if (direccionDeTecla(event->key()) == m_teclaActual)
        m_teclaActual = Ninguna;
    QGraphicsView::keyReleaseEvent(event);
}

void VistaJuego::focusOutEvent(QFocusEvent *event)
{
    m_teclaActual = Ninguna;
    QGraphicsView::focusOutEvent(event);
}

bool VistaJuego::intentarIniciarPaso()
{
    if (m_teclaActual == Ninguna || !m_jugador)
        return false;
    int df = 0, dc = 0;
    switch (m_teclaActual) {
    case Arriba:    df = -1; break;
    case Abajo:     df = 1;  break;
    case Izquierda: dc = -1; break;
    case Derecha:   dc = 1;  break;
    case Ninguna:   break;
    }
    if (!esTransitable(m_filaJugador + df, m_colJugador + dc))
        return false;
    m_pasoActual = m_teclaActual;
    m_progreso = 0.0;
    return true;
}

void VistaJuego::tickMovimiento()
{
    if (m_pasoActual == Ninguna) {
        m_timer->stop();
        return;
    }

    int df = 0, dc = 0;
    switch (m_pasoActual) {
    case Arriba:    df = -1; break;
    case Abajo:     df = 1;  break;
    case Izquierda: dc = -1; break;
    case Derecha:   dc = 1;  break;
    case Ninguna:   break;
    }

    m_progreso += m_velocidad * m_reloj.restart() / 1000.0;
    if (m_progreso >= 1.0) {
        m_filaJugador += df;
        m_colJugador += dc;
        m_jugador->setPos(aPixel(m_colJugador), aPixel(m_filaJugador));
        m_pasoActual = Ninguna;
        if (!intentarIniciarPaso())
            m_timer->stop();
        return;
    }

    qreal x = aPixel(m_colJugador) + dc * m_progreso * TAM_CELDA;
    qreal y = aPixel(m_filaJugador) + df * m_progreso * TAM_CELDA;
    m_jugador->setPos(x, y);
}
