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
    QTcpSocket *s = (QTcpSocket*)sender();

    for (QList<stUserInfo>::iterator iter = list.begin() ; list.end() != iter; )
    {
        QByteArray block;
        QDataStream dataStream(&block,QIODevice::ReadWrite);

        dataStream << quint16(0);
        dataStream << quint16(DEF_TYPE_TRY_LOGIN);
        dataStream << quint16(sizeof(quint16)); // 사이즈
        dataStream<<quint16(0); // flag 0 = exit

        qDebug() << iter->pTcpSocket->write(block);
        iter->pTcpSocket->waitForBytesWritten(10000);
        iter = list.erase(iter);
    }

    delete ui;
}

void MainWindow::addConnection()
{
    QTcpSocket * s;
    s = server.nextPendingConnection();
    stUserInfo user;
    user.pTcpSocket = s;
    list.append(user);
    connect(s,SIGNAL(disconnected()),this,SLOT(removeConnection()));
    connect(s,SIGNAL(readyRead()),this,
            SLOT(recvMsg()));
    ui->textEdit->append(QString("새로운 사용자입장"));
}



void MainWindow::recvMsg()
{   
    QTcpSocket * tcpSocket = (QTcpSocket*)sender();
    QDataStream dataStream(tcpSocket);

    QString str;
    foreach(stUserInfo user,list)
    {
       if(user.pTcpSocket == tcpSocket)
       {
            // without connect message
           str = processRecvMsg(tcpSocket);
           ui->textEdit->append(str);

           if(str.compare("Exit")==0||str.right(7).compare("SIGN_UP")==0 )
           {
               tcpSocket->close();
           }
           break;
       }
    }

    /*
    QByteArray arrIDChk(tcpSocket->readAll());
    QString strIDChk(arrIDChk.data());
    if(strIDChk.compare("김봉상") == 0)
    {

    }

    */
}

void MainWindow::removeConnection()
{
    QTcpSocket *s = (QTcpSocket*)sender();

    for (QList<stUserInfo>::iterator iter = list.begin() ; list.end() != iter; )
    {
        if(iter->pTcpSocket == s)
        {
            iter = list.erase(iter);
            s->close();
        }
        else
        {
            iter++;
        }
    }
    //s->deleteLater();
    ui->textEdit->append(QString("사용자 1명 나감"));
}
