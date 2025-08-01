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
#include "validstylesdialog.h"
#include "ui_validstylesdialog.h"

ValidStylesDialog::ValidStylesDialog(QWidget *parent, QList<int> *validStyles) :
    QDialog(parent),
    ui(new Ui::ValidStylesDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Select Valid Styles");

    checkBoxList = {ui->unknown, ui->waypoint, ui->airfieldGrass, ui->outlanding,
                     ui->airfieldGliding, ui->airfieldSolid, ui->mountainPass, ui->mountainTop,
                     ui->transmitterMast, ui->vor, ui->ndb, ui->coolingTower, ui->dam,
                    ui->tunnel, ui->bridge, ui->powerPlant, ui->castle, ui->intersection};

    foreach(int style, *validStyles) {
        checkBoxList.at(style)->setCheckState(Qt::Checked);
    }
}

ValidStylesDialog::~ValidStylesDialog()
{
    delete ui;
}

void ValidStylesDialog::accept()
{
    QList<int> validStyles;
    for(int i = 0; i < checkBoxList.length(); ++i) {
        if(checkBoxList.at(i)->isChecked()) {
            validStyles.append(i);
        }
    }

    emit changed(validStyles);

    QDialog::accept();
}
