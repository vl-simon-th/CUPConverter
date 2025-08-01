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
#include "selectairportdialog.h"
#include "ui_selectairportdialog.h"

SelectAirportDialog::SelectAirportDialog(QWidget *parent, QList<Airport> *n_aptList) :
    QDialog(parent),
    ui(new Ui::SelectAirportDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Select Airport");

    ui->aptListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->acceptButtonBox, &QDialogButtonBox::accepted, this, &SelectAirportDialog::selectionDone);
    connect(ui->aptListWidget, &QListWidget::doubleClicked, this, &SelectAirportDialog::selectionDone);
    aptList = n_aptList;

    updateAptListView("");
    connect(ui->searchLineEdit, &QLineEdit::textEdited, this, &SelectAirportDialog::updateAptListView);
}

SelectAirportDialog::~SelectAirportDialog()
{
    delete ui;
}

void SelectAirportDialog::updateAptListView(QString search)
{
    ui->aptListWidget->clear();
    foreach (Airport apt, *aptList) {
        if(apt.getName().startsWith(search, Qt::CaseInsensitive)) {
            ui->aptListWidget->addItem(apt.getName());
        }
    }
}

void SelectAirportDialog::selectionDone()
{
    if(ui->aptListWidget->selectedItems().length() == 0) {
        emit selected(nullptr);
    } else {
        foreach(Airport apt, *aptList) {
            if(apt.getName() == ui->aptListWidget->selectedItems().first()->text()) {
                emit selected(&apt);
            }
        }
    }
    accept();
}
