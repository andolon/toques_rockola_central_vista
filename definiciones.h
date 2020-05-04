#ifndef DEFINICIONES
#define DEFINICIONES

#include <QString>

#define QT_NO_DEBUG_OUTPUT 1

const QString APLICACION_ROCKOLA("Central Vista");

const QString SCRIPT_SQL_CANCIONES("canciones.sql");

const QString BASE_DATOS_ACCESS("datosp1.mdb");

const QString ARCHIVO_CSV("toques.csv");

const QString NOMBRE_APLICACION("rktoques");

const QString MDB_PROGRAMA("/usr/bin/mdb-export");

const QString DATABASEFILE_K("canciones.sqlite");

const QString DIR_BORRABLES("_BORRABLES_MUSICA");

const QString DIR_TOP_XX("top_musica");

const int TOCADAS_   =    10;
const int OLVIDADAS_ =    16;
const int TODAS_     =    14;

//#define  TOP_XX        600

const QString myStilo("QLineEdit { \
                padding: 1px; \
                border-style: solid; \
                border: 2px solid gray; \
                border-radius: 8px; \
                } \
                 \
                QLabel { \
                font-weight: bold; \
                font-size: 12px; \
                } \
                 \
                QPushButton { \
                background-color: rgb(234, 231, 255); \
                border-width: 1px; \
                border-color: #339; \
                border-style: solid; \
                border-radius: 7; \
                padding: 3px; \
                font-size: 10px; \
                font: bold large \"Arial\"; \
                padding-left: 5px; \
                padding-right: 5px; \
                min-width: 50px; \
                min-height: 13px; \
                } \
                \
                QProgressBar { \
                border-color: rgb(85, 170, 255); \
                border-bottom-right-radius: 1px; \
                border-bottom-left-radius: 1px; \
                border: 1px solid black; \
                } \
                 \
                QProgressBar::chunk { \
                background-color: qlineargradient(spread:pad, x1:0.064, y1:0.590909, x2:1, y2:0.592, stop:0 rgba(0, 184, 119, 255), stop:1 rgba(243, 243, 121, 255)); \
                border-bottom-right-radius: 1px; \
                border-bottom-left-radius: 1px; \
                border: 1px solid black; \
                } \
                  \
                ");





#endif // DEFINICIONES

