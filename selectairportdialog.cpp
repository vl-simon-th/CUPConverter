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
