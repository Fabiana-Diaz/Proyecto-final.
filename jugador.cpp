#include "jugador.h"
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPainterPath>
#include "Plataforma.h"
#include "Banglades.h"

// Constructor
jugador::jugador(float x, float y)
    : QObject(), QGraphicsPixmapItem(),
    personaje(x, y),
    velocidadY(0), enElAire(false), teclaPresionada(false),
    animacionEnProgreso(false), estaMuerto(false), saltoPermitido(true),
    enPlataforma(false), velocidadMovimiento(4.0f), vida(200), victoriaMostrada(false)
{
    // Sprites de Goku (quieto, preparación, movimiento, salto, muerto)
    spriteQuieto      = QPixmap(":/sprites/goku_frente.png");         // sprite de pie (quieto)
    spritePreparacion = QPixmap(":/sprites/goku_izq2.png");
    spriteCaminando   = QPixmap(":/sprites/goku_izquierda.png");
    spriteSalto       = QPixmap(":/sprites/goku_salto.png");
    spriteMuerto      = QPixmap(":/sprites/goku_muerto.png");
    spriteVictoria = QPixmap(":/sprites/goku_victoria.png");
    if (spriteVictoria.isNull()) {
        qDebug() << "❌ No se pudo cargar spriteVictoria";
    } else {
        qDebug() << "✅ spriteVictoria cargado correctamente";
    }

    // Sprites de ataque
    spritePuno1    = QPixmap(":/sprites/goku_puno1.png");           // Brazo listo para golpear
    spritePuno2    = QPixmap(":/sprites/goku_puno2.png");           // Puño extendido
    spriteEspecial = QPixmap(":/sprites/goku_ataque.png");          // Ataque especial

    setPixmap(spriteQuieto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    cambioSpriteTimer = new QTimer(this);
    cambioSpriteTimer->setSingleShot(true);
    connect(cambioSpriteTimer, &QTimer::timeout, this, [=]() {
        if (!estaMuerto && !victoriaMostrada && spriteCaminando.cacheKey() != pixmap().cacheKey()) {
            setPixmap(spriteCaminando.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        animacionEnProgreso = false;
    });
}

void jugador::keyPressEvent(QKeyEvent* event) {
    if (estaMuerto || victoriaMostrada) return;

    // ATAQUE NORMAL: Z (solo una vez por pulsación)
    if (event->key() == Qt::Key_Z && !enAtaque && !zPresionada) {
        zPresionada = true;
        enAtaque = true;
        setPixmap(spritePuno1.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QTimer::singleShot(80, this, [this]() {
            setPixmap(spritePuno2.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            for (QGraphicsItem *item : collidingItems()) {
                Banglades* enemigo = dynamic_cast<Banglades*>(item);
                if (enemigo) {
                    enemigo->recibirGolpe(10);
                }
            }
            QTimer::singleShot(100, this, [this]() {
                setPixmap(spriteQuieto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                enAtaque = false;
            });
        });
        return;
    }
// MOVIMIENTO IZQUIERDA/DERECHA
    if ((event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) && !teclaPresionada) {
        setPixmap(spritePreparacion.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        animacionEnProgreso = true;
        cambioSpriteTimer->start(150);
        teclaPresionada = true;

        if (event->key() == Qt::Key_Left) {
            moverIzquierda();
        } else {
            moverDerecha();
        }
        return;
    }

    // SALTO
    if (event->key() == Qt::Key_Space && !enElAire && saltoPermitido) {
        saltar();
        saltoPermitido = false;
        setPixmap(spriteSalto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug() << "⬆ Goku saltó con dirección:" << ultimaDireccion;
        return;
    }
}
void jugador::keyReleaseEvent(QKeyEvent* event) {
    if (estaMuerto || victoriaMostrada) return;

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        setPixmap(spritePreparacion.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        cambioSpriteTimer->stop();
        teclaPresionada = false;
        animacionEnProgreso = false;
    }
    if (event->key() == Qt::Key_Space) {
        saltoPermitido = true;
    }
    if (event->key() == Qt::Key_Z) {
        zPresionada = false;
    }
}


void jugador::aplicarFisica() {
    if (estaMuerto || victoriaMostrada) return;

    if (enElAire) {
        velocidadY += 0.7;
        setY(y() + velocidadY);

        // Movimiento horizontal durante salto
        if (ultimaDireccion == "derecha") {
            setX(x() + velocidadMovimiento);
        } else if (ultimaDireccion == "izquierda") {
            setX(x() - velocidadMovimiento);
        }

        qDebug() << "⬇ Goku cayendo: Y =" << y() << ", VelY =" << velocidadY << ", Dirección:" << ultimaDireccion;
    } else {
        velocidadY = 0;
    }

    if (y() >= 580 && !estaMuerto) {
        estaMuerto = true;
        setPixmap(spriteMuerto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        update();
        QMessageBox::information(nullptr, "Game Over", "💀 Goku cayó fuera de la plataforma.");
    }
}
void jugador::verificarColisionInicial(const QList<Plataforma*>& plataformas) {
    for (Plataforma* plat : plataformas) {
        QRectF pies = mapToScene(shape()).boundingRect();
        QRectF platRect = plat->sceneBoundingRect();

        bool encima =
            pies.bottom() >= platRect.top() &&
            pies.bottom() <= platRect.top() + 15 &&
            pies.right() > platRect.left() &&
            pies.left() < platRect.right();

        if (encima) {
            setY(platRect.top() - boundingRect().height());
            enElAire = false;
            velocidadY = 0;
            enPlataforma = true;
            qDebug() << "✅ Goku inicia correctamente sobre la plataforma.";
            return;
        }
    }

    enElAire = true;
    enPlataforma = false;
    qDebug() << "⚠ Goku inicia en el aire.";
}
void jugador::resolverColision(QGraphicsItem* plataforma) {
    if (victoriaMostrada) return;

    QRectF pies = mapToScene(shape()).boundingRect();
    QRectF plat = plataforma->sceneBoundingRect();

    bool estaSobre =
        velocidadY > 0 &&
        pies.bottom() >= plat.top() &&
        pies.bottom() <= plat.top() + 15 &&
        pies.right() > plat.left() &&
        pies.left() < plat.right();

    if (estaSobre) {
        setY(plat.top() - boundingRect().height());
        velocidadY = 0;
        enElAire = false;
        saltoPermitido = true;

        if (pixmap().cacheKey() != spritePreparacion.cacheKey()) {
            setPixmap(spritePreparacion.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        qDebug() << "✅ Goku aterrizó correctamente sobre la plataforma.";
    }
}

