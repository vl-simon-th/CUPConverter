/*
Copyright (C) 2025  Simon Thal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
