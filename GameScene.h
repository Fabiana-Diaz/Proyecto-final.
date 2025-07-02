#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "goku.h"  // Clase Goku personalizada

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

private:
    jugador* jugadorGoku;

};

#endif // GAMESCENE_H
