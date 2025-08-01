#include "airport.h"

Airport::Airport(QString &n_name, QString &n_code, QString &n_country, QGeoCoordinate &n_coordinate, int &n_style,
                int &n_rwdir, double &n_rwlen, double &n_freq, QString &n_desc)
{
    name = n_name;
    code = n_code;
    country = n_country;
    coordinate = n_coordinate;
    style = n_style;
    rwdir = n_rwdir;
    rwlen = n_rwlen;
    freq = n_freq;
    desc = n_desc;
}

Airport::Airport(QString &cupString, bool rwWidthAvailable)
{
    // Regular expression to split by comma but ignore commas inside quotes
    static QRegularExpression re(R"((\"[^\"]*\")|[^,]+|(?<=,)(?=,))");
    QRegularExpressionMatchIterator i = re.globalMatch(cupString);
    QList<QString> data;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString matchedStr = match.captured(0);
        matchedStr.remove('"'); // Remove double quotes if any
        data.append(matchedStr.trimmed());
    }

    if(rwWidthAvailable) {
        data.removeAt(9);
    }

    if(data.length() < 11) {
        data.append("");
    }

    name = data[0];
    code = data[1];
    country = data[2];
    coordinate = QGeoCoordinate(convertLatitudeFromCup(data[3]), convertLongitudeFromCup(data[4]), data[5].removeLast().toDouble());
    style = data[6].toInt();
    rwdir = data[7].toInt();
    rwlen = data[8].removeLast().toDouble();
    freq = data[9].toDouble();
    desc = data[10];
}

Airport::Airport(Airport *n_apt)
{
    name = n_apt->getName();
    code = n_apt->getCode();
    country = n_apt->getCountry();
    coordinate = n_apt->getCoordinate();
    style = n_apt->getStyle();
    rwdir = n_apt->getRwdir();
    rwlen = n_apt->getRwlen();
    freq = n_apt->getFreq();
    desc = n_apt->getDesc();
}

void Airport::fillRow(QTableWidget *tableWidget, int row)
{
    tableWidget->setItem(row, 0, new QTableWidgetItem(name));
    tableWidget->setItem(row, 1, new QTableWidgetItem(code));
    tableWidget->setItem(row, 2, new QTableWidgetItem(country));

    QString coordinateString = coordinate.toString(QGeoCoordinate::DegreesMinutesWithHemisphere);
    QList<QString> data = coordinateString.split(',');
    tableWidget->setItem(row, 3, new QTableWidgetItem(data[0]));
    tableWidget->setItem(row, 4, new QTableWidgetItem(data[1].removeFirst()));
    tableWidget->setItem(row, 5, new QTableWidgetItem(data[2].removeFirst()));

    tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(style)));
    tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(rwdir)));
    tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(rwlen) + "m"));
    tableWidget->setItem(row, 9, new QTableWidgetItem(convertFreq(freq, true)));
    tableWidget->setItem(row, 10, new QTableWidgetItem(desc));
}

QString Airport::convertFreq(double freq, bool usePoint)
{
    if(freq == 0.0) return "";

    QString freqString = QString::number(freq * 1000);
    if(usePoint) freqString.insert(3, ".");

    return freqString;
}

double Airport::convertLatitudeFromCup(QString inp)
{
    double degree = inp.sliced(0,2).toDouble();
    degree += inp.sliced(2, 6).toDouble() / 60;

    if (inp.sliced(8, 1) == "S") {
        degree *= -1;
    }

    return degree;
}

double Airport::convertLongitudeFromCup(QString inp)
{
    double degree = inp.sliced(0,3).toDouble();
    degree += inp.sliced(3, 6).toDouble() / 60;

    if (inp.sliced(9, 1) == "W") {
        degree *= -1;
    }

    return degree;
}

double Airport::convertCoordinate(QString inp, QWidget *parent)
{
    QList<QString> cSplit = inp.split(QChar(0x00B0));
    double degrees = cSplit.first().toDouble();
    QList<QString> aSplit = cSplit.last().split("'");
    degrees += aSplit.first().toDouble() / 60;

    if(aSplit.last().length() > 0) {
        if(aSplit.last().last(1) == "S" || aSplit.last().last(1) == "W") {
            degrees = -degrees;
        }
    }

    if(cSplit.first() == cSplit.last() || aSplit.first() == aSplit.last()) {
        QMessageBox::warning(parent, "Error", "One of Coordinates was not entered correctly. Pleas check the Airport.");
    }

    return degrees;
}

QString Airport::convertCUPToLXeStyle(int style)
{
    switch(style) {
    case 2:
        return "G";
    case 3:
        return "G";
    case 4:
        return "G";
    case 5:
        return "C";
    default:
        return "I";
    }
}

QList<QString> Airport::convertCoordinateToCUP(QGeoCoordinate inpCoordinate)
{
    QList<QString> data = inpCoordinate.toString(QGeoCoordinate::DegreesMinutesWithHemisphere).split(",");
    data[1].removeFirst(); // space after comma
    data[2].removeFirst(); // space after comma

    data[0].remove(data[0].length() - 3, 2); // space & ' between minutes and S/N
    data[1].remove(data[1].length() - 3, 2); // space & ' between minutes and W/E

    QList<QString> data0List = data[0].split(QChar(0x00B0));
    while(data0List.first().length() < 2) { data0List.first().prepend("0"); }
    data0List.last().removeFirst();
    while(data0List.last().length() < 7) { data0List.last().prepend("0"); }
    data[0] = data0List.first() + data0List.last();

    QList<QString> data1List = data[1].split(QChar(0x00B0));
    while(data1List.first().length() < 3) { data1List.first().prepend("0"); }
    data1List.last().removeFirst();
    while(data1List.last().length() < 7) { data1List.last().prepend("0"); }
    data[1] = data1List.first() + data1List.last();

    return data;
}

QString Airport::convertCoordinateToWZ(QString corString, int degreeLength)
{

    if(corString.startsWith(" ")) corString.removeFirst();

    QList<QString> corSplitA = corString.split(QChar(0x00B0));

    int corDegInt = corSplitA.first().toInt();

    QList<QString> corSplitB = corSplitA.last().split("'");
    int corMinInt = corSplitB.first().removeFirst().toInt();

    QList<QString> corSplitC = corSplitB.last().split("\"");
    int corSecInt = std::round(corSplitC.first().removeFirst().toDouble());
    if(corSecInt == 60) {
        corSecInt = 0;
        corMinInt += 1;

        if(corMinInt == 60) {
            corMinInt = 0;
            corDegInt += 1;
        }
    }

    QString corDegString = QString::number(corDegInt);
    QString corMinString = QString::number(corMinInt);
    QString corSecString = QString::number(corSecInt);


    while(corDegString.length() < degreeLength) {
        corDegString.prepend("0");
    }
    while(corMinString.length() < 2) {
        corMinString.prepend("0");
    }
    while(corSecString.length() < 2) {
        corSecString.prepend("0");
    }

    QString corHemString = corSplitC.last().removeFirst();

    return corDegString + corMinString + corSecString + corHemString;
}

bool Airport::isValid(QList<int> *validStyles)
{
    // look up styles here: http://download.naviter.com/docs/CUP-file-format-description.pdf
    return validStyles->indexOf(style) != -1;
}

bool Airport::isAllreadyKnown(QList<Airport> *airports)
{
    for(int i = 0; i < airports->length(); ++i) {
        if(this->equals(airports->at(i))) {
            return true;
        }
    }

    return false;
}

bool Airport::equals(Airport airport)
{
    if(name == airport.getName()) {
        return true;
    } else if(coordinate == airport.getCoordinate()) {
        return true;
    } else if(code != "" && code == airport.getCode()) {
        return true;
    } else {
        return false;
    }
}

QString Airport::getName() const
{
    return name;
}

void Airport::setName(const QString &newName)
{
    name = newName;
}

QString Airport::getCode() const
{
    return code;
}

void Airport::setCode(const QString &newCode)
{
    code = newCode;
}

QString Airport::getCountry() const
{
    return country;
}

void Airport::setCountry(const QString &newCountry)
{
    country = newCountry;
}

int Airport::getStyle() const
{
    return style;
}

void Airport::setStyle(int newStyle)
{
    style = newStyle;
}

int Airport::getRwdir() const
{
    return rwdir;
}

void Airport::setRwdir(int newRwdir)
{
    rwdir = newRwdir;
}

double Airport::getRwlen() const
{
    return rwlen;
}

void Airport::setRwlen(double newRwlen)
{
    rwlen = newRwlen;
}

double Airport::getFreq() const
{
    return freq;
}

void Airport::setFreq(double newFreq)
{
    freq = newFreq;
}

QString Airport::getDesc() const
{
    return desc;
}

void Airport::setDesc(const QString &newDesc)
{
    desc = newDesc;
}

QGeoCoordinate Airport::getCoordinate() const
{
    return coordinate;
}

void Airport::setCoordinate(const QGeoCoordinate &newCoordinate)
{
    coordinate = newCoordinate;
}

QString Airport::generateLXeTXTFileLine()
{
    QString line = "," + name + ",T, ";
    QList<QString> coordinateStrings = coordinate.toString(QGeoCoordinate::Degrees).split(",");
    QString lat = coordinateStrings.at(0);
    lat.removeLast();
    QString lon = coordinateStrings.at(1);
    lon.removeLast();
    QString elev = coordinateStrings.at(2);
    elev.removeLast();
    elev = QString::number(std::round(elev.toDouble() * 3.28084));
    line += lat + "," + lon + "," + elev + ",";
    line += convertFreq(freq, false) + "," + QString::number(std::round(rwlen * 3.28084)) + "," + QString::number(std::round(rwdir / 10.0)) + "," + convertCUPToLXeStyle(style) + ",3,I,254,,," + code;

    QMap<QChar, QString> accentMap = {
        {QChar(0x00C7), "C"}, {QChar(0x00E7), "c"}, // Ç, ç
        {QChar(0x00C0), "A"}, {QChar(0x00C1), "A"}, {QChar(0x00C2), "A"}, {QChar(0x00C3), "A"}, {QChar(0x00C4), "A"}, {QChar(0x00C5), "A"},
        {QChar(0x00E0), "a"}, {QChar(0x00E1), "a"}, {QChar(0x00E2), "a"}, {QChar(0x00E3), "a"}, {QChar(0x00E4), "a"}, {QChar(0x00E5), "a"},
        {QChar(0x00C8), "E"}, {QChar(0x00C9), "E"}, {QChar(0x00CA), "E"}, {QChar(0x00CB), "E"},
        {QChar(0x00E8), "e"}, {QChar(0x00E9), "e"}, {QChar(0x00EA), "e"}, {QChar(0x00EB), "e"},
        {QChar(0x00CC), "I"}, {QChar(0x00CD), "I"}, {QChar(0x00CE), "I"}, {QChar(0x00CF), "I"},
        {QChar(0x00EC), "i"}, {QChar(0x00ED), "i"}, {QChar(0x00EE), "i"}, {QChar(0x00EF), "i"},
        {QChar(0x00D2), "O"}, {QChar(0x00D3), "O"}, {QChar(0x00D4), "O"}, {QChar(0x00D5), "O"}, {QChar(0x00D6), "O"}, {QChar(0x00D8), "O"},
        {QChar(0x00F2), "o"}, {QChar(0x00F3), "o"}, {QChar(0x00F4), "o"}, {QChar(0x00F5), "o"}, {QChar(0x00F6), "o"}, {QChar(0x00F8), "o"},
        {QChar(0x00D9), "U"}, {QChar(0x00DA), "U"}, {QChar(0x00DB), "U"}, {QChar(0x00DC), "U"},
        {QChar(0x00F9), "u"}, {QChar(0x00FA), "u"}, {QChar(0x00FB), "u"}, {QChar(0x00FC), "u"},
        {QChar(0x00DD), "Y"}, {QChar(0x00FD), "y"}, {QChar(0x00FF), "y"},
        {QChar(0x00D1), "N"}, {QChar(0x00F1), "n"},
        {QChar(0x00B4), "'"},  // ´ (acute accent)
        {QChar(0x0060), "`"},  // ` (grave accent)
        {QChar(0x005E), "^"},  // ^ (circumflex)
        {QChar(0x007E), "~"},  // ~ (tilde)
        {QChar(0x00A8), "\""}, // ¨ (diaeresis)
        {QChar(0x00B8), ","},  // ¸ (cedilla)
        {QChar(0x00E5), "a"}, {QChar(0x00C5), "A"}, // å, Å
        {QChar(0x00E6), "ae"}, {QChar(0x00C6), "AE"}, // æ, Æ
        {QChar(0x00FE), "th"}, {QChar(0x00DE), "TH"}, // þ, Þ
        {QChar(0x00F0), "dh"}, {QChar(0x00D0), "DH"}, // ð, Ð
        {QChar(0x00DF), "ss"}, // ß
        {QChar(0x0152), "OE"}, {QChar(0x0153), "oe"}, // Œ, œ
        {QChar(0x010C), "C"}, {QChar(0x010D), "c"}, // Č, č
        {QChar(0x0106), "C"}, {QChar(0x0107), "c"}, // Ć, ć
        {QChar(0x0110), "D"}, {QChar(0x0111), "d"}, // Đ, đ
        {QChar(0x0160), "S"}, {QChar(0x0161), "s"}, // Š, š
        {QChar(0x017D), "Z"}, {QChar(0x017E), "z"}, // Ž, ž
        {QChar(0x010E), "D"}, {QChar(0x010F), "d"}, // Ď, ď
        {QChar(0x0164), "T"}, {QChar(0x0165), "t"}, // Ť, ť
        {QChar(0x013D), "L"}, {QChar(0x013E), "l"}, // Ľ, ľ
        {QChar(0x0147), "N"}, {QChar(0x0148), "n"}, // Ň, ň
        {QChar(0x0162), "T"}, {QChar(0x0163), "t"}, // Ţ, ţ
        {QChar(0x015A), "S"}, {QChar(0x015B), "s"}, // Ś, ś
        {QChar(0x0179), "Z"}, {QChar(0x017A), "z"}, // Ź, ź
        {QChar(0x0102), "A"}, {QChar(0x0103), "a"}, // Ă, ă
        {QChar(0x0170), "U"}, {QChar(0x0171), "u"}, // Ű, ű
        {QChar(0x0154), "R"}, {QChar(0x0155), "r"}, // Ŕ, ŕ
        {QChar(0x0158), "R"}, {QChar(0x0159), "r"}, // Ř, ř
        {QChar(0x0139), "L"}, {QChar(0x013A), "l"}, // Ĺ, ĺ
        {QChar(0x0143), "N"}, {QChar(0x0144), "n"}, // Ń, ń
        {QChar(0x0136), "K"}, {QChar(0x0137), "k"}, // Ķ, ķ
        {QChar(0x013B), "L"}, {QChar(0x013C), "l"}, // Ļ, ļ
        {QChar(0x0145), "N"}, {QChar(0x0146), "n"}, // Ņ, ņ
        {QChar(0x0156), "R"}, {QChar(0x0157), "r"}, // Ŗ, ŗ
        {QChar(0x0166), "T"}, {QChar(0x0167), "t"}, // Ŧ, ŧ
        {QChar(0x0126), "H"}, {QChar(0x0127), "h"}, // Ħ, ħ
        {QChar(0x0132), "IJ"}, {QChar(0x0133), "ij"}, // Ĳ, ĳ
        {QChar(0x011E), "G"}, {QChar(0x011F), "g"}, // Ğ, ğ
        {QChar(0x0122), "G"}, {QChar(0x0123), "g"}, // Ģ, ģ
        {QChar(0x011C), "G"}, {QChar(0x011D), "g"}, // Ĝ, ĝ
        {QChar(0x0120), "G"}, {QChar(0x0121), "g"}, // Ġ, ġ
        {QChar(0x0124), "H"}, {QChar(0x0125), "h"}, // Ĥ, ĥ
        {QChar(0x0130), "I"}, {QChar(0x0131), "i"}, // İ, ı
        {QChar(0x02C7), "v"},  // ˇ (caron)
        {QChar(0x02D8), "u"},  // ˘ (breve)
        {QChar(0x02D9), "."},  // ˙ (dot above)
        {QChar(0x02DA), "o"},  // ˚ (ring above)
        {QChar(0x02DB), "a"},  // ˛ (ogonek)
        {QChar(0x02DD), "\""}  // ˝ (double acute)
    };

    QString convLine;

    foreach(QChar ch, line) {
        if(accentMap.contains(ch)) {
            convLine.append(accentMap[ch]);
        } else {
            convLine.append(ch);
        }
    }

    return convLine;
}

QString Airport::generateCUPFileLine()
{
    //name,code,country,lat,lon,elev,style,rwdir,rwlen,freq,desc


    QString line;
    if(name.length() > 0) line += "\"" + name + "\"";
    line += ",\"" + code + "\"," + country + ",";
    QList<QString> coordinateCupString = convertCoordinateToCUP(coordinate);
    line += coordinateCupString[0] + "," + coordinateCupString[1] + "," + coordinateCupString[2] + ",";
    line += QString::number(style) + "," + QString::number(rwdir) + "," + QString::number(rwlen)  + "m,";
    line += QString::number(freq) + ",";
    if(desc.length() > 0) line += "\"" + desc + "\"";

    return line;
}

QString Airport::generateWZFileLine()
{
    QString line;

    QString tempName = removeSpecialSigns(&name);
    QString tempDesc = removeSpecialSigns(&desc);

    if(tempName.length() >= 12) {
        line += tempName.first(12);
    } else {
        line += tempName;
        while(line.length() < 12) {
            line += " ";
        }
    }
    line += " ";

    QList<QString> coordinateStrings = coordinate.toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere).split(",");

    line += convertCoordinateToWZ(coordinateStrings.at(0), 2) + " ";
    line += convertCoordinateToWZ(coordinateStrings.at(1), 3) + " ";

    QString elev = coordinateStrings[2];
    elev.removeFirst();
    elev.removeLast();
    elev = QString::number(std::round(elev.toDouble()));
    while(elev.length() < 4) {
        elev.prepend("0");
    }

    line += elev + " ";

    if(freq != 0.0) {
        line += convertFreq(freq, true);
    } else {
        if (QList<int>({2, 4, 5}).indexOf(style) != -1){
            line += "1";
        } else {
            line += "2";
        }

        if(tempDesc.length() > 8) {
            line += tempDesc.first(8);
        } else {
            line += tempDesc;
        }
    }

    while(line.length() < 44) {
        line.append(" ");
    }

    return line.toUpper().replace("?", " ").replace(",", " ").replace("'", " ");
}

QString Airport::removeSpecialSigns(QString* in)
{
    QString out = in->normalized(QString::NormalizationForm_KD);
    return out;
}
