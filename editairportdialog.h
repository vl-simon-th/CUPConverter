#ifndef EDITAIRPORTDIALOG_H
#define EDITAIRPORTDIALOG_H

#include <QDialog>
#include <QString>
#include <QGeoCoordinate>

#include "airport.h"

namespace Ui {
class EditAirportDialog;
}

class EditAirportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAirportDialog(QWidget *parent = nullptr, QString name = "", QString code = "", QString country = "",
                               QGeoCoordinate coordinate = QGeoCoordinate(), int style = 0,
                               int rwdir = 0, double rwlen = 0.0, double freq = 0.0, QString desc = "");
    ~EditAirportDialog();

signals:
    void changesDone(QString name, QString code, QString country,
                     QGeoCoordinate coordinate, int style,
                     int rwdir, double rwlen, double freq, QString desc);

private:
    Ui::EditAirportDialog *ui;

private slots:
    void acceptPressed();
};

#endif // EDITAIRPORTDIALOG_H
