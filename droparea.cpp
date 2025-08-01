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
