#ifndef LISTAARTISTAS_H
#define LISTAARTISTAS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDesktopWidget>
#include <QRect>
#include <QList>
#include <QMessageBox>


#include "datos_tipos.h"


namespace Ui {
class ListaArtistas;
}

class ListaArtistas : public QWidget
{
    Q_OBJECT

public:
    explicit ListaArtistas(QWidget *parent = 0);
    ~ListaArtistas();

    void set_contenido(QStringList lista, QString genero);

    QStringList *mi_lista_borrado;

    void init_artistas(int maximo);

    void init_archivos(int maximo);

    void artistas_set_valor(int valor);

    void artistas_set_texto(QString texto);

    void archivos_set_valor(int valor);

    void oculta_barras(void);

    //BorradoProgreso *borrado_progreso;

private slots:
    void on_pushButton_cerrar_clicked();

    void on_pushButton_confirmar_clicked();

private:
    Ui::ListaArtistas *ui;


    void borra_widgets_hijos();

signals:
    void confirma_borrado(void);

    void borrar_artistas();

};

#endif // LISTAARTISTAS_H
