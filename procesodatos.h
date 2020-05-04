#ifndef PROCESODATOS_H
#define PROCESODATOS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <QProgressBar>
#include <QTextEdit>
#include <QList>
#include <QString>
#include <QDateTime>

#include "definiciones.h"
#include "datamanager.h"
#include "datos_tipos.h"
#include "utilidades.h"

class ProcesoDatos : public QObject
{
    Q_OBJECT
public:
    explicit ProcesoDatos(QObject *parent = 0);

    ~ProcesoDatos();

    void set_progressBar(QProgressBar *bar);

    void set_textEdit(QTextEdit *textEdit);

    long acumula_numero_archivos(QList<DatoConsultaBasica> resultset);

    QString get_tmp_dir(void);

    double espacio_libre;
    double espacio_top;

    QString directorio_para_top;

signals:
    void error_permisos(void);

public slots:

        void exporta_top_x(void);

        QList<DatoConsultaBasica>  consulta_generos_canciones_alfa(int opcion, bool recorte=false);

        QList<DatoConsultaBasica>  consulta_generos_canciones(int opcion);

        QList<DatoConsultaBasica>  consulta_generos_menos_tocados(void);

        QList<DatoConsultaBasica>  consulta_artistas_mas_tocados(QString genero_sel);

        QList<DatoConsultaBasica>  consulta_formatos(void);

        QList<DatoTopX>            consulta_top_x(void);

        QStringList          consulta_ruta_top_x(void);

        QStringList consulta_artistas_nunca_tocados(QString genero_sel);

        QList<DatoConsultaBasica> consulta_generos_nunca_tocados(void);

        QStringList consulta_archivos_nunca_tocados(QString genero_filtro, QString artista_filtro);

        QStringList consulta_rutas_artistas(QString genero, QStringList artista, bool todos=false);

        bool corre_mdbexport(QString ruta_mdb);

        void crea_baseDatos(bool borrado=false);

        void inserta_gps(QString latitud, QString longitud);

private:

    QDir mi_dir;
    QString tmp_dir;
    QString tmp_archivo_mdb;
    QString tmp_archivo_script_sql;
    QString tmp_archivo_sqlite;

    Datamanager *datamanager;

    QStringList create_sql;
    QStringList drop_sql;

    int numero_de_registros;

    QProgressBar *barra_de_progreso;
    QTextEdit    *text_edit;

private slots:

    void insertaNuevalinea();

    void poblar_baseDatos();

    QStringList informe_basico();


};

#endif // PROCESODATOS_H
