#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void AddItemToTreeWidget(QString text);
private:
    void SlotAddButtonClicked();
    void InitUI();
    void ConnectSinals();
private:
    Ui::Widget *ui;
    QList<QListWidgetItem> Items;
};

#endif // WIDGET_H
