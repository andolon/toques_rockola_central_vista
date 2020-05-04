#ifndef ESPERA_H
#define ESPERA_H

#include <QWidget>
#include <QThread>
#include <QDesktopWidget>

namespace Ui {
class Espera;
}

class Espera : public QWidget
{
    Q_OBJECT

public:
    explicit Espera(QWidget *parent = 0);
    ~Espera();

    void tick(void);

private:
    Ui::Espera *ui;
    //volatile bool fin;
    int contador;

};

#endif // ESPERA_H
