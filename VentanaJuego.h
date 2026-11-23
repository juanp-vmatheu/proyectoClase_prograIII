#ifndef VENTANAJUEGO_H
#define VENTANAJUEGO_H

#include <QMainWindow>

#include "Tipos.h"

class QGraphicsScene;
class VistaJuego;

// Ventana principal del juego (Persona B).
// Su widget central es VistaJuego, que dibuja el mapa y mueve al jugador.
class VentanaJuego : public QMainWindow
{
    Q_OBJECT

public:
    // Nivel 1: matriz de 11 filas x 13 columnas
    static const int FILAS = 11;
    static const int COLUMNAS = 13;

    explicit VentanaJuego(QWidget *parent = nullptr);
    ~VentanaJuego() override;

    VentanaJuego(const VentanaJuego &) = delete;
    VentanaJuego &operator=(const VentanaJuego &) = delete;

    QGraphicsScene *escena() const;

private:
    VistaJuego *m_vista;

    // TEMPORAL: la matriz la tendra MapaBomberman (dentro de JuegoEngine).
    CeldaMapa **m_mapa = nullptr;
};

#endif // VENTANAJUEGO_H
