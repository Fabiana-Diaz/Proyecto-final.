#include "akkuman.h"
#include <QDebug>
#include <QGraphicsScene>
#include <algorithm>
#include "Resplandor.h"



Akkuman::Akkuman(float x, float y, float masa, QGraphicsItem* parent)
    : QObject(), personaje(x, y, masa), QGraphicsPixmapItem(parent),
    velocidadX(1.0f), velocidadY(0.8f), direccionX(1), direccionY(1), objetivo(nullptr)
{

    vidaMaxima = 150;
    vida = vidaMaxima;

    setPixmap(QPixmap(":/sprites/akkumam.png").scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);

    timerVuelo = new QTimer(this);
    connect(timerVuelo, &QTimer::timeout, this, &Akkuman::mover);
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &Akkuman::dispararResplandor);
    timerDisparo->start(3000);  // Dispara cada 3 segundos

}

void Akkuman::iniciarVuelo() {
    timerVuelo->start(30);
}

void Akkuman::detenerVuelo() {
    timerVuelo->stop();
}

void Akkuman::mover() {
    // Movimiento vertical oscilante
    int nuevaY = y() + direccionY * 1;
    if (nuevaY <= limiteSuperior || nuevaY >= limiteInferior) {
        direccionY = -direccionY;
    }
    setY(qBound(limiteSuperior, static_cast<float>(nuevaY), limiteInferior));

    // Movimiento horizontal para acercarse a Goku
    if (objetivo) {
        float objetivoX = objetivo->x();
        float distanciaX = objetivoX - x();
        float velocidadX = 1.5f;

        // Limitar movimiento horizontal dentro de límites
        float nuevaX = x();

        if (std::abs(distanciaX) > 20) {
            if (distanciaX > 0) {
                nuevaX = std::min(static_cast<float>(x() + velocidadX), limiteDerecho);
            } else {
                nuevaX = std::max(static_cast<float>(x() - velocidadX), limiteIzquierdo);
            }
        }
        setX(nuevaX);
    }

    qDebug() << "Akkuman pos:" << pos();

}



void Akkuman::aplicarFisica() {
    if (!objetivo) return;

    // Actualizar posiciones usando la física propia
    posX = x();
    posY = y();

    float diffX = objetivo->getX() - posX;
    float diffY = objetivo->getY() - posY;

    direccionX = (diffX > 0) ? 1 : -1;
    direccionY = (diffY > 0) ? 1 : -1;

    posX += velocidadX * direccionX;
    posY += velocidadY * direccionY;

    setPos(posX, posY);

    // Evitar salir de área (ajusta según tu escena)
    if (posX < 150) posX = 150;
    if (posX > 1100) posX = 1100;
    if (posY < 150) posY = 150;
    if (posY > 600) posY = 600;

    setPos(posX, posY);

    // Detectar colisión con Goku
    if (collidesWithItem(objetivo)) {
        qDebug() << "⚠ Akkuman tocó a Goku!";
    }
}

void Akkuman::recibirDanio(int cantidad, float fuerza) {
    vida -= cantidad;
    if (vida < 0) vida = 0;

    // Actualizar barra de vida
    if (barraVida) {
        float anchoMaximo = 150.0f; // ancho original de la barra en pixeles
        float nuevoAncho = (vida * anchoMaximo) / static_cast<float>(vidaMaxima);
        barraVida->setRect(0, 0, nuevoAncho, barraVida->rect().height());
    }

    // Actualizar texto de vida
    if (textoVida) {
        textoVida->setPlainText(QString("Akkuman: %1").arg(vida));
    }

    // Si vida llega a 0, detener vuelo, emitir señal y eliminar objeto seguro
    if (vida == 0) {
        detenerVuelo();
        qDebug() << "✅ Akkuman derrotado!";

        emit enemigoDerrotado(this);  // Señal para GameScene

        if (scene()) {
            scene()->removeItem(this);
        }

        disconnect();  // Desconecta todas las señales para evitar callbacks

        deleteLater(); // Marca para eliminación segura
    }
}

void Akkuman::setObjetivo(jugador* objetivo) {
    this->objetivo = objetivo;
}

void Akkuman::dispararResplandor() {
    if (!objetivo) return;

    // Posición de inicio para disparos
    float startX = x() - 30;
    float startY = y() + boundingRect().height() / 2;

    // Resplandor horizontal (hacia la izquierda)
    Resplandor* resplandorH = new Resplandor(startX, startY, objetivo, DireccionResplandor::Horizontal);
    scene()->addItem(resplandorH);

    // Resplandor vertical (hacia abajo)
    Resplandor* resplandorV = new Resplandor(x(), y() + boundingRect().height(), objetivo, DireccionResplandor::Vertical);
    scene()->addItem(resplandorV);

    qDebug() << "Akkuman disparó resplandores horizontal y vertical!";
}

void Akkuman::setBarraVida(QGraphicsRectItem* barra) {
    barraVida = barra;
}


void Akkuman::setTextoVida(QGraphicsTextItem* texto) {
    this->textoVida = texto;
}
