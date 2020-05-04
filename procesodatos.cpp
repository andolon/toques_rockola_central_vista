#include "procesodatos.h"

#include <unistd.h>



ProcesoDatos::ProcesoDatos(QObject *parent) : QObject(parent)
{

    tmp_dir.clear();

    bool retorno=false;

    //tmp_dir = "/tmp/" + QString("%1").arg(NOMBRE_APLICACION) + QString("%1").arg(getpid());

    tmp_dir = "/tmp/" + QString("%1").arg(NOMBRE_APLICACION);

    tmp_archivo_sqlite = QString(tmp_dir +"/"+DATABASEFILE_K);



    if(!mi_dir.exists(tmp_dir))
    {
        retorno = mi_dir.mkdir(tmp_dir);

        if(retorno == false)
        {
            QMessageBox msgBox;
            msgBox.setText("Alerta");
            msgBox.setInformativeText("No pudo ser creado directorio \n temporal");
            msgBox.exec();

            // emit(error_permisos());
        }

    }



    //qDebug() << "Directorio actual   " << mi_dir.currentPath();
    qDebug() << "Directorio temporal " << tmp_dir;

}

void ProcesoDatos::crea_baseDatos(bool borrado)
{
    QProcess _proceso;


    if(borrado)
    {

        if(QFileInfo::exists(tmp_archivo_sqlite))
        {
            if(false == QFile::remove(tmp_archivo_sqlite))
            {
                qDebug() << "no se pudo borrar archivo sqlite";
                return;
            }
        }

        QString cmd = QString("sqlite3 %1 \"create table aTable(field1 int); drop table aTable;\"").arg(tmp_archivo_sqlite);

        _proceso.start(cmd);    // start cmd
        _proceso.waitForFinished(-1);          // wait till done...
        QString output = _proceso.readAllStandardOutput();     // get std output
        QString strerror = _proceso.readAllStandardError();    // get std output errors
        qDebug() << "sqlite: " << output;     // print output
        qDebug() << "error:  " << strerror;   // print errors

    }


    datamanager = new Datamanager(this, tmp_dir);


    if(borrado)
    {
        drop_sql.append( QString("DROP TABLE  IF EXISTS  TBLCanciones;"));

        create_sql.append(QString("CREATE TABLE \"TBLCanciones\" ( \
                             \"nombre\" VARCHAR, \
                             \"ruta\" VARCHAR, \
                             \"cantidad\" INTEGER, \
                             \"tipo\" VARCHAR, \
                             \"genero\" VARCHAR, \
                             \"artista\" VARCHAR, \
                             \"codigo\" INTEGER PRIMARY KEY  NOT NULL \
                             ); "));

         create_sql.append(QString("CREATE TABLE \"gps\" ( \
        \"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , \
        \"latitud\" DOUBLE, \
        \"longitud\" DOUBLE); "));



        for(int k=0; k<drop_sql.size(); k++)
        {
            //datamanager->query->exec("BEGIN TRANSACTION");

            datamanager->query->exec(drop_sql.at(k));

            //datamanager->query->exec("COMMIT TRANSACTION");

            if (datamanager->query->lastError().isValid())
            {

                qDebug() << "Error: " << datamanager->query->lastError().text().toLatin1().constData();

            }
            else
            {
                qDebug()  << "borrada tabla" ;
            }
        }


        for(int k=0; k<create_sql.size(); k++)
        {

            //datamanager->query->exec("BEGIN TRANSACTION");

            datamanager->query->exec(create_sql.at(k));

            //datamanager->query->exec("COMMIT TRANSACTION");

            if (datamanager->query->lastError().isValid())
            {

                qDebug() << "Error: " << datamanager->query->lastError().text().toLatin1().constData();

            }
            else
            {
                qDebug()  << "creada tabla" ;
            }

        }

    }
}


bool ProcesoDatos::corre_mdbexport(QString ruta_mdb)
{

    tmp_archivo_mdb.clear();
    tmp_archivo_mdb = QString(tmp_dir+"/"+QString("%1").arg(BASE_DATOS_ACCESS));

    tmp_archivo_script_sql.clear();
    tmp_archivo_script_sql = QString(tmp_dir+"/"+QString("%1").arg(SCRIPT_SQL_CANCIONES));


    if(QFileInfo::exists(tmp_archivo_script_sql))
    {
        if(false == QFile::remove(tmp_archivo_script_sql))
        {
            qDebug() << "no se pudo borrar archivo " << tmp_archivo_script_sql;
            return false;
        }
    }


    if(QFileInfo::exists(tmp_archivo_mdb))
    {
        if(false == QFile::remove(tmp_archivo_mdb))
        {
            qDebug() << "no se pudo borrar archivo  " << tmp_archivo_mdb;
            return false;
        }
    }

    //-- Copia .mdb a directorio temporal
    if(false == QFile::copy(ruta_mdb, tmp_archivo_mdb))
    {
        qDebug() << "no pudo copiar archivo " << tmp_archivo_mdb;
    }


    QProcess _proceso;
    QString cmd = QString("%1 -I sqlite %2 TBLCanciones").arg(MDB_PROGRAMA , tmp_archivo_mdb);


    _proceso.start(cmd);    // start cmd
    _proceso.waitForFinished(-1);          // wait till done...
    QString output = _proceso.readAllStandardOutput();     // get std output
    //QString strerror = gettables.readAllStandardError();    // get std output errors
    //qDebug() << output;     // print output
    //qDebug() << strerror;   // print errors

    numero_de_registros = output.count('\n');

    qDebug() << "hay " << numero_de_registros;

    QFile file(tmp_archivo_script_sql);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream.setCodec("ISO 8859-1");
        stream << output << endl;
    }


    file.close();

    poblar_baseDatos();

    QString mensaje;
    mensaje.append("finalizado");

    QMessageBox msgBox;
    msgBox.setText(tr("proceso"));
    msgBox.setInformativeText(mensaje);
    msgBox.exec();


    barra_de_progreso->hide();


    this->text_edit->show();

    QStringList informe = informe_basico();

    for(int r=0; r < informe.size();r++)
    {
        //count_warnings++;
        this->text_edit->textCursor().insertHtml(QString("%1 ").arg(informe.at(r)));
    }





    return true;
}


void ProcesoDatos::insertaNuevalinea()
{

    QFile file("/tmp/canciones.sql");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Open new file to write
        QFile temp("/tmp/canciones2.sql");
        if (temp.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("ISO 8859-1");

            QTextStream out(&temp);
            out.setCodec("ISO 8859-1");


            QString line;
            do {
                line = stream.readLine(1);

                out << line;

                if(line.contains(";"))
                {
                    out << "\n";
                }

                //qDebug() << line;
            } while(!line.isNull());

            temp.close();
        }
        file.close();
    }


}

void ProcesoDatos::poblar_baseDatos()
{

    bool sql_ok = true;

    QFile file(tmp_archivo_script_sql);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        int i=0;


        datamanager->_rk_database.transaction();


        QTextStream stream(&file);
        stream.setCodec("ISO 8859-1");


        barra_de_progreso->show();
        barra_de_progreso->setValue(0);
        barra_de_progreso->setRange(0, numero_de_registros);

        int cuenta_errores=0;

        QString line;
        do {

            barra_de_progreso->setValue(i);
            i++;

            QCoreApplication::processEvents();

            line = stream.readLine();

            //qDebug() << line;

            sql_ok = datamanager->query->exec(line);


            if(!sql_ok && (line.length()>0))
            {
                QString error;

                error = datamanager->query->lastError().text();

                if(false == error.contains("empty query", Qt::CaseInsensitive))
                {
                    cuenta_errores++;

                    qDebug() <<"e1:" << "fallo la inserción" ;
                    qDebug() <<"e2" << error;
                    qDebug() <<"s:" << line;
                }

            }
            else
            {
#ifdef EN_DEBUG
                // cout << endl << "sin error" ;
#endif
            }



        } while(!line.isNull());

        if(cuenta_errores>20)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("proceso"));
            msgBox.setInformativeText("Hubo errores en la base de datos.\n \
                                      Reinicie la aplicación y repita la operación");
                                      msgBox.exec();
        }

        file.close();

        datamanager->_rk_database.commit();
    }
}

void ProcesoDatos::set_progressBar(QProgressBar *bar)
{
    this->barra_de_progreso = bar;
}

void ProcesoDatos::set_textEdit(QTextEdit *textEdit)
{
    this->text_edit = textEdit;
}

QStringList ProcesoDatos::informe_basico()
{
    QStringList informe;

    QString q;

    //generos

    q = QString("SELECT  genero, count(*) FROM tblcanciones GROUP BY genero ORDER BY count(*) DESC;");

    datamanager->query->exec(q);

    if (datamanager->query->lastError().isValid())
    {

        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();

    }

    informe.append("<b> Generos : </b>");

    while( datamanager->query->next() )
    {
        informe.append( datamanager->query->value(0).toString() );
        informe.append(": ");
        informe.append( datamanager->query->value(1).toString() );

        informe.append(" - ");
    }

    informe.append("<br><br>");



    // total canciones no tocadas

    q = QString("select count(*) from tblcanciones where cantidad==0;");

    datamanager->query->exec(q);

    if (datamanager->query->lastError().isValid())
    {
#ifdef EN_DEBUG
        cout << endl << "Error " << datamanager->query->lastError().text().toLatin1().constData();
#endif
    }

    informe.append("<b> Canciones no tocadas : </b>");

    while( datamanager->query->next() )
    {
        informe.append( datamanager->query->value(0).toString() );
    }



    return informe;
}

QList<DatoConsultaBasica>   ProcesoDatos::consulta_generos_canciones(int opcion)
{
    QList<DatoConsultaBasica> resultset;

    QString query;

    //generos

    if(opcion==TODAS_)
    {
        //----- TEMAS OLVIDADOS
        query = QString("SELECT  genero, count(*) FROM tblcanciones GROUP BY genero ORDER BY genero ASC;");
    }

    if(opcion==OLVIDADAS_)
    {
        //----- TEMAS OLVIDADOS
        query = QString("SELECT  genero, count(*) FROM tblcanciones where cantidad==0 GROUP BY genero ORDER BY genero ASC;");
    }

    if(opcion==TOCADAS_)
    {
        //----- TEMAS TOCADOS
        query = QString("SELECT  genero, count(*) FROM tblcanciones where cantidad>=1 GROUP BY genero ORDER BY genero ASC;");
    }

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    //informe.append("<b> Generos : </b>");

    while( datamanager->query->next() )
    {
        DatoConsultaBasica dato;
        dato.item   = datamanager->query->value(0).toString();
        dato.cantidad_int  = datamanager->query->value(1).toReal();
        resultset.append(  dato );
    }

    //------------

    return resultset;
}

QList<DatoConsultaBasica>   ProcesoDatos::consulta_generos_menos_tocados(void)
{
    QList<DatoConsultaBasica> resultset;

    QString query;

    //generos

    //----- TEMAS OLVIDADOS
    query = QString("SELECT  genero, count(*) FROM tblcanciones where cantidad==0 GROUP BY genero ORDER BY genero ASC;");

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    //informe.append("<b> Generos : </b>");

    while( datamanager->query->next() )
    {
        DatoConsultaBasica dato;
        dato.item   = datamanager->query->value(0).toString();
        dato.cantidad_int  = datamanager->query->value(1).toReal();
        resultset.append(  dato );
    }

    //------------

    return resultset;
}



long ProcesoDatos::acumula_numero_archivos(QList<DatoConsultaBasica> resultset)
{
    long acumulador = 0;

    QString query;

    QList<DatoConsultaBasica> copia = resultset;

    for(int o=0; o< copia.size();o++)
    {
        //qDebug() <<"set:" << copia.at(o).item << copia.at(o).cantidad_int ;

        query = QString("select count(*) from tblcanciones where cantidad>=0 and genero == \"%1\";").arg(copia.at(o).item);

        //qDebug() << query;

        datamanager->query->exec(query);

        if (datamanager->query->lastError().isValid())
        {
            qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
        }

        while( datamanager->query->next() )
        {
            acumulador += datamanager->query->value(0).toInt();
        }

    }

    return acumulador;
}

QList<DatoConsultaBasica>   ProcesoDatos::consulta_generos_canciones_alfa(int opcion, bool recorte)
{
    QList<DatoConsultaBasica> resultset;

    QStringList generos_lista;

    QString query;

    //generos

    query = QString("select distinct genero from tblcanciones order by genero asc;");

    //qDebug() << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {
        generos_lista.append(datamanager->query->value(0).toString());

        //qDebug() << datamanager->query->value(0).toString();
    }


    for(int k=0; k<generos_lista.size(); k++)
    {
        if(opcion==TODAS_)
            query = QString("select genero, count(*) from tblcanciones where cantidad>=0 and genero == \"%1\";").arg(generos_lista.at(k));

        if(opcion==TOCADAS_)
            query = QString("select genero, count(*) from tblcanciones where cantidad>0 and genero == \"%1\";").arg(generos_lista.at(k));

        if(opcion==OLVIDADAS_)
            query = QString("select genero, count(*) from tblcanciones where cantidad==0 and genero == \"%1\";").arg(generos_lista.at(k));

        //qDebug() << query;

        datamanager->query->exec(query);

        if (datamanager->query->lastError().isValid())
        {
            qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
        }

        while( datamanager->query->next() )
        {
            DatoConsultaBasica dato;


            dato.cantidad_int = datamanager->query->value(1).toInt();

            dato.item     = datamanager->query->value(0).toString();

            if(dato.item.size()==0)
            {
                dato.item = generos_lista.at(k);
            }



            resultset.append(dato);

            //qDebug() << dato.item << dato.cantidad;
        }

    }


    if((recorte==true)&&(opcion==TOCADAS_))
    {

        bubble_sort(resultset);

        resultset = resultset.mid(0, 10);

        //acumula_numero_archivos(resultset);

        //qDebug() << "acumulador:" << acumulador;

    }

    return resultset;
}



QList<DatoConsultaBasica>   ProcesoDatos::consulta_formatos(void)
{
    QList<DatoConsultaBasica> resultset;

    QString query;

    //----- ARCHIVOS DE AUDIO
    query = QString("SELECT count(*) FROM tblcanciones where tipo == \".mp3\" or tipo == \".wav\";");

    qDebug() << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }



    while( datamanager->query->next() )
    {
        DatoConsultaBasica dato;
        dato.item = "Audio";
        dato.cantidad_int  = datamanager->query->value(0).toInt();
        resultset.append(  dato );
    }

    //------
    //----- ARCHIVOS DE AUDIO
    query = QString("SELECT count(*) FROM tblcanciones where tipo == \".wmv\" or tipo ==  \".mpg\" or tipo == \".avi\" or tipo == \"mpeg\";");

    qDebug() << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }



    while( datamanager->query->next() )
    {
        DatoConsultaBasica dato;
        dato.item = "Video";
        dato.cantidad_int  = datamanager->query->value(0).toInt();
        resultset.append(  dato );
    }


    //------------

    return resultset;
}

QList<DatoConsultaBasica> ProcesoDatos::consulta_generos_nunca_tocados(void)
{
    QList<DatoConsultaBasica> generos_no_tocados;


    QString query;

    //-----  GENEROS NUNCA TOCADOS
    query = QString("select genero, count(*) from TBLCanciones where cantidad == 0 group by genero order by count(*) desc;");

    //qDebug() << "borrar: " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {
        //resulset_mas_que_ceros.append(datamanager->query->value(0).toString());
        //qDebug() << datamanager->query->value(0).toString() <<  " "  << datamanager->query->value(1).toInt();
        DatoConsultaBasica dato;

        dato.item = datamanager->query->value(0).toString();
        dato.cantidad_int = datamanager->query->value(1).toInt();
        generos_no_tocados.append(dato);
    }

    return generos_no_tocados;
}

QStringList ProcesoDatos::consulta_archivos_nunca_tocados(QString genero_filtro, QString artista_filtro)
{
    QStringList archivos_nunca_tocados;


    QString query;

    //-----  NUNCA TOCADOS
    query = QString("select ruta from TBLCanciones where cantidad == 0 and genero== '%1' and artista == '%2' order by ruta asc;").arg(genero_filtro, artista_filtro);

    //qDebug() << "borrar: " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {
        //resulset_mas_que_ceros.append(datamanager->query->value(0).toString());
        //qDebug() << datamanager->query->value(0).toString() <<  " "  << datamanager->query->value(1).toInt();
        archivos_nunca_tocados.append(datamanager->query->value(0).toString());
    }

    return archivos_nunca_tocados;

}

QStringList   ProcesoDatos::consulta_artistas_nunca_tocados(QString genero_sel)
{
    QSet<QString> resulset_ceros;
    QSet<QString> resulset_mas_que_ceros;

    QString query;

    //----- ARTISTAS NUNCA SELECCIONADOS
    //-- primero los pongo todos
    query = QString("select distinct artista from tblcanciones where genero == '%1' AND cantidad==0 order by artista ASC;").arg(genero_sel);

    qDebug() << "borrar: " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {

        //resulset_ceros.append(  datamanager->query->value(0).toString() );
        resulset_ceros.insert( datamanager->query->value(0).toString() );
    }

    //------------
    //qDebug() << "==0 hay " << resulset_ceros.count();

    //----- ARTISTAS NUNCA SELECCIONADOS
    // -- luego salvo a los que ya tienen algo

    query = QString("select distinct artista from tblcanciones where genero == '%1' AND cantidad>0 order by artista ASC;").arg(genero_sel);

    //qDebug() << "borrar: " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {
        //resulset_mas_que_ceros.append(datamanager->query->value(0).toString());

        resulset_mas_que_ceros.insert( datamanager->query->value(0).toString() );
    }

    //qDebug() << ">0 hay " << resulset_mas_que_ceros.count();

    resulset_ceros = resulset_ceros.subtract( resulset_mas_que_ceros );

    //resulset_ceros.intersect( resulset_mas_que_ceros );

    /*
    for(int i=0; i < resulset_ceros.size(); i++)
    {
        for(int j=0; j < resulset_mas_que_ceros.size(); j++)
        {
            if(0 == QString::compare(resulset_ceros.at(i), resulset_mas_que_ceros.at(j)  ))
            {
            resulset_ceros.removeAt(i);
            }
        }
    }*/



    //qDebug() << "==0 hay " << resulset_ceros.count();

    QStringList ret = resulset_ceros.toList();

    qSort(ret);

    return ret;
}


QList<DatoConsultaBasica>   ProcesoDatos::consulta_artistas_mas_tocados(QString genero_sel)
{
    QList<DatoConsultaBasica> resultset;

    QString query;

    //----- ARTISTAS MAS SELECCIONADOS
    query = QString("select distinct artista, cantidad from tblcanciones where genero == '%1' AND cantidad>0 order by cantidad DESC, artista ASC").arg(genero_sel);

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    //informe.append("<b> Generos : </b>");

    while( datamanager->query->next() )
    {
        DatoConsultaBasica dato;
        dato.item   = datamanager->query->value(0).toString();
        dato.cantidad_int  = datamanager->query->value(1).toReal();
        resultset.append(  dato );
    }

    //------------

    return resultset;
}

QStringList ProcesoDatos::consulta_rutas_artistas(QString genero, QStringList artista, bool todos)
{
    QStringList rutas;

    QString query;

    for(int i=0; i<artista.size(); i++)
    {

        //----   RUTA DE ARTISTA

        if(todos==false)
        {
            query = QString("select ruta from tblcanciones where artista == '%1' and genero == '%2' limit 1;").arg(artista.at(i), genero);
        }
        else
        {
            query = QString("select ruta from tblcanciones where artista == '%1' and genero == '%2';").arg(artista.at(i), genero);
        }

        //qDebug() << " " << query;

        datamanager->query->exec(query);

        if (datamanager->query->lastError().isValid())
        {
            qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
        }

        //informe.append("<b> Generos : </b>");

        while( datamanager->query->next() )
        {
            rutas.append( datamanager->query->value(0).toString() );
        }
    }

    return rutas;
}

//QList<DatoTopX> ProcesoDatos::consulta_top_x(int top)
QList<DatoTopX> ProcesoDatos::consulta_top_x(void)
{
    QList<DatoTopX>  mi_top_x;

    QString query;

    //top >0
    //query = QString("select genero, artista, nombre, cantidad from tblcanciones where cantidad >0 order by cantidad desc, genero asc, artista asc, nombre asc limit %1; ").arg(top);
    query = QString("select genero, artista, nombre, cantidad from tblcanciones where cantidad >=2 order by cantidad desc, genero asc, artista asc, nombre asc limit 1000;");

    //qDebug() << " " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {

        DatoTopX dato;
        dato.genero   = datamanager->query->value(0).toString();
        dato.artista  = datamanager->query->value(1).toString();
        dato.nombre   = datamanager->query->value(2).toString();
        dato.cantidad = datamanager->query->value(3).toInt();

        mi_top_x.append(dato);

    }

    return mi_top_x;
}


QStringList ProcesoDatos::consulta_ruta_top_x(void)
{
    QStringList  mi_top_x;

    QString query;

    //query = QString("select ruta, genero, artista, nombre, cantidad from tblcanciones where cantidad >0 order by cantidad desc, genero asc, artista asc, nombre asc limit %1; ").arg(top);
    query = QString("select ruta, genero, artista, nombre, cantidad from tblcanciones where cantidad >=2 order by cantidad desc, genero asc, artista asc, nombre asc limit 1000;");

    //qDebug() << " " << query;

    datamanager->query->exec(query);

    if (datamanager->query->lastError().isValid())
    {
        qDebug() << "Error " << datamanager->query->lastError().text().toLatin1().constData();
    }

    while( datamanager->query->next() )
    {
        mi_top_x.append(datamanager->query->value(0).toString());
    }

    return mi_top_x;
}

void ProcesoDatos::exporta_top_x(void)
{
    QList<DatoTopX> top_x;

    DatoTopX item;

    QString tmp_archivo_csv;

    QString mac = getMacAddress().join("");

    directorio_para_top = dialogo_directorio(directorio_para_top, NULL);

    tmp_archivo_csv = QString(directorio_para_top+"/"+  mac + QDateTime::currentDateTime().toString("_yyyy_MM_dd_")+QString("%1").arg(ARCHIVO_CSV));


    if(QFileInfo::exists(tmp_archivo_csv))
    {
        if(false == QFile::remove(tmp_archivo_csv))
        {
            qDebug() << "no se pudo borrar archivo csv";
            return;
        }
    }

    top_x = this->consulta_top_x();



    // Open new file to write
    QFile archivo(tmp_archivo_csv);
    if (archivo.open(QIODevice::ReadWrite | QIODevice::Text))
    {

        QTextStream out(&archivo);
        //out.setCodec("ISO 8859-1");



        out << "Pos" << "," << "Genero" <<","<< "Artista" <<","<< "Nombre" <<"," << "Cantidad" << "\n";

        for( int r = 0; r < top_x.size(); ++r )
        {
            item = top_x.at(r);
            item.genero.replace("," , "-");
            item.artista.replace("," , "-");
            item.nombre.replace("," , "-");

            out << r+1 << "," << item.genero <<","<< item.artista <<","<< item.nombre <<"," << item.cantidad << "\n";

        }

        archivo.close();
    }


}

QString ProcesoDatos::get_tmp_dir(void)
{
    return tmp_dir;
}

void ProcesoDatos::inserta_gps(QString latitud, QString longitud)
{
QString query;

query =  QString("DELETE FROM gps;");

datamanager->query->exec(query);


query =  QString("INSERT INTO gps (id, latitud, longitud) VALUES (NULL, '%1', '%2');").arg(latitud, longitud);

datamanager->query->exec(query);

if (datamanager->query->lastError().isValid())
{

    qDebug() << "Error: " << datamanager->query->lastError().text().toLatin1().constData();

}
else
{
    qDebug()  << "insert realizado" ;
}

}

ProcesoDatos::~ProcesoDatos()
{



}

