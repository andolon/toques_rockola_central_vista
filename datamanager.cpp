#include "datamanager.h"

Datamanager::Datamanager(QObject *parent, QString data_dir) : QObject(parent)
{

    _rk_database = QSqlDatabase::addDatabase("QSQLITE","datamanager");

    _rk_database = QSqlDatabase::database("datamanager");

#ifdef EN_DEBUG
    cout << endl << "nombre base datos: " << _rk_database.connectionName().toLatin1().constData();
#endif

    QString dbpath;

          //QDir::separator

    dbpath = QString("%1/%2").arg( data_dir , DATABASEFILE_K);

    //dbpath = dbpath.replace("/","\\");

    qDebug()  << "archivo de base de datos " << dbpath;


    _rk_database.setDatabaseName(dbpath);

    _rk_database.open();

    if(_rk_database.isOpen())
    {
        databaseIsOk = true;
#ifdef EN_DEBUG
            cout << endl << "conexion ok";
#endif
    }
    else
    {
#ifdef EN_DEBUG
        cout << endl <<"error abriendo " << _rk_database.databaseName().toLatin1().constData();
#endif

        databaseIsOk = false;

        QMessageBox::critical(
          NULL,
          tr("Error en base de datos "),
          tr("No se pudo abrir conexión: %1").arg(_rk_database.databaseName()),
          QMessageBox::Close
                    );


        exit(EXIT_FAILURE);

    }

    query = new QSqlQuery(_rk_database);

    /*
     *  http://www.java2s.com/Code/Cpp/Qt/OperateSqlitedatabasewithQSqlDatabase.htm
     *
     * qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (1, 'John', 'Doe')" );
  qry.exec();
     */

    query->prepare("PRAGMA foreign_keys = ON;");
    query->exec();

    if(query->lastError().isValid())
    {
#ifdef EN_DEBUG
        cout << endl;
        cout << endl << "fallo pragma foreign keys" ;
        cout << endl << query->lastError().text().toLatin1().constData();
#endif

        QMessageBox::critical(
          NULL,
          tr("Error en base de datos "),
          tr("fallo pragma"),
          QMessageBox::Close
                    );

        exit(EXIT_FAILURE);

    }

}

bool Datamanager::testDatabase(void)
{
return databaseIsOk;
}

/*
void Datamanager::fillCreateScrip(void)
{
createScript = new QStringList();

createScript->append(
QString("CREATE TABLE \"genre\" (\
        \"genre_index\" INTEGER NOT NULL,\
        \"name\" TEXT NOT NULL,\
        PRIMARY KEY (\"genre_index\") \
        );"));

createScript->append(
QString("CREATE TABLE \"artist\" (\
                      \"artist_index\" INTEGER NOT NULL,\
                      \"name\" TEXT NOT NULL,\
                      PRIMARY KEY (\"artist_index\") ,\
                      CONSTRAINT \"fk_artist_genre_1\" FOREIGN KEY (\"artist_index\") REFERENCES \"genre\" (\"genre_index\")\
                      );" ));

createScript->append(
QString("CREATE TABLE \"theme\" (\
        \"theme_index\" INTEGER NOT NULL,\
        \"name\" TEXT NOT NULL,\
        \"code\" INTEGER NOT NULL,\
        \"path\" TEXT NOT NULL,\
        \"played_times\" INTEGER DEFAULT 0,\
        PRIMARY KEY (\"theme_index\") ,\
        CONSTRAINT \"fk_theme_artist_1\" FOREIGN KEY (\"theme_index\") REFERENCES \"artist\" (\"artist_index\")\
        );"));

createScript->append(
QString("CREATE TABLE \"defect\" (\
        \"defect_index\" INTEGER NOT NULL,\
        \"date\" TEXT NOT NULL,\
        PRIMARY KEY (\"defect_index\") ,\
        CONSTRAINT \"fk_defect_theme_1\" FOREIGN KEY (\"defect_index\") REFERENCES \"theme\" (\"theme_index\")\
        );"));

createScript->append(
QString("CREATE TABLE \"reject\" (\
        \"reject_index\" INTEGER NOT NULL,\
        \"date\" TEXT NOT NULL,\
        PRIMARY KEY (\"reject_index\") ,\
        CONSTRAINT \"fk_reject_theme_1\" FOREIGN KEY (\"reject_index\") REFERENCES \"theme\" (\"theme_index\")\
        );"));

createScript->append(
QString("CREATE TABLE \"event\" (\
        \"event_index\" INTEGER NOT NULL,\
        \"description\" TEXT NOT NULL,\
        \"date\" TEXT NOT NULL,\
        \"table_name\" TEXT NOT NULL,\
        \"table_index\" INTEGER NOT NULL,\
        PRIMARY KEY (\"event_index\") \
        );"));

createScript->append(
QString("CREATE TABLE \"running\" (\
        \"credits\" INTEGER DEFAULT NULL,\
        \"gps_latitude\" REAL,\
        \"gps_longitude\" REAL\
        );"));

createScript->append(
QString("CREATE TABLE \"play_list\" (\
        \"playlist_index\" INTEGER NOT NULL,\
        \"priority\" INTEGER DEFAULT NULL,\
        PRIMARY KEY (\"playlist_index\") ,\
        CONSTRAINT \"fk_playlist_theme_1\" FOREIGN KEY (\"playlist_index\") REFERENCES \"theme\" (\"theme_index\")\
        );"));

}
*/

/*
void Datamanager::createAllTables(void)
{

    for(int i=0; i< createScript->size();i++)
    {
        query->prepare(createScript->at(i));
        query->exec();

        if(query->lastError().isValid())
        {
#ifdef EN_DEBUG
            cout << endl;
            cout << endl << "fallo la creacion" ;
            cout << endl << query->lastError().text().toLatin1().constData();
            cout << endl << createScript->at(i).toLatin1().constData();
#endif
        }
        else
        {
#ifdef EN_DEBUG
            cout << endl << "sin error" ;
#endif
        }

    }

}
*/
void Datamanager::close()
{
    //database.removeDatabase("datamanager");
    //database.close();

    //delete query;

/*
    if(database.isOpen())
    {
    database.close();
    }
    //delete dbms;


    QStringList list = QSqlDatabase::connectionNames();
    for(int i = 0; i < list.count(); ++i) {
    QSqlDatabase::removeDatabase(list[i]);
    }
*/


    QString connection;
    connection = _rk_database.connectionName();
    _rk_database = QSqlDatabase();
    //m_db.close();
    _rk_database.removeDatabase(connection);

}

Datamanager::~Datamanager()
{
     delete query;


    if(_rk_database.isOpen())
    {
    _rk_database.close();
    }



    QStringList list = QSqlDatabase::connectionNames();
    for(int i = 0; i < list.count(); ++i) {
    QSqlDatabase::removeDatabase(list[i]);
    }

    //this->close();

}

