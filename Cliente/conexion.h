#ifndef CONEXION_H
#define CONEXION_H

#include <QDialog>

#include <QSettings>

namespace Ui {
class Conexion;
}

class Conexion : public QDialog
{
    Q_OBJECT

public:
    explicit Conexion(QWidget *parent = 0);
    ~Conexion();

private slots:

    void on_buttonClose_clicked();

    void on_buttonOkManual_clicked();

    void on_buttonOkDefault_clicked();

private:
    Ui::Conexion *ui;
    QSettings *setting_;
};

#endif // CONEXION_H
