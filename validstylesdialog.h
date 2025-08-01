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
