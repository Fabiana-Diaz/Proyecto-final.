#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class jugador : public QObject, public personaje {
    Q_OBJECT

public:
    jugador(float x, float y);

    QGraphicsPixmapItem* getSprite();
    void keyPressEvent(QKeyEvent* event);
    void aplicarFisica();

private:
    float velocidadY;
    bool enElAire;
    QGraphicsPixmapItem* sprite;
};

#endif // JUGADOR_H

