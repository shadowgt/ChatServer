#ifndef CDATABASE_H
#define CDATABASE_H

#include "define.h"

class CDatabase
{
public:
    CDatabase();
    ~CDatabase();

protected:
    void connectDatabase();
    void deleteDatabase();
    int sendQuery(QString i_query);

public:
    QSqlDatabase m_db;
};

#endif // CDATABASE_H
