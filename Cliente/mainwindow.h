#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCamera>
#include <QCameraViewfinder>
#include <QtWidgets>

#include <QByteArray>
#include <QPainter>
#include <QTime>

#include <QString>

#include "preferencias.h"
#include "capturebuffer.h"

#include <QTcpSocket>

#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exit_clicked();

    void on_actionSalir_triggered();

    void on_actionAbrir_triggered();

    void on_start_clicked();

    void on_stop_clicked();

    void on_autoinicio_stateChanged(int arg1);

    void on_actionAcerca_de_triggered();

    //void showFrame(const QRect& rect);

    void on_actionCapturar_triggered();

    void on_actionPreferencias_triggered();

    void image1(QImage image); //Slot para modificar los frames

    void on_actionConexion_triggered();

    void connected();


private:
    Ui::MainWindow *ui_;
    QMovie *movie_;
    QSettings *setting_;
    QCamera *camera_;
    QCameraViewfinder *viewfinder_;
    QByteArray dispdefault_;
    QByteArray dispchoise_;
    captureBuffer *captureB_;
    QTcpSocket *tcpSocket_;
    QList<QByteArray> devices_;
    bool connectedServer_;
    QString host_;
    int port_;
};

#endif // MAINWINDOW_H
