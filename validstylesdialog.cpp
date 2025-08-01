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
