#ifndef GENEROSELECCION_H
#define GENEROSELECCION_H

#include <QWidget>

#include "definiciones.h"

#include <QDebug>

namespace Ui {
class GeneroSeleccion;
}

class GeneroSeleccion : public QWidget
{
    Q_OBJECT

public:
    explicit GeneroSeleccion(QWidget *parent = 0, int numero = -1);

    void set_genero_datos(QString genero, int valor);

    void set_lista_borrado(QStringList *lista);

    ~GeneroSeleccion();

private slots:

    void on_checkBox_borrado_toggled(bool checked);

private:
    Ui::GeneroSeleccion *ui;

    QStringList* lista_borrado;

public slots:
    void set_toggled(bool checked);
};

#endif // GENEROSELECCION_H
