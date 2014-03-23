#include "conexion.h"
#include "ui_conexion.h"

Conexion::Conexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Conexion)
{
    ui->setupUi(this);
    setting_ = new QSettings("Leonor", "viewer");
}

Conexion::~Conexion()
{
    delete ui;
    delete setting_;
}

void Conexion::on_buttonClose_clicked()
{
    reject();
}

void Conexion::on_buttonOkManual_clicked()
{
    int port = ui->lineEditPuerto->text().toInt();
    QString host = ui->lineEditHost->text();
    setting_->setValue("viewer/host", host);
    setting_->setValue("viewer/port", port);
    reject();
}

void Conexion::on_buttonOkDefault_clicked()
{
    setting_->setValue("viewer/host", QString("127.0.0.1"));
    setting_->setValue("viewer/port", 9600);
    reject();
}
