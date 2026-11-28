#include "ColaBombas.h"

#include <QTimer>

ColaBombas::ColaBombas(MapaBomberman *mapa, QObject *parent)
    : QObject(parent), m_mapa(mapa)
{
}

ColaBombas::~ColaBombas()
{
    limpiar();
}

//Pone una bomba en la celda y arranca su mecha.
//Se agrega al inicio de la lista enlazada.
bool ColaBombas::colocar(int f, int c, int alcance)
{
    if (m_mapa == nullptr || !m_mapa->ponerBomba(f, c)) {
        return false;
    }

    NodoBomba *nuevo = new NodoBomba;
    nuevo->fila = f;
    nuevo->columna = c;
    nuevo->alcance = alcance;
    //como maximo hay llama en el centro y "alcance" celdas en cada direccion
    nuevo->celdasLlama = new Posicion[1 + 4 * alcance];

    nuevo->mecha = new QTimer(this);
    nuevo->mecha->setSingleShot(true);
    connect(nuevo->mecha, &QTimer::timeout, this, [this, nuevo]() {
        explotar(nuevo);
    });

    nuevo->llama = new QTimer(this);
    nuevo->llama->setSingleShot(true);
    connect(nuevo->llama, &QTimer::timeout, this, [this, nuevo]() {
        apagarLlamas(nuevo);
    });

    nuevo->siguiente = m_cabeza;
    m_cabeza = nuevo;

    nuevo->mecha->start(MS_MECHA);
    emit celdaCambio(f, c);
    return true;
}

int ColaBombas::bombasActivas() const
{
    int total = 0;
    for (NodoBomba *aux = m_cabeza; aux != nullptr; aux = aux->siguiente) {
        if (!aux->exploto) {
            ++total;
        }
    }
    return total;
}

NodoBomba *ColaBombas::buscar(int f, int c) const
{
    for (NodoBomba *aux = m_cabeza; aux != nullptr; aux = aux->siguiente) {
        if (!aux->exploto && aux->fila == f && aux->columna == c) {
            return aux;
        }
    }
    return nullptr;
}

void ColaBombas::agregarLlama(NodoBomba *nodo, int f, int c)
{
    if (!m_mapa->ponerLlamas(f, c)) {
        return;
    }
    nodo->celdasLlama[nodo->cantLlamas].filas = f;
    nodo->celdasLlama[nodo->cantLlamas].columnas = c;
    nodo->cantLlamas++;
    emit celdaCambio(f, c);
    emit llamaEn(f, c);
}

//La llama sale en cruz desde la bomba:
// - se detiene en un muro fijo
// - un bloque destructible recibe dano y la llama se detiene ahi
// - si toca otra bomba, esa bomba explota tambien (reaccion en cadena)
void ColaBombas::explotar(NodoBomba *nodo)
{
    if (nodo == nullptr || nodo->exploto) {
        return;
    }
    nodo->exploto = true;
    nodo->mecha->stop();
    m_mapa->quitarBomba(nodo->fila, nodo->columna);

    agregarLlama(nodo, nodo->fila, nodo->columna);

    const int df[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    for (int d = 0; d < 4; ++d) {
        for (int paso = 1; paso <= nodo->alcance; ++paso) {
            int f = nodo->fila + df[d] * paso;
            int c = nodo->columna + dc[d] * paso;

            if (!m_mapa->enLimites(f, c) || m_mapa->esMuroFijo(f, c)) {
                break;
            }

            if (m_mapa->esDestructible(f, c)) {
                ResultadoDaño r = m_mapa->dañarBloque(f, c);
                emit celdaCambio(f, c);
                if (r == ResultadoDaño::Destruido) {
                    emit bloqueDestruido(f, c);
                    agregarLlama(nodo, f, c);
                }
                break;
            }

            NodoBomba *otra = buscar(f, c);
            if (otra != nullptr) {
                explotar(otra);
                break;
            }

            agregarLlama(nodo, f, c);
        }
    }

    nodo->llama->start(MS_LLAMA);
}

//Se acaba el tiempo de la llama: se limpian las celdas
//y el nodo sale de la lista.
void ColaBombas::apagarLlamas(NodoBomba *nodo)
{
    for (int i = 0; i < nodo->cantLlamas; ++i) {
        int f = nodo->celdasLlama[i].filas;
        int c = nodo->celdasLlama[i].columnas;
        m_mapa->quitarLlama(f, c);
        emit celdaCambio(f, c);
    }
    quitarDeLista(nodo);
    borrarNodo(nodo);
}

void ColaBombas::quitarDeLista(NodoBomba *nodo)
{
    if (m_cabeza == nodo) {
        m_cabeza = nodo->siguiente;
        return;
    }
    NodoBomba *aux = m_cabeza;
    while (aux != nullptr && aux->siguiente != nodo) {
        aux = aux->siguiente;
    }
    if (aux != nullptr) {
        aux->siguiente = nodo->siguiente;
    }
}

//Libera la memoria de un nodo. Los QTimer se borran con deleteLater()
//porque este metodo se llama desde el timeout del mismo timer, y borrar
//un objeto de Qt dentro de su propia senal puede cerrar el programa.
void ColaBombas::borrarNodo(NodoBomba *nodo)
{
    nodo->mecha->stop();
    nodo->llama->stop();
    nodo->mecha->disconnect();
    nodo->llama->disconnect();
    nodo->mecha->deleteLater();
    nodo->llama->deleteLater();
    nodo->mecha = nullptr;
    nodo->llama = nullptr;

    delete[] nodo->celdasLlama;
    nodo->celdasLlama = nullptr;

    delete nodo;
}

void ColaBombas::limpiar()
{
    while (m_cabeza != nullptr) {
        NodoBomba *aux = m_cabeza;
        m_cabeza = m_cabeza->siguiente;

        if (m_mapa != nullptr) {
            m_mapa->quitarBomba(aux->fila, aux->columna);
            for (int i = 0; i < aux->cantLlamas; ++i) {
                m_mapa->quitarLlama(aux->celdasLlama[i].filas, aux->celdasLlama[i].columnas);
            }
        }
        borrarNodo(aux);
    }
    m_cabeza = nullptr;
}
