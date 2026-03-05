#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Config.h"
#include "Features/FileCipher.h"
#include "Features/FileSorter.h"

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
#include <QProcess>
#include <QTimer>
#include <QEventLoop>
#include <QFileInfo>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setVisible(false);

    setupStartSettings();
    loadLastSession();

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

    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::onTableItemChanged);
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
    tempCryptoPath = QDir::tempPath() + "/ciphersort_crypto.tmp";
    tempSortPath = QDir::tempPath() + "/ciphersort_sort.tmp";

    ui->pushCancelCipher->setEnabled(false);
    ui->pushCancelSort->setEnabled(false);
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

    ui->label->setText("");
    ui->label->setVisible(false);

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
    isLoadingFile = true;

    ui->tableWidget->setRowCount(0);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        addLog(3, "Помилка: Не вдалося відкрити файл для читання!");
        return;
    }

   if (filePath != tempWorkPath && filePath != tempCryptoPath && filePath != tempSortPath)
    {
        currentFilePath = filePath;

        if (QFile::exists(tempWorkPath))
            QFile::remove(tempWorkPath);
        if (QFile::exists(tempBackupPath))
            QFile::remove(tempBackupPath);

        QFile::copy(currentFilePath, tempWorkPath);
        QFile::copy(currentFilePath, tempBackupPath);

        ui->pushCancel->setEnabled(false);
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

    isLoadingFile = false;
}


void MainWindow::saveTableToTempWorkFile()
{
    QString targetPath = tempWorkPath;
    if (EnDecTempFileNowActive)
        targetPath = tempCryptoPath;
    else if (SortTempFileNowActive)
        targetPath = tempSortPath;

    QFile file(targetPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);

        if (item)
            out << item->text() << "\n";
        else
            out << "\n";
    }
}


void MainWindow::onTableItemChanged(QTableWidgetItem *item)
{
    if (isLoadingFile)
        return;

    saveTableToTempWorkFile();

    ui->pushCancel->setEnabled(true);
    ui->label->setText("Файл изменен (не сохранено)");
    ui->label->setVisible(true);
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

    string key = pass.toStdString();
    string inPath = tempWorkPath.toStdString();
    string outPath = tempCryptoPath.toStdString();
    CipherMethod Method;
    if(ui->radioButtonRC4->isChecked())
        Method = CipherMethod::RC4;
    else if(ui->radioButtonXOR->isChecked())
        Method = CipherMethod::XOR;
    else
        Method = CipherMethod::Undefined;

    CipherResult result = ProcessFile(key, inPath, outPath, Method);

    switch (result)
    {
    case CipherResult::Success:
    {
        addLog(1, "Файл успешно зашифрован/дешифрован");
        loadFileToEditor(tempCryptoPath);
        ui->label->setText("Предпросмотр файла (Изменения не сохранены)");
        ui->label->setVisible(true);
        ui->pushCancelCipher->setEnabled(true);
        EnDecTempFileNowActive = true;
        break;
    }
    case CipherResult::FileNotFound:
        addLog(3, "Файл не найден");
        break;
    case CipherResult::WrongPassword:
        addLog(3, "Неправильный пароль");
        break;
    case CipherResult::SaveError:
        addLog(3, "Ошибка при сохранении");
        break;
    case CipherResult::UndefinedMethod:
        addLog(3, "Не известный метод шифрования");
        break;
    case CipherResult::EncryptionError:
        addLog(3, "Ошибка при шифровании/дешифровании");
        break;
    case CipherResult::EmptyFile:
        addLog(3, "Файл пустой");
        break;
    }
}


void MainWindow::on_Save_clicked()
{
    if (currentFilePath.isEmpty())
    {
        addLog(2, "Ошибка, файл не выбран!");
        return;
    }

    QString sourcePath = tempWorkPath;
    if (EnDecTempFileNowActive)
        sourcePath = tempCryptoPath;
    else if (SortTempFileNowActive)
        sourcePath = tempSortPath;

    if (!QFile::exists(sourcePath))
    {
        addLog(2, "Нету изменений для сохранения");
        return;
    }

    if (QFile::exists(currentFilePath))
        QFile::remove(currentFilePath);

    if (QFile::copy(sourcePath, currentFilePath))
    {
        if (EnDecTempFileNowActive)
        {
            if (QFile::exists(tempWorkPath))
                QFile::remove(tempWorkPath);
            QFile::copy(tempCryptoPath, tempWorkPath);

            EnDecTempFileNowActive = false;
            ui->pushCancelCipher->setEnabled(false);
        }
        else if (SortTempFileNowActive)
        {
            if (QFile::exists(tempWorkPath))
                QFile::remove(tempWorkPath);
            QFile::copy(tempSortPath, tempWorkPath);

            SortTempFileNowActive = false;
            ui->pushCancelSort->setEnabled(false);
        }

        addLog(1, "Изменения успешно сохранены");
        ui->label->setText("");
        ui->label->setVisible(false);

        loadFileToEditor(currentFilePath);
    }
}


void MainWindow::on_SaveAs_clicked()
{
    QString sourcePath = tempWorkPath;
    if (EnDecTempFileNowActive)
        sourcePath = tempCryptoPath;
    else if (SortTempFileNowActive)
        sourcePath = tempSortPath;

    if (!QFile::exists(sourcePath))
    {
        addLog(2, "Нету изменений для сохранения");
        return;
    }

    QString newPath = QFileDialog::getSaveFileName(this, "Сохранить файл как...", currentFilePath, "All Files (*.*)");

    if (newPath.isEmpty())
        return;

    if (QFile::exists(newPath))
        QFile::remove(newPath);

    if (QFile::copy(sourcePath, newPath))
    {
        addLog(1, "Файл успешно сохранён");
        ui->label->setText("");
        ui->label->setVisible(false);
    }
    else
        addLog(3, "Ошибка при сохранении файла!");
}


void MainWindow::on_pushCancel_clicked()
{
    if (QFile::exists(tempWorkPath))
        QFile::remove(tempWorkPath);

    if (QFile::copy(tempBackupPath, tempWorkPath))
    {
        ui->pushCancel->setEnabled(false);
        ui->label->setText("");
        ui->label->setVisible(false);

        loadFileToEditor(tempWorkPath);

        addLog(4, "Изменения отменены. Восстановлено стартовое состояние.");
    }
    else
        addLog(3, "Ошибка при отмене изменений!");
}


void MainWindow::on_pushCancelCipher_clicked()
{
    if (QFile::exists(tempCryptoPath))
        QFile::remove(tempCryptoPath);
    ui->pushCancelCipher->setEnabled(false);
    EnDecTempFileNowActive = false;
    loadFileToEditor(tempWorkPath);
    ui->label->setText("");
    ui->label->setVisible(false);
}


void MainWindow::on_sort_clicked()
{
    if (EnDecTempFileNowActive)
    {
        addLog(3, "Ошибка: Нельзя сортировать зашифрованный файл, иначе данные вернуть будет почти не возможно");
        return;
    }

    SortConfig config;

    int methodIndex = ui->comboBoxMethod->currentIndex();
    if (methodIndex == 0)
        config.method = SortMethod::Prefix;
    else if (methodIndex == 1)
        config.method = SortMethod::Alphabetical;
    else if (methodIndex == 2)
        config.method = SortMethod::WholeLine;
    else if (methodIndex == 3)
        config.method = SortMethod::PrefixAlphabetical;

    int dupIndex = ui->comboBoxDuplicate->currentIndex();
    if (dupIndex == 0)
        config.dupMode = DuplicateMode::KeepAll;
    else if (dupIndex == 1)
        config.dupMode = DuplicateMode::Deduplicate;
    else if (dupIndex == 2)
        config.dupMode = DuplicateMode::MoveToEnd;

    config.nChars = ui->SizePref->value();
    config.includeGroupNames = ui->chkShowHeaders->isChecked();
    config.keepRowFormatting = false;

    std::vector<Group> resultGroups;

    SortResult res = ProcessorSorting(config, tempWorkPath.toStdString(), resultGroups);

    if (res == SortResult::Success)
    {
        SaveResultToFile(tempSortPath.toStdString(), resultGroups, config.includeGroupNames);

        loadFileToEditor(tempSortPath);

        ui->pushCancelSort->setEnabled(true);
        SortTempFileNowActive = true;

        ui->label->setText("Предпросмотр файла (Сортировка не сохранена)");
        ui->label->setVisible(true);

        addLog(1, "Сортировка успешно завершена");
    }
    else if (res == SortResult::EmptyFile)
        addLog(2, "Файл пустой, сортировать нечего");
    else
        addLog(3, "Ошибка во время сортировки");
}

void MainWindow::on_pushCancelSort_clicked()
{
    if (QFile::exists(tempSortPath))
        QFile::remove(tempSortPath);

    ui->pushCancelSort->setEnabled(false);
    SortTempFileNowActive = false;

    loadFileToEditor(tempWorkPath);
    ui->label->setText("");
    ui->label->setVisible(false);
}


void MainWindow::on_pushHttpOpem_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    if (selectedItems.isEmpty())
    {
        addLog(2, "Немає виділених рядків для відкриття посилань!");
        return;
    }

    bool isPrivate = ui->checkBoxOpenInPrivWindow->isChecked();
    int openedCount = 0;

    QString pF = qEnvironmentVariable("PROGRAMW6432");
    QString pF86 = qEnvironmentVariable("PROGRAMFILES(X86)");
    QString lApp = qEnvironmentVariable("LOCALAPPDATA");

    for (QTableWidgetItem* item : selectedItems)
    {
        QString content = item->text().trimmed();
        bool looksLikeLink = content.contains("http") || content.contains("www");

        if (looksLikeLink)
        {
            if (!content.startsWith("http"))
                content = "http://" + content;

            if (isPrivate)
            {
                bool success = false;

                auto tryLaunch = [&](const QString& exeName, const QStringList& paths, const QString& flag) -> bool
                {
                    for (const QString& p : paths)
                        if (QFileInfo::exists(p))
                            return QProcess::startDetached(p, QStringList() << flag << content);

                    return QProcess::startDetached(exeName, QStringList() << flag << content);
                };

                if (tryLaunch("brave", {pF + "/BraveSoftware/Brave-Browser/Application/brave.exe", pF86 + "/BraveSoftware/Brave-Browser/Application/brave.exe"}, "--incognito"))
                    success = true;
                else if (tryLaunch("chrome", {pF + "/Google/Chrome/Application/chrome.exe", pF86 + "/Google/Chrome/Application/chrome.exe"}, "-incognito"))
                    success = true;
                else if (tryLaunch("opera", {lApp + "/Programs/Opera/launcher.exe"}, "--private"))
                    success = true;
                // 4. Opera GX
                else if (tryLaunch("opera-gx", {lApp + "/Programs/Opera GX/launcher.exe"}, "--private"))
                    success = true;
                // 5. Edge
                else if (tryLaunch("msedge", {pF + "/Microsoft/Edge/Application/msedge.exe", pF86 + "/Microsoft/Edge/Application/msedge.exe"}, "-inprivate"))
                    success = true;

                if (!success)
                {
                    addLog(2, "Браузери для приватного режиму не знайдені. Відкрито звичайно.");
                    QDesktopServices::openUrl(QUrl(content));
                }
            }
            else
            {
                QDesktopServices::openUrl(QUrl(content));
            }

            openedCount++;

            QEventLoop loop;
            QTimer::singleShot(150, &loop, &QEventLoop::quit);
            loop.exec();
        }
    }

    if (openedCount > 0)
        addLog(1, QString("Успішно відкрито посилань: %1").arg(openedCount));
    else
        addLog(2, "У виділених рядках не знайдено посилань.");
}

