#include "widget.h"
#include "ui_widget.h"
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QTextBlock>
#include "qdebug.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButtonGetFilePath,&QPushButton::clicked,this,&Widget::SlotGetFilePath);
    connect(ui->pushButtonReadFiletext,&QPushButton::clicked,[&](){
        SlotGetFileText(ui->lineEdit->text());
    });
    m_tableView = ui->tableView;
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableView->setEditTriggers(QTableView::NoEditTriggers);
    m_itemModel = new QStandardItemModel;
    m_itemModel->setColumnCount(4);
    m_itemModel->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    m_itemModel->setHeaderData(1,Qt::Horizontal,tr("文件路径"));
    m_itemModel->setHeaderData(2,Qt::Horizontal,tr("行数"));
    m_itemModel->setHeaderData(3,Qt::Horizontal,tr("内容"));
    connect(m_itemModel,&QStandardItemModel::itemChanged,this,&Widget::SlotItemChanged);
    m_tableView->setModel(m_itemModel);
}
void Widget::SlotGetFilePath()
{
    // 获取文件夹路径
    QString filepath = QFileDialog::getExistingDirectory(this,"输入项目地址","./");
    if (filepath.isEmpty()) {
        return;
    }
    else {
        ui->lineEdit->setText(filepath); //将文件路径显示在lineEdit控件中
    }
}

void Widget::SlotGetFileText(const QString& nowPath)
{

    QString directoryPath = nowPath; // 替换为你的目录路径
    QDir directory(directoryPath);

    foreach (const QFileInfo &fileInfo, directory.entryInfoList(QDir::Files | QDir::Dirs| QDir::NoDotAndDotDot))
    {
        if(fileInfo.isDir())
        {
           SlotGetFileText(fileInfo.absoluteFilePath());
        }
        else
        {
            // 设置过滤器，只获取.cpp和.h文件        // 检查文件扩展名
            QStringList filters;
            filters << "*.cpp" << "*.h";
            directory.setNameFilters(filters);
            m_StringListText.clear();
            bool beginAnnotation = false;
            if (fileInfo.suffix() == "cpp" || fileInfo.suffix() == "h") {
                QFile file(fileInfo.absoluteFilePath());
                QString fileName = fileInfo.fileName();
                if(fileName.startsWith("ui_") || fileName.startsWith("moc_"))
                    continue;
                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream.setCodec("utf-8");
                    QRegularExpression regexChinese("[\u4e00-\u9fa5]+");
                    QRegularExpression regexAnnoation("//");

                    while(!stream.atEnd())
                    {
                        QString line = stream.readLine().trimmed();
                        m_StringListText.append(line);
                        QRegularExpressionMatchIterator chineseIterator = regexChinese.globalMatch(line);
                        QRegularExpressionMatchIterator annotationIteartor = regexAnnoation.globalMatch(line);
                        if(line.contains("/*"))
                        {
                            beginAnnotation = true;
                        }
                        if(line.contains("*/"))
                        {
                            beginAnnotation = false;
                        }
                        if(annotationIteartor.hasNext() || line.contains("tr(")
                                || line.startsWith("/*") || line.endsWith("*/")
                                || line.contains("qDebug()")  || line.contains("qWarning()")
                                || (line.contains("/*") && line.contains("*/")))
                        {
                            continue;
                        }
                        if(chineseIterator.hasNext() && !beginAnnotation)
                        {
                            CreateItemRow(line,fileName,fileInfo.absoluteFilePath(),m_StringListText.size());
                        }
                        QString text = "这是一个测试";
                    }
                }
            }
        }
    }
}
void Widget::CreateItemRow(const QString &text,const QString& textFileName,const QString& textFilePath,const int& rowIndex)
{
    QList<QStandardItem*>itemRow;
    itemRow.append(CreateItem(textFileName,0));
    itemRow.append(CreateItem(textFilePath));
    itemRow.append(CreateItem(QString::number(rowIndex)));
    itemRow.append(CreateItem(text));
    m_itemModel->insertRow(m_itemModel->rowCount(),itemRow);

}
QStandardItem* Widget::CreateItem(const QString& text,int index)
{
    QStandardItem* item = new QStandardItem(text);
    item->setForeground(QBrush(Qt::red));
    if(index == 0)
    {
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
    }
    return item;
}
void Widget::ChangeItemStyle(QStandardItem *item, bool checked)
{
    if(item && checked)
    {
        QFont font;
        font.setStrikeOut(true);
        m_itemModel->blockSignals(true);
        item->setFont(font);
        item->setForeground(QBrush(Qt::black));
        m_itemModel->blockSignals(false);
    }
    else
    {
        m_itemModel->blockSignals(true);
        item->setForeground(QBrush(Qt::red));
        m_itemModel->blockSignals(false);
    }
}

void Widget::SlotItemChanged(QStandardItem *item){
    bool checked = item->checkState() == Qt::Checked;
    QModelIndex index = m_itemModel->indexFromItem(item);
    for(int i = 0;i<m_itemModel->columnCount();++i)
    {
        qDebug()<<"row is :"<<item->row()<<"  column is"<<i;
        QStandardItem *t = m_itemModel->item(index.row(),i);
        ChangeItemStyle(t,checked);
    }
    m_tableView->viewport()->update();
}

Widget::~Widget()
{
    delete ui;
    delete m_itemModel;
    m_itemModel = nullptr;
}
