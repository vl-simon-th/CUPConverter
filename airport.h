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
#ifndef AIRPORT_H
#define AIRPORT_H

#include <algorithm>
#include <QString>
#include <QTableWidget>
#include <QList>
#include <QGeoCoordinate>
#include <QMessageBox>

class Airport
{
public:
    Airport(QString &n_name, QString &n_code, QString &n_country, QGeoCoordinate &n_coordinate, int &n_style,
            int &n_rwdir, double &n_rwlen, double &n_freq, QString &n_desc);
    Airport(QString &cupString, bool rwWidthAvailable);
    Airport(Airport *n_apt);

    void fillRow(QTableWidget *tableWidget, int row);
    static QString convertFreq(double freq, bool usePoint);
    static double convertLatitudeFromCup(QString inp);
    static double convertLongitudeFromCup(QString inp);
    static double convertCoordinate(QString inp, QWidget *parent);
    static QString convertCUPToLXeStyle(int style);
    static QList<QString> convertCoordinateToCUP(QGeoCoordinate inpCoordinate);
    static QString convertCoordinateToWZ(QString corString, int degreeLength);
    bool isValid(QList<int> *validStyles);
    bool isAllreadyKnown(QList<Airport> *airports);
    bool equals(Airport airport);

    QString getName() const;
    void setName(const QString &newName);

    QString getCode() const;
    void setCode(const QString &newCode);

    QString getCountry() const;
    void setCountry(const QString &newCountry);

    int getStyle() const;
    void setStyle(int newStyle);

    int getRwdir() const;
    void setRwdir(int newRwdir);

    double getRwlen() const;
    void setRwlen(double newRwlen);

    double getFreq() const;
    void setFreq(double newFreq);

    QString getDesc() const;
    void setDesc(const QString &newDesc);


    QGeoCoordinate getCoordinate() const;
    void setCoordinate(const QGeoCoordinate &newCoordinate);

    QString generateLXeTXTFileLine();
    QString generateCUPFileLine();
    QString generateWZFileLine();

private:
    QString name;
    QString code;
    QString country;
    QGeoCoordinate coordinate;
    int style;
    int rwdir;
    double rwlen;
    double freq;
    QString desc;

    QString removeSpecialSigns(QString* in);
};

#endif // AIRPORT_H
