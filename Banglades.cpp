#include "Banglades.h"
#include <QPixmap>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include "venda.h"

Banglades::Banglades(float x, float y, Plataforma* plataformaAsociada, jugador* objetivoGoku)
    : personaje(x, y, 1.0f), direccion(1.0f), vida(100), plataforma(plataformaAsociada), estaMuerto(false), jugadorGoku(objetivoGoku)
{
    qDebug() << "🧟 Banglades creado en:" << x << y;
    setPixmap(QPixmap(":/sprites/banglades.png").scaled(50, 70));
    setPos(x, y);
    setScale(1.0);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Banglades::actualizarMovimiento);
    timerMovimiento->start(30);

    // Timer para disparar vendas
    timerVendas = new QTimer(this);
    connect(timerVendas, &QTimer::timeout, this, &Banglades::dispararVendaje);
    timerVendas->start(1300); // Dispara una venda cada 1.3s
}

void Banglades::aplicarFisica()
{
    // La momia no salta ni cae
}

void Banglades::actualizarMovimiento()
{
    if (estaMuerto) {
        qDebug() << "⛔ Banglades muerto, no se mueve.";
        return;
    }

    qreal nuevaX = x() + direccion * 2.0;
    if (nuevaX <= 150 || nuevaX + boundingRect().width() >= 650) {
        direccion *= -1;
        qDebug() << "🔄 Banglades cambia de dirección:" << direccion;
    } else {
        setX(nuevaX);
        // qDebug() << "➡ Banglades se mueve a X:" << nuevaX;
    }
}

void Banglades::detenerMovimiento()
{
    qDebug() << "🛑 Banglades detiene movimiento";
    if (timerMovimiento)
        timerMovimiento->stop();
}

void Banglades::detenerVendas()
{
    qDebug() << "🛑 Banglades detiene vendas, total:" << listaVendas.size();
    if (timerVendas) {
        timerVendas->stop();
        timerVendas->disconnect();
        qDebug() << "🛑 Timer de vendas detenido y desconectado";
    }

    for (Venda* venda : listaVendas) {
        if (venda && !venda->yaDetenida) {  // <--- SOLO si no está eliminada ya
            qDebug() << "❌ Eliminando venda:" << static_cast<const void*>(venda);
            if (venda->scene()) venda->scene()->removeItem(venda);
            venda->detenerse();
        } else {
            qDebug() << "⚠️ Venda ya eliminada, se ignora:" << static_cast<const void*>(venda);
        }
    }
    listaVendas.clear();
    qDebug() << "✅ Todas las vendas eliminadas";
}


void Banglades::recibirGolpe(int cantidad) {
    qDebug() << "👊 Banglades recibe golpe:" << cantidad << "vida antes:" << vida;
    vida -= cantidad;
    if (vida < 0) vida = 0;

    if (barraVida) barraVida->setRect(0, 0, vida, 10);
    if (textoVida) textoVida->setPlainText(QString("Banglades: %1").arg(vida));

    if (vida == 0 && !estaMuerto) {
        qDebug() << "💀 Banglades murió!";
        estaMuerto = true;

        // Cambia sprite a muerto
        QPixmap muerto(":/sprites/banglades_muerto.png");
        if (muerto.isNull()) {
            qDebug() << "❌ ERROR: No se pudo cargar el sprite de Banglades muerto.";
        } else {
            setPixmap(muerto.scaled(50, 70));
        }
        // Posicionar sobre la plataforma (ajusta según tu plataforma)
        if (plataforma) {
            setY(plataforma->y() - this->boundingRect().height() + plataforma->boundingRect().height()/2);
        }

        // Detener movimientos y vendas
        detenerMovimiento();
        detenerVendas();

        // Emitir señal de victoria
        qDebug() << "🏆 Emitiendo señal de bandagesDerrotado()";
        emit bandagesDerrotado();
    }
}

void Banglades::dispararVendaje()
{
    if (estaMuerto) {
        qDebug() << "⛔ Banglades muerto, no dispara vendas.";
        return;
    }

    if (jugadorGoku) {
        // Aquí reemplazas la línea actual:
        Venda* venda = new Venda(jugadorGoku, x() + boundingRect().width()/2, y() + boundingRect().height()/2, this);
        if (scene()) scene()->addItem(venda);
        listaVendas.append(venda);
        qDebug() << "🎯 Banglades lanza venda:" << static_cast<const void*>(venda) << "Total vendas activas:" << listaVendas.size();
    }
}

void Banglades::eliminarVenda(Venda* v) {
    listaVendas.removeAll(v);
    qDebug() << "🧹 Venda eliminada de la lista de Banglades:" << static_cast<const void*>(v);
}


