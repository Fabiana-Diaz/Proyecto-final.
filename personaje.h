#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QGraphicsPixmapItem>

class personaje : public QGraphicsPixmapItem {
public:
    personaje(float x, float y);              // Constructor por defecto (usa masa 1.0)
    personaje(float x, float y, float masa);  // Constructor con masa específica

    virtual void aplicarFisica() = 0; // Método abstracto

    float getX() const;
    float getY() const;

    virtual void recibirDanio(int cantidad, float fuerza);

protected:
    float posX, posY;
    float velX, velY;
    float masa;
    int vida;
};

#endif // PERSONAJE_H
