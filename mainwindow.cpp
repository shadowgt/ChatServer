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

    bool bLoginChk = false;
    QString str;
    foreach(stUserInfo user,list)
    {
       if(user.pTcpSocket == tcpSocket)
       {
            // without connect message
           str = processRecvMsg(tcpSocket);
           ui->textEdit->append(str);

           if(str.compare("Exit")==0)
           {
               removeConnection(tcpSocket);
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

void MainWindow::removeConnection(QTcpSocket * i_s)
{
    QTcpSocket * s = nullptr;
    bool bFunctionCall = false;

    if(i_s == nullptr)
    {
        s =(QTcpSocket*)sender();
    }
    else
    {
        s = i_s;
        bFunctionCall = true;
    }

    for (QList<stUserInfo>::iterator iter = list.begin() ; list.end() != iter; )
    {
        if(iter->pTcpSocket != s)
        {
            iter = list.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    if(bFunctionCall == false )
    s->deleteLater();
    ui->textEdit->append(QString("사용자 1명 나감"));
}
