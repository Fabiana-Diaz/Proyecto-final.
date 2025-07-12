#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameScene.h"
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Al iniciar, OCULTA el QGraphicsView (el juego) para solo ver el menú
    ui->graphicsView->setVisible(false);
}

// Botón para iniciar el juego (Nivel 1)
void MainWindow::on_pushButton_clicked()
{
    // OCULTA el menú (título, imagen, botones)
    ui->label->setVisible(false);        // Fondo/título
    ui->label_2->setVisible(false);      // Imagen de título (si tienes)
    ui->pushButton->setVisible(false);   // Botón nivel 1
    ui->pushButton_2->setVisible(false); // Botón nivel 2

    // Muestra el QGraphicsView (el juego)
    ui->graphicsView->setVisible(true);

    // Crea y carga la escena del juego en el QGraphicsView
    GameScene* nivel1 = new GameScene(this);
    ui->graphicsView->setScene(nivel1);

    // ¡CLAVE! Dale FOCO al QGraphicsView y a la escena y a Goku
    ui->graphicsView->setFocusPolicy(Qt::StrongFocus);
    ui->graphicsView->setFocus();

    // Usamos un QTimer para asegurarnos que el foco va al jugador (por si Qt tarda en refrescar)
    QTimer::singleShot(10, this, [nivel1](){
        nivel1->focusGoku();
    });
}

// Si tienes botón para Nivel 2 puedes repetir la lógica
void MainWindow::on_pushButton_2_clicked()
{
    // OCULTA el menú y muestra el juego
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->graphicsView->setVisible(true);

    // Aquí iría la escena del segundo nivel
    // GameScene2* nivel2 = new GameScene2(this);
    // ui->graphicsView->setScene(nivel2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
