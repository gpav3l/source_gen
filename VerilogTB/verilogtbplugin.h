#ifndef VERILOGTBPLUGIN_H
#define VERILOGTBPLUGIN_H

#include "PluginInterface.h"

class VerilogTBPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID INTERFACE FILE "VerilogTB.json")

public:
    explicit VerilogTBPlugin(QObject *parent = nullptr);
    QString process(QString text);

private:
    QString itemPattern = "(input|output|inout)\\s*(wire|reg)?\\s*(\\[.*\\])?\\s*(\\w*)";

    QString rawText;
    QString genText;
    QString signal_declare;
    QString module_connect;
    QString initPart;

    QList<QString> getLines();
    void getTb(QString line);
};

#endif // VERILOGTBPLUGIN_H
