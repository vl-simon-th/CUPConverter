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
