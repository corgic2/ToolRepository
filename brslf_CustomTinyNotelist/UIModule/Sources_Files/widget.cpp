#include "widget.h"
#include "noteitem.h"
#include "ui_widget.h"
#include "qDebug.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitUI();
    ConnectSinals();
}
void Widget::SlotAddButtonClicked()
{
    NoteItem* newItem = new NoteItem(this);
    newItem->show();
}
void Widget::ConnectSinals()
{
    connect(ui->toolButton,&QToolButton::clicked,this,&Widget::SlotAddButtonClicked);
}
void Widget::InitUI()
{
    ui->TitleLabel->setText("便签小工具");
}
void Widget::AddItemToTreeWidget(QString text)
{

}
Widget::~Widget()
{
    delete ui;
}
