#ifndef VENTANAJUEGO_H
#define VENTANAJUEGO_H

#include <QMainWindow>

class QGraphicsView;
class QGraphicsScene;

// Ventana principal del juego (Persona B).
// Contiene una QGraphicsView que muestra la QGraphicsScene donde
// se dibujaran el mapa, el jugador, las bombas y los enemigos.
class VentanaJuego : public QMainWindow
{
    Q_OBJECT

public:
    // Tamaño del tablero en casillas y tamaño de cada casilla en píxeles
    static const int COLUMNAS = 15;
    static const int FILAS = 13;
    static const int TAM_CASILLA = 48;

    explicit VentanaJuego(QWidget *parent = nullptr);
    ~VentanaJuego() override;

    QGraphicsScene *escena() const { return m_escena; }

private:
    void configurarEscena();
    void configurarVista();

    QGraphicsScene *m_escena;
    QGraphicsView *m_vista;
};

#endif // VENTANAJUEGO_H
