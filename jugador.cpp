#include "jugador.h"
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPainterPath>
#include "Plataforma.h"
#include "Banglades.h"
#include "akkuman.h"


// Constructor
jugador::jugador(float x, float y)
    : QObject(), QGraphicsPixmapItem(),
    personaje(x, y),
    velocidadY(0), enElAire(false), teclaPresionada(false),
    animacionEnProgreso(false), estaMuerto(false), saltoPermitido(true),
    enPlataforma(false), velocidadMovimiento(4.0f), vida(100), victoriaMostrada(false),
    estadoSuperGolpe(false)
{
    // Sprites de Goku
    spriteQuieto      = QPixmap(":/sprites/goku_frente.png");
    spritePreparacion = QPixmap(":/sprites/goku_izq2.png");
    spriteCaminando   = QPixmap(":/sprites/goku_izquierda.png");
    spriteSalto       = QPixmap(":/sprites/goku_salto.png");
    spriteMuerto      = QPixmap(":/sprites/goku_muerto.png");
    spriteVictoria    = QPixmap(":/sprites/goku_victoria.png");

    // Sprites de ataque
    spritePuno1    = QPixmap(":/sprites/goku_puno1.png");
    spritePuno2    = QPixmap(":/sprites/goku_puno2.png");
    spriteEspecial = QPixmap(":/sprites/super_ataque.png");

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

    // Inicialización del timer para super golpe
    timerSuperGolpe = new QTimer(this);
    timerSuperGolpe->setSingleShot(true);
    connect(timerSuperGolpe, &QTimer::timeout, this, [this]() {
        estadoSuperGolpe = false;
        qDebug() << "Super golpe desactivado";
    });
}


void jugador::keyPressEvent(QKeyEvent* event) {
    if (estaMuerto || victoriaMostrada) return;

    // SUPERGOLPE con tecla X
    if (event->key() == Qt::Key_X && puedeUsarSuperGolpe()) {
        qDebug() << "Supergolpe activado por Goku!";

        setPixmap(spriteEspecial.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));  // Cambia sprite a superataque

        for (QGraphicsItem* item : collidingItems()) {
            Akkuman* enemigoAkkuman = dynamic_cast<Akkuman*>(item);
            if (enemigoAkkuman) {
                enemigoAkkuman->recibirDanio(20, 2.0f);  // Daño aumentado
            }
        }

        // Resetear supergolpe
        superGolpeActivo = false;
        estrellasRecogidas = 0;

        // Cambia sprite o animación especial aquí si quieres

        return;  // No hacer más nada este ciclo
    }

    // ATAQUE NORMAL: Z
    if (event->key() == Qt::Key_Z && !enAtaque && !zPresionada) {
        zPresionada = true;
        enAtaque = true;
        setPixmap(spritePuno1.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        QTimer::singleShot(80, this, [this]() {
            setPixmap(spritePuno2.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            for (QGraphicsItem* item : collidingItems()) {
                // Dañar a Banglades
                Banglades* enemigoBanglades = dynamic_cast<Banglades*>(item);
                if (enemigoBanglades) {
                    qDebug() << "¡Goku golpeó a Banglades!";
                    enemigoBanglades->recibirGolpe(10);
                }
                // Dañar a Akkuman
                Akkuman* enemigoAkkuman = dynamic_cast<Akkuman*>(item);
                if (enemigoAkkuman) {
                    qDebug() << "¡Goku golpeó a Akkuman!";
                    enemigoAkkuman->recibirDanio(10, 1.0f);
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
            qDebug() << "Goku moviéndose a la izquierda";
        } else {
            moverDerecha();
            qDebug() << "Goku moviéndose a la derecha";
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

    // Al soltar tecla izquierda o derecha
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        // Cambiar sprite a preparación o quieto
        setPixmap(spritePreparacion.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        cambioSpriteTimer->stop();      // Detener timer de animación de caminata
        teclaPresionada = false;        // Permitir nuevo movimiento
        animacionEnProgreso = false;    // Indicar que no hay animación de movimiento
    }

    // Al soltar tecla espacio (salto), permitir nuevo salto
    if (event->key() == Qt::Key_Space) {
        saltoPermitido = true;
    }

    // Al soltar tecla Z (ataque), permitir nuevo ataque
    if (event->key() == Qt::Key_Z) {
        zPresionada = false;
    }
}


// ----- NIVEL 1 -----
void jugador::aplicarFisica() {
    if (estaMuerto || victoriaMostrada) return;

    if (enElAire) {
        velocidadY += 0.7;
        setY(y() + velocidadY);

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

void jugador::activarCaidaLibre() {
    if (!enElAire && !enPlataforma) {
        enElAire = true;
    }
}

// -------- NIVEL 2: FÍSICAS Y COLISIONES EXCLUSIVAS --------

void jugador::verificarColisionInicialNivel2(const QList<Plataforma*>& plataformas) {
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
            qDebug() << "✅ Goku inicia correctamente sobre la plataforma (NIVEL 2).";
            return;
        }
    }
    enElAire = true;
    enPlataforma = false;
    qDebug() << "⚠ Goku inicia en el aire (NIVEL 2).";
}

void jugador::resolverColisionNivel2(QGraphicsItem* plataforma) {
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
        enPlataforma = true;
        qDebug() << "✅ Goku aterrizó correctamente sobre la plataforma NIVEL 2.";
    }
}

void jugador::activarCaidaLibreNivel2() {
    if (!enElAire && !enPlataforma) {
        enElAire = true;
        qDebug() << "⬇️ Goku empieza a caer (NIVEL 2)";
    }
}

void jugador::aplicarFisicaNivel2() {
    if (enElAire) {
        velocidadY += 0.7;
        if (velocidadY > 20.0f) velocidadY = 20.0f;
        setY(y() + velocidadY);

        if (ultimaDireccion == "derecha") {
            setX(x() + velocidadMovimiento);
        } else if (ultimaDireccion == "izquierda") {
            setX(x() - velocidadMovimiento);
        }

        qDebug() << "🟦 Goku EN EL AIRE: Y =" << y() << "VelY =" << velocidadY;
    } else {
        velocidadY = 0;
    }

    // Verificación de muerte al caer fuera de límites (ajusta límite)
    if (y() >= 580 && !estaMuerto) {
        estaMuerto = true;
        setPixmap(spriteMuerto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        update();
        QMessageBox::information(nullptr, "Game Over", "💀 Goku cayó fuera de la plataforma.");
    }
}

QPainterPath jugador::shape() const {
    QPainterPath path;
    path.addRect(25, 80, 50, 15);  // Área de colisión (pies)
    return path;
}

void jugador::saltar() {
    if (!enElAire) {
        velocidadY = -12;
        if (ultimaDireccion == "derecha") {
            // movimiento horizontal positivo
        } else if (ultimaDireccion == "izquierda") {
            // movimiento horizontal negativo
        }
        enElAire = true;
    }
}

void jugador::moverIzquierda() {
    setX(x() - velocidadMovimiento);
    ultimaDireccion = "izquierda";
}

void jugador::moverDerecha() {
    setX(x() + velocidadMovimiento);
    ultimaDireccion = "derecha";
}

void jugador::recibirDanio(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;

    if (textoVida) textoVida->setPlainText(QString("Goku: %1").arg(vida));
    if (barraVida) barraVida->setRect(0, 0, vida, 10);

    if (vida == 0 && !estaMuerto) {
        estaMuerto = true;
        setPixmap(spriteMuerto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        update();
        emit jugadorMuerto();
        QMessageBox::information(nullptr, "Game Over", "💀 Goku fue derrotado por las vendas.");
    }
}

void jugador::mostrarVictoria() {
    qDebug() << "¡¡¡Goku celebra la victoria!!!";
    setPixmap(spriteVictoria.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    victoriaMostrada = true;
}

float jugador::getVelocidadY() const {
    return velocidadY;
}

void jugador::setVelocidadY(float v) {
    velocidadY = v;
}

bool jugador::isEnElAire() const {
    return enElAire;
}

void jugador::setEnElAire(bool estado) {
    enElAire = estado;
}
bool jugador::isSaltoPermitido() const {
    return saltoPermitido;
}

void jugador::setSaltoPermitido(bool estado) {
    saltoPermitido = estado;
}

bool jugador::getEnElAire() const {
    return enElAire;
}

// jugador.cpp
void jugador::sumarEstrella() {
    estrellasRecogidas++;
    qDebug() << "Estrellas recogidas:" << estrellasRecogidas;

    if (estrellasRecogidas >= 3) {
        superGolpeActivo = true;
        qDebug() << "¡Supergolpe activado! Presiona X para usarlo.";
    }
}

bool jugador::puedeUsarSuperGolpe() const {
    return superGolpeActivo;
}


void jugador::activarSuperGolpe() {
    superGolpeActivo = true;
    qDebug() << "Super Golpe ACTIVADO! Presiona X para usarlo.";
    estrellasRecolectadas = 0;

    if (!timerSuperGolpe) {
        timerSuperGolpe = new QTimer(this);
        timerSuperGolpe->setSingleShot(true);
        connect(timerSuperGolpe, &QTimer::timeout, this, &jugador::desactivarSuperGolpe);
    }
    timerSuperGolpe->start(10000);  // dura 10 segundos
}

void jugador::desactivarSuperGolpe() {
    superGolpeActivo = false;
    qDebug() << "Super Golpe DESACTIVADO!";
}
