#ifndef VALUEBASE
#define VALUEBASE

#include<defines.h>

namespace RobotSDK
{

class NODE_VALUE_BASE_TYPE
{
public:
    NODE_VALUE_BASE_TYPE();
    virtual ~NODE_VALUE_BASE_TYPE();
protected:
    QList< std::function< void(XMLDomInterface &, NODE_VALUE_BASE_TYPE *) > > _xmlloadfunclist;
public:
    void loadXMLValues(QString configFileName, QString nodeFullName);
};

class NODE_PARAMS_BASE_TYPE : public NODE_VALUE_BASE_TYPE
{
    friend class Node;
public:
    NODE_PARAMS_BASE_TYPE();
    ~NODE_PARAMS_BASE_TYPE();
public:
    QString nodeclass;
    QString nodename;
    QString exname;
};

class NodeSwitcher : public QPushButton
{
    Q_OBJECT
    friend class Node;
protected:
    QObject * _node;
public:
    NodeSwitcher(QWidget * parent=0);
public slots:
    void slotSwitchNode();
    void slotNodeState(bool openFlag, QString nodeFullName);
};

class NODE_VARS_BASE_TYPE : public NODE_VALUE_BASE_TYPE
{
    friend class InputPorts;
    friend class Node;
    friend class Graph;
public:
    NODE_VARS_BASE_TYPE();
    ~NODE_VARS_BASE_TYPE();
protected:
    QMutex _inputportlock;
    uint _inputportnum;
    QVector< uint > _buffersize;
    QVector< ObtainBehavior > _obtaindatabehavior;
    QVector< uint > _obtaindatasize;
    QVector< bool > _triggerflag;
    bool _guithreadflag;
public:
    void setInputPortBufferSize(uint portID, uint bufferSize);
    void setInputPortBufferSize(QList< uint > bufferSize);
    void setInputPortObtainDataBehavior(uint portID, ObtainBehavior obtainDataBehavior);
    void setInputPortObtainDataBehavior(QList< ObtainBehavior > obtainDataBehavior);
    void setInputPortObtainDataSize(uint portID, uint obtainDataSize);
    void setInputPortObtainDataSize(QList< uint > obtainDataSize);
    void setInputPortTriggerFlag(uint portID, bool triggerFlag);
    void setInputPortTriggerFlag(QList< bool > triggerFlag);
    void setNodeGUIThreadFlag(bool guiThreadFlag);
protected:
    QMap< QString, QObject * > _qobjecttriggermap;
    QMap< QString, bool > _qobjecttriggerpoolthreadflagmap;
    QMap< QString, QObject * > _qwidgettriggermap;
    QMultiMap< QObject *, QString > _defaultconnectionmap;
    QMultiMap< QObject *, QPair< QString, QString > > _userconnectionmap;
    QMultiMap< QPair< QObject *, QObject * > , QPair< QString, QString > > _connectionmap;
    QMap< QString, QWidget * > _qwidgetmap;
    QMap< QString, QLayout * > _qlayoutmap;
public:
    ADD_QWIDGET(QWidget, widget)
    ADD_QWIDGET(NodeSwitcher, nodeSwitcher)
public:
    QWidget * getWidget();
    NodeSwitcher * getNodeSwitcher();
protected:
    QObject * _node;
public:
    const QObject *getNode();
};

class NODE_DATA_BASE_TYPE : public NODE_VALUE_BASE_TYPE
{
    friend class OutputPort;
public:
    NODE_DATA_BASE_TYPE();
    ~NODE_DATA_BASE_TYPE();
protected:
    QList< bool > _filterflag;
    uint portid;
public:
    void setOutputPortFilterFlag(QList< bool > filterFlag);
public:
    QTime timestamp;
};

}

#endif // VALUEBASE

