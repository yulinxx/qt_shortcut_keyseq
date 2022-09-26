#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeySequence>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ui->tableWidget->set_show_reset_buttons(true);
//    qDebug()<<ui->keyTableWidget->showResetBtns();

    ui->keyTableWidget->appendWindow(this);
    ui->keyTableWidget->setShowResetBtns(false);
//    QKeySequence seq(QKeySequence::Open);
//    ui->actionDid->setShortcut(seq);

}

MainWindow::~MainWindow()
{
    delete ui;
}
