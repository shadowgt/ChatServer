#include "cserver.h"



CServer::CServer()
{

}

QString CServer::processRecvMsg(QTcpSocket * tcpSocket)
{
    QDataStream dataStream(tcpSocket);

    QByteArray block;
    QDataStream newDataSteam(&block,QIODevice::ReadWrite);

    quint16 nextBlockSize = 0;
    QString strReturn;

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

                newDataSteam << quint16(0);
                newDataSteam << quint16(msgType);

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
                QString strID,strMessage;

                QByteArray arr;
                dataStream>>arr;
                strID = arr.data();
                strReturn.append(arr); // ID
                dataStream>>arr;
                strReturn.append(" : ");
                strReturn.append(arr); // Message
                strMessage = arr.data();

                newDataSteam<<quint16(strMessage.size() + strID.size());
                newDataSteam << strID.toUtf8();
                newDataSteam << strMessage.toUtf8();

            }
            else if(msgType == DEF_TYPE_CHANNEL_CHANGE)
            {
                QByteArray arr;
                dataStream>>arr;
                strReturn.append("\"");
                strReturn.append(arr); // ID
                strReturn.append("\"");
                dataStream>>arr;
                strReturn.append(" joined channel ");
                strReturn.append("\#");
                strReturn.append(arr); // Channel

                foreach(stUserInfo user,list)
                {
                    if(user.pTcpSocket == tcpSocket)
                    {
                        user.channal = arr;
                    }
                }

                QByteArray buffer( strReturn.toUtf8());

                newDataSteam<<quint16(buffer.size());
                newDataSteam << strReturn.toUtf8();
            }
            else if(msgType == DEF_TYPE_FILE)
            {
                qDebug() << "File received";
            }
            else if(msgType == DEF_TYPE_TRY_LOGIN)
            {
                QString strID, strPassword;
                QByteArray arr;
                dataStream>>arr; // ID;
                strID.append(arr);
                dataStream>>arr; // Password
                strPassword.append(arr);

                // DB 연동 체크 코드 작성 해야함

                QString query("select email,password from userInfo_tb where email='"+strID+"' and password='"+strPassword+"' ");

                int rtQuery = sendQuery(query);

                qDebug() << strID;
                qDebug() << strPassword;

                // login as guest
                if(strPassword.compare("guest")==0)
                {
                    rtQuery = 1;
                    strID.append("_guest");
                }

                // *****************************

                if(rtQuery != 1)
                {
                    strReturn.append("Exit");
                }
                else
                {
                    strReturn.append(rtQuery);
                }

                // setUserID
                foreach(stUserInfo user,list)
                {
                    if(user.pTcpSocket == tcpSocket)
                    {
                        user.userId = strID;
                    }
                }


                newDataSteam<<quint16(sizeof(quint16)); // 사이즈
                qDebug() << strReturn.size();
                newDataSteam<<quint16(rtQuery);
            }
            else if(msgType == DEF_TYPE_SIGN_UP)
            {
                QString strID,strPassword,strName;
                int nStatus;

                QByteArray arr;
                dataStream>>arr;
                strID.append(arr);
                dataStream>>arr;
                strPassword.append(arr);
                dataStream>>arr;
                strName.append(arr);
                dataStream>>nStatus;

                QString chkQuery("select email,password from userInfo_tb where email='"+strID+"'");
                //ID , Name 중복 검사 query

                int chkID = sendQuery(chkQuery);

                chkQuery = "select email,password from userInfo_tb where name='"+strName+"' ";

                int chkName = sendQuery(chkQuery);

                if(chkID == 0 && chkName ==0)
                {
                    QString query("INSERT INTO userInfo_tb (email,password,name) VALUES ('"+strID+"','"+strPassword+"','"+strName+"')");

                    int rtQuery = sendQuery(query);
                    if(rtQuery == 1)
                    {
                        strReturn = "SUCCESSED_SIGN_UP";
                    }
                    else
                    {
                        strReturn = "ERROR_SIGN_UP";
                    }
                }
                else
                {
                    if(chkID != 0)
                    {
                        strReturn = "DUPLICATE_ID";
                    }
                    else
                    {
                        strReturn = "DUPLICATE_NAME";
                    }
                }

                QByteArray buffer( strReturn.toUtf8());

                newDataSteam<<quint16(buffer.size());
                newDataSteam << strReturn.toUtf8();


            }
            nextBlockSize = 0;
            break;
        }
    }

    if(msgType == DEF_TYPE_SIGN_UP)
    {
        foreach(stUserInfo user,list)
        {
            tcpSocket->write(block);
        }
    }
    else
    {
        stUserInfo sendUser;
        foreach(stUserInfo user,list)
        {
            if(user.pTcpSocket == tcpSocket)
            {
                sendUser = user;
            }
        }

        foreach(stUserInfo user,list)
        {
            if(user.channal.compare(sendUser.channal)==0)
            {
                user.pTcpSocket->write(block);
            }
        }
    }

    return strReturn;
}


