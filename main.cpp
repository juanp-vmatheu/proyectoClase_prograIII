#include <QApplication>

#include "MenuInicio.h"
#include "VentanaJuego.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MenuInicio menu;
    VentanaJuego *juego = nullptr;

    //al presionar Jugar se esconde el menu y se abre el juego
    QObject::connect(&menu, &MenuInicio::jugar, [&]() {
        if (juego == nullptr) {
            juego = new VentanaJuego;
        }
        menu.hide();
        juego->show();
    });

    menu.show();
    int resultado = app.exec();

    delete juego;
    juego = nullptr;
    return resultado;
}
