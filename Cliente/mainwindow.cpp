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
    movie_=NULL;
    camera_=NULL;
    viewfinder_=NULL;
    captureB_=NULL;

    setting_ = new QSettings("Leonor", "viewer"); //configura QSetting
    ui_->autoinicio->setChecked(setting_->value("viewer/autoinicio",true).toBool()); //setChecked necesita un bool como arg.

    devices_=QCamera::availableDevices();
    dispdefault_ = setting_->value("viewer/deviceDefault",devices_[0]).toByteArray();
    dispchoise_ = setting_->value("viewer/deviceChoise",dispdefault_).toByteArray();

    tcpSocket_ = new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete movie_;
    delete camera_;
    delete viewfinder_;
    delete setting_;
    delete captureB_;
    delete tcpSocket_;
}


//Funcion para abrir archivo de video
void MainWindow::on_actionAbrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir archivo", QString() ,"Video (*.mjpeg)"); //te devuelve el nombre del archivo
    if(!fileName.isEmpty()) {
        QPixmap foto(fileName);
        if(foto.isNull()) {
            QMessageBox::information(this, "Abrir archivo", "El archivo no pudo ser abierto. ");
        }
        else {
            movie_ = new QMovie(fileName);
            ui_->label->setMovie(movie_);
            if(ui_->autoinicio->isChecked())
                movie_->start();

        }//endelse
    }//endif
}

void MainWindow::on_actionCapturar_triggered()
{ 
    qDebug()<<dispdefault_<<dispchoise_;
     if(operator!= (dispdefault_,dispchoise_)){  
        camera_->stop();
        delete camera_;
        camera_ = new QCamera(dispchoise_);
     }
     else{
        camera_ = new QCamera(dispdefault_);
     }

     captureB_ = new captureBuffer;
     camera_->setViewfinder(captureB_);

    //objeto que emite la señal, señal emitida, objeto que recibe la señal, accion que desencadena esa señal
     connect(captureB_, SIGNAL(signalImage(QImage)), this, SLOT(image1(QImage)));

    // connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    // connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError));

     //Conectarnos al servidor
     QString host = setting_->value("viewer/host", "127.0.0.1").toString();
     int port = setting_->value("viewer/puerto", "9600").toInt();

     //como la conexion es asincrona, esperamos a que se conecte.
     tcpSocket_->connectToHost(host, port);
     tcpSocket_->waitForConnected();

     camera_->setCaptureMode(QCamera::CaptureViewfinder);
     camera_->start();
}

void MainWindow::image1(QImage image){

    //Modificar (pintar) la imagen para imprimirla en el label
    QTime time;
    QTime currenTime= time.currentTime();
    QString stringTime=currenTime.toString();

    QPixmap pixmap(QPixmap::fromImage(image));

    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 25));
    painter.drawText(0, 0,pixmap.width(), pixmap.height(), Qt::AlignBottom, stringTime,0);

    ui_->label->setPixmap(pixmap);

    //Codificar la imagen para enviarla por la red
    QBuffer buffer;
    QImageWriter writer(&buffer, "jpeg"); //Para controlar el nivel de compresión, el de gamma o algunos otros parámetros específicos del formato, tendremos que emplear un objeto QImageWriter.

    QImage imageSend; //creación de la imagen a enviar
    imageSend=pixmap.toImage(); //conversión del pixmap (con la hora pintada) en un QImage

    writer.setCompression(70);
    writer.write(imageSend); //aplicar lo anterior a la imagen
    QByteArray bytes = buffer.buffer();
    QByteArray jpegHeader(bytes.constData(), 6);

    tcpSocket_->write(jpegHeader); //Enviar al socket la imagen codificada
}

void MainWindow::on_start_clicked()
{
    if(!Paused){
        movie_->setPaused(1);
        Paused = true;
    }
    else{
        movie_->start();
        Paused = false;
    }
}

void MainWindow::on_stop_clicked()
{
    movie_->stop();
    Paused = true;
}

void MainWindow::on_exit_clicked()
{
    qApp->quit(); //qApp = QApplication del main
}

void MainWindow::on_actionSalir_triggered()
{
    qApp->quit();
}

void MainWindow::on_autoinicio_stateChanged(int)
{
    setting_->setValue("viewer/autoinicio", ui_->autoinicio->isChecked());
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


void MainWindow::on_actionConexion_triggered()
{
    Conexion conexion(this);
    conexion.exec();
}
