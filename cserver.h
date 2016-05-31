#ifndef CSERVER_H
#define CSERVER_H

#include "define.h"
#include "cdatabase.h"


class CServer : public CDatabase
{

public:
    CServer();

protected:
    QTcpServer server;
    QList<stUserInfo> list;

    int m_nextBlockSize;

    QString processRecvMsg(QTcpSocket * tcpSocket);


};

#endif // SERVER_H
