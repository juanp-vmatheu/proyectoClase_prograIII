#ifndef COLABOMBAS_H
#define COLABOMBAS_H

#include <QObject>
#include "Tipos.h"
#include "MapaTemporal.h"

class QTimer;

//Explicacion: cada bomba puesta en el mapa es un nodo de una lista
//enlazada simple. Cada nodo tiene dos QTimer de un solo disparo:
// - mecha: cuando se acaba, la bomba explota
// - llama: cuando se acaba, se apagan las llamas y se borra el nodo
struct NodoBomba {
    int fila = 0;
    int columna = 0;
    int alcance = 1;
    bool exploto = false;

    QTimer *mecha = nullptr;
    QTimer *llama = nullptr;

    //celdas donde quedaron llamas (arreglo dinamico)
    Posicion *celdasLlama = nullptr;
    int cantLlamas = 0;

    NodoBomba *siguiente = nullptr;
};

class ColaBombas : public QObject
{
    Q_OBJECT

public:
    static const int MS_MECHA = 3000;
    static const int MS_LLAMA = 500;

    explicit ColaBombas(MapaBomberman *mapa, QObject *parent = nullptr);
    ~ColaBombas() override;

    ColaBombas(const ColaBombas &) = delete;
    ColaBombas &operator=(const ColaBombas &) = delete;

    bool colocar(int f, int c, int alcance);//true si se pudo poner la bomba
    int bombasActivas() const;//bombas que todavia no explotan
    void limpiar();//borra todas las bombas (por ejemplo al cambiar de nivel)

signals:
    void celdaCambio(int f, int c);//la vista debe redibujar esa celda
    void llamaEn(int f, int c);//hay una llama nueva en esa celda
    void bloqueDestruido(int f, int c);

private:
    void explotar(NodoBomba *nodo);
    void apagarLlamas(NodoBomba *nodo);
    void agregarLlama(NodoBomba *nodo, int f, int c);
    NodoBomba *buscar(int f, int c) const;
    void quitarDeLista(NodoBomba *nodo);
    void borrarNodo(NodoBomba *nodo);

    MapaBomberman *m_mapa;
    NodoBomba *m_cabeza = nullptr;
};

#endif // COLABOMBAS_H
