#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(this->ui->tableView);

    theModel = new QStandardItemModel(2, FixedColumnCount, this);//数据模型
    theSelection = new QItemSelectionModel(theModel);//选择模型
    connect(theSelection, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));
    this->ui->tableView->setModel(theModel);
    this->ui->tableView->setSelectionModel(theSelection);
    this->ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    LabCurFile = new QLabel(this);
    LabCellPos = new QLabel(this);
    LabCellText = new QLabel(this);
    this->ui->statusBar->addWidget(LabCurFile);
    this->ui->statusBar->addWidget(LabCellPos);
    this->ui->statusBar->addWidget(LabCellText);

    //this->setWindowState(Qt::WindowMaximized);
//    this->setWindowState(Qt::WindowFullScreen);
    this->setWindowOpacity(0.5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setACellText(int row, int column, QString text)
{
    //定位到单元格，并设置字符串
    QModelIndex index = theModel->index(row, column);//获取模型索引
    theSelection->clearSelection();
    theSelection->setCurrentIndex(index, QItemSelectionModel::Select);
    theModel->setData(index, text, Qt::DisplayRole);//设置单元格字符串

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //窗口退出时询问是否退出
    QMessageBox::StandardButton result = QMessageBox::question(this, "确认", "确认要退出本程序吗?", QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel, QMessageBox::No);
    if(result == QMessageBox::Yes){
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::setActLocateEnable(bool enable)
{
    this->ui->actTab_Locate->setEnabled(enable);
}

void MainWindow::setDlgLocateNull()
{
    dlgLocate = nullptr;
}

void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //选择单元格变化时的响应

    if(current.isValid()){
        LabCellPos->setText(QString::asprintf("当前单元格：%d 行, %d 列", current.row(), current.column()));
        QStandardItem * aItem = theModel->itemFromIndex(current);
        this->LabCellText->setText("单元格内容：" + aItem->text());

//        QFont font = aItem->font();
//        this->ui->actFontBold->font();
//        this->ui->actFontBold->setChecked(font.bold());
    }
}

void MainWindow::on_actTab_SetSize_triggered()
{
    //模态对话框，动态创建，用过之后删除
    QWDialogSize * dlgTableSize = new QWDialogSize(this);
    Qt::WindowFlags flags = dlgTableSize->windowFlags();
    dlgTableSize->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    dlgTableSize->setRowColumn(theModel->rowCount(), theModel->columnCount());
    int ret = dlgTableSize->exec();//以模态方式显示对话框
    if(ret == QDialog::Accepted){
        //Ok按钮被按下，获取对话框上的输入，设置行数和列数
        int cols = dlgTableSize->columuCount();
        theModel->setColumnCount(cols);
        int rows = dlgTableSize->rowCount();
        theModel->setRowCount(rows);
     }
    delete dlgTableSize;
}

void MainWindow::on_actTab_SetHeader_triggered()
{
    //一次创建，多次调用
    if(nullptr == dlgSetHeaders){
        dlgSetHeaders = new QWDialogHeaders(this);
    }
    if(dlgSetHeaders->headerList().count() != theModel->columnCount()){
        //如果表头列数变化,重新初始化
        QStringList strList;
        for(int i = 0; i < theModel->columnCount(); ++i){
            //获取现有的表头标题
            strList.append(theModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        }
        dlgSetHeaders->setHeaderList(strList);//对话框初始化显示
    }
    int ret = dlgSetHeaders->exec();//以模态方式显示对话框
    if(ret == QDialog::Accepted){//ok键被按下
        QStringList strList = dlgSetHeaders->headerList();
        theModel->setHorizontalHeaderLabels(strList);//设置模型的表头标题
    }
}

void MainWindow::on_actTab_Locate_triggered()
{
    //创建stayOnTop的对话框，对话框关闭时，自动删除
    this->ui->actTab_Locate->setEnabled(false);
//    dlgLocate = new QWDialogLocate(this);
    QWDialogLocate * dlgLocate = new QWDialogLocate(this);
    dlgLocate->setAttribute(Qt::WA_DeleteOnClose);//对话框关闭时自动删除
    Qt::WindowFlags flags = dlgLocate->windowFlags();//获取已有的flags
    dlgLocate->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    dlgLocate->setSpinRange(theModel->rowCount()-1, theModel->columnCount()-1);
    QModelIndex curIndex = theSelection->currentIndex();
    if(curIndex.isValid()){
        dlgLocate->setSpinValue(curIndex.row(), curIndex.column());
    }
    /*绑定信号与槽*/
    //对话框发射信号，设置单元格文字
    connect(dlgLocate, SIGNAL(changeCellText(int, int QString &)), this, SLOT(setACellText(int, int, QString &)));
    //对话框发射信号，设置actTab_Locate的属性
    connect(dlgLocate, SIGNAL(changeActionEnable(bool)), this, SLOT(setActLocateEnable(bool)));
    //主窗口发射信号，修改对话框上的spinBox的值
    connect(this, SIGNAL(cellIndexChanged(int, int)), dlgLocate, SLOT(setSpinValue(int, int)));



    dlgLocate->show();//非模态显示对话框
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    //单机单元格时，将单元格的行号，列号设置到对话框上
    emit cellIndexChanged(index.row(), index.column());
//    if(dlgLocate != nullptr){
//        dlgLocate->setSpinValue(index.row(), index.column());
//    }
}
