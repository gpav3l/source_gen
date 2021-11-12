#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QDir>
#include <QPluginLoader>
#include <QJsonArray>
#include "interfaces/PluginInterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct sGenInfo
{
    QString path;
} tGenInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_activated(int index);

private:
    QList<tGenInfo> genList;
    Ui::MainWindow *ui;
    PluginInterface *tgen = nullptr;

    void showPluginInfo(QJsonObject plugMetaData);
    void initGenList();
    void printAtOut(QString text);
};
#endif // MAINWINDOW_H
