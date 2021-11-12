#include "xlnxipdriver.h"

XlnxIpDriver::XlnxIpDriver(QObject *parent)
{
}

/*!
 * \brief XlnxIpDriver::process
 * \param text
 * \return
 */
QString XlnxIpDriver::process(QString text)
{
    QString retStr;
    QList<QString> lineList = text.split("\n");

    if(lineList.count() == 0)
        return "ERROR: empty input";

    if(lineList.at(0) == "")
        return "ERROR: empty input";

    genInit(lineList.at(0));

    for(int i=1; i<lineList.count(); i++)
    {
        genRegAccess(lineList.at(i));
    }

    retStr.append(QString("===== Header ====\n%1").arg(header));
    retStr.append(QString("\n===== Source ====\n%1").arg(source));

    return retStr;
}

/*!
 * \brief XlnxIpDriver::parseRegInfo
 * \param reg_descr
 * \return
 */
tRegInfo XlnxIpDriver::parseRegInfo(QString reg_descr)
{
    tRegInfo ret = tRegInfo{"", "void", 0x0, false, false, 0};
    QList<QString> list = reg_descr.split(" ");

    if(list.count() >= 4)
    {
        ret.name = list.at(0);
        ret.bytes = list.at(2).toUInt()/8;
        switch (ret.bytes)
        {
        case 1: ret.type="uint8_t"; break;
        case 2: ret.type="uint16_t"; break;
        case 8: ret.type="uint64_t"; break;
        default: ret.type="uint32_t"; break;
        }

        if(list.at(1).contains('w')) ret.isWrite = true;
        if(list.at(1).contains('r')) ret.isRead = true;

        ret.offset = list.at(3);
    }

    return ret;
}

/*!
 * \brief XlnxIpDriver::genInit
 * \param ip_name
 */
void XlnxIpDriver::genInit(QString ip_name)
{
    ipName = ip_name;
    
    header.append(QString("#include \"xil_io.h\"\n"
                          "#include \"xparameters.h\"\n\n"));
                          
    header.append(QString("typedef struct {\n"
                          "\tuint16_t DeviceId;\n"
                          "\tuint32_t Axilites_BaseAddress;\n}%1_Config;\n\n").arg(ip_name));

    header.append(QString("typedef struct {\n"
                          "\tuint32_t Axilites_BaseAddress;\n"
                          "\tuint32_t IsReady;\n}%1;\n\n").arg(ip_name));

    header.append(QString("int %1_Initialize(%1 *InstancePtr, u16 DeviceId);\n"
                  "%1_Config* %1_LookupConfig(u16 DeviceId);\n"
                  "int %1_CfgInitialize(%1 *InstancePtr, %1_Config *ConfigPtr);\n\n").arg(ip_name));

    source.append(QString("extern %1_Config %1_ConfigTable[];\n").arg(ip_name));
    source.append(QString("int %1_CfgInitialize(%1 *InstancePtr, %1_Config *ConfigPtr) {\n"
                "\tXil_AssertNonvoid(InstancePtr != NULL);\n"
                "\tXil_AssertNonvoid(ConfigPtr != NULL);\n"
                "\tInstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;\n"
                "\tInstancePtr->IsReady = XIL_COMPONENT_IS_READY;\n"
                "\treturn XST_SUCCESS;\n}\n\n").arg(ip_name));

    source.append(QString("%1_Config *%1_LookupConfig(u16 DeviceId) {\n"
                "\t%1_Config *ConfigPtr = NULL;\n"
                "\tint Index;\n"
                "\tfor (Index = 0; Index < XPAR_%2_NUM_INSTANCES; Index++) {\n"
                "\t\tif (%1_ConfigTable[Index].DeviceId == DeviceId) {\n"
                "\t\t\tConfigPtr = &%1_ConfigTable[Index];\n"
                "\t\t\tbreak;\n\t\t}\n\t}\n"
                "\treturn ConfigPtr;\n}\n\n").arg(ip_name).arg(ip_name.toUpper()));

    source.append(QString("int %1_Initialize(%1 *InstancePtr, u16 DeviceId) {\n"
                "\t%1_Config *ConfigPtr;\n"
                "\tXil_AssertNonvoid(InstancePtr != NULL);\n"
                "\tConfigPtr = %1_LookupConfig(DeviceId);\n"
                "\tif (ConfigPtr == NULL) {\n"
                "\t\tInstancePtr->IsReady = 0;\n"
                "\t\treturn (XST_DEVICE_NOT_FOUND);\n\t}\n"
                "\treturn %1_CfgInitialize(InstancePtr, ConfigPtr);\n}\n\n").arg(ip_name));
}

/*!
 * \brief XlnxIpDriver::genRegAccess
 * \param reg_descr
 */
void XlnxIpDriver::genRegAccess(QString reg_descr)
{
    tRegInfo reg = parseRegInfo(reg_descr);

    if(reg.name != "")
    {
        if(reg.isWrite) genWriteReg(reg);
        if(reg.isRead) genReadReg(reg);
    }
}

/*!
 * \brief XlnxIpDriver::genWriteReg
 * \param reg
 */
void XlnxIpDriver::genWriteReg(tRegInfo reg)
{
    header.append(QString("void %1_set%2(%1 *InstancePtr, %3 val);\n").arg(ipName).arg(reg.name).arg(reg.type));

    source.append(QString("void %1_set%2(%1 *InstancePtr, %3 val) {\n"
                          "\tXil_AssertVoid(InstancePtr != NULL);\n"
                          "\tXil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);\n").arg(ipName).arg(reg.name).arg(reg.type));
    switch (reg.bytes)
    {
    case 1:
        source.append(QString("\tXil_Out8(InstancePtr->Axilites_BaseAddress + %1, val);\n").arg(reg.offset));
        break;
    case 2:
        source.append(QString("\tXil_Out16(InstancePtr->Axilites_BaseAddress + %1, val);\n").arg(reg.offset));
        break;
    case 4:
        source.append(QString("\tXil_Out32(InstancePtr->Axilites_BaseAddress + %1, val);\n").arg(reg.offset));
        break;
    case 8:
        source.append(QString("\tXil_Out64(InstancePtr->Axilites_BaseAddress + %1, val);\n").arg(reg.offset));
        break;
    }

    source.append("}\n\n");
}

/*!
 * \brief XlnxIpDriver::genReadReg
 * \param reg
 */
void XlnxIpDriver::genReadReg(tRegInfo reg)
{
    header.append(QString("%3 %1_get%2(%1 *InstancePtr);\n").arg(ipName).arg(reg.name).arg(reg.type));

    source.append(QString("%3 %1_get%2(%1 *InstancePtr) {\n"
                          "\t%3 ret=0;\n"
                          "\tXil_AssertNonvoid(InstancePtr != NULL);\n"
                          "\tXil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);\n").arg(ipName).arg(reg.name).arg(reg.type));
    switch (reg.bytes)
    {
    case 1:
        source.append(QString("\tret = Xil_In8(InstancePtr->Axilites_BaseAddress + %1);\n").arg(reg.offset));
        break;
    case 2:
        source.append(QString("\tret = Xil_In16(InstancePtr->Axilites_BaseAddress + %1);\n").arg(reg.offset));
        break;
    case 4:
        source.append(QString("\tret = Xil_In32(InstancePtr->Axilites_BaseAddress + %1);\n").arg(reg.offset));
        break;
    case 8:
        source.append(QString("\tret = Xil_In64(InstancePtr->Axilites_BaseAddress + %1);\n").arg(reg.offset));
        break;
    }

    source.append("\treturn ret;\n}\n\n");
}
