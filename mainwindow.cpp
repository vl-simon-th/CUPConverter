#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    paths = new QList<QString>();
    airports = new QList<Airport>();

    ui->setupUi(this);
    this->setWindowTitle("CUP Converter by Simon Thal");

    connect(ui->actionAddInputFile, &QAction::triggered, this, &MainWindow::getNewFile);

    dropArea = new DropArea(this);
    connect(dropArea, &DropArea::urlDrop, this, &MainWindow::addUrls);

    ui->inputFilesListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->inputFilesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->inputFilesListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenuFiles);


    ui->aptTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->aptTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->aptTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->aptTableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenuAirport);

    connect(ui->aptImportButton, &QPushButton::released, this, &MainWindow::importAirports);
    connect(ui->aptClearButton, &QPushButton::released, this, &MainWindow::clearAirports);
    connect(ui->aptExportButton, &QPushButton::released, this, &MainWindow::exportAirports);

    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::importAirports);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::clearAirports);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::exportAirports);

    connect(ui->distanceRadioButton, &QRadioButton::clicked, this, &MainWindow::distanceRadioButtonClicked);
    connect(ui->amountRadioButton, &QRadioButton::clicked, this, &MainWindow::amountRadioButtonClicked);

    selectedAirport = nullptr;
    connect(ui->selectAptPushButton, &QPushButton::released, this, &MainWindow::selectAirport);

    aptInRange = new QList<Airport>();
    connect(ui->applyAptPushButton, &QPushButton::released, this, &MainWindow::applyAptSelection);

    validStyles = {2, 3, 4, 5};
    connect(ui->changeValidStylesPushButton, &QPushButton::released, this, &MainWindow::changeValidStylesDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addUrls(QList<QUrl> newUrls)
{
    QUrl currentUrl;
    foreach (currentUrl, newUrls) {
        QString currentPath = currentUrl.toLocalFile();
        if(!paths->contains(currentPath)) {
            if(currentPath.endsWith(".cup")) {
                paths->append(currentPath);
                QFileIconProvider iconProvider;
                QIcon fileIcon = iconProvider.icon(QFileInfo(currentPath));
                ui->inputFilesListWidget->addItem(new QListWidgetItem(fileIcon, currentPath));
            } else {
                QMessageBox::warning(this, "Error", "You did not select a '.cup' file");
            }
        } else {
            QMessageBox::warning(this, "Error", "You allready added '" + currentPath + "'");
        }
    }
}

void MainWindow::showContextMenuFiles(const QPoint pos)
{
    QMenu myMenu(this);
    myMenu.addAction("Insert", this, &MainWindow::getNewFile);
    myMenu.addAction("Delete",  this, &MainWindow::deleteFile);
    myMenu.exec(QCursor::pos());
}

void MainWindow::deleteFile()
{
    QListWidgetItem *currentWidgetItem;
    foreach (currentWidgetItem, ui->inputFilesListWidget->selectedItems()) {
        ui->inputFilesListWidget->removeItemWidget(currentWidgetItem);
        paths->remove(paths->indexOf(currentWidgetItem->text()));

        delete currentWidgetItem;
    }
}

void MainWindow::getNewFile()
{
    QList<QUrl> fileUrls = QFileDialog::getOpenFileUrls(this, "Select *.cup", QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)), "cup Files (*.cup)");
    addUrls(fileUrls);
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QFrame *inputFrame = ui->inputFrame;
    dropArea->setMinimumSize(inputFrame->size());
    dropArea->setMaximumSize(inputFrame->size());
    QPoint globalPos = inputFrame->mapToParent(QPoint(0,0));
    dropArea->move(globalPos.x(), globalPos.y() + ui->menubar->height());

    dropArea->setVisible(true);
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    dropArea->setVisible(false);
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    Q_UNUSED(event);

    dropArea->setVisible(false);
}

//technical

void MainWindow::exportAirports()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Export Database", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    "LXe File (*.txt);;cup File (*.cup);;WZ File (*.WZ)");
    QFile exportFile(filePath);
    exportFile.open(QIODevice::WriteOnly|QFile::Truncate);
    QTextStream stream(&exportFile);

    QList<Airport> *aptList;
    if(dbInUse == DBInUse::ALL) {
        aptList = airports;
    } else {
        aptList = aptInRange;
    }

    if(filePath.endsWith(".txt")) {
        stream << "*,UserAptAPT,UserAptAPTTYPE,UserAptLAT,UserAptLON,UserAptALT,UserAptF,UserAptRW(3digit),UserAptRWDIR(2digit),"
                  "UserAptRWTYPE,UserAptCALT,UserAptCDIR,UserAptCODE,UserAptRWN,UserAptAPT_SHORT,UserAptLABEL\n";

        foreach(Airport apt, *aptList) {
            stream << apt.generateLXeTXTFileLine() + "\n";
        }
    } else if(filePath.endsWith(".cup")) {
        stream << "name,code,country,lat,lon,elev,style,rwdir,rwlen,freq,desc\n";

        foreach(Airport apt, *aptList) {
            stream << apt.generateCUPFileLine() + "\n";
        }
    } else if(filePath.endsWith(".WZ")) {
        foreach(Airport apt, *aptList) {
            stream << apt.generateWZFileLine() + "\r\n";
        }
    }
    exportFile.close();
}

void MainWindow::changeValidStylesDialog()
{
    ValidStylesDialog changeDialog(this, &validStyles);
    connect(&changeDialog, &ValidStylesDialog::changed, this, &MainWindow::changeValidStyles);
    changeDialog.exec();
}
void MainWindow::changeValidStyles(QList<int> n_validStyles)
{
    validStyles.clear();
    validStyles.append(n_validStyles);
}

void MainWindow::importAirports()
{
    if(paths->length() == 0) {
        QMessageBox::warning(this, "Error", "You first have to select at least one '.cup' file");
        return;
    }

    foreach (QString currentPath, *paths) {
        QFile currentFile(currentPath);
        currentFile.open(QIODeviceBase::ReadOnly);
        QTextStream streamFile(&currentFile);

        bool rwWidthAvialable = streamFile.readLine().contains("rwwidth"); //skip the description and check for rwwidth

        QString currentLine;
        while(streamFile.readLineInto(&currentLine)) {
            if(!currentLine.startsWith("*")) {
                Airport newAirport = Airport(currentLine, rwWidthAvialable);
                if(newAirport.isValid(&validStyles) && !(newAirport.isAllreadyKnown(airports))){
                    airports->append(newAirport);
                }
            }
        }
        currentFile.close();
    }

    if(airports->length() == 0) {
        QMessageBox::warning(this, "Error", "There are not any usable entries in your '.cup' file");
        return;
    }

    sortAirports(airports, [](const Airport &a1, const Airport &a2)->bool{
        return (a1.getName().compare(a2.getName(), Qt::CaseInsensitive) < 0);
    });
}

template<typename SortFunc>
void MainWindow::sortAirports(QList<Airport> *aptList, SortFunc sortFunc)
{
    std::sort(aptList->begin(), aptList->end(), sortFunc);
    updateAptTableWidget();
}

void MainWindow::updateAptTableWidget()
{
    QList<Airport> *aptList = getDbInUse();

    QTableWidget *aptTableWidget = ui->aptTableWidget;
    aptTableWidget->clearContents();
    aptTableWidget->setRowCount(0);

    foreach(Airport apt, *aptList) {
        int rowCount = aptTableWidget->rowCount();
        aptTableWidget->insertRow(rowCount);
        apt.fillRow(aptTableWidget, rowCount);
    }

    ui->aptTableWidget->resizeColumnsToContents();
}

void MainWindow::clearAirports()
{
    QMessageBox confirmation(this);
    confirmation.setText("Are you sure you want to clear all airports");
    confirmation.setInformativeText("This will delete every airport");
    confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirmation.exec();
    if(ret == QMessageBox::Yes) {
        ui->aptTableWidget->clearContents();
        ui->aptTableWidget->setRowCount(0);
        aptInRange->clear();
        airports->clear();
        dbInUse = DBInUse::ALL;
    }
}

void MainWindow::addAirportToDB(QString name, QString code, QString country, QGeoCoordinate coordinate,
                                int style, int rwdir, double rwlen, double freq, QString desc)
{
    Airport apt(name, code, country, coordinate, style, rwdir, rwlen, freq, desc);

    int pos = 0;

    QList<Airport> *aptList = getDbInUse();

    if(aptList->length() > 0) {
        while(apt.getName().compare(aptList->at(pos).getName(), Qt::CaseInsensitive) > 0 && pos < (aptList->length() - 1)) {
            ++pos;
        }

        if(pos == aptList->length() - 1 && apt.getName().compare(aptList->at(pos).getName(), Qt::CaseInsensitive) > 0) {
            aptList->append(apt);
        } else {
            aptList->insert(pos, apt);
        }
    } else {
        aptList->append(apt);
    }
    updateAptTableWidget();
}

void MainWindow::distanceRadioButtonClicked(bool state)
{
    if(state) {
        ui->amountRadioButton->setChecked(false);
        ui->infoAptHorizontalSlider->setRange(0, 1500);
        ui->InfoAptSpinBox->setRange(0, 1500);
        ui->InfoAptSpinBox->setValue(450);
        ui->infoAptLabel->setText(ui->distanceRadioButton->text());
    } else {
        ui->amountRadioButton->click();
    }
}

void MainWindow::amountRadioButtonClicked(bool state)
{
    if(state) {
        ui->distanceRadioButton->setChecked(false);
        ui->infoAptHorizontalSlider->setRange(0, 15000);
        ui->InfoAptSpinBox->setRange(0, 15000);
        ui->InfoAptSpinBox->setValue(3500);
        ui->infoAptLabel->setText(ui->amountRadioButton->text());
    } else {
        ui->distanceRadioButton->click();
    }

}

void MainWindow::selectAirport()
{
    SelectAirportDialog selAptDialog(this, airports);
    connect(&selAptDialog, &SelectAirportDialog::selected, this, &MainWindow::aptSelectionDone);
    selAptDialog.exec();
}

void MainWindow::aptSelectionDone(Airport *apt)
{
    if(apt != nullptr) {
        delete selectedAirport;
        selectedAirport = new Airport(apt);
        ui->aptLabel->setText(apt->getName());
    }
}

void MainWindow::applyAptSelection()
{
    if(selectedAirport == nullptr) {
        QMessageBox::warning(this, "Error", "You did not select an airport");
    } else {
        QMessageBox confirmation;
        confirmation.setText("Are you sure you want to apply the selection");
        confirmation.setInformativeText("This will delete every airport out of the selected area");
        confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = confirmation.exec();
        if(ret == QMessageBox::Yes) {
            aptInRange->clear();
            if(ui->distanceRadioButton->isChecked()) {
                foreach(Airport apt, *airports) {
                    if(getDistanceToSelectedApt(&apt) < ui->infoAptHorizontalSlider->value() * 1000.0) {
                        aptInRange->append(apt);
                    }
                }
            } else {
                if(ui->infoAptHorizontalSlider->value() > airports->length()) {
                    QMessageBox confirmation;
                    confirmation.setText("You selected more Airports than there are in the Data - Do you want to reduce the amount?");
                    confirmation.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    int ret = confirmation.exec();
                    if(ret == QMessageBox::Ok) {
                        ui->InfoAptSpinBox->setValue(airports->length());
                        aptInRange->append(*airports);
                    }
                } else {
                    sortAirports(airports, [this](const Airport &a1, const Airport &a2){
                        return (this->getDistanceToSelectedApt(&a1) < this->getDistanceToSelectedApt(&a2));
                    });

                    aptInRange->append(airports->sliced(0, ui->infoAptHorizontalSlider->value()));

                    sortAirports(aptInRange, [](const Airport &a1, const Airport &a2)->bool{
                        return (a1.getName().compare(a2.getName(), Qt::CaseInsensitive) < 0);
                    });
                }
            }

            dbInUse = DBInUse::INRANGE;
            updateAptTableWidget();
        }
    }
}

qreal MainWindow::getDistanceToSelectedApt(const Airport *apt)
{
    return selectedAirport->getCoordinate().distanceTo(apt->getCoordinate());
}

//Dialog with user

void MainWindow::showContextMenuAirport(const QPoint pos)
{
    Q_UNUSED(pos);

    QMenu myMenu(this);
    myMenu.addAction("Add", this, &MainWindow::addAirport);
    myMenu.addAction("Edit", this, &MainWindow::editAirport);
    myMenu.addAction("Delete",  this, &MainWindow::deleteAirport);
    myMenu.exec(QCursor::pos());
}

QList<Airport>* MainWindow::getDbInUse() {
    if(dbInUse == DBInUse::ALL) {
        return airports;
    } else {
        return aptInRange;
    }
}

void MainWindow::addAirport()
{
    EditAirportDialog addAirportDialog(this);
    connect(&addAirportDialog, &EditAirportDialog::changesDone, this, &MainWindow::addAirportToDB);
    addAirportDialog.exec();
}

void MainWindow::editAirport()
{
    QList<Airport> *aptList = getDbInUse();

    QList<QTableWidgetSelectionRange> ranges = ui->aptTableWidget->selectedRanges();
    int row = ranges.first().topRow();
    Airport a = aptList->at(row);
    aptList->remove(row);

    EditAirportDialog addAirportDialog(this, a.getName(), a.getCode(), a.getCountry(), a.getCoordinate(), a.getStyle(), a.getRwdir(), a.getRwlen(), a.getFreq(), a.getDesc());
    connect(&addAirportDialog, &EditAirportDialog::changesDone, this, &MainWindow::addAirportToDB);
    if(addAirportDialog.exec() == QDialog::Rejected) {
        aptList->insert(row, a);
    }
}

void MainWindow::deleteAirport()
{
    QList<Airport> *aptList = getDbInUse();

    QMessageBox confirmation(this);
    confirmation.setText("Are you sure you want to delete this/these Airport/s");
    confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirmation.exec();
    if(ret == QMessageBox::Yes) {
        QList<QTableWidgetSelectionRange> ranges = ui->aptTableWidget->selectedRanges();
        foreach(QTableWidgetSelectionRange range, ranges) {
            aptList->remove(range.topRow(), range.rowCount());
        }
        updateAptTableWidget();
    }
}

