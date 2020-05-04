#include "espera.h"
#include "ui_espera.h"

Espera::Espera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Espera)
{
    ui->setupUi(this);


    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Popup  );

    contador = 0;
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);


    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    move(x, y);


}



void Espera::tick(void)
{
        if(contador > 100) contador = 0;
        ui->progressBar->setValue(contador++);
        QCoreApplication::processEvents();
}

Espera::~Espera()
{
    delete ui;
}
