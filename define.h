#ifndef DEFINE_H
#define DEFINE_H

#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QTsql>
#include <QFileInfo>


#define DEF_TYPE_MESSAGE quint16(1)
#define DEF_TYPE_FILE quint16(2)
#define DEF_TYPE_TRY_LOGIN quint16(3)


typedef struct stUserInfo
{
    QTcpSocket* pTcpSocket;
    QString userId;
}ST_USER_INFO;


#endif // DEFINE_H
