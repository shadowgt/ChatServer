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
#define DEF_TYPE_CHANNEL_CHANGE quint16(4)
#define DEF_TYPE_SIGN_UP quint16(5)
#define DEF_TYPE_REQUIRE_MEMBERS_INFO quint16(6)


typedef struct stUserInfo
{
    QTcpSocket* pTcpSocket;
    QString userId;
    QString name;
    QString channal;
    stUserInfo()
    {
        channal = "#Everyone";
    }

}ST_USER_INFO;


#endif // DEFINE_H
