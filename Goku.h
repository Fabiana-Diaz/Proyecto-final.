#ifndef GOKU_H
#define GOKU_H

#include "jugador.h"

#include <QKeyEvent>

class goku : public jugador {
    Q_OBJECT

public:
    goku(float x, float y);
    void keyPressEvent(QKeyEvent* event);
    void aplicarFisica();

private:
    float velocidadY;
    bool enElAire;
};

#endif // GOKU_H
