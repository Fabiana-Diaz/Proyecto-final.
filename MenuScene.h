#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QObject>

class MenuScene : public QGraphicsScene {
    Q_OBJECT
public:
    MenuScene(QObject* parent = nullptr);

signals:
    void nivelSeleccionado(int nivel);
};

#endif // MENUSCENE_H
