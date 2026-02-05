#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFile>
#include <QTextStream>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;

    void setupStartTreeVisuals();
    void syncTreeToPath(QString path);
    void syncLineToPath(QString path);
    void loadLastSession();
};
#endif // MAINWINDOW_H
