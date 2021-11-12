#ifndef PLUGININTERFAE_H
#define PLUGININTERFAE_H

#include <QObject>

#define INTERFACE "com.TextGen.Application.interface"

class PluginInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~PluginInterface() = default;
    virtual QString process(QString text) = 0;

};

Q_DECLARE_INTERFACE(PluginInterface, INTERFACE)

#endif // PLUGININTERFAE_H
