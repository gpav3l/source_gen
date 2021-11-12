#include "verilogtbplugin.h"

VerilogTBPlugin::VerilogTBPlugin(QObject *parent)
{
}

/*!
 * \brief VerilogTBPlugin::process
 */
QString VerilogTBPlugin::process(QString text)
{
    genText = "";
    rawText = text;
    signal_declare.clear();
    module_connect.clear();

    foreach(auto line, getLines())
    {
        getTb(line);
    }

    genText = QString("%1\n\n").arg(signal_declare);
    genText.append(QString("<module_name> target (\n%1\n);").arg(module_connect));

    return genText;
}

/*!
 * \brief VerilogTBPlugin::getLines
 * \return
 */
QList<QString> VerilogTBPlugin::getLines()
{
    QList<QString> retList;

    retList = rawText.split(QRegExp("(,\\s*|\\s*$)"));

    return retList;
}

/*!
 * \brief VerilogTBPlugin::getTb
 * \param line
 */
void VerilogTBPlugin::getTb(QString line)
{
    QRegExp parser(itemPattern);

    if(parser.indexIn(line) != -1)
    {
        if((parser.cap(1) == "input") || (parser.cap(1) == "inout"))
            signal_declare.append(QString("reg %1 %2 = 0;\n").arg(parser.cap(3)).arg(parser.cap(4)));
        else
            signal_declare.append(QString("wire %1 %2;\n").arg(parser.cap(3)).arg(parser.cap(4)));

        module_connect = module_connect.replace(QRegExp("\\)$"), "),\n");

        module_connect.append(QString(".%1 (%1)").arg(parser.cap(4)));
    }
}
