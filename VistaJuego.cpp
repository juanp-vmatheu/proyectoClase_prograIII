#include "VistaJuego.h"

#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPen>
#include "MapaTemporal.h"      // en lugar de "Tipos.h"
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

    cargarSprites();
    cargarSpritesLink();
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
    //los items los borra la escena (m_escena->clear()),
    //aqui solo se liberan los arreglos de punteros
    for (int f = 0; f < m_filas; ++f) {
        delete[] m_items[f];
        delete[] m_encima[f];
    }
    delete[] m_items;
    delete[] m_encima;
    m_items = nullptr;
    m_encima = nullptr;
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

//Recorta un pedazo de la hoja y lo escala al tamano de la celda.
//Se usa FastTransformation para que el pixel art no se vea borroso.
QPixmap VistaJuego::recortar(const QPixmap &hoja, int x, int y, int ancho, int alto,
                             const QColor &respaldo)
{
    if (hoja.isNull()) {
        QPixmap p(TAM_CELDA, TAM_CELDA);
        p.fill(respaldo);
        return p;
    }
    return hoja.copy(x, y, ancho, alto)
        .scaled(TAM_CELDA, TAM_CELDA, Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

void VistaJuego::cargarSprites()
{
    QPixmap overworld(":/sprites/zelda/overworld.png");
    QPixmap castillo(":/sprites/zelda/castillos.png");
    QPixmap bombas(":/sprites/zelda/bombas.png");

    //la hoja de bombas tiene fondo rosado (#FFC0FF), se vuelve transparente
    if (!bombas.isNull())
        bombas.setMask(bombas.createMaskFromColor(QColor(255, 192, 255)));

    m_spPasto      = recortar(overworld, 253, 57, 16, 16, QColor(60, 140, 70));
    m_spArbusto    = recortar(overworld, 304, 57, 16, 16, QColor(170, 110, 60));
    m_spRoca       = recortar(overworld, 320, 74, 16, 16, QColor(120, 70, 40));
    m_spRocaDanada = recortar(overworld, 304, 74, 16, 16, QColor(150, 90, 50));
    m_spMuro       = recortar(castillo, 211, 240, 16, 16, QColor(90, 90, 100));
    m_spBomba      = recortar(bombas, 2, 34, 16, 16, QColor(20, 20, 20));
    m_spLlama      = recortar(bombas, 177, 24, 32, 32, QColor(250, 160, 30));
}

//Recorta los cuadros de Link de la hoja. Cada cuadro mide 16x24 (17x24 de lado)
//y se escala x3 para que el cuerpo ocupe una celda y la cabeza sobresalga.
void VistaJuego::cargarSpritesLink()
{
    QPixmap hoja(":/sprites/zelda/link.png");
    const int escala = TAM_CELDA / 16;

    if (hoja.isNull()) {
        //respaldo: cuadro blanco como antes
        QPixmap p(TAM_CELDA, TAM_CELDA + 2 * escala * 4);
        p.fill(QColor(240, 240, 240));
        for (int d = 0; d < 4; ++d)
            for (int i = 0; i < CUADROS_LINK; ++i)
                m_spLink[d][i] = p;
        return;
    }

    //fondo de la hoja (#008080) y cajas de cada cuadro (#004040) se vuelven transparentes
    QImage img = hoja.toImage().convertToFormat(QImage::Format_ARGB32);
    const QRgb fondo1 = qRgb(0, 128, 128);
    const QRgb fondo2 = qRgb(0, 64, 64);
    for (int y = 0; y < img.height(); ++y) {
        QRgb *fila = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            QRgb sinAlfa = fila[x] | 0xFF000000;
            if (sinAlfa == fondo1 || sinAlfa == fondo2)
                fila[x] = qRgba(0, 0, 0, 0);
        }
    }

    //posicion x de cada cuadro en la hoja
    const int xFrente[CUADROS_LINK] = { 1, 19, 36, 53, 70, 87, 104, 121 };
    const int xLado[CUADROS_LINK]   = { 1, 20, 38, 55, 73, 91, 109, 126 };
    const int yArriba = 111;
    const int yAbajo = 3;
    const int yLado = 58;

    for (int i = 0; i < CUADROS_LINK; ++i) {
        QImage arriba = img.copy(xFrente[i], yArriba, 16, 24);
        QImage abajo = img.copy(xFrente[i], yAbajo, 16, 24);
        QImage derecha = img.copy(xLado[i], yLado, 17, 24);
        QImage izquierda = derecha.flipped(Qt::Horizontal);

        m_spLink[0][i] = QPixmap::fromImage(arriba.scaled(16 * escala, 24 * escala));
        m_spLink[1][i] = QPixmap::fromImage(abajo.scaled(16 * escala, 24 * escala));
        m_spLink[2][i] = QPixmap::fromImage(izquierda.scaled(17 * escala, 24 * escala));
        m_spLink[3][i] = QPixmap::fromImage(derecha.scaled(17 * escala, 24 * escala));
    }
}

//Elige el cuadro de Link segun hacia donde mira y si esta caminando
void VistaJuego::actualizarSpriteJugador()
{
    if (!m_jugador)
        return;
    int dir = 1; //abajo
    switch (m_mirando) {
    case Arriba:    dir = 0; break;
    case Abajo:     dir = 1; break;
    case Izquierda: dir = 2; break;
    case Derecha:   dir = 3; break;
    case Ninguna:   break;
    }
    int cuadro = 0;
    if (m_pasoActual != Ninguna) {
        //12 cuadros por segundo, se salta el cuadro 0 (quieto)
        cuadro = 1 + static_cast<int>(m_tiempoAnim * 12) % (CUADROS_LINK - 1);
    }
    const QPixmap &sp = m_spLink[dir][cuadro];
    m_jugador->setPixmap(sp);
    //los pies quedan en la parte baja de la celda y la cabeza sobresale arriba
    m_jugador->setOffset((TAM_CELDA - sp.width()) / 2, TAM_CELDA - sp.height());
}

//Devuelve el sprite del suelo o bloque segun el tipo de celda
const QPixmap &VistaJuego::spriteDe(const CeldaMapa &celda) const
{
    switch (celda.tipo) {
    case TipoCelda::MuroFijo:     return m_spMuro;
    case TipoCelda::Destructible: return m_spArbusto;
    case TipoCelda::Reforzado:
        //cuando ya recibio un golpe se ve como roca clara
        return celda.resistencia >= RESISTENCIA_REFORZADO ? m_spRoca : m_spRocaDanada;
    case TipoCelda::Vacio:        break;
    }
    return m_spPasto;
}

//Lo que va encima de la celda (llama tiene prioridad sobre bomba)
const QPixmap *VistaJuego::spriteEncimaDe(const CeldaMapa &celda) const
{
    if (celda.enLlamas)
        return &m_spLlama;
    if (celda.tieneBomba)
        return &m_spBomba;
    return nullptr;
}

void VistaJuego::cargarMapa(MapaBomberman *mapa, int filaJugador, int colJugador)
{
    m_timer->stop();
    m_escena->clear();
    m_jugador = nullptr;
    liberarItems();

    m_mapa = mapa;
    if (!m_mapa){

        return;
    }

    m_filas = m_mapa->getFilas();
    m_columnas = m_mapa->getColumnas();
    m_escena->setSceneRect(0, 0, aPixel(m_columnas), aPixel(m_filas));

    m_items = new QGraphicsPixmapItem **[m_filas];
    m_encima = new QGraphicsPixmapItem **[m_filas];
    for (int f = 0; f < m_filas; ++f) {
        m_items[f] = new QGraphicsPixmapItem *[m_columnas];
        m_encima[f] = new QGraphicsPixmapItem *[m_columnas];
        for (int c = 0; c < m_columnas; ++c) {
            QGraphicsPixmapItem *base = m_escena->addPixmap(m_spPasto);
            base->setPos(aPixel(c), aPixel(f));
            base->setZValue(0);
            m_items[f][c] = base;

            QGraphicsPixmapItem *arriba = m_escena->addPixmap(QPixmap());
            arriba->setPos(aPixel(c), aPixel(f));
            arriba->setZValue(5);
            m_encima[f][c] = arriba;

            actualizarCelda(f, c);
        }
    }

    m_jugador = m_escena->addPixmap(QPixmap());
    m_jugador->setZValue(10);
    m_filaJugador = filaJugador;
    m_colJugador = colJugador;
    m_jugador->setPos(aPixel(colJugador), aPixel(filaJugador));

    m_teclaActual = m_pasoActual = Ninguna;
    m_progreso = 0.0;
    m_mirando = Abajo;
    m_tiempoAnim = 0.0;
    actualizarSpriteJugador();

    setFixedSize(aPixel(m_columnas) + 2 * frameWidth(),
                 aPixel(m_filas) + 2 * frameWidth());
    setFocus();
}

void VistaJuego::actualizarCelda(int fila, int columna)
{
    if (!m_items || !m_mapa || !m_mapa->enLimites(fila, columna))
        return;
    const CeldaMapa &celda = m_mapa->celda(fila, columna);
    m_items[fila][columna]->setPixmap(spriteDe(celda));

    const QPixmap *encima = spriteEncimaDe(celda);
    m_encima[fila][columna]->setPixmap(encima ? *encima : QPixmap());
    m_encima[fila][columna]->setVisible(encima != nullptr);
}

void VistaJuego::setVelocidad(double celdasPorSegundo)
{
    if (celdasPorSegundo > 0)
        m_velocidad = celdasPorSegundo;
}

bool VistaJuego::esTransitable(int fila, int columna) const
{
    return m_mapa && m_mapa->esTransitable(fila, columna);
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
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        //si va a mitad de un paso, la bomba cae en la celda mas cercana
        int f = m_filaJugador, c = m_colJugador;
        if (m_pasoActual != Ninguna && m_progreso >= 0.5) {
            if (m_pasoActual == Arriba) f--;
            if (m_pasoActual == Abajo) f++;
            if (m_pasoActual == Izquierda) c--;
            if (m_pasoActual == Derecha) c++;
        }
        emit pedirBomba(f, c);
        return;
    }
    Direccion d = direccionDeTecla(event->key());
    if (d == Ninguna) {
        QGraphicsView::keyPressEvent(event);
        return;
    }
    if (event->isAutoRepeat())
        return;
    m_teclaActual = d;
    if (m_pasoActual == Ninguna) {
        m_mirando = d;
        actualizarSpriteJugador();
    }
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
    m_mirando = m_teclaActual;
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

    double segundos = m_reloj.restart() / 1000.0;
    m_progreso += m_velocidad * segundos;
    m_tiempoAnim += segundos;
    if (m_progreso >= 1.0) {
        m_filaJugador += df;
        m_colJugador += dc;
        m_jugador->setPos(aPixel(m_colJugador), aPixel(m_filaJugador));
        m_pasoActual = Ninguna;
        emit jugadorLlegoA(m_filaJugador, m_colJugador);
        if (!intentarIniciarPaso()) {
            m_timer->stop();
            m_tiempoAnim = 0.0;
        }
        actualizarSpriteJugador();
        return;
    }
    actualizarSpriteJugador();

    qreal x = aPixel(m_colJugador) + dc * m_progreso * TAM_CELDA;
    qreal y = aPixel(m_filaJugador) + df * m_progreso * TAM_CELDA;
    m_jugador->setPos(x, y);
}
