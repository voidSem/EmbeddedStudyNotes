#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Server.listen(QHostAddress::Any,10086);


    connect(&Server,SIGNAL(newConnection()),this,SLOT(NewConnectionComming()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_pressed()
{//响
    unsigned int val = 1 ;
    Socket->write((const char *)&val,4);
}

void MainWindow::on_pushButton_released()
{//不响
    unsigned int val = 0 ;
    Socket->write((const char *)&val , 4);
}

void MainWindow::NewConnectionComming()
{
    Socket = Server.nextPendingConnection();
    qDebug() << "someone connect me";
}
