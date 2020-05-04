#ifndef ARTISTASELECCION_H
#define ARTISTASELECCION_H

#include <QWidget>
#include <QList>

#include "datos_tipos.h"
#include "definiciones.h"

namespace Ui {
class ArtistaSeleccion;
}

class ArtistaSeleccion : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistaSeleccion(QWidget *parent = 0, int numero = -1);

    void set_artista_nombre(QString artista);

    void set_lista_artistas(QStringList* lista);



    ~ArtistaSeleccion();

private slots:

    void on_checkBox_borrado_toggled(bool checked);

private:
    Ui::ArtistaSeleccion *ui;

    QStringList* lista_copia;

};

#endif // ARTISTASELECCION_H
