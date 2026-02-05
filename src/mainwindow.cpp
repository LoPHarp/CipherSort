#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Config.h"

#include <QDesktopServices>
#include <QUrl>
#include <QHeaderView>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CipherSort QU");
    ui->progressBar->setVisible(false);

    setupStartTreeVisuals();

    loadLastSession();
}

MainWindow::~MainWindow()
{
    QString currentPath = ui->lePath->text();

    if(!currentPath.isEmpty())
    {
        QFile file(FileToSavePath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << currentPath;
            file.close();
        }
    }

    delete ui;
}

void MainWindow::setupStartTreeVisuals()
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

void MainWindow::syncTreeToPath(QString path)
{
    QModelIndex index = model->index(path);
    if(index.isValid())
    {
        if(model->isDir(index))
        {
            ui->treeView->expand(index);
        }
        else
            ui->treeView->expand(index.parent());

        ui->treeView->scrollTo(index);
        ui->treeView->setCurrentIndex(index);
    }
}

void MainWindow::syncLineToPath(QString path)
{
    ui->lePath->setText(QDir::toNativeSeparators(path));
}

void MainWindow::loadLastSession()
{
    QFile file(FileToSavePath);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString savedPath = file.readLine();
        file.close();

        if(!savedPath.isEmpty() && QFileInfo::exists(savedPath))
        {
            syncLineToPath(savedPath);
            syncTreeToPath(savedPath);
        }
    }
}


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    syncLineToPath(path);
}


void MainWindow::on_lePath_returnPressed()
{
    QString path = ui->lePath->text();
    syncTreeToPath(path);
}


void MainWindow::on_btnOpenFolder_clicked()
{
    QString path = ui->lePath->text();
    if(path.isEmpty())
        return;

    QFileInfo info(path);
    QString dirOpen;

    if(info.isFile())
    {
        dirOpen = info.absolutePath();
    }
    else
        dirOpen = info.absoluteFilePath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(dirOpen));
}


void MainWindow::on_btnOpenFile_clicked()
{
    QString path = ui->lePath->text();

    if(!path.isEmpty() && !QFileInfo::exists(path))
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


void MainWindow::on_btnBrowse_clicked()
{
    QString currentPath = ui->lePath->text();

    if(currentPath.isEmpty())
    {
        currentPath = QDir::homePath();
    }

    QString path = QFileDialog::getOpenFileName(this, "Выберите файл\папку", currentPath);

    if(!path.isEmpty())
    {
        syncLineToPath(path);
        syncTreeToPath(path);
    }
}

//Нужно сделать сохранение выбранного пути к файлу
