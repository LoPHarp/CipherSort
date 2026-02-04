#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QHeaderView>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CipherSort QU");
    ui->progressBar->setVisible(false);



    QString savePath;
    QFile file("SavedPath.exe");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureTreeVisuals()
{
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::rootPath());
    ui->treeView->setModel(model);

    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
    ui->treeView->setHeaderHidden(true);

    ui->treeView->setIndentation(10);
    ui->treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setStretchLastSection(false);
}

void MainWindow::tryOpenPath(QString path)
{
    ui->lePath->setText(path);
    QModelIndex index = model->index(path);
    if(model->isDir(index))
    {
        ui->treeView->expand(index);
    }
    else
    {
        ui->treeView->expand(index.parent());
    }

    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    setPathInView(path);
}

