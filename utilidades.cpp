#include "utilidades.h"


string encryptDecrypt(string toEncrypt) {
    char key[3] = {'C', 'A', 'D'}; //Any chars will work
    string output = toEncrypt;

    for (unsigned int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return output;
}

/*
string encryptDecrypt(string toEncrypt) {

char key = 'K'; //Any char will work

string output = toEncrypt;

char OP ='0';

for (int i = 0; i < toEncrypt.size(); i++)

output[i] = toEncrypt[i] ^ key + 4567845 + 1 * OP * OP;

return output;

}
*/



QList<DatoConsultaBasica> BubbleSort(QList<DatoConsultaBasica> lista, int size)
{
    int i, j;

    DatoConsultaBasica temp;

     for (i=1; i<size; i++)
          for( j=0 ; j<size - 1; j++)
            {
                 if (lista.at(j).cantidad_int > lista.at(j+1).cantidad_int)
                 {
                      temp = lista.at(j);
                      lista.insert(j, lista.at(j+1));
                      lista.insert(j+1, temp);
                 }
            }

     return lista;
}

//borreme
#include <QDebug>

QString de_windows_a_unix(QString ruta)
{
    int pos_puntos;

        ruta.replace("\\","/");

        pos_puntos = ruta.indexOf(":");

        ruta = ruta.mid(pos_puntos+1);

        //qDebug() << "" << pos_puntos;

        return ruta;
}


void bubble_sort(QList<DatoConsultaBasica> &resultset)
{
    int size = resultset.size();
    DatoConsultaBasica temp;
    for(int i=0; i<size; i++)
    {
        for(int j=size-1; j>i; j--)
        {
            if(resultset.at(j).cantidad_int < resultset.at(j-1).cantidad_int)
            {
                temp=resultset.at(j-1);
                resultset.replace(j-1, resultset.at(j));
                resultset.replace(j , temp);
            }
        }
    }
}

QString dialogo_directorio(QString inicio, QWidget *parent)
{
    QString ruta;

    QFileDialog dialogForDirectory(parent,"seleccione diectorio", inicio, "*" );

    dialogForDirectory.setStyleSheet("QFileDialog { background: yellow; }");

    dialogForDirectory.setWindowFlags(Qt::WindowStaysOnTopHint);
    dialogForDirectory.setFileMode(QFileDialog::Directory);
    dialogForDirectory.setOption(QFileDialog::ShowDirsOnly);

    dialogForDirectory.move(30,10);



    if( dialogForDirectory.exec() == QFileDialog::Accepted)
    {
        //qDebug() << "acepto";
        //qDebug() <<
        ruta = dialogForDirectory.directory().absolutePath();
    }

    /*
     ruta =  dialogForDirectory.getExistingDirectory(
                 parent,
                 QObject::tr("Ubicación del directorio: "),
                 inicio );
*/

     return ruta;
}

QStringList getMacAddress(void)
{
    QString net;

    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
        {
         net = netInterface.hardwareAddress();
            return net.split(":");
        }
    }

    return QStringList();
}
