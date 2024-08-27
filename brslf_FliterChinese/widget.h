#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QList>
#include <QTableView>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void SlotGetFileText(const QString& nowPath);
    void SlotGetFilePath();
    void CreateItemRow(const QString &text,const QString& textFileName,const QString& textFilePath,const int& rowIndex);
    void ChangeItemStyle(QStandardItem *item, bool checked);
    void SlotItemChanged(QStandardItem *item);
    QStandardItem* CreateItem(const QString& text,int index = 1);
private:
    Ui::Widget *ui;
    QList<QString> m_StringListText;
    QStandardItemModel * m_itemModel = nullptr;
    QTableView * m_tableView = nullptr;
};

#endif // WIDGET_H
