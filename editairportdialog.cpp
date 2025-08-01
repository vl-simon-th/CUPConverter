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
#include "editairportdialog.h"
#include "ui_editairportdialog.h"

EditAirportDialog::EditAirportDialog(QWidget *parent, QString name, QString code, QString country,
                                     QGeoCoordinate coordinate, int style, int rwdir,
                                     double rwlen, double freq, QString desc) :
    QDialog(parent),
    ui(new Ui::EditAirportDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit Airport");

    ui->nameLineEdit->setText(name);
    ui->codeLineEdit->setText(code);
    ui->countryLineEdit->setText(country);

    qDebug() << coordinate.toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere);
    QString coordinateString = coordinate.toString(QGeoCoordinate::DegreesMinutesWithHemisphere);
    QList<QString> data = coordinateString.split(',');
    if(data.length() == 3) {
        ui->latLineEdit->setText(data[0]);
        ui->lonLineEdit->setText(data[1].removeFirst());
        ui->elevLineEdit->setText(data[2].removeFirst());
    }
    if(ui->elevLineEdit->text().length() == 0) ui->elevLineEdit->setText("0m");
    ui->styleComboBox->setCurrentIndex(style);
    ui->rwdirLineEdit->setText(QString::number(rwdir));
    ui->rwlenLineEdit->setText(QString::number(rwlen) + "m");
    ui->freqLineEdit->setText(Airport::convertFreq(freq, true));
    ui->descLineEdit->setText(desc);

    connect(this, &QDialog::accepted, this, &EditAirportDialog::acceptPressed);


}

EditAirportDialog::~EditAirportDialog()
{
    delete ui;
}

void EditAirportDialog::acceptPressed()
{

    if(ui->elevLineEdit->text().length() == 0) ui->elevLineEdit->setText("0m");
    if(ui->rwlenLineEdit->text().length() == 0) ui->rwdirLineEdit->setText("0m");

    QString name = ui->nameLineEdit->text();
    QString code = ui->codeLineEdit->text();
    QString country = ui->countryLineEdit->text();
    QGeoCoordinate coordinate(Airport::convertCoordinate(ui->latLineEdit->text(), this),
                   Airport::convertCoordinate(ui->lonLineEdit->text(), this),
                   ui->elevLineEdit->text().removeLast().toDouble());
    int style = ui->styleComboBox->currentIndex();
    int rwdir = ui->rwdirLineEdit->text().toInt();
    double rwlen = ui->rwlenLineEdit->text().removeLast().toDouble();
    double freq = ui->freqLineEdit->text().toDouble();
    QString desc = ui->descLineEdit->text();

    emit changesDone(name, code, country, coordinate, style, rwdir, rwlen, freq, desc);
}
