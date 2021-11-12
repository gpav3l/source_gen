#ifndef XLNXIPDRIVER_H
#define XLNXIPDRIVER_H

#include "PluginInterface.h"

typedef struct sRegInfo
{
    QString name;
    QString type;
    QString offset;
    bool isRead;
    bool isWrite;
    int bytes;
}tRegInfo;

class XlnxIpDriver : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID INTERFACE FILE "XlnxIpDriver.json")

public:
    explicit XlnxIpDriver(QObject *parent = nullptr);
    QString process(QString text);

private:
    QString header;
    QString source;
    QString ipName;

    tRegInfo parseRegInfo(QString reg_descr);
    void genInit(QString ip_name);
    void genRegAccess(QString reg_descr);
    void genWriteReg(tRegInfo reg);
    void genReadReg(tRegInfo reg);

};

#endif // XLNXIPDRIVER_H
