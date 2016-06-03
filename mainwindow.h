#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "define.h"
#include "cserver.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow ,CServer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addConnection();
    void recvMsg();
    void removeConnection();


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
