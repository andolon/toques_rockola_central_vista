#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

//#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

#include "definiciones.h"

//!  Clase Datamanager para manejar base de datos.
/*!
  Clase que intenta encapsular las funciones principales
  para manejar la base de datos.
*/

class Datamanager : public QObject
{
    Q_OBJECT

public:

    //! Constructor por defecto toma como parametro el puntero al objeto QObjetc.
    /*!
      \param *parent
      \sa ~Datamanager
    */
    explicit Datamanager(QObject *parent = 0, QString data_dir = "/tmp");



    //! testDatabase
    /*!
      Detecta si la base de datos pudo ser abierta normalmente, retorna false si no pudo ser abierta.
      \return bool , true si funciona, false sino funciona
    */
    bool testDatabase(void);

    //void fillCreateScrip(void);

    //void createAllTables(void);

    //!
    /*!

    */

    //! close
    /*!
      Cierra la base de datos eliminando las conexiones activas para proteger los datos.
    */
    void close();

    //! Destructor, busca la base de datos y la cierra, elimina punteros.
    /*!

    */
    ~Datamanager(void);

    QSqlDatabase _rk_database; /**< Base de datos, abre, conecta e instancia. */

    bool databaseIsOk;  /**< Booleano, abre, para saber si la base de datos pudo ser abierta exitosamente o ser encontrada en la ruta adecuada, si es negativo hay error en la apertura. */

    QSqlQuery *query;  /**< Para insertar el código sql, ejecutarlo y obtener la respuesta */

private:



//    QStringList *createScript;

signals:
    void has_error(void);


public slots:
};

#endif // DATAMANAGER_H
