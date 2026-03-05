#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class LogType {
    Info,
    Success,
    Warning,
    Error
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_lePath_returnPressed();

    void on_btnOpenFolder_clicked();

    void on_btnOpenFile_clicked();

    void on_btnBrowse_clicked();

    void on_EnDecButton_clicked();

    void on_Save_clicked();

    void on_SaveAs_clicked();

    void on_pushCancel_clicked();

    void on_pushCancelCipher_clicked();

    void on_sort_clicked();

    void on_pushCancelSort_clicked();

    void on_pushHttpOpem_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;

    QString currentFilePath;
    QString tempWorkPath;
    QString tempBackupPath;
    QString tempCryptoPath;
    QString tempSortPath;
    bool isLoadingFile = false;
    bool EnDecTempFileNowActive = false;
    bool SortTempFileNowActive = false;

    void setupStartSettings();

    void loadLastSession();

    void syncTreeToPath(QString path);
    void syncLineToPath(QString path);

    void addLog(int type, const QString &message);
    void loadFileToEditor(const QString &filePath);
    void onTableItemChanged(QTableWidgetItem *item);
    void saveTableToTempWorkFile();
};
#endif // MAINWINDOW_H
