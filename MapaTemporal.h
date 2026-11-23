#ifndef MAPATEMPORAL_H
#define MAPATEMPORAL_H

#include "Tipos.h"

namespace MapaTemporal {
CeldaMapa **crear(int filas, int columnas, int porcentajeDestructibles);

void liberar(CeldaMapa **&mapa, int filas);

void imprimir(CeldaMapa **mapa, int filas, int columnas);

}

#endif
