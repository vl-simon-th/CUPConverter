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
#include "droparea.h"

DropArea::DropArea(QWidget *parent)
    : QLabel{parent}
{
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    setStyleSheet("border: solid 10px grey;  background-color: rgba(190,190,190,127); color:rgb(255,255,255)");
    QPixmap icon(":/Icons/Drag&Drop.png");
    setPixmap(icon.scaledToHeight(75));
    setVisible(false);
}

void DropArea::clear()
{
    setVisible(false);
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    setVisible(false);
    event->accept();
}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    setVisible(true);
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if(mimeData->hasUrls()) {
        emit urlDrop(mimeData->urls());
    }

    clear();

    event->acceptProposedAction();
}
