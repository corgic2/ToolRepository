#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QWidget>
#include <QList>
#include <QDebug>
#include <QTreeView>
#include <QStandardItemModel>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void SlotGetFilePath();
    void SlotGenerateFlies();
    void CreateMoziFile();
    void InitDiractory();
    void InitModel();
    void InitView();
    void SlotItemChanged(QStandardItem * item);
    void ChangeItemStyle(QStandardItem* item,bool checked);
    QStandardItem* CreateItem(const QString& text,int index = -1);
private:
    Ui::Widget *ui;
    QString m_filepath;
    QString m_className;
    QString m_prefix = "FCpt";
    QString m_module = "MOZIMultiImage";
    QString m_scriptModule = "MTApiRtStructOperations";
    QString m_modulePA = "NmoduleTargetDelineatPA";
    QList<QString> m_business;
    QList<QString> m_exportFiles;
    QList<QString> m_interaction;
    QList<QString> m_WorkFlow;
    QList<QString> m_uiModule;
    QTreeView* m_treeView = nullptr;
    QStandardItemModel* m_model = nullptr;
    QFont m_font;
    QBrush m_brush;
};

#endif // WIDGET_H
