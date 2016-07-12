#include "cdatabase.h"

CDatabase::CDatabase()
{

}

CDatabase::~CDatabase()
{

}

int CDatabase::sendQuery(QString i_query)
{


    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("Bong.db");

    int count = 0;

    if(m_db.open()) //데이터베이스 오픈
    {
        m_db.exec();
        /*
        QSqlQuery CreateQuery1( "CREATE TABLE userInfo_tb ("
                               "email	TEXT NOT NULL,"
                               "password	TEXT NOT NULL,"
                               "name	TEXT NOT NULL"
                               ");");
        qDebug() << "create 문 체크 :" << CreateQuery1.exec();
        qDebug() << CreateQuery1.lastError();
        */
        /*
        QSqlQuery CreateQuery;
        qDebug() << "insert 문 체크 :" << CreateQuery.exec("INSERT INTO userInfo_tb (email,password,name) VALUES ('shadowgt@naver.com','rlaqhdtkd1','돈내코')");
        qDebug() << CreateQuery.lastError();
*/

        QStringList tables = m_db.tables(QSql::AllTables);

        for (int i = 0; i < tables.size(); ++i)
               qDebug() << tables.at(i).toLocal8Bit().constData() << endl;

        qDebug() << "database connected";
        //QSqlQuery query(i_query); //데이터베이스에 전달할 쿼리변수
        qDebug() << i_query;


        QSqlQuery query;
        //query.prepare("SELECT * FROM userInfo_tb where email='shadowgt@naver.com' and name='shadowgt' and password='rlaqhdtkd1'");
        qDebug() << i_query;
        if(query.exec(i_query))
        {
            qDebug() <<"active" << query.isActive();
            qDebug() <<"row size : " << query.size();
            qDebug() << "query ok";


            while(query.next())
            {
                count++;
            }

            if (count == 1)
            {
                qDebug() << "it is correct";
            }
            else if(count <  1)
            {
                qDebug() << "it is not correct";
            }
            else if (count > 1)
            {
                qDebug() << " it is Duplicate";
            }

            qDebug() << i_query.left(6);

            if(count == 0 && i_query.left(6).compare("INSERT")==0 )
            {
                qDebug() << "insert query";
                count = 1;

                qDebug() << query.lastError().type();
            }
        }
        else
        {
            qDebug() << query.lastError();
            qDebug() << query.lastError().type();
        }
    }
    m_db.close();

    return count;
    //int prevFolderId=query.value(0).toInt(); //가져온 변수를 int 타입으로 변환
}

void CDatabase::connectDatabase()
{

}

void CDatabase::deleteDatabase()
{
    //데이터를 모두 가져왔으면 데이터베이스 종료
}
