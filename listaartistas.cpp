

#include "listaartistas.h"
#include "artistaseleccion.h"
#include "ui_listaartistas.h"

#include <QDebug>

ListaArtistas::ListaArtistas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListaArtistas)
{
    ui->setupUi(this);

    this->setStyleSheet(myStilo);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    int alto_pantalla;
    int ancho_pantalla;
    QRect rec = QApplication::desktop()->availableGeometry(-1);
    alto_pantalla  = rec.height()-80;
    ancho_pantalla = rec.width();

    //qDebug() << "alto: " << alto_pantalla;

    //this->setMinimumSize(this->width(), alto_pantalla);
    //this->setGeometry(5,5,this->width(),alto_pantalla);
    this->resize(this->width(),alto_pantalla);
    this->move(ancho_pantalla-this->width(),5);

    mi_lista_borrado = new QStringList;



    this->hide();

    //borrado_progreso = new BorradoProgreso(this);
    //borrado_progreso->hide();

    /*

    //boreme
    qsrand(qrand());
    //QStringList mi_lista_original;
    for(int i=0; i<50; i++)
    {
    mi_lista_borrado->append(QString("r:%1").arg(qrand()));
    }

    */

}

void ListaArtistas::set_contenido(QStringList lista, QString genero)
{

    borra_widgets_hijos();

    mi_lista_borrado->clear();

    *mi_lista_borrado = lista;


    ui->label_genero->setText(genero);

    artistas_set_texto("Seleccione y confirme");


    for (int i=0; i<mi_lista_borrado->size(); i++)
    {
        ArtistaSeleccion *artista = new ArtistaSeleccion(NULL, i+1);

        //introduce el nombre al widget
        artista->set_artista_nombre(mi_lista_borrado->at(i));

        //pasa el puntero de lista de valores para ser manipulado por el
        //widget
        artista->set_lista_artistas(mi_lista_borrado);
        ui->verticalLayout_scroll->addWidget(artista);
    }

    ui->verticalLayout_scroll->addItem(new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding));

    this->show();
}

ListaArtistas::~ListaArtistas()
{
    delete ui;
}

void ListaArtistas::borra_widgets_hijos()
{
    QLayoutItem *child;

    //qDebug()  << "antes:" << ui->verticalLayout_scroll->count();

    while ((child = ui->verticalLayout_scroll->takeAt(0)) != 0)
    {

//        delete child;

        if(child->widget())
        {
           child->widget()->setParent(NULL);
        }

    delete child;
    }

    //ui->verticalLayout_scroll->removeWidget();

    //qDebug()  << "despues:" << ui->verticalLayout_scroll->count();
    this->update();
    this->repaint();

}

void ListaArtistas::on_pushButton_cerrar_clicked()
{
    borra_widgets_hijos();


    this->hide();
    emit confirma_borrado();
}




void ListaArtistas::on_pushButton_confirmar_clicked()
{


    QMessageBox msgBox;
    msgBox.setText(tr("Confirmación"));
    msgBox.setInformativeText(tr("Desea borrar los artistas marcados"));

    QPushButton *connectButton = msgBox.addButton(tr("aceptar"),    QMessageBox::ActionRole);
    QPushButton *abortButton   = msgBox.addButton(tr("cancelar"), QMessageBox::ActionRole);

    msgBox.exec();

    if (msgBox.clickedButton() == connectButton)
    {
        // connect

        msgBox.hide();

        if(mi_lista_borrado->size()==0)
        {
            return;
        }

        emit borrar_artistas();

    } else if (msgBox.clickedButton() == abortButton) {

        return;
    }
}


void ListaArtistas::init_artistas(int maximo)
{
    ui->progressBar_artistas->show();
    ui->progressBar_artistas->setValue(0);
    ui->progressBar_artistas->setRange(0, maximo);
    ui->label_artista_progreso->setText("");
}

void ListaArtistas::init_archivos(int maximo)
{
    ui->progressBar_archivos->show();
    ui->progressBar_archivos->setValue(0);
    ui->progressBar_archivos->setRange(0, maximo);
}

void ListaArtistas::artistas_set_valor(int valor)
{
    ui->progressBar_artistas->setValue(valor);
}

void ListaArtistas::artistas_set_texto(QString texto)
{
    ui->label_artista_progreso->setText(texto);
}

void ListaArtistas::archivos_set_valor(int valor)
{
    ui->progressBar_archivos->setValue(valor);
}

void ListaArtistas::oculta_barras(void)
{
    ui->progressBar_artistas->hide();
    ui->progressBar_archivos->hide();
    ui->label_artista_progreso->setText("");
}
