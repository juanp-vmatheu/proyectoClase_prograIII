#ifndef MAPATEMPORAL_H
#define MAPATEMPORAL_H

#include "Tipos.h"

// TEMPORAL (Persona B): genera una matriz de prueba para poder dibujar y
// moverse mientras Persona A termina MapaBomberman::generarNivel.
// Cuando exista MapaBomberman, este archivo se borra.
namespace MapaTemporal {

// Reserva una matriz filas x columnas con new y la llena como el nivel 1:
// bordes y pilares fijos, zona libre de aparicion en (1,1), y
// destructibles en ~porcentaje% de las celdas disponibles.
CeldaMapa **crear(int filas, int columnas, int porcentajeDestructibles);

// Libera cada fila con delete[], luego el arreglo de filas, y deja nullptr.
void liberar(CeldaMapa **&mapa, int filas);

// Imprime la matriz con qDebug(): # muro fijo, + destructible, R reforzado, . vacio
void imprimir(CeldaMapa **mapa, int filas, int columnas);

}

#endif // MAPATEMPORAL_H
