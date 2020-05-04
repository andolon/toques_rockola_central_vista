#include "artistaseleccion.h"
#include "ui_artistaseleccion.h"

//boreme
#include <QDebug>


ArtistaSeleccion::ArtistaSeleccion(QWidget *parent, int numero) :
    QWidget(parent),
    ui(new Ui::ArtistaSeleccion)
{
    ui->setupUi(this);

    this->setStyleSheet(myStilo);

    ui->label_item_numero->setText(QString("%1").arg(numero));
}




void ArtistaSeleccion::set_artista_nombre(QString artista)
{
    ui->lineEdit_artista->setText(artista);
}

void ArtistaSeleccion::set_lista_artistas(QStringList *lista)
{
lista_copia = lista;
}

ArtistaSeleccion::~ArtistaSeleccion()
{


    delete ui;
}

void ArtistaSeleccion::on_checkBox_borrado_toggled(bool checked)
{

    if(checked==true)
    {
    //qDebug() <<"seleccionado:" << ui->lineEdit_artista->text();
    lista_copia->append(ui->lineEdit_artista->text());
    //qDebug() << "ahora tengo +: " << lista_copia->size();
    }

    if(checked==false)
    {
    //qDebug() <<"desmarcado :" << ui->lineEdit_artista->text();
    lista_copia->removeOne(ui->lineEdit_artista->text());
    //qDebug() << "ahora tengo -: " << lista_copia->size();
    }

}

