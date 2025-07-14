#ifndef AKKUMAN_H
#define AKKUMAN_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "personaje.h"
#include "jugador.h"

class Akkuman : public QObject, public personaje, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Akkuman(float x, float y, float masa = 1.0f, QGraphicsItem* parent = nullptr);

    void iniciarVuelo();
    void detenerVuelo();
    void recibirDanio(int cantidad, float fuerza) override;
    void aplicarFisica() override;
    void setObjetivo(jugador* objetivo);
    void dispararResplandor();
    void mover();
    void setBarraVida(QGraphicsRectItem* barra);
    void setTextoVida(QGraphicsTextItem* texto);



signals:
    void enemigoDerrotado(Akkuman* enemigo);

private:
    jugador* objetivo;
    QTimer* timerVuelo;
     QTimer* timerDisparo;

    float velocidadX;
    float velocidadY;
    int direccionX;
    int direccionY; // dirección vertical actual (1 o -1)
    float limiteSuperior = 150.0f;   // Ajusta según la posición de tu plataforma y fondo
    float limiteInferior = 350.0f;   // Ajusta según la posición de tu plataforma y fondo
    float limiteIzquierdo = 200.0f;  // Límite izquierdo para que no salga del escenario
    float limiteDerecho = 1100.0f;   // Límite derecho para que no salga del escenario

    QGraphicsRectItem* barraVida = nullptr;
    QGraphicsTextItem* textoVida = nullptr;

    int vidaMaxima = 150;
    int vida;  // vida actual

    // Heredados de personaje: posX, posY, velX, velY, masa, vida
};

#endif // AKKUMAN_H
