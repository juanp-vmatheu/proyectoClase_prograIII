#ifndef MENUINICIO_H
#define MENUINICIO_H

#include <QWidget>

//Menu de inicio: titulo y tres botones
class MenuInicio : public QWidget
{
    Q_OBJECT

public:
    explicit MenuInicio(QWidget *parent = nullptr);

signals:
    void jugar();

private:
    void mostrarInstrucciones();
};

#endif // MENUINICIO_H
