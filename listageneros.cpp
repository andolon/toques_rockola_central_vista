#include "listageneros.h"
#include "ui_listageneros.h"

ListaGeneros::ListaGeneros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListaGeneros)
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

        lista_borrado = new QStringList();

}

void ListaGeneros::set_contenido(ProcesoDatos *proc)
{
    borra_widgets_hijos();

    lista_borrado->clear();

    mi_lista_generos = new QList<DatoConsultaBasica>();

    //proceso_datos = new ProcesoDatos(this);

    *mi_lista_generos = proc->consulta_generos_nunca_tocados();

    //mi_lista_borrado->clear();

    for (int i=0; i<mi_lista_generos->size(); i++)
    {
        lista_borrado->append( mi_lista_generos->at(i).item );
    }

    for (int i=0; i<mi_lista_generos->size(); i++)
    {

        GeneroSeleccion *genero  = new GeneroSeleccion(NULL, i+1);

        //introduce el nombre al widget

        genero->set_genero_datos(mi_lista_generos->at(i).item, mi_lista_generos->at(i).cantidad_int );


        connect(this, SIGNAL(click_todos(bool)), genero, SLOT(set_toggled(bool)));


        //pasa el puntero de lista de valores para ser manipulado por el
        //widget
        genero->set_lista_borrado(lista_borrado);

        ui->verticalLayout_scroll->addWidget(genero);
    }

    ui->verticalLayout_scroll->addItem(new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding));

this->show();

}

void ListaGeneros::borra_widgets_hijos()
{
    QLayoutItem *child;

    //qDebug()  << "antes:" << ui->verticalLayout_scroll->count();

    while ((child = ui->verticalLayout_scroll->takeAt(0)) != 0)
    {

    disconnect(child->widget(), 0, 0, 0);

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

ListaGeneros::~ListaGeneros()
{
    delete ui;
}

void ListaGeneros::on_pushButton_confirmar_clicked()
{
    //hace algo borra
    emit borra_generos_nulos();

    this->hide();
}

void ListaGeneros::on_pushButton_cerrar_clicked()
{
    mi_lista_generos->clear();
    borra_widgets_hijos();
    this->hide();
}

void ListaGeneros::on_checkBox_todoGen_toggled(bool checked)
{
    emit click_todos(checked);
    emit click_todos(checked);
}
