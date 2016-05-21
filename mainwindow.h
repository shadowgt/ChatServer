#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#define DEF_TYPE_MESSAGE 1
#define DEF_TYPE_FILE 2

typedef struct stMsg
{
    QByteArray msg;
    QByteArray userId;
}STMSG;




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addConnection();
    void removeConnection();
    void recvMsg();



private:
    Ui::MainWindow *ui;

    QTcpServer server;
    QList<QTcpSocket*> list;

    int m_nextBlockSize;
};

#endif // MAINWINDOW_H
