#ifndef NODE_H
#define NODE_H

#include<port.h>

namespace RobotSDK
{

class Node : public QObject
{
    Q_OBJECT
    friend class Graph;
public:
    Node(QString libraryFileName, QString configFileName, QString nodeClass, QString nodeName, QString exName);
    ~Node();
private:
    QString _nodeclass;
    QString _nodename;
    QString _exname;
private:
    std::shared_ptr<QThread> _inputthread;
    std::shared_ptr<QThread> _poolthread;
    std::shared_ptr<QThread> _outputthread;
private:
    InputPorts * _inputports;
    OutputPorts * _outputports;
protected:
    QString _libraryfilename;
    QList< QString > _funcptrlist;
    QMap< QString, std::function< QFunctionPointer(QString, QString, QString) > > _funcptrcloadmap;
    QMap< QString, QFunctionPointer > _funcptrmap;
protected:
    uint _inputportnum;
    uint _outputportnum;
protected:
    bool _loadflag;
    bool _initializeflag;
    bool _openflag;
protected:
    QString _configfilename;
    XML_PARAMS_BASE_TYPE NODE_PARAMS_ARG;
    XML_VARS_BASE_TYPE NODE_VARS_ARG;
protected:
    XML_DATA_BASE_TYPE NODE_DATA_ARG;
protected:
    PORT_PARAMS_CAPSULE INPUT_PARAMS_ARG;
    PORT_DATA_CAPSULE INPUT_DATA_ARG;
protected:
    bool eventFilter(QObject * obj, QEvent * ev);
private slots:
    void slotDefaultTrigger();
    void slotObtainParamsData(PORT_PARAMS_CAPSULE inputParams, PORT_DATA_CAPSULE inputData);
signals:
    void signalSendParamsData(TRANSFER_NODE_PARAMS_TYPE outputParams, TRANSFER_NODE_DATA_TYPE outputData);
private:
    InputPort * getInputPort(uint portID);
    OutputPort * getOutputPort(uint portID);
public:
    ADD_NODE_DEFAULT_FUNC_PTR(uint, getInputPortNum)
    ADD_NODE_DEFAULT_FUNC_PTR(uint, getOutputPortNum)
    ADD_NODE_DEFAULT_FUNC_PTR(XML_PARAMS_BASE_TYPE, generateNodeParams)
    ADD_NODE_DEFAULT_FUNC_PTR(XML_VARS_BASE_TYPE, generateNodeVars)
    ADD_NODE_DEFAULT_FUNC_PTR(XML_DATA_BASE_TYPE, generateNodeData)
    ADD_NODE_FUNC_PTR(bool, initializeNode)
    ADD_NODE_FUNC_PTR(bool, openNode)
    ADD_NODE_FUNC_PTR(bool, closeNode)
    ADD_NODE_FUNC_PTR(bool, main)
};

}

#endif // NODE_H

