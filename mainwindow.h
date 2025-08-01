#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <vector>

#include <QMainWindow>
#include <QUrl>
#include <QList>
#include <QDragEnterEvent>
#include <QPoint>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QDrag>
#include <QCursor>
#include <QMenu>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QSlider>
#include <QSpinBox>
#include <QStandardPaths>
#include <QProgressDialog>

#include "droparea.h"
#include "airport.h"
#include "editairportdialog.h"
#include "selectairportdialog.h"
#include "validstylesdialog.h"

enum DBInUse {
    ALL = 0,
    INRANGE = 1
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DropArea *dropArea;
    QList<QString> *paths;
    QList<Airport> *airports;
    QList<Airport> *aptInRange;
    Airport *selectedAirport;
    DBInUse dbInUse = DBInUse::ALL;

    QList<int> validStyles;

    QList<Airport>* getDbInUse();

    template<typename SortFunc>
    void sortAirports(QList<Airport> *aptList, SortFunc sortFunc);
    qreal getDistanceToSelectedApt(const Airport *apt);

private slots:
    void getNewFile();
    void addUrls(QList<QUrl> newUrls);
    void showContextMenuFiles(const QPoint pos);
    void deleteFile();

    void importAirports();
    void clearAirports();
    void exportAirports();
    void updateAptTableWidget();

    void changeValidStyles(QList<int> n_validStyles);
    void changeValidStylesDialog();

    void showContextMenuAirport(const QPoint pos);
    void addAirport();
    void editAirport();
    void deleteAirport();

    void addAirportToDB(QString name, QString code, QString country, QGeoCoordinate coordinate, int style, int rwdir, double rwlen, double freq, QString desc);

    void distanceRadioButtonClicked(bool state);
    void amountRadioButtonClicked(bool state);
    void selectAirport();
    void aptSelectionDone(Airport *apt);
    void applyAptSelection();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

};
#endif // MAINWINDOW_H
