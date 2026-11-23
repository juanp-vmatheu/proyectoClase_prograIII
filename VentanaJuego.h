#ifndef VENTANAJUEGO_H
#define VENTANAJUEGO_H

#include <QMainWindow>

#include "Tipos.h"

class QGraphicsScene;
class VistaJuego;

class VentanaJuego : public QMainWindow
{
    Q_OBJECT

public:
    static const int FILAS = 11;
    static const int COLUMNAS = 13;

    explicit VentanaJuego(QWidget *parent = nullptr);
    ~VentanaJuego() override;

    VentanaJuego(const VentanaJuego &) = delete;
    VentanaJuego &operator=(const VentanaJuego &) = delete;

    QGraphicsScene *escena() const;

private:
    VistaJuego *m_vista;

    CeldaMapa **m_mapa = nullptr;
};

#endif
