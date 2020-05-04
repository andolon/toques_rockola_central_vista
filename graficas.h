#ifndef GRAFICAS_H
#define GRAFICAS_H

#include <QMainWindow>
#include <QList>
#include <QDesktopServices>


#include "../qcustomplot/qcustomplot.h"
#include "datos_tipos.h"
#include "procesodatos.h"
#include "utilidades.h"
#include "listaartistas.h"
#include "listageneros.h"
#include "espera.h"



namespace Ui {
class Graficas;
}

class Graficas : public QMainWindow
{
    Q_OBJECT

public:
    explicit Graficas(QWidget *parent = 0, ProcesoDatos *pros_dat=0);
    ~Graficas();

    void set_musica_dir(QString ruta);

public slots:
    void configura_barras_tocadas_todas(QList<DatoConsultaBasica> dato_tocados, QList<DatoConsultaBasica> dato_todos);

    void configura_barras_primeras_menos_tocadas(QList<DatoConsultaBasica> dato_tocados);

    void configura_barras_generos_menos_tocados(QList<DatoConsultaBasica> dato);

    void configura_barras_formatos(QList<DatoConsultaBasica> dato_formato);

    void grafica_temas_tocados_olvidados(void);

    void grafica_generos_menos_tocados(void);

    void grafica_primeros_temas_menos_tocados(void);

    void grafica_formatos(void);

    //void set_proceso_datos(ProcesoDatos *proceso_datos);

private slots:

    void on_pushButton_clicked();

    void on_comboBox_graficas_currentIndexChanged(int index);

    void on_pushButton_genero_sel_clicked();

    void borrado_artistas(void);

    void borrado_generos(void);

    //QStringList analiza_top_x(QStringList rutas);

    void copia_top_x(void);

    void on_pushButton_topx_clicked();

    void on_pushButton_copia_top_x_clicked();

    void on_pushButton_baobab_clicked();

    void on_pushButton_borra_todo_clicked();

private:
    Ui::Graficas *ui;
    QString demoName;
    ProcesoDatos *mi_proceso_datos;
    ListaArtistas *listaartista;
    ListaGeneros  *listageneros;

    QString genero_seleccionado;
    QString ruta_musica;
    QList<Ruta_Antes_Despues> rutas_mover;

    Espera *espera;

    void limpia_grafica();

signals:

    void top_copiado(void);
};

#endif // GRAFICAS_H
