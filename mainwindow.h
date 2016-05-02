#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

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
};

#endif // MAINWINDOW_H
