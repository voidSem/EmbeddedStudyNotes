#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTcpServer  Server ;
    QTcpSocket  *Socket;

private:
    Ui::MainWindow *ui;

public slots:
    void NewConnectionComming(void);

private slots:
    void on_pushButton_released();
    void on_pushButton_pressed();
};

#endif // MAINWINDOW_H
