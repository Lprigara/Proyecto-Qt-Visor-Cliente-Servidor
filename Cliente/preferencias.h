#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QCamera>
#include <QSettings>

namespace Ui {
class Preferencias;
}

class Preferencias : public QDialog
{
    Q_OBJECT

public:
    explicit Preferencias(QWidget *parent = 0);
    ~Preferencias();
    QCamera getaux();

private slots:

    void on_buttonBox_accepted();


private:
    Ui::Preferencias *ui_;
    QStringListModel *model_;
    QSettings *setting_;
    QList<QByteArray> devices_;
    QStringList List_;


};

#endif // PREFERENCIAS_H
