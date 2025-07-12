#ifndef PERSONAJE_H
#define PERSONAJE_H

class personaje {
public:
    personaje(float x, float y, float masa);   // Constructor principal
    personaje(float x, float y);               // Constructor alternativo sin masa

    float getX() const;                        // Getter de X
    float getY() const;                        // Getter de Y

    virtual void aplicarFisica() = 0;          // Método puro para física
    virtual void recibirDanio(int cantidad, float fuerza);  // Recibir daño

protected:
    float posX, posY;
    float velX, velY;
    float masa;
    int vida;
};

#endif // PERSONAJE_H

