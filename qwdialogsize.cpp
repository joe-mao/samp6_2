#include "qwdialogsize.h"
#include "ui_qwdialogsize.h"
#include <QMessageBox>

QWDialogSize::QWDialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogSize)
{
    ui->setupUi(this);
}

QWDialogSize::~QWDialogSize()
{
    QMessageBox::information(this, "提示", "设置表格行列数对话框被删除");
    delete ui;
}

int QWDialogSize::rowCount()
{
    //用于主窗口调用获得行数的输入值
    return this->ui->spinBoxRow->value();
}

int QWDialogSize::columuCount()
{
    //用于主窗口调用获得列数的输入值
    return this->ui->spinBoxColumn->value();
}

void QWDialogSize::setRowColumn(int row, int column)
{
    //初始化数据显示
    this->ui->spinBoxRow->setValue(row);
    this->ui->spinBoxColumn->setValue(column);
}


