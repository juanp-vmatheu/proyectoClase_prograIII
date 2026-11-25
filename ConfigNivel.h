#ifndef CONFIGNIVEL_H
#define CONFIGNIVEL_H
#include <string>
#include "Tipos.h"

using namespace std;

//eestructura encargada de guardar las configuraciones de los
//distintos niiveles. Esta informacion se envia a
//Mapatemporal


struct ConfigNivel{
    int numero=0;
    string nombre;
    int filas=0;
    int columnas=0;
    ParametrosTerreno terreno;

    int enemigosBasicos = 0;
    int enemigosPerseguidores = 0;
    int enemigosEspectrales = 0;
    double factorVelocidadEnemigos = 1.0;


    int tiempoLimite = 0;
    int eliteAlAgotarTiempo = 0;
    bool enemigosSueltanBonus = false;
};

ConfigNivel obtenerConfigNivel(int numero);

#endif // CONFIGNIVEL_H
