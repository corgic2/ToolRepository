#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include "qdebug.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_treeView = ui->treeView;
    InitDiractory();
    InitModel();
    InitView();
    QString stylestr = "QTreeView::branch:closed:has-children:!has-siblings,\
                                                          QTreeView::branch:closed:has-children:has-siblings\
                                                          {background-color:rgba(212,212,212,0.5);}\
                                                         QTreeView::branch:open:has-children:!has-siblings,\
                                                         QTreeView::branch:open:has-children:has-siblings\
                                                         {background-color:rgba(212,212,212,1);}";
    ui->treeView->setStyleSheet(stylestr);

    ui->lineEditDir->setEnabled(false);
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::SlotGetFilePath);
    connect(ui->pushButtonAutoCreate,&QPushButton::clicked,this,&Widget::SlotGenerateFlies);
    connect(ui->lineEditDir,&QLineEdit::textChanged,[&](const QString& dir){
       m_filepath = dir;
    });
    connect(ui->lineEditClass,&QLineEdit::textChanged,[&](const QString &className){
       m_className = ui->lineEditClass->text();
    });
    connect(m_model,&QStandardItemModel::itemChanged,this,&Widget::SlotItemChanged);


}

void Widget::InitDiractory()
{
    m_business.append("Impl.cpp");
    m_business.append("Impl.h");
    m_exportFiles.append("Creator.h");
    m_exportFiles.append("MacroDefine.h");
    m_exportFiles.append("NotifyActionCallBack.h");
    m_exportFiles.append("OperateParam.h");
    m_interaction.append("Creator.cpp");
    m_interaction.append("Interaction.h");
    m_interaction.append("Interaction.cpp");
    m_interaction.append("Object.cpp");
    m_interaction.append("Object.h");
    m_WorkFlow.append("WFNode.cpp");
    m_WorkFlow.append("WFNode.h");
    m_uiModule.append(".cpp");
    m_uiModule.append(".h");
    m_uiModule.append(".ui");
}

void Widget::InitModel()
{
    m_model = new QStandardItemModel;
    m_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件名/文件夹")<<QStringLiteral("路径"));

}

void Widget::InitView()
{
    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeView->setEditTriggers(QTreeView::NoEditTriggers);
    m_brush = QBrush(Qt::red);

    m_treeView->setModel(m_model);

}
void Widget::ChangeItemStyle(QStandardItem *item, bool checked)
{
    if(item && checked)
    {
        QFont font = m_font;
        font.setStrikeOut(true);
        QColor color(Qt::black);
        m_model->blockSignals(true);
        item->setFont(font);
        item->setForeground(QBrush(Qt::black));
        m_model->blockSignals(false);
    }
    else
    {
        m_model->blockSignals(true);
        item->setFont(m_font);
        item->setForeground(m_brush);
        m_model->blockSignals(false);
    }
}

void Widget::SlotItemChanged(QStandardItem *item){
    QStandardItem* parent = item->parent();
    bool checked = item->checkState() == Qt::Checked;
    QModelIndex index = m_model->indexFromItem(item);
    int columnCount = parent->columnCount();
    for(int column = 0;column<columnCount;++column)
    {
        QStandardItem* t = parent->child(index.row(),column);
        qDebug()<<"the row is"<<index.row()<<"and the column is"<<column<<"and the text is"<<t->text();
        ChangeItemStyle(t,checked);
    }
    qDebug()<<checked;
    m_treeView->viewport()->update();
}

QStandardItem* Widget::CreateItem(const QString &text,int index)
{
    QStandardItem * tmp = new QStandardItem(text);
    tmp->setForeground(m_brush);
    if(index == 0)
    {
        tmp->setCheckable(true);
        tmp->setCheckState(Qt::Unchecked);
    }
    return tmp;
}

void Widget::SlotGetFilePath()
{
    // 获取文件夹路径
    QString filepath = QFileDialog::getExistingDirectory(this,"输入SDK组件路径地址","./");
    if (filepath.isEmpty()) {
        return;
    }
    else {
        ui->lineEditDir->setText(filepath); //将文件路径显示在lineEdit控件中
    }
}

void Widget::CreateMoziFile()
{
    QStandardItem* m_rootPath = new QStandardItem("SDK");
    m_model->appendRow(m_rootPath);
    QString filepath = m_filepath+"/"+ m_prefix + m_className;
    // 判断文件夹是否存在，不存在则创建
    QDir dir(filepath);
    if (!dir.exists()) {
        bool ismkdir = QDir().mkdir(filepath);
        if (!ismkdir)
            qDebug() << "Create path fail" << endl;
        else
            qDebug() << "Create fullpath success" << endl;
    }
    else {
        qDebug() << "fullpath exist" << endl;
    }
    //业务逻辑
    {
        QDir().mkdir(filepath + "/Business");
        QStandardItem* dir = new QStandardItem(filepath + "/Business");
        m_rootPath->appendRow(dir);
        QString BusinessDirectoryPath = filepath + "/Business/";
        for(int i = 0;i<m_business.size();++i)
        {
            QString fileName = BusinessDirectoryPath + m_prefix + m_className + m_business[i];
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.close();
            QList<QStandardItem*> list;
            list.append(CreateItem(m_prefix + m_className + m_business[i],0));
            list.append(CreateItem(fileName));
            dir->appendRow(list);
        }
    }
    //对外暴露接口
    {
        QDir().mkdir(filepath + "/ExportHeaderFile");
        QStandardItem* dir = new QStandardItem(filepath + "/ExportHeaderFile");
        m_rootPath->appendRow(dir);
        QString BusinessDirectoryPath = filepath + "/ExportHeaderFile/";
        for(int i = 0;i<m_exportFiles.size();++i)
        {
            QString fileName = BusinessDirectoryPath + m_prefix + m_className + m_exportFiles[i];
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.close();
            QList<QStandardItem*> list;
            list.append(CreateItem(m_prefix + m_className + m_exportFiles[i],0));
            list.append(CreateItem(fileName));
            dir->appendRow(list);
        }
    }
    //交互对象
    {
        QDir().mkdir(filepath + "/Interaction");
        QStandardItem* dir = new QStandardItem(filepath + "/Interaction");
        m_rootPath->appendRow(dir);
        QString BusinessDirectoryPath = filepath + "/Interaction/";
        for(int i = 0;i<m_interaction.size();++i)
        {
            QString fileName = BusinessDirectoryPath + m_prefix + m_className + m_interaction[i];
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.close();
            QList<QStandardItem*> list;
            list.append(CreateItem(m_prefix + m_className + m_interaction[i],0));
            list.append(CreateItem(fileName));
            dir->appendRow(list);
        }
    }
    //Ui文件
    {
        QDir().mkdir(filepath + "/UIModule");
        QStandardItem* dir = new QStandardItem(filepath + "/UIModule");
        m_rootPath->appendRow(dir);
        QString BusinessDirectoryPath = filepath + "/UIModule/";
        for(int i = 0;i<m_uiModule.size();++i)
        {
            QString fileName = BusinessDirectoryPath + m_className + m_uiModule[i];
            QList<QStandardItem*> list;
            list.append(CreateItem(m_className + m_uiModule[i],0));
            list.append(CreateItem(fileName));
            dir->appendRow(list);
        }
    }
    //功能组件工作流
    {
        QDir().mkdir(filepath + "/WorkFlow");
        QStandardItem* dir = new QStandardItem(filepath + "/WorkFlow");
        m_rootPath->appendRow(dir);
        QString BusinessDirectoryPath = filepath + "/WorkFlow/";
        for(int i = 0;i<m_WorkFlow.size();++i)
        {
            QString fileName = BusinessDirectoryPath + m_prefix + m_className + m_WorkFlow[i];
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            file.close();
            QList<QStandardItem*> list;
            list.append(CreateItem(m_prefix + m_className + m_WorkFlow[i],0));
            list.append(CreateItem(fileName));
            dir->appendRow(list);
        }
    }
    QStandardItem* dirChoice = new QStandardItem("可能需要修改的文件（可选）");
    m_rootPath->appendRow(dirChoice);

    //可能需要修改的路径
    {
        QString filepath = "Component/MTCompenent/" + m_module + "/Resources/" + m_module + ".qrc";
        QList<QStandardItem*> list;
        //1.qrc
        {

            list.append(CreateItem(m_module + ".qrc",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "Component/MTCompenent/" + m_module + "/Resources/images/FuncComponent/TADBtn_" + m_className + ".png";
            list.clear();
            list.append(CreateItem("TADBtn_" + m_className + ".png",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
        //2.MTApiRtStructOperations.h
        {
            filepath = "Component/MTScript/" + m_scriptModule + "/ExportHeaderFile/" + m_scriptModule + ".h";
            list.clear();
            list.append(CreateItem(m_scriptModule + ".h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "Component/MTScript/" + m_scriptModule + "/source/" + m_scriptModule + ".cpp";
            list.clear();
            list.append(CreateItem(m_scriptModule + ".cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "Component/MTScript/" + m_scriptModule + "/include/ComponentHeadFiles.h";
            list.clear();
            list.append(CreateItem("ComponentHeadFiles.h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
        //3. NmoduleTargetDelineatPA
        {
            filepath = "ModulePA/" + m_modulePA + "/Interaction/NModuleTADCptInteract.h";
            list.clear();
            list.append(CreateItem("NModuleTADCptInteract.h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
            filepath = "ModulePA/" + m_modulePA + "/Interaction/FCptInteract/" + m_prefix + m_className + "NotifyProc.cpp";
            list.clear();
            list.append(CreateItem(m_prefix + m_className + "NotifyProc.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
            filepath = "ModulePA/" + m_modulePA + "/Interaction/FCptInteract/" + m_prefix + m_className + "NotifyProc.h";
            list.clear();
            list.append(CreateItem(m_prefix + m_className + "NotifyProc.h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/Interaction/NModuleTADCptInteract.cpp";
            list.clear();
            list.append(CreateItem("NModuleTADCptInteract.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/Interaction/TADCptInteractAccessor.cpp";
            list.clear();
            list.append(CreateItem("TADCptInteractAccessor.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/Interaction/TADCptInteractAccessor.h";
            list.clear();
            list.append(CreateItem("TADCptInteractAccessor.h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/Interaction/VCptInteract/VCptROINotifyProc.cpp";
            list.clear();
            list.append(CreateItem("VCptROINotifyProc.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/Resources/configfiles/Layout/NModuleTargetDelineatPA_ToolLayout.json";
            list.clear();
            list.append(CreateItem("NModuleTargetDelineatPA_ToolLayout.json",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);

            filepath = "ModulePA/" + m_modulePA + "/NModuleInit/NModuleTADCptCreatorInit.cpp";
            list.clear();
            list.append(CreateItem("NModuleTADCptCreatorInit.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
        //4. Creator
        {
            filepath = "ModulePA/MModuleMOZIPA/MModuleInit/MModuleCptCreatorInit.cpp";
            list.clear();
            list.append(CreateItem("MModuleCptCreatorInit.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
        //5. cmakeList
        {
            list.clear();
            list.append(CreateItem("CMakeLists.txt",0));
            list.append(CreateItem(m_filepath));
            dirChoice->appendRow(list);
        }
        //6. ApiOpt_ROIBuild.h
        {
            filepath = "Component/MTComDataDefine/MTApiCommonData/ExportHeaderFile/ApiRtStructData/ApiOpt_ROIBuild.h";
            list.clear();
            list.append(CreateItem("ApiOpt_ROIBuild.h",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
        //7. ApiOpt_ROIBuild_PyModule.cpp
        {
            filepath = "Component/MTScript/MTApiRtStructOperations/source/ApiOpt_ROIBuild_PyModule.cpp";
            list.clear();
            list.append(CreateItem("ApiOpt_ROIBuild_PyModule.cpp",0));
            list.append(CreateItem(filepath));
            dirChoice->appendRow(list);
        }
    }
}

void Widget::SlotGenerateFlies()
{
    if(m_className.size() == 0)
    {
        return;
    }
    CreateMoziFile();
}
Widget::~Widget()
{
    delete ui;
    delete m_model;
    m_model = nullptr;
}
