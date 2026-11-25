#ifndef MAPATEMPORAL_H
#define MAPATEMPORAL_H

#include "Tipos.h"

class MapaBomberman
{
    CeldaMapa** mapa;
    int filas;
    int columnas;
public:

    //Ciclo de vida: aqui se encuentra el constructor, dstructor y lineeas que
    //evitan que se copie el mapa
    MapaBomberman(int nFilas, int nCol);
    ~MapaBomberman();
    MapaBomberman(const MapaBomberman&)=delete;
    MapaBomberman& operator=(const MapaBomberman&)=delete;

    //---Generacion----
    void generarNivel(const ParametrosTerreno& parametros);//Genera la creacion del terreo. llamdo a otras funciones

    //Funciones que nos ayudaran a realizar consultas
    //a lo largo de las partidas:
    //"const" ayuda a que se consulte y se promote no cambiar el mapa
    bool enLimites(int f, int c) const;//verifica si las posicion existe en el mapa
    bool esTransitable(int f, int c) const;//dice si el jugador puede caminar por esa celda
    bool esTransitableEspectral(int f, int c) const;//Unicamente para el enemigo espectarl que atraviesa bloques
    bool esMuroFijo(int f, int c) const;//Verifica si cierto muro es estructible
    bool esDestructible(int f, int c)const;//veerifica si se puede destruir un muro
    bool esZonaAparicion(int f, int c) const;//verifica las celdas libres para el movimiento del jugador
    const CeldaMapa& celda(int f, int c)const;//dvueelve una celda para leerla
    Posicion celdaLibreAleatoria()const;//devuelve celdas para ubicar enemigos
    int getFilas() const;
    int getColumnas()const;
    Posicion posicionAparicion() const;
    //funciones que modifican el mapa:
    ResultadoDaño dañarBloque(int f, int c);
    bool ponerBomba(int f, int c);
    bool quitarBomba(int f, int c);
    bool ponerLlamas(int f, int c);
    void quitarLlama(int f, int c);
    Contenido recogerContenido(int f, int c);

    //Funcion para probar en consola;
    void imprimir();


private:
    //Funciones privadas: Son parte de los procesos internos
    //del mapa y el conjunto de ellas generan un nivel:
    void vaciarCelda();//listoo
    void colocarCeldasFijas();//listo
    void reservarZonaAparicion();
    int contarCeldasDisponibles() const;
    void colocarBloquesDestructibles(int cantidad);
    void marcarCeldasReforzadas(int cantidad);
    void colocarContenido(Contenido tipo, int cantidad);

    Posicion elegirCelda(TipoCelda tipoRequerido, bool soloSinContenido)const;
    //funciones para liberar memoria;
    void liberar();

    //datos


};

#endif
