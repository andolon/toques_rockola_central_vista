#ifndef UTILIDADES
#define UTILIDADES

#include <QList>
#include <QString>
#include <QFileDialog>
#include <QObject>
#include <QtNetwork/QNetworkInterface>
#include <iostream>

#include "datos_tipos.h"



using namespace std;

string encryptDecrypt(string toEncrypt);

QString de_windows_a_unix(QString ruta);

void bubble_sort(QList<DatoConsultaBasica> &resultset);

QString dialogo_directorio(QString inicio, QWidget *parent);

QStringList getMacAddress(void);

#endif // UTILIDADES

