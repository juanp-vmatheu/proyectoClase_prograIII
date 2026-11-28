#ifndef VISTAJUEGO_H
#define VISTAJUEGO_H

#include <QElapsedTimer>
#include <QGraphicsView>
#include <QPixmap>
#include "Tipos.h"
#include "MapaTemporal.h"

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsPixmapItem;
class QTimer;

class VistaJuego : public QGraphicsView
{
    Q_OBJECT

public:
    explicit VistaJuego(QWidget *parent = nullptr);
    ~VistaJuego() override;

    VistaJuego(const VistaJuego &) = delete;
    VistaJuego &operator=(const VistaJuego &) = delete;

    void cargarMapa(MapaBomberman *mapa, int filaJugador = 1, int colJugador = 1);

    void actualizarCelda(int fila, int columna);

    int filaJugador() const {
        return m_filaJugador; }
    int columnaJugador() const {
        return m_colJugador; }

    void setVelocidad(double celdasPorSegundo);

signals:
    void pedirBomba(int fila, int columna);//se presiono espacio
    void jugadorLlegoA(int fila, int columna);//termino un paso

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void tickMovimiento();

private:
    enum Direccion { Ninguna, Arriba, Abajo, Izquierda, Derecha };

    static Direccion direccionDeTecla(int tecla);
    static QColor colorDe(const CeldaMapa &celda);

    //Sprites: se recortan de las hojas de assets/sprites/zelda.
    //Si una hoja no carga se usa el color de colorDe() como respaldo.
    static QPixmap recortar(const QPixmap &hoja, int x, int y, int ancho, int alto,
                            const QColor &respaldo);
    void cargarSprites();
    const QPixmap &spriteDe(const CeldaMapa &celda) const;
    const QPixmap *spriteEncimaDe(const CeldaMapa &celda) const;
    void cargarSpritesLink();
    void actualizarSpriteJugador();

    void liberarItems();
    bool esTransitable(int fila, int columna) const;
    bool intentarIniciarPaso();

    QGraphicsScene *m_escena;
    QTimer *m_timer;

    MapaBomberman *m_mapa = nullptr;
    int m_filas = 0;
    int m_columnas = 0;

    QGraphicsPixmapItem ***m_items = nullptr;   //suelo o bloque de cada celda
    QGraphicsPixmapItem ***m_encima = nullptr;  //bomba o llama encima de la celda

    QPixmap m_spPasto;
    QPixmap m_spMuro;
    QPixmap m_spArbusto;
    QPixmap m_spRoca;
    QPixmap m_spRocaDanada;
    QPixmap m_spBomba;
    QPixmap m_spLlama;

    //Link: 4 direcciones (arriba, abajo, izquierda, derecha) x 8 cuadros.
    //El cuadro 0 es Link quieto, los demas son la caminata.
    static const int CUADROS_LINK = 8;
    QPixmap m_spLink[4][CUADROS_LINK];
    Direccion m_mirando = Abajo;
    double m_tiempoAnim = 0.0;

    QGraphicsPixmapItem *m_jugador = nullptr;
    int m_filaJugador = 0;
    int m_colJugador = 0;

    Direccion m_teclaActual = Ninguna;
    Direccion m_pasoActual = Ninguna;
    double m_progreso = 0.0;
    double m_velocidad = 4.0;
    QElapsedTimer m_reloj;
};

#endif
