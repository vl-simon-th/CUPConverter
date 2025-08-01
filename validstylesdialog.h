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
#ifndef VALIDSTYLESDIALOG_H
#define VALIDSTYLESDIALOG_H

#include <QDialog>
#include <QList>
#include <QCheckBox>

namespace Ui {
class ValidStylesDialog;
}

class ValidStylesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ValidStylesDialog(QWidget *parent = nullptr, QList<int> *validStyles = nullptr);
    ~ValidStylesDialog();

signals:
    void changed(QList<int> validStyles);

private:
    QList<QCheckBox *> checkBoxList;

    Ui::ValidStylesDialog *ui;

protected:
    void accept() override;
};

#endif // VALIDSTYLESDIALOG_H
