#ifndef GAMESCENE2_H
#define GAMESCENE2_H

#include <QGraphicsScene>
#include "plataforma.h"
#include "jugador.h"
#include "akkuman.h"  // Incluye la clase Akkuman
//#include <QMediaPlayer>
class GameScene2 : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene2(QObject *parent = nullptr);
    void focusGoku();
    void mostrarVictoria();


protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;


private:
    Plataforma* plataformaPrincipal;
    jugador* jugadorGoku;
    QList<Plataforma*> plataformas;
    Akkuman* akkuman;




};

#endif // GAMESCENE2_H
