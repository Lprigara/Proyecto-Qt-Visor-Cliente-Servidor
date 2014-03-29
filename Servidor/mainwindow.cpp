#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "acerca.h"
#include "preferencias.h"
#include "conexion.h"

bool Paused = true;  //variable para parar/pausar

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    clientConnection_=NULL;
    tcpServer_=NULL;

    setting_ = new QSettings("Leonor", "viewer"); //configura QSetting
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete setting_;
    delete tcpServer_;
}

void MainWindow::on_exit_clicked()
{
    qApp->quit(); //qApp = QApplication del main
}
void MainWindow::on_actionSalir_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAcerca_de_triggered()
{
    Acerca acercaDe(this);
    acercaDe.exec();
}

void MainWindow::on_actionPreferencias_triggered()
{
   Preferencias prefe(this);
   prefe.exec();
}

void MainWindow::on_actionCapturar_de_red_triggered()
{
    //Configurar un servidor para que escuche en un puerto TCP específico
    port_ = setting_->value("viewer/port", "9600").toInt();
    qDebug()<<port_;
    tcpServer_ = new QTcpServer(this);
    tcpServer_->listen(QHostAddress::Any,port_);
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    while(tcpServer_->hasPendingConnections())
    {
          clientConnection_=tcpServer_->nextPendingConnection();
          connect(clientConnection_, SIGNAL(disconnected()), clientConnection_, SLOT(deleteLater()));
          connect(clientConnection_, SIGNAL(readyRead()), this, SLOT(startRead())); //si esta listo para leer, entra por el slot de leer (startRead)
          stateClient_ = 0; //variable de estados
          sizeImage_ = 0;
    }
}

void MainWindow::startRead()
{
    QByteArray clientName;
    QByteArray timestamp;
    QByteArray sizeIm;
    QByteArray aux;

    //while(clientConnection_->bytesAvailable()){
         if(stateClient_ == 0)//NOMBRE CLIENTE
         {
            if(clientConnection_->canReadLine())//mientras haya una linea que leer
            {
                aux.clear();
                clientName.clear();
                //guardamos la linea en un objeto QByteArray, que luego recorreremos para imprimir su contenido sin el caracter \n
               aux= clientConnection_->readLine(); //lee hasta que encuentre un caracter \n
                int i=0;
                while(aux[i] != '\n')
                {
                    clientName[i]=aux[i];
                    i++;
                }
                qDebug()<<"Nombre Cliente: "<<clientName;
                stateClient_=1;
            }
         }

         if(stateClient_ == 1)//TIMESTAMP
         {
             if(clientConnection_->canReadLine())
             {
                aux.clear();
                timestamp.clear();
                //guardamos la linea en un objeto QByteArray, que luego recorreremos para imprimir su contenido sin el caracter \n
                aux= clientConnection_->readLine();
                int i=0;
                while(aux[i] != '\n')
                {
                    timestamp[i]=aux[i];
                    i++;
                }
                qDebug()<<"Fecha en ms: "<< timestamp;
                stateClient_=2;
             }
         }

         if(stateClient_ == 2)//TAMAÑO IMAGEN
         {
            if(clientConnection_->canReadLine())
            {
                sizeIm.clear();
                aux.clear();
                //guardamos la linea en un objeto QByteArray, que luego recorreremos para imprimir su contenido sin el caracter \n
                sizeIm=clientConnection_->readLine(); //size=6
                int i=0;
                while(sizeIm[i] != '\n')
                {
                    aux[i]=sizeIm[i];
                    i++;
                }

                sizeImage_ = aux.toInt(); //convertimos el QByteArray del tamaño de la imagen en un objeto tipo int para indicarle al read siguiente cuantos caracteres hay que leer.
                stateClient_=3;
            }
        }

        if(stateClient_ == 3)//IMAGEN
        {
            if(clientConnection_->bytesAvailable() >= sizeImage_)
            {
                QBuffer img;
                img.setData(clientConnection_->read(sizeImage_)); //lee el numero de caracteres que ocupa la imagen y el contenido lo metemos en un buffer
                QImage image;
                image.load(&img, "jpeg");

                QString stringName = "Client Name: " + clientName;
                QString stringTimestamp = "Date in ms: "+ timestamp;

                //dibujamos el nombre del cliente y timestamp en el pixmap
                QPixmap pixmap(QPixmap::fromImage(image));
                QPainter painter(&pixmap);
                painter.setPen(Qt::white);
                painter.setFont(QFont("Times", 15));
                painter.drawText(0, 0,pixmap.width(), pixmap.height(), Qt::AlignTop | Qt::AlignLeft, stringName,0);
                painter.drawText(0, 0,pixmap.width(), pixmap.height(), Qt::AlignTop | Qt::AlignRight, stringTimestamp,0);
                ui_->label->setPixmap(pixmap);
                stateClient_=0;
            }
        }
  // }

}
