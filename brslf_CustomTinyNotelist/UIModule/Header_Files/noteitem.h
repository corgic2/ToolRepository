#ifndef NOTEITEM_H
#define NOTEITEM_H

#include <QWidget>
#include <QString>
namespace Ui {
class NoteItem;
}

class NoteItem : public QWidget
{
    Q_OBJECT
    enum State{
        Yes,
        No
    };
public:
    explicit NoteItem(QWidget *parent = 0);
    ~NoteItem();
    QString GetContentStr();
    QString GetTimeStr();
signals:
    void PushButtonRightClicked();
    void PushButtonLeftClicked();
private:
    void InitUi();
    void ConnectSignals();
private:
    Ui::NoteItem *ui;
};

#endif // NOTEITEM_H
