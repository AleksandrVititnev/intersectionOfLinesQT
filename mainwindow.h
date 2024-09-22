#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>

#include <paintscene.h>
#include <enums.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    PaintScene *scene;

private:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slotTimer();
    void on_drawLine_clicked();
    void on_drawRay_clicked();
    void on_drawSection_clicked();
};
#endif // MAINWINDOW_H
