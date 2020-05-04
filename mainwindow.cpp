#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_fecha->setText( QString(__TIME__) +" "+ QString(__DATE__)  );

    this->setStyleSheet(myStilo);

    int alto_pantalla;
    int ancho_pantalla;
    QRect rec = QApplication::desktop()->availableGeometry(-1);
    alto_pantalla  = rec.height();
    ancho_pantalla = rec.width();

    this->resize(ancho_pantalla, alto_pantalla);

    //QSize t_e = ui->textEdit_temas->size();

    //alto_pantalla - t_e.height();

    QPoint pos_ = ui->textEdit_temas->pos();

    //ui->textEdit_temas->resize(ancho_pantalla-20, ui->textEdit_temas->sizeHint().height());

    ui->textEdit_temas->setGeometry(pos_.x(), pos_.y(), ancho_pantalla-20, alto_pantalla - (ui->pushButton_salir->y() + 20));

    if(false == QFileInfo::exists(MDB_PROGRAMA))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(tr("No existe herramienta"));
        msgBox.exec();

        exit (0);

        return;
    }


    limpia_pantalla();

    ui->lineEdit_ruta_app->setFocus();

    proceso_datos = new ProcesoDatos(this);

    //DESCOMENTAR PARA INICIAR
    proceso_datos->crea_baseDatos(true);
    proceso_datos->set_progressBar(ui->progressBar_datos);
    proceso_datos->set_textEdit(ui->textEdit_temas);


    //    que fraficador tenga un procesodatos como parametro de construccion
    mi_graficador = new Graficas(this, proceso_datos);

    //mi_graficador->set_proceso_datos(proceso_datos);


    connect(proceso_datos, SIGNAL(error_permisos()), qApp, SLOT(quit()));

    connect(mi_graficador, SIGNAL(top_copiado()), this, SLOT(imprime_espacio()) );


    //connect(ui->lineEdit_gps, SIGNAL(textChanged(QString)), this, SLOT(mi_coordenada_gps(QString)) );


    QPoint pos = this->pos();
    this->move(0, pos.y());

    imprime_espacio();

    proceso_datos->directorio_para_top = QDir::homePath() + "/Escritorio" ;






    //qDebug() << "path:" << proceso_datos->directorio_top;

/*
    int numero;
    //QString str;
    QStringList net = getMacAddress();
    //qDebug() <<  net.at(1) << net.at(3);
    //qDebug() <<  net.at(1).toInt(Q_NULLPTR, 16) << net.at(3).toInt(Q_NULLPTR,16);
    numero = net.at(1).toInt(Q_NULLPTR, 16)*1000 + net.at(3).toInt(Q_NULLPTR,16);
    //str = QString( net.at(1) + net.at(3));

    //numero = 0000;

    string encrypted = encryptDecrypt(QString::number(numero).toStdString());
    cout << "Encrypted:" << encrypted << endl;

    string decrypted = encryptDecrypt(encrypted);
    cout << "Decrypted:" << decrypted << endl;

    qDebug() << "numero: " << numero;
    //qDebug()  << str << endl;

    cout << endl;
    cout << endl;

    exit(0);
*/

    //qDebug() << hay_audios_en("/win2/musica_A");
}

void MainWindow::imprime_espacio(void)
{
    QProcess _proceso;
    QString cmd;
    QString output;
    QString error;
    QString GbLibres;
    QString PorcentajeUso;

    cmd = QString("/bin/df / -h --output=avail");
    _proceso.start(cmd);    // start cmd
    _proceso.waitForFinished(-1);          // wait till done...
    output = _proceso.readAllStandardOutput();     // get std output
    error  = _proceso.readAllStandardError();

    GbLibres = output.split("\n").at(1);
    GbLibres = GbLibres.remove(" ");
    GbLibres = GbLibres.remove("G");
    GbLibres = GbLibres.replace(",",".");

    cmd = QString("/bin/df / -h --output=pcent");
    _proceso.start(cmd);    // start cmd
    _proceso.waitForFinished(-1);          // wait till done...
    output = _proceso.readAllStandardOutput();     // get std output
    error  = _proceso.readAllStandardError();

    PorcentajeUso = output.split("\n").at(1);
    PorcentajeUso = PorcentajeUso.remove(" ");
    PorcentajeUso = PorcentajeUso.remove("%");

    //qDebug() << "espacio: " << PorcentajeUso;
    //qDebug() << "espacio: " << error;


    proceso_datos->espacio_libre = GbLibres.toDouble();

    //qDebug() << "proceso_datos->espacio_libre" << proceso_datos->espacio_libre;

   // proceso_datos->espacio_libre

    ui->label_espacio_valor->setText(QString("%1 Gigas").arg(GbLibres));

    ui->progressBar_espacio->setValue(0);
    ui->progressBar_espacio->setRange(0, 100);
    ui->progressBar_espacio->setValue(PorcentajeUso.toInt());

}

void MainWindow::limpia_pantalla()
{
    //ruta_mdb.clear();
    ui->progressBar_datos->hide();
    ui->pushButton_graficar->hide();
    ui->textEdit_temas->hide();
    //ui->lineEdit_ruta_app->clear();
}

/*
void MainWindow::mi_coordenada_gps(QString gps)
{
    //qDebug() << gps;

    if(gps.size() == 0)
    {
        latitud.clear();
        longitud.clear();
        return;
    }

    //qDebug() << "1";
    if(gps.right(1).at(0).isLetter())
    {
//qDebug() << "2";
        if(gps.size()==1)
        {
            ui->lineEdit_gps->clear();
            latitud.clear();
            longitud.clear();

            return;
        }
//qDebug() << "3";

        if(gps.size()>1)
        {
            gps.remove(gps.size()-1, 1);
            ui->lineEdit_gps->setText(gps);
        }
    }

    if(true == gps.contains(';'))
    {
    latitud = gps.split(";").at(0);
    longitud = gps.split(";").at(1);
    //qDebug() << "latitud: " << latitud << " " << "longitud:" << longitud;
    }

    //latitud;
    //longitud;

}
*/

MainWindow::~MainWindow()
{

    delete proceso_datos;
    delete ui;
}


void MainWindow::on_pushButton_ruta_app_clicked()
{


    ruta_app_rk.clear();

    ui->textEdit_temas->hide();

    //muestra dialogo del sistema para ubicar la carpeta

    ruta_app_rk = dialogo_directorio(QDir::rootPath()+"media/", this);


     //qDebug() << "Ruta : " <<  ruta;

    if( ruta_app_rk.isNull() )
    {
       // ui->lineEdit_ruta_app->setText("");
    }

    // si el directorio es valido emite señal
    if( !ruta_app_rk.isNull() )
    {
         //qDebug() << "Ruta : " << ruta;

        ui->lineEdit_ruta_app->setText(ruta_app_rk);


        ruta_app_rk.append("/");
        ruta_app_rk.append(BASE_DATOS_ACCESS);

        QFile archivo_mdb(ruta_app_rk);


        if(!archivo_mdb.exists())
        {
            QString mensaje;
            mensaje.append(APLICACION_ROCKOLA);
            mensaje.append(" no se encuentra aqui.");

            QMessageBox msgBox;
            msgBox.setText(tr("Alerta"));
            msgBox.setInformativeText(mensaje);
            msgBox.exec();

            ruta_app_rk.clear();

            return;
        }


        //ui->pushButton_siguiente->show();

            //qDebug() << ruta;

        //emit dir_Choosed(ruta_app_rk);
    }
    else
    {
        return;
    }


}

void MainWindow::on_pushButton_salir_clicked()
{
    // confirma salir

    QMessageBox msgBox;
    msgBox.setText(tr("Confirmación"));
    msgBox.setInformativeText(tr("Desea salir de la aplicación"));

    QPushButton *connectButton = msgBox.addButton(tr("salir"),    QMessageBox::ActionRole);
    QPushButton *abortButton   = msgBox.addButton(tr("cancelar"), QMessageBox::ActionRole);

    msgBox.exec();

    if (msgBox.clickedButton() == connectButton) {
        // connect
        emit quit();

    } else if (msgBox.clickedButton() == abortButton) {
        // abort
    }



}

void MainWindow::on_pushButton_inicio_clicked()
{

    limpia_pantalla();


    if(ruta_app_rk.size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("Ubique la carpeta de %1").arg(APLICACION_ROCKOLA));
        msgBox.exec();
        return;
    }

    if(ruta_app_rk.contains(BASE_DATOS_ACCESS)==false)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("Esta carpeta no contiene a %1").arg(APLICACION_ROCKOLA));
        msgBox.exec();
        return;
    }

    if(ruta_musica_rk.size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("Ubique la carpeta de música"));
        msgBox.exec();
        return;
    }

    //QDir mdir(ruta_musica_rk);

    if(false == hay_audios_en(ruta_musica_rk))
    {

        return;
    }

    /*
    if( (latitud.toDouble()==0.0) || (longitud.toDouble() == 0.0) )
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("falta coordenadas gps"));
        msgBox.exec();
        return;
    }
    */

    proceso_datos->crea_baseDatos(true);

    //proceso_datos->inserta_gps(latitud, longitud);

    proceso_datos->corre_mdbexport(ruta_app_rk);

    ui->pushButton_graficar->show();

}

void MainWindow::on_pushButton_graficar_clicked()
{

    this->mi_graficador->set_musica_dir(ruta_musica_rk);

    this->mi_graficador->show();

}

void MainWindow::on_pushButton_ruta_musica_clicked()
{

    ruta_musica_rk.clear();

    ui->textEdit_temas->hide();

    //muestra dialogo del sistema para ubicar la carpeta

    ruta_musica_rk = dialogo_directorio(QDir::rootPath()+"media/", this);


     //qDebug() << "Ruta : " <<  ruta;

    if( ruta_musica_rk.isNull() )
    {
    //    ui->lineEdit_ruta_musica->setText("");
    }

    // si el directorio es valido emite señal
    if( !ruta_musica_rk.isNull() )
    {
         //qDebug() << "Ruta : " << ruta;

        ui->lineEdit_ruta_musica->setText(ruta_musica_rk);


        //ruta_musica_rk.append("/");

/*
        QFile archivo_mus(ruta_musica_rk);

        if(!archivo_mus.exists())
        {
            QString mensaje;
            mensaje.append(APLICACION_ROCKOLA);
            mensaje.append(" no se encuentra aqui.");

            QMessageBox msgBox;
            msgBox.setText(tr("Alerta"));
            msgBox.setInformativeText(mensaje);
            msgBox.exec();

            ruta_app_rk.clear();

            return;
        }
*/

        //emit dir_Choosed(ruta_app_rk);
    }
    else
    {
        return;
    }

}

bool MainWindow::hay_audios_en(QString ruta)
{
    int contador=0;

    QDirIterator it(ruta, QStringList() << "*.mp3" << "*.MP3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if(contador>30) break;
        contador++;
        //qDebug() <<
                    it.next();
    }

    if(contador>29) return true;

    QMessageBox msgBox;
    msgBox.setText(tr("Alerta"));
    msgBox.setInformativeText(QString("El directorio %1 no tiene archivos de audio.").arg(ruta));
    msgBox.exec();

    ruta_musica_rk.clear();

    ui->lineEdit_ruta_musica->clear();

return false;
}

void MainWindow::on_lineEdit_ruta_musica_textChanged(const QString &arg1)
{
    if(false == hay_audios_en(arg1))
    {

        return;
    }


}


