#ifndef DATOBASICO_H
#define DATOBASICO_H

#include <QString>
#include <qmath.h>

class DatoConsultaBasica
{
public:
    DatoConsultaBasica();

    QString item;
    int      cantidad_int;
    double   cantidad_double;
};


class DatoTopX
{
public:
    DatoTopX();

    QString genero;
    QString artista;
    QString nombre;
    long  cantidad;
};


class Ruta_Antes_Despues
{
public:
    Ruta_Antes_Despues();

    QString origen;
    QString destino;
};

/*
class DatoGeneroArtista
{
public:
    DatoGeneroArtista();

    QString genero;
    QString artista;

};*/


/*
class DatoArtistaSeleccionado
{
public:
    DatoArtistaSeleccionado();

    QString artista;
    int     codigo;
}; */

#endif // DATOBASICO_H
