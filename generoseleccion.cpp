#include "generoseleccion.h"
#include "ui_generoseleccion.h"

GeneroSeleccion::GeneroSeleccion(QWidget *parent, int numero) :
    QWidget(parent),
    ui(new Ui::GeneroSeleccion)
{
    ui->setupUi(this);

    this->setStyleSheet(myStilo);

    lista_borrado = new QStringList();

    ui->label_item_numero->setText(QString("%1").arg(numero));
}

void GeneroSeleccion::set_genero_datos(QString genero, int valor)
{
    ui->lineEdit_genero->setText(genero);
    ui->label_item_numero->setText(QString::number(valor));
}


void GeneroSeleccion::set_lista_borrado(QStringList *lista)
{
lista_borrado = lista;
}


GeneroSeleccion::~GeneroSeleccion()
{
    delete ui;
}

void GeneroSeleccion::on_checkBox_borrado_toggled(bool checked)
{
    if(checked==true)
    {
    //qDebug() <<"seleccionado:" << ui->lineEdit_genero->text();
    lista_borrado->append(ui->lineEdit_genero->text());
    //qDebug() << "ahora tengo +: " << lista_borrado->size();
    }

    if(checked==false)
    {
    //qDebug() <<"desmarcado :" << ui->lineEdit_genero->text();
    lista_borrado->removeOne(ui->lineEdit_genero->text());
    //qDebug() << "ahora tengo -: " << lista_borrado->size();
    }
}

void GeneroSeleccion::set_toggled(bool checked)
{
    if(checked==true) ui->checkBox_borrado->setChecked(true);
    if(checked==false) ui->checkBox_borrado->setChecked(false);
}
