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
            qDebug() << "nextBlockSize == 0";
            //수신된 데이터가 nextBlockSize 바이트보다 큰지 확인
            if(tcpSocket->bytesAvailable() < sizeof(quint16))
                qDebug() << "tcpSocket->bytesAvailable() < sizeof(quint16)";
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
                QString strName;
                QString strChannal;
                QString strBeforeChannal;

                QByteArray arr;
                dataStream>>arr;
                strReturn.append("\"");
                strReturn.append(arr); // ID

                strReturn.append("\"");
                dataStream>>arr;
                strReturn.append(" joined channel ");
                strReturn.append("\#");
                strReturn.append(arr); // Channel
                strChannal.append(arr);

                foreach(stUserInfo user,list)
                {
                    if(user.pTcpSocket == tcpSocket)
                    {
                        strBeforeChannal = user.channal;
                        user.channal = arr.data();
                        strName = user.name;
                    }
                }

                eraseChannalMember(strName,strBeforeChannal);

                // 새 정보 등록 (채널)
                m_mapChannalMember.insert(strChannal,strName);

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
                QString strID, strPassword ,strName;
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
                    m_db = QSqlDatabase::addDatabase("QSQLITE");
                    m_db.setDatabaseName("Bong.db");

                    if(m_db.open()) //데이터베이스 오픈
                    {
                        m_db.exec();
                        query = "select name from userInfo_tb where email='"+strID+"' and password='"+strPassword+"' ";

                        QSqlQuery sqlQuery;
                        if(sqlQuery.exec(query))
                        {
                            sqlQuery.next();
                            strName = sqlQuery.value(0).toString();
                        }
                        m_db.close();
                    }
                    strReturn.append(rtQuery);
                }

                // setUserID
                foreach(stUserInfo user,list)
                {
                    if(user.pTcpSocket == tcpSocket)
                    {
                        user.userId = strID;
                        user.name = strName;

                        // 새 정보 등록 (채널)
                        m_mapChannalMember.insert(user.channal,strName);
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
            else if(msgType == DEF_TYPE_REQUIRE_MEMBERS_INFO)
            {
                stUserInfo sendUser;
                foreach(stUserInfo user,list)
                {
                    if(user.pTcpSocket == tcpSocket)
                    {
                        sendUser = user;
                    }
                }
                QByteArray arr;
                dataStream >> arr;
                QString str(arr);
                qDebug() << str;

                // 블록 사이즈 구하기
                int blockSize = 0;
                int nCount = 0;
                QMap<QString,QString>::const_iterator iter = m_mapChannalMember.find(sendUser.channal);
                // 채널 Member 정보
                while (iter != m_mapChannalMember.end() && iter.key() == sendUser.channal)
                {
                    blockSize = blockSize + QString(iter.value()).size();

                    qDebug()<<"String size :" << QString(iter.value()).size();

                    iter++;
                    nCount++;
                }

                newDataSteam<<quint16(blockSize+sizeof(quint16));
                newDataSteam<<quint16(nCount);

                qDebug()<<"블록 사이즈" << blockSize << " + "<< sizeof(quint16);
                qDebug()<<"이름 갯수" << nCount;
                // 블록 데이터 스트림에 넣기
                iter = m_mapChannalMember.find(sendUser.channal);
                // 채널 Member 정보
                while (iter != m_mapChannalMember.end() && iter.key() == sendUser.channal)
                {
                    qDebug()<<"test";
                    newDataSteam<<QString(iter.value()).toUtf8();
                    iter++;
                }


               // newDataSteam << strReturn.toUtf8();

               // list.begin();

            }
            nextBlockSize = 0;
            break;
        }
    }

    if(msgType == DEF_TYPE_MESSAGE || msgType == DEF_TYPE_CHANNEL_CHANGE)
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
    else
    {
        foreach(stUserInfo user,list)
        {
            tcpSocket->write(block);
        }
    }

    return strReturn;
}

void CServer::eraseChannalMember(QString i_name, QString i_channal)
{
    QMap<QString,QString>::iterator iter = m_mapChannalMember.find(i_channal);
    // 이전 정보 삭제
    while (iter != m_mapChannalMember.end() && iter.key() == i_channal) {
        if(QString(iter.value()).compare(i_name)==0)
        {
            iter = m_mapChannalMember.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}
