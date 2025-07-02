#include "personaje.h"

personaje::personaje(float x, float y, float masa)
    : posX(x), posY(y), velX(0), velY(0), masa(masa), vida(100)
{
    setPos(x, y);
}

personaje::personaje(float x, float y)
    : personaje(x, y, 1.0f) // llama al otro constructor con masa por defecto 1.0
{}

float personaje::getX() const {
    return posX;
}

float personaje::getY() const {
    return posY;
}

void personaje::recibirDanio(int cantidad, float fuerza) {
    vida -= cantidad;
    float aceleracion = fuerza / masa;
    velX = -aceleracion;
}
