#ifndef VENTANAJUEGO_H
#define VENTANAJUEGO_H

#include <QMainWindow>
#include "MapaTemporal.h"
#include "Jugador.h"
#include "Tipos.h"

class QGraphicsScene;
class VistaJuego;
class ColaBombas;

class VentanaJuego : public QMainWindow
{
    Q_OBJECT

public:
    static const int FILAS = 11;
    static const int COLUMNAS = 13;
    static const int MS_RELOJ = 100;

    explicit VentanaJuego(QWidget *parent = nullptr);
    ~VentanaJuego() override;

    VentanaJuego(const VentanaJuego &) = delete;
    VentanaJuego &operator=(const VentanaJuego &) = delete;

    QGraphicsScene *escena() const;

private:
    MapaBomberman m_mapa;
    Jugador m_jugador;
    VistaJuego *m_vista;
    ColaBombas *m_bombas;

    void ponerBomba(int f, int c);
    void revisarGolpe(int f, int c);//si hay llama donde esta el jugador pierde vida
    void actualizarTitulo();

};

#endif
