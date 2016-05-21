#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

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
    QTcpSocket * tcpSocket = (QTcpSocket*)sender();
    QDataStream dataStream(tcpSocket);
    quint16 nextBlockSize = 0;
    int msgType = 0;

    while(true)
    {
        if(nextBlockSize == 0)
        {
            //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                ;
            else
            {
                dataStream>>msgType;
                dataStream>>nextBlockSize;

                qDebug() << " received file size of block :" << nextBlockSize;
            }
            continue;
        }
        //nextBlcokSize가 도착하면 사이즈만큼 데이터가 도착했는지 확인
        else if(tcpSocket->bytesAvailable() < nextBlockSize)
            continue;

        //데이터를 표시
        else if(tcpSocket->bytesAvailable() >= nextBlockSize)
        {
            if(msgType == DEF_TYPE_MESSAGE)
            {
                QByteArray arr;
                dataStream>>arr;
                QString str;
                str.append(arr);
                dataStream>>arr;
                str.append(arr);
                ui->textEdit->append(str);
                nextBlockSize = 0;

            }
            else if(msgType == DEF_TYPE_FILE)
            {
                qDebug() << "File received";
            }
            break;
        }
    }

    //ui->textEdit->append(str);

    foreach(QTcpSocket* sock,list)
    {
       // sock->write(arr);
       // sock->flush();
    }
    //qDebug() << "ok";



}
