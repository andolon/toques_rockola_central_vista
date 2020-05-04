#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <QDebug>
#include <QString>
#include <QDir>


#include "procesodatos.h"
#include "definiciones.h"
#include "graficas.h"
#include "datos_tipos.h"

#include "artistaseleccion.h"
#include "listaartistas.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
    //! dir_Choosed.
    /*!
      Señal emitida cuando el usuario escoge el directorio.
    */

    //void dir_Choosed(QString);

    void quit(void);


private slots:

    void on_pushButton_ruta_app_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_inicio_clicked();

    void on_pushButton_graficar_clicked();

    void on_pushButton_ruta_musica_clicked();

    void on_lineEdit_ruta_musica_textChanged(const QString &arg1);

    void imprime_espacio(void);

    //void mi_coordenada_gps(QString gps);

private:
    Ui::MainWindow *ui;
    QString ruta_app_rk;
    QString ruta_musica_rk;
    //QString coord_gps;
    //QString latitud;
    //QString longitud;
    Graficas *mi_graficador;
    ProcesoDatos *proceso_datos;

    void limpia_pantalla();

    //QString dialogo_directorio(QString inicio);

    bool hay_audios_en(QString ruta);

    QStringList getMacAddress(void);



};

#endif // MAINWINDOW_H
