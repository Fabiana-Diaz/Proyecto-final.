#include "GameWindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QGraphicsView(parent) {
    setFixedSize(800, 600);
    GameScene *scene = new GameScene();
    setScene(scene);
}
