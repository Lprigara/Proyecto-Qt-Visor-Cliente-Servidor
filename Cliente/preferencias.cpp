#include "preferencias.h"
#include "ui_preferencias.h"

Preferencias::Preferencias(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Preferencias)
{
    ui_->setupUi(this);

    model_ = new QStringListModel(this);
    devices_=QCamera::availableDevices();//array de dispositivos (camaras) disponibles

    //Insertar lista de dispositivos en el widget comboboxDispositivos
    //Recorre los dispositivos y los mete en la lista
    for(int i=0; i<devices_.size();i++)
    List_ << QCamera::deviceDescription(devices_[i]);

    model_->setStringList(List_);
    ui_->comboBoxDispositivo->setModel(model_);

    //Obtener valor de dispositivo en el fichero de configuracion
    setting_ = new QSettings("Leonor", "viewer");
    QString aux;
    aux = setting_->value("viewer/deviceDefault").toByteArray();

    //Establecer gráficamente el valor guardado
    for (int i = 0; i < List_.size(); i++)
    { if (devices_[i] == aux)
        { ui_->comboBoxDispositivo->setCurrentIndex(i); }
    }
}

Preferencias::~Preferencias()
{
    delete ui_;
    delete setting_;
    delete model_;
}

void Preferencias::on_buttonBox_accepted()
{
    //Establecer en archivo QSetting el valor del dispositivo actual
    setting_->setValue("viewer/deviceChoise", devices_[ui_->comboBoxDispositivo->currentIndex()]);
}
