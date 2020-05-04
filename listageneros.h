#ifndef LISTAGENEROS_H
#define LISTAGENEROS_H

#include <QDialog>
#include <QDesktopWidget>



#include "generoseleccion.h"
#include "procesodatos.h"

namespace Ui {
class ListaGeneros;
}

class ListaGeneros : public QDialog
{
    Q_OBJECT

public:
    explicit ListaGeneros(QWidget *parent = 0);

    void set_contenido(ProcesoDatos *proc);

    QList<DatoConsultaBasica> *mi_lista_generos;

    QStringList *lista_borrado;

    ~ListaGeneros();

private slots:
    void on_pushButton_confirmar_clicked();

    void on_pushButton_cerrar_clicked();

    void on_checkBox_todoGen_toggled(bool checked);

private:
    Ui::ListaGeneros *ui;



    void borra_widgets_hijos(void);

signals:
    void borra_generos_nulos(void);
    void click_todos(bool);
};

#endif // LISTAGENEROS_H
