#ifndef ESTRELLA_H
#define ESTRELLA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class jugador;

class Estrella : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Estrella(float x, float y, jugador* objetivo);

private slots:
    void caer();

private:
    QTimer* timerCaida;
    float velocidadCaida = 3.0f;
    jugador* objetivoGoku;
};

#endif // ESTRELLA_H
