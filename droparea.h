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
#ifndef DROPAREA_H
#define DROPAREA_H

#include <QLabel>
#include <QList>
#include <QUrl>
#include <QDragEnterEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class DropArea : public QLabel
{
    Q_OBJECT
public:
    explicit DropArea(QWidget *parent = nullptr);

signals:
    void urlDrop(QList<QUrl> urls);

private slots:
    void clear();

protected:
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;

};

#endif // DROPAREA_H
