#include "noteitem.h"
#include "ui_noteitem.h"
#include <QFont>
NoteItem::NoteItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteItem)
{
    ui->setupUi(this);
    setFixedSize(400,150);
    InitUi();
    ConnectSignals();
}

void NoteItem::InitUi()
{
    ui->ContentLabel->setText("便签内容");
    ui->TimeLabel->setText("截止时间");
    ui->PushButtonRight->setText("确认");
    ui->PushButtonLeft->setText("取消");
    s
}
void NoteItem::ConnectSignals()
{
    connect(ui->PushButtonRight,&QPushButton::clicked,this,&NoteItem::PushButtonRightClicked);
    connect(ui->PushButtonLeft,&QPushButton::clicked,this,&NoteItem::PushButtonLeftClicked);
}

QString NoteItem::GetContentStr()
{
    return ui->ContentLineEdit->text();
}
QString NoteItem::GetTimeStr()
{
    return ui->TimeLineEdit->text();
}

NoteItem::~NoteItem()
{
    delete ui;
}
