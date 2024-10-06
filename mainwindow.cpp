#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new PaintScene(ui->textResult);
    ui->graphicsView->setScene(scene);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);

    return;
}

void MainWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0, 0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
    scene->drawGrid(50);

    return;
}



void MainWindow::on_drawLine_clicked()
{
    scene->setTypeItem(ItemTypes::Line);

    return;
}


void MainWindow::on_drawRay_clicked()
{
    scene->setTypeItem(ItemTypes::Ray);

    return;
}


void MainWindow::on_drawSection_clicked()
{
    scene->setTypeItem(ItemTypes::Section);

    return;
}

