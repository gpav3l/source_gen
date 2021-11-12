#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGenList();

    ui->pushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    if(tgen != nullptr)
    {
        printAtOut(tgen->process(ui->plainTextEditIn->toPlainText()));
    }
    else
        printAtOut("ERROR: Empty service");

}

/*!
 * \brief MainWindow::on_comboBox_activated
 * \param index
 */
void MainWindow::on_comboBox_activated(int index)
{
    if(index < genList.count())
    {
        QPluginLoader loader(genList.at(index).path);

        showPluginInfo(loader.metaData().value("MetaData").toObject());

        tgen = qobject_cast<PluginInterface*>(loader.instance());
        if(tgen == nullptr)
        {
            printAtOut("Can't load plugin");
            ui->pushButton->setEnabled(false);
        }
        else
            ui->pushButton->setEnabled(true);
    }
    else
    {
        tgen = nullptr;
        printAtOut("Wrong index of generator");
    }

}

/*!
 * \brief MainWindow::showPluginInfo
 * \param plugMetaData
 */
void MainWindow::showPluginInfo(QJsonObject plugMetaData)
{
    ui->plainTextEditAbout->clear();

    for(auto it : plugMetaData.value("Description").toArray())
    {
        ui->plainTextEditAbout->appendPlainText(it.toString());
    }

    ui->plainTextEditAbout->appendPlainText("Patterns:");

    if(plugMetaData.value("Patterns").toArray().count() != 0)
    {
        for(auto it : plugMetaData.value("Patterns").toArray())
        {
            ui->plainTextEditAbout->appendPlainText(it.toString());
        }
    }
    else
        QString("<none>");

}

/*!
 * \brief MainWindow::initGenList
 */
void MainWindow::initGenList()
{
    QStringList listFiles;
    QDir dir(QApplication::applicationDirPath() + "/Plugins/");

    genList.clear();

    if(dir.exists())
        listFiles = dir.entryList(QStringList("*"), QDir::Files);

    for(QString str: listFiles)
    {
        QPluginLoader loader(dir.absolutePath() + "/" +str);
        QJsonObject jsObj = loader.metaData();
        if(jsObj.contains("IID"))
        {
            if(jsObj.value("IID").toString() == INTERFACE)
            {
                ui->comboBox->addItem(jsObj.value("MetaData").toObject().value("Name").toString());
                genList.append({dir.absolutePath() + "/" +str});
            }
        }
    }

    ui->comboBox->setCurrentIndex(-1);
}

/*!
 * \brief MainWindow::printAtOut
 * \param text
 */
void MainWindow::printAtOut(QString text)
{
    ui->plainTextEditOut->clear();
    ui->plainTextEditOut->appendPlainText(text);
}
