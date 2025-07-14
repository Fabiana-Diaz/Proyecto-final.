#include "venda.h"
#include <QtMath>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

Venda::Venda(jugador* objetivo, float inicioX, float inicioY, Banglades* padreBanglades)
    : objetivoGoku(objetivo), bangladesPadre(padreBanglades),
    angulo(qAtan2(objetivo->y() - inicioY, objetivo->x() - inicioX)),
    radio(0),
    velocidadAngular(0.15),
    tiempoActivo(0)

{
    QPixmap img(":/sprites/vendas.png");
    if (img.isNull()) {
        qDebug() << "❌ No se pudo cargar la imagen de la venda.";
    } else {
        setPixmap(img.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    setPos(inicioX, inicioY);
    centroInicial = QPointF(inicioX, inicioY);
    setZValue(2);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Venda::mover);
    timerMovimiento->start(30);

    qDebug() << "🌀 Venda creada en:" << pos();
}

void Venda::mover() {
    angulo += velocidadAngular;
    radio += 1.5;

    float dx = radio * qCos(angulo);
    float dy = radio * qSin(angulo);
    setPos(centroInicial.x() + dx, centroInicial.y() + dy);

    tiempoActivo += 30;
    // qDebug() << "🌀 Venda moviéndose en:" << pos();

    // --- Colisión con Goku ---
    if (objetivoGoku && collidesWithItem(objetivoGoku)) {
        qDebug() << "🎯 Venda impactó a Goku!";
        objetivoGoku->recibirDanio(20);

        detenerse();  // Llama tu método de limpieza
        return;
    }

    // --- Fuera de la escena ---
    if (scene()) {
        QRectF sceneRect = scene()->sceneRect();
        if (!sceneRect.intersects(this->sceneBoundingRect())) {
            qDebug() << "🚫 Venda fuera de la escena.";
            detenerse();
            return;
        }
    }

    // --- Por tiempo ---
    if (tiempoActivo > 4000) {
        qDebug() << "⏳ Venda eliminada por tiempo";
        detenerse();
        return;
    }
}


void Venda::detenerse() {
    qDebug() << "⏸️ Venda::detenerse() llamada para:" << static_cast<const void*>(this);

    if (yaDetenida) {
        qDebug() << "⚠️ Venda ya estaba detenida:" << static_cast<const void*>(this);
        return;
    }
    yaDetenida = true;

    if (timerMovimiento) {
        timerMovimiento->stop();
        timerMovimiento->disconnect();
        qDebug() << "🛑 Timer de movimiento detenido y desconectado para venda:" << static_cast<const void*>(this);
    }

    if (scene()) {
        scene()->removeItem(this);
        qDebug() << "🧹 Venda removida de la escena:" << static_cast<const void*>(this);
    }

    qDebug() << "🔥 Venda marcada para borrarse (deleteLater):" << static_cast<const void*>(this);
    deleteLater();
}

Venda::~Venda() {
    if (bangladesPadre) {
        bangladesPadre->eliminarVenda(this);
    }
    qDebug() << "🔥 Venda destruida correctamente:" << static_cast<const void*>(this);
}


