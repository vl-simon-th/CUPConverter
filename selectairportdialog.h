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
