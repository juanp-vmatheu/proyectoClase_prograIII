#ifndef VISTAJUEGO_H
#define VISTAJUEGO_H

#include <QElapsedTimer>
#include <QGraphicsView>
#include "Tipos.h"

class QGraphicsScene;
class QGraphicsRectItem;
class QTimer;

// Vista del tablero (Persona B): dibuja la matriz CeldaMapa** con rectangulos
// y mueve al jugador celda por celda con el teclado y un QTimer.
// No es duena de la matriz: solo la lee (la duena sera MapaBomberman/JuegoEngine).
class VistaJuego : public QGraphicsView
{
    Q_OBJECT

public:
    explicit VistaJuego(QWidget *parent = nullptr);
    ~VistaJuego() override;

    VistaJuego(const VistaJuego &) = delete;
    VistaJuego &operator=(const VistaJuego &) = delete;

    // Dibuja la matriz completa y coloca al jugador en (filaJugador, colJugador).
    void cargarMapa(CeldaMapa **mapa, int filas, int columnas,
                    int filaJugador = 1, int colJugador = 1);

    // Vuelve a pintar una sola celda (por ejemplo cuando se destruye un bloque).
    void actualizarCelda(int fila, int columna);

    int filaJugador() const { return m_filaJugador; }
    int columnaJugador() const { return m_colJugador; }

    // Celdas por segundo (el power-up de velocidad lo podra subir).
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

    // TEMPORAL: cuando exista MapaBomberman se reemplaza por mapa->esTransitable(f, c).
    bool esTransitable(int fila, int columna) const;

    void liberarItems();
    bool intentarIniciarPaso();

    QGraphicsScene *m_escena;
    QTimer *m_timer;

    CeldaMapa **m_mapa = nullptr;
    int m_filas = 0;
    int m_columnas = 0;

    // Matriz paralela de punteros a los rectangulos dibujados, para poder
    // recolorearlos o borrarlos despues sin recorrer la escena.
    QGraphicsRectItem ***m_items = nullptr;

    QGraphicsRectItem *m_jugador = nullptr;
    int m_filaJugador = 0;
    int m_colJugador = 0;

    Direccion m_teclaActual = Ninguna;   // tecla mantenida mas reciente
    Direccion m_pasoActual = Ninguna;    // paso en curso (animacion)
    double m_progreso = 0.0;             // 0..1 dentro del paso actual
    double m_velocidad = 4.0;            // celdas por segundo
    QElapsedTimer m_reloj;               // mide el tiempo real entre ticks
};

#endif // VISTAJUEGO_H
