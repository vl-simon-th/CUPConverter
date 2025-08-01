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
#ifndef SELECTAIRPORTDIALOG_H
#define SELECTAIRPORTDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidget>

#include "airport.h"

namespace Ui {
class SelectAirportDialog;
}

class SelectAirportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAirportDialog(QWidget *parent = nullptr, QList<Airport> *n_aptList = nullptr);
    ~SelectAirportDialog();

signals:
    void selected(Airport *apt);

private slots:
    void updateAptListView(QString search);
    void selectionDone();

private:
    Ui::SelectAirportDialog *ui;
    QList<Airport> *aptList;
};

#endif // SELECTAIRPORTDIALOG_H
