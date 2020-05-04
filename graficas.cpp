#include "graficas.h"
#include "ui_graficas.h"

Graficas::Graficas(QWidget *parent, ProcesoDatos *pros_dat) :
    QMainWindow(parent),
    ui(new Ui::Graficas)
{
    ui->setupUi(this);

    mi_proceso_datos = pros_dat;

    if(mi_proceso_datos == 0)
    {

    }

    this->setStyleSheet(myStilo);





    int alto_pantalla;
    int ancho_pantalla;
    QRect rec = QApplication::desktop()->availableGeometry(-1);
    alto_pantalla  = rec.height();
    ancho_pantalla = rec.width();

    this->resize(ancho_pantalla, alto_pantalla);

    QPoint pos_ = ui->customPlot->pos();

    ui->customPlot->setGeometry(pos_.x(), pos_.y(), ancho_pantalla-20, alto_pantalla-(ui->groupBox_opciones->height()+10+pos_.y()) );

    ui->groupBox_opciones->resize(ancho_pantalla-20,ui->groupBox_opciones->height());

    ui->groupBox_opciones->move(pos_.x(), ui->customPlot->height()+pos_.y());


    QStringList items;

    items << "[Seleccione opción de la lista] V" //0
          << "Generos con todas los temas y temas tocados ->>"
          << "Géneros menos usados según porcentaje de uso ->>"
          << "10 primeros generos menos tocados"
          << "Formatos audio y video ->>"
             ;



    ui->comboBox_graficas->addItems(items);

    listaartista = new ListaArtistas(NULL);

    listageneros = new ListaGeneros(NULL);


    connect(listaartista, SIGNAL(borrar_artistas()), this, SLOT(borrado_artistas())  );

    connect(listageneros, SIGNAL(borra_generos_nulos()), this, SLOT(borrado_generos()));


    ui->comboBox_generos->clear();
    ui->comboBox_generos->hide();
    ui->pushButton_genero_sel->hide();
    ui->label_acumulador->hide();

    ui->comboBox_graficas->setCurrentIndex(0);
    ui->comboBox_graficas->show();



    //listageneros->lista_borrado->append("VALLENATO");
    //borrado_generos();

}

Graficas::~Graficas()
{
    delete ui;
}

void Graficas::on_pushButton_clicked()
{
    this->hide();
}

void Graficas::borrado_generos(void)
{
    //qDebug() << "he borrado generos completos";

    //qDebug() << "quiero borrar solo:" <<endl;

    //qDebug() << *listageneros->lista_borrado << endl;

    QStringList rutas_canciones_a_borrar;
    QStringList lista_artistas;
    QString archivo_a_borrar;
    QString ruta_inicial_archivo;
    QString ruta_final_archivo;
    QString tmp;
    QStringList ruta_windows;
    QString dir_acum;
    QDir DIR;

    espera = new Espera(NULL);
    espera->setStyleSheet(myStilo);
    espera->setWindowFlags(Qt::WindowStaysOnTopHint);
    espera->hide();

    espera->show();


    //xxxx
    //QStringList lista_artistas = mi_proceso_datos->consulta_artistas_nunca_tocados(genero_seleccionado);

    for(int i=0; i<listageneros->lista_borrado->size();i++)
    {
        lista_artistas = mi_proceso_datos->consulta_artistas_nunca_tocados(listageneros->lista_borrado->at(i));

        for(int k=0; k < lista_artistas.size(); k++ )
        {
            //qDebug() << "k:" << k;

            rutas_canciones_a_borrar = mi_proceso_datos->consulta_archivos_nunca_tocados(listageneros->lista_borrado->at(i), lista_artistas.at(k)) ;

            for(int j=0; j<rutas_canciones_a_borrar.size(); j++)
            {
                archivo_a_borrar = de_windows_a_unix( rutas_canciones_a_borrar.at(j) );

                ruta_windows = archivo_a_borrar.split("/");

                //tmp = archivo_a_borrar;

                //ruta_inicial_archivo = ruta_musica + tmp.remove(0, ruta_windows[1].size() +1); //+1 corrige algo
                ruta_inicial_archivo = ruta_musica  + "/" + ruta_windows[2] + "/" + ruta_windows[3] + + "/" + ruta_windows[4];

                //tmp = archivo_a_borrar;

                /*
                1   "music (J.D.P)1.3.14 ok"
                2   "Rock"
                3   "one republic"
                4   "Stop and Stare.wmv"
                */


                ruta_final_archivo = ruta_musica + DIR_BORRABLES  + "/" + ruta_windows[2] + "/" + ruta_windows[3] + + "/" + ruta_windows[4];
                        //tmp.remove(0, ruta_musica.size());

                /*
                qDebug() << endl;
                qDebug() << "___: " << archivo_a_borrar  << endl;
                qDebug() << "ini: " << ruta_inicial_archivo  << endl;
                qDebug() << "fin: " << ruta_final_archivo  << endl;
                */


                tmp = ruta_final_archivo.mid(0 , ruta_final_archivo.lastIndexOf("/"));

                QStringList directorios = tmp.split("/");

                dir_acum = directorios.at(0);

                for(int i = 1; i < directorios.size(); i++)
                {
                    //qDebug() << directorios.at(i);
                    dir_acum += "/" + directorios.at(i);
                    //qDebug() << dir_acum;
                    if(!DIR.exists(dir_acum))
                    {
                        //qDebug() << "no se pudo crear" <<  ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);

                        if(DIR.mkdir(dir_acum+"/"))
                        {
                            //qDebug() << "creado: " << dir_acum;

                            //ui->label_trabajando->setText(QString("Creando directorio: %1").arg(dir_acum));

                            //QCoreApplication::processEvents();
                        }
                    }
                }

                QFile FILE(ruta_inicial_archivo);
                //qDebug() << "de:    " << ruta_origen + mi_ruta;
                //qDebug() << "hacia: " << dir_destino + mi_ruta;

                if(FILE.exists())
                {
                    QFile::rename(ruta_inicial_archivo, ruta_final_archivo);
                }



                QString cmd;
                cmd = QString("find %1 -type d -empty -delete").arg(ruta_inicial_archivo);
                QProcess::startDetached(cmd);
                cmd = QString("find %1 -type d -exec rmdir {} + 2>/dev/null").arg(ruta_inicial_archivo);
                QProcess::startDetached(cmd);

                espera->tick();

            }
        }
    }



    espera->deleteLater();


}

void Graficas::borrado_artistas(void)
{

    QStringList ruta_primer_archivo = mi_proceso_datos->consulta_rutas_artistas(genero_seleccionado, *listaartista->mi_lista_borrado,false);
    QString ancestro_musica;
    QString     ruta_0;
    QStringList ruta_1;
    QString     ruta_origen;
    //--QString     ruta_genero_actual;
    QStringList ruta_3;
    QString     dir_destino;
    QString genero;
    QString artista;
    QDir DIR;

    listaartista->init_artistas(ruta_primer_archivo.size());

    //listaartista->borrado_progreso->show();

    listaartista->artistas_set_texto("Seleccione y confirme");

    for(int i=0; i<ruta_primer_archivo.size(); i++)
    {
        listaartista->artistas_set_valor(i);

        ruta_0 = de_windows_a_unix( ruta_primer_archivo.at(i) );

        ruta_1  = ruta_0.split("/");
        genero  = ruta_1.at(2);
        artista = ruta_1.at(3);

        listaartista->artistas_set_texto(artista);

        ancestro_musica = ruta_musica.mid(0,ruta_musica.lastIndexOf('/'));
        //qDebug() << "ancestro" << ancestro_musica;

        ruta_origen        = ruta_musica + "/"  + genero + "/" + artista + "/";

        //--ruta_genero_actual = ruta_musica + "/"  + genero ;

        ruta_3.clear();
        ruta_3 << ancestro_musica + "/";  //0
        ruta_3 << QString(DIR_BORRABLES) + "/";      //1
        ruta_3 << genero + "/";       //2
        ruta_3 << artista + "/";      //3

        //qDebug() << "ruta_m: " << ruta_musica;
        //qDebug() << "ruta_origen: " << ruta_origen;


        DIR.mkdir(ruta_3.at(0));
        DIR.mkdir(ruta_3.at(0)+ruta_3.at(1));
        DIR.mkdir(ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2));
        dir_destino = ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);
        DIR.mkdir(dir_destino);

        //qDebug() << "dir   : " << dir_destino;

        if(!DIR.exists(dir_destino))
        {
            qDebug() << "no se pudo crear" <<  ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);
            continue;
        }

        QStringList ruta_todos_archivos = mi_proceso_datos->consulta_rutas_artistas(genero_seleccionado, *listaartista->mi_lista_borrado,true);
        QString fonograma;
        int pos_b;

        listaartista->init_archivos(ruta_todos_archivos.size());

        for(int o=0; o < ruta_todos_archivos.size(); o++)
        {
            listaartista->archivos_set_valor(o);

            fonograma = ruta_todos_archivos.at(o);
            pos_b = fonograma.lastIndexOf("\\")+1;
            fonograma = fonograma.mid(pos_b);

            QFile FILE(ruta_origen + fonograma);
            //qDebug() << "de:    " << ruta_origen + mi_ruta;
            //qDebug() << "hacia: " << dir_destino + mi_ruta;

            if(FILE.exists())
            {
                QFile::rename(ruta_origen + fonograma, dir_destino + fonograma);
            }
        }

        //QDir DIR_2(ruta_origen);

        QString cmd;

        //DIR_2.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

        //qDebug() << "intento borrar: " << ruta_origen;
        //if(0 == DIR_2.count())
        //{
        //QFile::rename(ruta_origen, "/tmp");
        //DIR_2.remove(ruta_origen);
        cmd = QString("find %1 -type d -empty -delete").arg(ruta_origen);
        QProcess::startDetached(cmd);
        cmd = QString("find %1 -type d -exec rmdir {} + 2>/dev/null").arg(ruta_origen);
        QProcess::startDetached(cmd);
        //}
    }


    listaartista->oculta_barras();
    listaartista->artistas_set_texto("Terminado");

}

/*
QStringList Graficas::analiza_top_x(QStringList rutas)
{
QStringList ruta_convertida;

    for(int i=0; i<rutas.size(); i++)
    {
        ruta_convertida.append(de_windows_a_unix( ruta_convertida.at(i) ));
    }

return ruta_convertida;
}
*/

void Graficas::copia_top_x(void)
{

    QStringList ruta_archivo_top = mi_proceso_datos->consulta_ruta_top_x();
    QString ancestro_musica;
    QString     ruta_0;
    QStringList ruta_1;
    QString     ruta_origen;
    QStringList ruta_3;
    QString     dir_destino;
    QString genero;
    QString artista;
    QDir DIR;

    //--listaartista->init_artistas(ruta_archivo_top.size());
    //--listaartista->artistas_set_texto("Seleccione y confirme");

    mi_proceso_datos->directorio_para_top = dialogo_directorio(mi_proceso_datos->directorio_para_top, this);

    if(mi_proceso_datos->directorio_para_top.size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("Top cancelado"));
        msgBox.exec();

        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    DIR.mkdir(mi_proceso_datos->directorio_para_top);

    for(int i=0; i<ruta_archivo_top.size(); i++)
    {

        ruta_0 = de_windows_a_unix( ruta_archivo_top.at(i) );

        ruta_1  = ruta_0.split("/");
        genero  = ruta_1.at(2);
        artista = ruta_1.at(3);

        listaartista->artistas_set_texto(artista);

        ancestro_musica = ruta_musica.mid(0,ruta_musica.lastIndexOf('/'));
        //qDebug() << "ancestro" << ancestro_musica;

        ruta_origen        = ruta_musica + "/"  + genero + "/" + artista + "/";

        //--ruta_genero_actual = ruta_musica + "/"  + genero ;

        ruta_3.clear();

        //ruta_3 << ancestro_musica + "/";  //0 <<<--- ESTE FUNCIONA
        ruta_3 << mi_proceso_datos->directorio_para_top + "/";
        ruta_3 << QString(DIR_TOP_XX) + "/";      //1
        ruta_3 << genero + "/";       //2
        ruta_3 << artista + "/";      //3

        //qDebug() << "ruta_m: " << ruta_musica;
        //qDebug() << "ruta_origen: " << ruta_origen;


        DIR.mkdir(ruta_3.at(0));
        DIR.mkdir(ruta_3.at(0)+ruta_3.at(1));
        DIR.mkdir(ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2));
        dir_destino = ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);
        DIR.mkdir(dir_destino);

        //qDebug() << "dir   : " << dir_destino;

        if(!DIR.exists(dir_destino))
        {
            qDebug() << "no se pudo crear" <<  ruta_3.at(0)+ruta_3.at(1)+ruta_3.at(2)+ruta_3.at(3);
            continue;
        }

        //QStringList ruta_todos_archivos = mi_proceso_datos->consulta_rutas_artistas(genero_seleccionado, *listaartista->mi_lista_borrado,true);
        QString fonograma;
        int pos_b;

        //--listaartista->init_archivos(ruta_todos_archivos.size());

        //for(int o=0; o < ruta_todos_archivos.size(); o++)

        //--listaartista->archivos_set_valor(o);

        fonograma = ruta_archivo_top.at(i);
        pos_b = fonograma.lastIndexOf("\\")+1;
        fonograma = fonograma.mid(pos_b);

        Ruta_Antes_Despues r;

        r.origen  = ruta_origen + fonograma;
        r.destino = dir_destino + fonograma;
        rutas_mover.append(r);

        //qDebug() << "de:    " << ruta_origen + fonograma;
        //qDebug() << "hacia: " << dir_destino + fonograma;


    }



    mi_proceso_datos->espacio_top = 0;

    //desactivado
    /*

    double d_temporal = 0;

    for(int o=0; o <rutas_mover.size(); o++ )
    {
        QFileInfo INFO(rutas_mover.at(o).origen);

        d_temporal += (double) INFO.size();

    }

    mi_proceso_datos->espacio_top = d_temporal / (double) 1073741824;

    //qDebug() << "mi_proceso_datos->espacio_top" << mi_proceso_datos->espacio_top;

    //qDebug() << "mi_proceso_datos->espacio_libre" << mi_proceso_datos->espacio_libre;


    if(mi_proceso_datos->espacio_top > mi_proceso_datos->espacio_libre)
    {

        QMessageBox msgBox;
        msgBox.setText(tr("Alerta"));
        msgBox.setInformativeText(QString("El top consume %1 Gigabytes.\n Libere espacio").arg(QString::number(mi_proceso_datos->espacio_top)));
        msgBox.exec();

        QApplication::restoreOverrideCursor();

        listaartista->oculta_barras();
        listaartista->artistas_set_texto("Terminado");


        return;
    }
*/

    QProgressBar *barra = new QProgressBar(this);

    barra->move(320,240);
    barra->show();
    barra->setValue(0);
    barra->setRange(0, rutas_mover.size());



    for(int o=0; o <rutas_mover.size(); o++ )
    {
        QFile FILE(rutas_mover.at(o).origen);

        if(FILE.exists())
        {
            QFile::copy(rutas_mover.at(o).origen, rutas_mover.at(o).destino);
        }

        barra->setValue(o);

    }

    QApplication::restoreOverrideCursor();

    listaartista->oculta_barras();
    listaartista->artistas_set_texto("Terminado");

    barra->deleteLater();

    emit(top_copiado());

}

void Graficas::limpia_grafica()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
    ui->customPlot->clearFocus();
    ui->customPlot->clearMask();
    ui->customPlot->clearPlottables();
}

void Graficas::configura_barras_tocadas_todas(QList<DatoConsultaBasica> dato_tocados, QList<DatoConsultaBasica> dato_todos)
{

    limpia_grafica();

    demoName = "Generos y temas";
    // create empty bar chart objects:

    //QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *barra_todos = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *barra_tocados = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    //ui->customPlot->addPlottable(regen);
    ui->customPlot->addPlottable(barra_todos);
    ui->customPlot->addPlottable(barra_tocados);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    barra_tocados->setName("Temas tocados");
    pen.setColor(QColor(128, 128, 128));
    barra_tocados->setPen(pen);
    barra_tocados->setBrush(QColor(204, 0, 153));

    barra_todos->setName("Temas todos");
    pen.setColor(QColor(128, 128, 128));
    barra_todos->setPen(pen);
    barra_todos->setBrush(QColor(153, 221, 255));

    //regen->setName("Regenerative");
    pen.setColor(QColor(150, 222, 0));

    //regen->setPen(pen);
    //regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    //barra_tocados->moveAbove(barra_todos);
    barra_tocados->moveBelow(barra_todos);
    //regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;

    int k;

    for(k=1; k <= dato_todos.size(); k++)
    {
        ticks.append(k);
        labels.append(dato_todos.at(k-1).item);
    }


    ticks.append(k++);
    ticks.append(k++);

    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 6);
    ui->customPlot->xAxis->grid()->setVisible(true);



    ui->customPlot->xAxis->setRange(0, dato_todos.size()+5);


    //int maximo_valor_tocados_y   = 0;
    double maximo_valor_todos_y = 0;
    //int maximo_valor_total_y = 0;

    // Add data:

    //QVector<double> fossilData, nuclearData, regenData;

    QVector<double> temaTocadoData;
    QVector<double> temaTodosData;

    //fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;

    /*
     for(int v=1; v <= dato_tocados.size(); v++)
     {
          temaTocadoData.append(dato_tocados.at(v-1).cantidad_int);
     }
    */

    for(int v=1; v <= dato_todos.size(); v++)
    {
        if ( (int) maximo_valor_todos_y   < dato_todos.at(v-1).cantidad_int)
        {
            maximo_valor_todos_y  = dato_todos.at(v-1).cantidad_int;
        }
    }

    maximo_valor_todos_y = (1.2) * maximo_valor_todos_y;

    double valor_negativo = (-1)*(0.02)*maximo_valor_todos_y;

    bool hay_datos_cero = false;

    for(int v=1; v <= dato_todos.size(); v++)
    {
        if(dato_tocados.at(v-1).cantidad_int==0)
        {
            temaTocadoData.append(valor_negativo);
            hay_datos_cero = true;
        }
        else
        {
            temaTocadoData.append(dato_tocados.at(v-1).cantidad_int);
        }

        temaTodosData.append(dato_todos.at(v-1).cantidad_int - dato_tocados.at(v-1).cantidad_int);
    }


    if(hay_datos_cero==false) valor_negativo = 0.0;


    //maximo_valor_total_y = maximo_valor_todos_y + maximo_valor_tocados_y;


    //qDebug() << "valor maximo :" << maximo_valor_y;

    // prepare y axis:
    ui->customPlot->yAxis->setRange(valor_negativo, maximo_valor_todos_y);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border

    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setTickStep( qRound( maximo_valor_todos_y/10.0));


    ui->customPlot->yAxis->setLabel("Cantidad");
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);


    barra_tocados->setData(ticks, temaTocadoData);
    barra_todos->setData(ticks, temaTodosData);
    //regen->setData(ticks, regenData);

    // setup legend:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


    //ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}


void Graficas::configura_barras_primeras_menos_tocadas(QList<DatoConsultaBasica> dato_tocados)
{

    limpia_grafica();

    demoName = "Generos y temas";
    // create empty bar chart objects:

    //QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    QCPBars *barra_tocados = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    ui->customPlot->addPlottable(barra_tocados);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    barra_tocados->setName("Temas tocados");
    pen.setColor(QColor(128, 128, 128));
    barra_tocados->setPen(pen);
    barra_tocados->setBrush(QColor(204, 0, 153));

    //regen->setName("Regenerative");
    pen.setColor(QColor(150, 222, 0));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;

    int k;

    for(k=1; k <= dato_tocados.size(); k++)
    {
        ticks.append(k);
        labels.append(dato_tocados.at(k-1).item);
    }


    ticks.append(k++);
    ticks.append(k++);

    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 6);
    ui->customPlot->xAxis->grid()->setVisible(true);

    ui->customPlot->xAxis->setRange(0, dato_tocados.size()+5);

    double maximo_valor_todos_y = 10;

    // Add data:

    QVector<double> temaTocadoData;

    for(int v=1; v <= dato_tocados.size(); v++)
    {
        if ( (int) maximo_valor_todos_y   < dato_tocados.at(v-1).cantidad_int)
        {
            maximo_valor_todos_y  = dato_tocados.at(v-1).cantidad_int;
        }

        if(dato_tocados.at(v-1).cantidad_int == 0)
        {
            temaTocadoData.append(-2);
        }
        else
        {
            temaTocadoData.append(dato_tocados.at(v-1).cantidad_int);
        }
    }

    maximo_valor_todos_y = (1.2) * maximo_valor_todos_y;


    //qDebug() << "valor maximo :" << maximo_valor_y;

    // prepare y axis:
    ui->customPlot->yAxis->setRange(-2, maximo_valor_todos_y);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border

    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setTickStep( qRound( maximo_valor_todos_y/10.0));

    ui->customPlot->yAxis->setLabel("Cantidad");
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);

    barra_tocados->setData(ticks, temaTocadoData);

    // setup legend:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->customPlot->replot();
}

void Graficas::configura_barras_generos_menos_tocados(QList<DatoConsultaBasica> dato_tocados)
{

    limpia_grafica();

    demoName = "Generos y temas";
    // create empty bar chart objects:

    //QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    //QCPBars *barra_olvidados = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *barra_tocados = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    //ui->customPlot->addPlottable(regen);
    //ui->customPlot->addPlottable(barra_olvidados);
    ui->customPlot->addPlottable(barra_tocados);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    barra_tocados->setName("% Temas tocados");
    pen.setColor(QColor(34, 51, 0));
    barra_tocados->setPen(pen);
    barra_tocados->setBrush(QColor(0, 179, 60, 50));

    //barra_olvidados->setPen(pen);
    //barra_olvidados->setBrush(QColor(1, 92, 191, 50));

    //regen->setName("Regenerative");
    pen.setColor(QColor(150, 222, 0));

    //regen->setPen(pen);
    //regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    //barra_olvidados->moveAbove(barra_tocados);
    //regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;

    int k;

    for(k=1; k <= dato_tocados.size(); k++)
    {
        ticks.append(k);
        labels.append(dato_tocados.at(k-1).item);
    }


    ticks.append(k++);
    ticks.append(k++);

    //ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;

    //labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";

    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 6);
    ui->customPlot->xAxis->grid()->setVisible(true);


    ui->customPlot->xAxis->setRange(0, dato_tocados.size()+5);


    double maximo_valor_tocados_y   = 0;

    // Add data:

    //QVector<double> fossilData, nuclearData, regenData;

    QVector<double> temaTocadoData;


    //fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;

    for(int v=1; v <= dato_tocados.size(); v++)
    {

        temaTocadoData.append(dato_tocados.at(v-1).cantidad_double);

        if (maximo_valor_tocados_y < dato_tocados.at(v-1).cantidad_double)
        {
            maximo_valor_tocados_y = dato_tocados.at(v-1).cantidad_double;
        }
    }

    maximo_valor_tocados_y += maximo_valor_tocados_y/20;

    //nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
    //regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;

    //qDebug() << "valor maximo :" << maximo_valor_y;

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, maximo_valor_tocados_y);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border

    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setTickStep( qRound( maximo_valor_tocados_y/10.0));


    ui->customPlot->yAxis->setLabel("Porcentaje tocados %");
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);


    barra_tocados->setData(ticks, temaTocadoData);
    //barra_olvidados->setData(ticks, temaOlvidadoData);
    //regen->setData(ticks, regenData);

    // setup legend:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


    //ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}


void Graficas::configura_barras_formatos(QList<DatoConsultaBasica> dato_formato)
{

    limpia_grafica();

    demoName = "Formatos";
    // create empty bar chart objects:

    QCPBars *barra_formatos = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

    ui->customPlot->addPlottable(barra_formatos);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    barra_formatos->setName("Formatos");
    pen.setColor(QColor(200, 50, 50));
    barra_formatos->setPen(pen);
    barra_formatos->setBrush(QColor(240, 100, 100));

    //pen.setColor(QColor(1, 92, 191));

    //pen.setColor(QColor(200, 50, 50));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;

    int k;

    for(k=1; k <= dato_formato.size(); k++)
    {
        ticks.append(k);
        labels.append(dato_formato.at(k-1).item);
    }


    ticks.append(k++);
    ticks.append(k++);

    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 6);
    ui->customPlot->xAxis->grid()->setVisible(true);

    ui->customPlot->xAxis->setRange(0, dato_formato.size()+5);


    int maximo_valor_tocados_y   = 0;

    QVector<double> temaTocadoData;

    for(int v=1; v <= dato_formato.size(); v++)
    {
        temaTocadoData.append(dato_formato.at(v-1).cantidad_int);
        if (maximo_valor_tocados_y < dato_formato.at(v-1).cantidad_int)
        {
            maximo_valor_tocados_y = dato_formato.at(v-1).cantidad_int;
        }
    }

    maximo_valor_tocados_y += (int) maximo_valor_tocados_y/20;

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, maximo_valor_tocados_y);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border

    ui->customPlot->yAxis->setAutoTickStep(false);
    ui->customPlot->yAxis->setTickStep( qRound( maximo_valor_tocados_y/10.0));


    ui->customPlot->yAxis->setLabel("Cantidad de archivos");
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);

    barra_formatos->setData(ticks, temaTocadoData);

    // setup legend:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->customPlot->replot();
}

void Graficas::on_comboBox_graficas_currentIndexChanged(int index)
{
    //qDebug() << "seleccionado : " << index;

    ui->label_acumulador->hide();

    switch (index)
    {

    case 0:
        limpia_grafica();
        break;

    case 1:
        grafica_temas_tocados_olvidados();
        break;

    case 2:
        grafica_generos_menos_tocados();
        break;

    case 3:
        grafica_primeros_temas_menos_tocados();
        break;

    case 4:
        grafica_formatos();
        break;


    default:
        break;
    }

    //
}

void Graficas::grafica_temas_tocados_olvidados(void)
{

    ui->customPlot->clearGraphs();

    //borre esto
    QList<DatoConsultaBasica>  lista_tocados = mi_proceso_datos->consulta_generos_canciones_alfa(TOCADAS_);

    /*
    for(int i=0; i< lista_tocados.size(); i++)
    {
        qDebug() << lista_tocados.at(i).item << " " << lista_tocados.at(i).tema_cantidad;
    }*/

    QList<DatoConsultaBasica>  lista_todos = mi_proceso_datos->consulta_generos_canciones_alfa(TODAS_);

    ui->comboBox_generos->clear();

    for(int i=0; i< lista_todos.size(); i++)
    {
        //   qDebug() << lista_olvidados.at(i).item << " " << lista_olvidados.at(i).tema_cantidad;
        ui->comboBox_generos->addItem(lista_todos.at(i).item);
    }

    configura_barras_tocadas_todas(lista_tocados, lista_todos);



    ui->comboBox_generos->show();
    ui->pushButton_genero_sel->show();

}

void Graficas::grafica_primeros_temas_menos_tocados(void)
{

    ui->customPlot->clearGraphs();

    //borre esto
    QList<DatoConsultaBasica>  lista_tocados = mi_proceso_datos->consulta_generos_canciones_alfa(TOCADAS_,true);

    /*
    for(int i=0; i< lista_tocados.size(); i++)
    {
        qDebug() << lista_tocados.at(i).item << " " << lista_tocados.at(i).tema_cantidad;
    }*/

    ui->comboBox_generos->clear();

    for(int i=0; i< lista_tocados.size(); i++)
    {
        ui->comboBox_generos->addItem(lista_tocados.at(i).item);
    }

    long acumulado;

    acumulado = mi_proceso_datos->acumula_numero_archivos(lista_tocados);


    //qDebug() << "acum" << acumulado;

    ui->label_acumulador->setText(QString("Se liberarian %1 archivos si se borran todos los archivos").arg(acumulado));

    ui->label_acumulador->show();



    configura_barras_primeras_menos_tocadas(lista_tocados);

    ui->comboBox_generos->show();
    ui->pushButton_genero_sel->show();

}

void Graficas::grafica_formatos(void)
{

    ui->customPlot->clearGraphs();

    //borre esto
    QList<DatoConsultaBasica>  lista_formatos = mi_proceso_datos->consulta_formatos();

    ui->comboBox_generos->clear();

    //configura_barras_tocadas_olvidadas(lista_tocados, lista_olvidados);
    configura_barras_formatos(lista_formatos);

    ui->comboBox_generos->hide();
    ui->pushButton_genero_sel->hide();

}

void Graficas::grafica_generos_menos_tocados(void)
{

    ui->customPlot->clearGraphs();

    QList<DatoConsultaBasica>  cociente;

    //borre esto
    QList<DatoConsultaBasica>  lista_todos = mi_proceso_datos->consulta_generos_canciones_alfa(TODAS_);

    //qDebug() << " todos" << lista_todos.size();


    ui->comboBox_generos->clear();

    for(int i=0; i< lista_todos.size(); i++)
    {
        //qDebug() << lista_tocados.at(i).item << " " << lista_tocados.at(i).tema_cantidad;
        ui->comboBox_generos->addItem(lista_todos.at(i).item);
    }

    QList<DatoConsultaBasica>  lista_tocados = mi_proceso_datos->consulta_generos_canciones_alfa(TOCADAS_);

    //qDebug() << " olvidados" << lista_olvidados.size();

    /*
    for(int i=0; i< lista_olvidados.size(); i++)
    {
        qDebug() << lista_olvidados.at(i).item << " " << lista_olvidados.at(i).tema_cantidad;
    }*/

    qreal coc;

    for(int i=0; i< lista_todos.size(); i++)
    {
        DatoConsultaBasica dato;

        //dato.tema_cantidad = (int) (lista_tocados.at(i).tema_cantidad / lista_olvidados.at(i).tema_cantidad ) ;

        coc = ((double) lista_tocados.at(i).cantidad_int / (double) lista_todos.at(i).cantidad_int ) ;

        //coc = coc * 100.0;

        dato.cantidad_double = coc*100.0;

        //dato.cantidad = qRound((double) dato.cantidad/100.0);

        dato.item = lista_todos.at(i).item;

        cociente.append(dato);

        //qDebug() << " " << lista_todos.at(i).item << " " << lista_tocados.at(i).cantidad_int << lista_todos.at(i).cantidad_int << coc << dato.cantidad_double;

    }

    //qSort(cociente.begin(), cociente.end());
    //cociente = BubbleSort(cociente, 0);

    configura_barras_generos_menos_tocados(cociente);

    //configura_barras_tocadas_olvidadas(lista_tocados, lista_olvidados);
    ui->comboBox_generos->show();
    ui->pushButton_genero_sel->show();

}

/*
void Graficas::set_proceso_datos(ProcesoDatos *proceso_datos)
{
    mi_proceso_datos = proceso_datos;
}
*/

void Graficas::on_pushButton_genero_sel_clicked()
{
    genero_seleccionado.clear();
    //qDebug() << "genero seleccionado: " << ui->comboBox_generos->currentText();
    genero_seleccionado = ui->comboBox_generos->currentText();

    if(genero_seleccionado.size()==0)
    {
        return;
    }

    QStringList lista_artistas = mi_proceso_datos->consulta_artistas_nunca_tocados(genero_seleccionado);

    listaartista->set_contenido(lista_artistas, genero_seleccionado);
}

void Graficas::on_pushButton_topx_clicked()
{
    mi_proceso_datos->exporta_top_x();

    QDesktopServices::openUrl(mi_proceso_datos->directorio_para_top);
}

void Graficas::set_musica_dir(QString ruta)
{
    ruta_musica = ruta;
}

void Graficas::on_pushButton_copia_top_x_clicked()
{
    this->copia_top_x();
}

void Graficas::on_pushButton_baobab_clicked()
{
    QProcess::startDetached(QString("/usr/bin/baobab %1").arg(ruta_musica));
}

void Graficas::on_pushButton_borra_todo_clicked()
{

    if(mi_proceso_datos != NULL)
    {
        listageneros->set_contenido(mi_proceso_datos);

        listageneros->show();
    }
}
