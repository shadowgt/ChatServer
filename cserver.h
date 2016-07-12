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

    QMap<QString,QString> m_mapChannalMember;

    int m_nextBlockSize;

    QString processRecvMsg(QTcpSocket * tcpSocket);

    void eraseChannalMember(QString i_name , QString i_channal);

};

#endif // SERVER_H
