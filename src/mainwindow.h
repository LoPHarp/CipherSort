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

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;

    QString currentFilePath;
    QString tempWorkPath;
    QString tempBackupPath;

    void setupStartSettings();

    void syncTreeToPath(QString path);
    void syncLineToPath(QString path);
    void setingsWhenSetFile(const QString &filePath);
    void loadLastSession();

    void addLog(int type, const QString &message);
    void loadFileToEditor(const QString &filePath);
};
#endif // MAINWINDOW_H
