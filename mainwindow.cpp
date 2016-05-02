#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server.listen(QHostAddress::Any , 30000);
    connect(&server,SIGNAL(newConnection()),this, SLOT(addConnection()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addConnection()
{
    QTcpSocket * s;
    s = server.nextPendingConnection();
    list.append(s);
    connect(s,SIGNAL(disconnected()),this,SLOT(removeConnection()));
    connect(s,SIGNAL(readyRead()),this,
            SLOT(recvMsg()));
    ui->textEdit->append(QString("새로운 사용자입장"));
}

void MainWindow::removeConnection()
{
    QTcpSocket * s =(QTcpSocket*)sender();
    list.removeAll(s);
    s->deleteLater();
    ui->textEdit->append(QString("사용자 1명 나감"));
}

void MainWindow::recvMsg()
{
    QTcpSocket * s = (QTcpSocket*)sender();
    QByteArray arr(s->readAll());
    foreach(QTcpSocket* sock,list)
    {
        sock->write(arr);
        sock->flush();
    }
    QString str(arr);
    ui->textEdit->append(QString("메시지:"+str));

}
