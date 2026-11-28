#include "MenuInicio.h"

#include <QLabel>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>

MenuInicio::MenuInicio(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Bomberman - Programacion III");
    setFixedSize(400, 350);
    //fondo verde como el pasto del mapa
    QPalette paleta = palette();
    paleta.setColor(QPalette::Window, QColor(60, 140, 70));
    setPalette(paleta);
    setAutoFillBackground(true);

    QLabel *titulo = new QLabel("BOMBERMAN");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("color: white; font-size: 36px; font-weight: bold;");

    QLabel *subtitulo = new QLabel("Programacion III");
    subtitulo->setAlignment(Qt::AlignCenter);
    subtitulo->setStyleSheet("color: white; font-size: 16px;");

    QPushButton *btnJugar = new QPushButton("Jugar");
    QPushButton *btnInstrucciones = new QPushButton("Como jugar");
    QPushButton *btnSalir = new QPushButton("Salir");

    QString estiloBoton = "background-color: rgb(240, 240, 240); color: black; font-size: 16px; padding: 8px;";
    btnJugar->setStyleSheet(estiloBoton);
    btnInstrucciones->setStyleSheet(estiloBoton);
    btnSalir->setStyleSheet(estiloBoton);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(80, 30, 80, 30);
    layout->setSpacing(12);
    layout->addWidget(titulo);
    layout->addWidget(subtitulo);
    layout->addStretch();
    layout->addWidget(btnJugar);
    layout->addWidget(btnInstrucciones);
    layout->addWidget(btnSalir);

    connect(btnJugar, &QPushButton::clicked, this, &MenuInicio::jugar);
    connect(btnInstrucciones, &QPushButton::clicked, this, &MenuInicio::mostrarInstrucciones);
    connect(btnSalir, &QPushButton::clicked, this, &MenuInicio::close);
}

void MenuInicio::mostrarInstrucciones()
{
    QString texto =
        "CONTROLES\n"
        "Flechas o WASD: mover al jugador\n"
        "Espacio: poner una bomba\n\n"
        "BOMBAS\n"
        "La bomba explota a los 3 segundos y la llama sale en cruz.\n"
        "Los arbustos se rompen de un golpe y las rocas de dos.\n"
        "Los muros de piedra no se rompen.\n\n"
        "VIDAS\n"
        "Tienes 3 vidas. Si la llama te toca pierdes una.";

    QMessageBox::information(this, "Como jugar", texto);
}
