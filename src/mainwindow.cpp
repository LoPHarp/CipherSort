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
#include <QDateTime>
#include <QTableWidgetItem>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setVisible(false);

    setupStartSettings();
    loadLastSession();

    ui->pushCancel->hide();
    connect(ui->comboBoxMethod, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
    {
        if(index == 0 || index == 3)
        {
            ui->SizePrefText->show();
            ui->SizePref->show();
        }
        else
        {
            ui->SizePrefText->hide();
            ui->SizePref->hide();
        }
    });

    ui->inPassword->setEchoMode(QLineEdit::Password);
    connect(ui->viewPass, &QCheckBox::checkStateChanged, [this](Qt::CheckState state)
    {
        if(state == Qt::Checked)
            ui->inPassword->setEchoMode(QLineEdit::Normal);
        else
            ui->inPassword->setEchoMode(QLineEdit::Password);
    });

    ui->twLog->hide();
    connect(ui->chkShowLog, &QCheckBox::checkStateChanged, [this](Qt::CheckState state)
    {
        if(state == Qt::Checked)
            ui->twLog->show();
        else
            ui->twLog->hide();
    });

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


void MainWindow::setupStartSettings()
{
    setWindowTitle("CipherSort QU");

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

    ui->twLog->setColumnCount(1);
    ui->twLog->horizontalHeader()->setStretchLastSection(true);
    ui->twLog->horizontalHeader()->hide();
    ui->twLog->verticalHeader()->hide();
    ui->twLog->setShowGrid(false);
    ui->twLog->verticalHeader()->setDefaultSectionSize(20);
    QFont logFont = ui->twLog->font();
    logFont.setPointSize(9);
    ui->twLog->setFont(logFont);

    tempWorkPath = QDir::tempPath() + "/ciphersort_work.tmp";
    tempBackupPath = QDir::tempPath() + "/ciphersort_backup.tmp";
    ui->pushCancel->setEnabled(false);

    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(22);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setStyleSheet(
        "QTableView { gridline-color: rgba(128, 128, 128, 40); }"
        "QHeaderView::section:vertical { background: transparent; color: #888888; border: none; padding-right: 5px; }"
        );
    QFont editorFont("Consolas", 10);
    ui->tableWidget->setFont(editorFont);
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
            addLog(4, "Сессия загружена: восстановлен путь " + savedPath);
        }
    }
}


void MainWindow::addLog(int type, const QString &message)
{
    QString time = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->twLog->insertRow(0);

    QTableWidgetItem *item = new QTableWidgetItem(time + " | " + message);
    QColor bgColor;
    QIcon icon;

    switch (type)
    {
    case 1:
        bgColor = QColor(0, 255, 0, 10);
        icon = QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton);
        break;
    case 2:
        bgColor = QColor(255, 255, 0, 10);
        icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
        break;
    case 3:
        bgColor = QColor(255, 0, 0, 10);
        icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
        break;
    case 4:
        bgColor = QColor(0, 191, 255, 10);
        icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
        break;
    default:
        bgColor = QColor(255, 255, 255, 0);
        break;
    }

    item->setBackground(bgColor);
    item->setIcon(icon);

    ui->twLog->setItem(0, 0, item);
}


void MainWindow::loadFileToEditor(const QString &filePath)
{
    ui->tableWidget->setRowCount(0);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        addLog(3, "Помилка: Не вдалося відкрити файл для читання!");
        return;
    }

    if (filePath != tempWorkPath)
    {
        currentFilePath = filePath;

        if (QFile::exists(tempWorkPath))
            QFile::remove(tempWorkPath);
        if (QFile::exists(tempBackupPath))
            QFile::remove(tempBackupPath);

        QFile::copy(currentFilePath, tempWorkPath);
        QFile::copy(currentFilePath, tempBackupPath);

        ui->pushCancel->setEnabled(false);
        ui->label->setText("Предварительный просмотр файла");
        ui->label->setStyleSheet("");
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    int row = 0;
    ui->tableWidget->setUpdatesEnabled(false);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *item = new QTableWidgetItem(line);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        ui->tableWidget->setItem(row, 0, item);
        row++;
    }

    ui->tableWidget->setUpdatesEnabled(true);
}


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString path = model->filePath(index);
    syncLineToPath(path);
    loadFileToEditor(path);
}


void MainWindow::on_lePath_returnPressed()
{
    QString path = ui->lePath->text();
    syncTreeToPath(path);
    loadFileToEditor(path);
}


void MainWindow::on_btnBrowse_clicked()
{
    QString currentPath = ui->lePath->text();

    if(currentPath.isEmpty())
        currentPath = QDir::homePath();

    QString path = QFileDialog::getOpenFileName(this, "Выберите файл/папку", currentPath);

    if(!path.isEmpty())
    {
        syncLineToPath(path);
        syncTreeToPath(path);
        loadFileToEditor(path);
    }
}


void MainWindow::on_btnOpenFolder_clicked()
{
    QString path = ui->lePath->text();
    if(path.isEmpty())
    {
        addLog(2, "Путь к файлу не установлен");
        return;
    }

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
    {
        addLog(2, "Путь к файлу не установлен");
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}





void MainWindow::on_EnDecButton_clicked()
{
    QString pass = ui->inPassword->text();
    if(pass.isEmpty())
    {
        addLog(2, "Поле для ввода пароля, пустое!!!");
        return;
    }


}

