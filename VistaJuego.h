#ifndef VISTAJUEGO_H
#define VISTAJUEGO_H

#include <QElapsedTimer>
#include <QGraphicsView>
#include "Tipos.h"
#include "MapaTemporal.h"

class QGraphicsScene;
class QGraphicsRectItem;
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

    int filaJugador() const { return m_filaJugador; }
    int columnaJugador() const { return m_colJugador; }

    void setVelocidad(double celdasPorSegundo);

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

    void liberarItems();
    bool esTransitable(int fila, int columna) const;
    bool intentarIniciarPaso();

    QGraphicsScene *m_escena;
    QTimer *m_timer;

    MapaBomberman *m_mapa = nullptr;
    int m_filas = 0;
    int m_columnas = 0;

    QGraphicsRectItem ***m_items = nullptr;

    QGraphicsRectItem *m_jugador = nullptr;
    int m_filaJugador = 0;
    int m_colJugador = 0;

    Direccion m_teclaActual = Ninguna;
    Direccion m_pasoActual = Ninguna;
    double m_progreso = 0.0;
    double m_velocidad = 4.0;
    QElapsedTimer m_reloj;
};

#endif
