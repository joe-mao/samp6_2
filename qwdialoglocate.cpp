#include "qwdialoglocate.h"
#include "ui_qwdialoglocate.h"
#include "mainwindow.h"
#include <QDebug>

void QWDialogLocate::closeEvent(QCloseEvent *event)
{
//    MainWindow * parWind = static_cast<MainWindow *>(parentWidget());//获取父窗口指针
//    parWind->setActLocateEnable(true);//使能actTab_Locate
//    parWind->setDlgLocateNull();//将窗口指针设置为NULL
    emit changeActionEnable(true);

}

void QWDialogLocate::showEvent(QShowEvent *event)
{
    emit changeActionEnable(false);
}

QWDialogLocate::QWDialogLocate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogLocate)
{
    ui->setupUi(this);
}

QWDialogLocate::~QWDialogLocate()
{
    delete ui;
}

void QWDialogLocate::setSpinRange(int rowCount, int colCount)
{
    this->ui->spinBoxRow->setMaximum(rowCount);
    this->ui->spinBoxColumn->setMaximum(colCount);
}

void QWDialogLocate::setSpinValue(int rowNo, int colNo)
{
    //设置spinbox组件的数值
    this->ui->spinBoxRow->setValue(rowNo);
    this->ui->spinBoxColumn->setValue(colNo);
}

void QWDialogLocate::on_btnSetText_clicked()
{
    //定位到单元格,并设置字符串
    int row = this->ui->spinBoxRow->value();//行号
    int col = this->ui->spinBoxColumn->value();//列号
//    MainWindow * parWind = static_cast<MainWindow *>(parentWidget());//获取主窗口
//    parWind->setACellText(row, col, this->ui->edtCaption->text());//调用主窗口函数
    QString text = this->ui->edtCaption->text();
    emit changeCellText(row, col, text);

    if(this->ui->chkBoxRow->isChecked()){
        //行增
        this->ui->spinBoxRow->setValue(1+ui->spinBoxRow->value());
    }
    if(this->ui->chkBoxColumn->isChecked()){
        //列增
        this->ui->spinBoxColumn->setValue(1+ui->spinBoxColumn->value());
    }
}



