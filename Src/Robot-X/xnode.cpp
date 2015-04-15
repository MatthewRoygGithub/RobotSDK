#include "xnode.h"

XNode::XNode(RobotSDK::Graph *graph, QString nodeFullName)
    : QGraphicsProxyWidget(NULL)
{
    assert(graph!=NULL);

    _graph=graph;
    connect(this,SIGNAL(signalOpenNode(QString)),_graph,SLOT(openNode(QString)));
    connect(this,SIGNAL(signalCloseNode(QString)),_graph,SLOT(closeNode(QString)));
    connect(this,SIGNAL(signalShowWidget(QString)),_graph,SLOT(showWidget(QString)));
    connect(this,SIGNAL(signalHideWidget(QString)),_graph,SLOT(hideWidget(QString)));

    connect(this,SIGNAL(signalChangeNodeExName(QString,QString)),_graph,SLOT(changeNodeExName(QString,QString)));
    connect(this,SIGNAL(signalChangeNodeLibrary(QString,QString)),_graph,SLOT(changeNodeLibrary(QString,QString)));
    connect(this,SIGNAL(signalChangeNodeConfigFile(QString,QString)),_graph,SLOT(changeNodeConfigFile(QString,QString)));

    connect(_graph,SIGNAL(addNodeResult(bool,QString,const RobotSDK::Node*)),this,SLOT(slotAddNodeResult(bool,QString,const RobotSDK::Node*)));
    connect(_graph,SIGNAL(changeNodeExNameResult(bool,QString,const RobotSDK::Node*)),this,SLOT(slotChangeNodeExNameResult(bool,QString,const RobotSDK::Node*)));
    connect(_graph,SIGNAL(changeNodeLibraryResult(bool,QString,const RobotSDK::Node*)),this,SLOT(slotChangeNodeLibraryResult(bool,QString,const RobotSDK::Node*)));

    widget=new QWidget;
    widget->setStyleSheet(QString("QWidget {border: 1px solid black}"));
    this->setWidget(widget);

    QHBoxLayout * layout=new QHBoxLayout;
    widget->setLayout(layout);

    inputports=new QVBoxLayout;
    layout->addLayout(inputports);

    nodelayout=new QVBoxLayout;
    layout->addLayout(nodelayout);

    outputports=new QVBoxLayout;
    layout->addLayout(outputports);

    nodefullname=new QLabel(nodeFullName);
    nodefullname->setStyleSheet("QLabel {border: 2px solid black; font: bold}");
    nodefullname->setAlignment(Qt::AlignCenter);
    nodefullname->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(nodefullname,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(slotNodeFullNameMenu(const QPoint &)));
    nodelayout->addWidget(nodefullname);

    libraryfilename=new QLineEdit;
    libraryfilename->setReadOnly(1);
    nodelayout->addWidget(libraryfilename);

    configfilename=new QLineEdit;
    configfilename->setReadOnly(1);
    nodelayout->addWidget(configfilename);

    opennode=new QPushButton("Open Node");
    connect(opennode,SIGNAL(clicked()),this,SLOT(slotOpenNode()));
    nodelayout->addWidget(opennode);
    opennode->setStyleSheet("QPushButton {background-color: red; color: black;}");

    showwidget=new QPushButton("Show Widget");
    connect(showwidget,SIGNAL(clicked()),this,SLOT(slotShowWidget()));
    nodelayout->addWidget(showwidget);
    showwidget->setStyleSheet("QPushButton {background-color: red; color: black;}");



    if(graph->contains(nodeFullName))
    {
        _node=_graph->getNode(nodeFullName);
        connect(_node,SIGNAL(signalNodeState(bool,QString)),this,SLOT(slotNodeState(bool,QString)),Qt::QueuedConnection);

        _inputportnum=_node->_inputportnum;
        _outputportnum=_node->_outputportnum;

        libraryfilename->setText(_node->_libraryfilename);
        libraryfilename->setToolTip(_node->_libraryfilename);
        configfilename->setText(_node->_configfilename);
        configfilename->setToolTip(_node->_configfilename);
    }
    else
    {
        _node=NULL;

        _inputportnum=0;
        _outputportnum=0;

        libraryfilename->setText("Developing...");
        libraryfilename->setToolTip("Virtual Node");
        configfilename->setText("Config.xml");
        configfilename->setText("Config.xml");
    }


    XPortHead *label;

    label=new XPortHead(QString("Input"));
    connect(label,SIGNAL(signalResetPortNum(QString,uint)),this,SLOT(slotResetPortNum(QString,uint)));
    label->setStyleSheet(QString("QLabel {border: 1px solid black}"));
    label->setAlignment(Qt::AlignCenter);
    label->nodefullname=nodeFullName;
    label->portnum=_inputportnum;
    inputports->addWidget(label);
    inputportslist.clear();
    slotResetPortNum("Input",_inputportnum);

    label=new XPortHead(QString("Output"));
    connect(label,SIGNAL(signalResetPortNum(QString,uint)),this,SLOT(slotResetPortNum(QString,uint)));
    label->setStyleSheet(QString("QLabel {border: 1px solid black}"));
    label->setAlignment(Qt::AlignCenter);
    label->nodefullname=nodeFullName;
    label->portnum=_outputportnum;
    outputports->addWidget(label);
    outputportslist.clear();
    slotResetPortNum("Output",_outputportnum);

    resizeFlag=1;
    widget->adjustSize();
}

XNode::~XNode()
{

}

void XNode::slotOpenNode()
{
    if(_graph->contains(nodefullname->text()))
    {
        if(opennode->text()==QString("Open Node"))
        {
            emit signalOpenNode(nodefullname->text());
        }
        else
        {
            emit signalCloseNode(nodefullname->text());
        }
    }
}

void XNode::slotNodeState(bool openFlag, QString nodeFullName)
{
    Q_UNUSED(nodeFullName);
    if(openFlag)
    {
        opennode->setStyleSheet("QPushButton {background-color: green; color: black;}");
        opennode->setText("Close Node");
    }
    else
    {
        opennode->setStyleSheet("QPushButton {background-color: red; color: black;}");
        opennode->setText("Open Node");
    }
}

void XNode::slotShowWidget()
{
    if(_graph->contains(nodefullname->text()))
    {
        if(showwidget->text()==QString("Show Widget"))
        {
            emit signalShowWidget(nodefullname->text());
            showwidget->setStyleSheet("QPushButton {background-color: green; color: black;}");
            showwidget->setText("Hide Widget");
        }
        else
        {
            emit signalHideWidget(nodefullname->text());
            showwidget->setStyleSheet("QPushButton {background-color: red; color: black;}");
            showwidget->setText("Show Widget");
        }
    }
}

void XNode::slotAddEdge(QString outputNodeFullName, uint outputPortID, QString inputNodeFullName, uint inputPortID)
{
    emit signalAddEdge(outputNodeFullName,outputPortID,inputNodeFullName,inputPortID);
}

void XNode::slotRemovePort(XPort::PORTTYPE portType, QString nodeFullName, uint portID)
{
    emit signalRemovePort(portType, nodeFullName,portID);
}

void XNode::slotResetPortNum(QString text, uint portNum)
{
    QVBoxLayout * tmplayout;
    if(text=="Input")
    {
        tmplayout=inputports;
        _inputportnum=portNum;
    }
    else if(text=="Output")
    {
        tmplayout=outputports;
        _outputportnum=portNum;
    }
    else
    {
        return;
    }
    uint count=tmplayout->count()-1;
    uint i;
    if(count>portNum)
    {
        for(i=count;i>portNum;i--)
        {
            delete(tmplayout->takeAt(i)->widget());
        }
    }
    else if(count<portNum)
    {
        for(i=count;i<portNum;i++)
        {
            XPort * port=new XPort;
            connect(port,SIGNAL(signalAddEdge(QString,uint,QString,uint)),this,SLOT(slotAddEdge(QString,uint,QString,uint)),Qt::QueuedConnection);
            connect(port,SIGNAL(signalRemovePort(XPort::PORTTYPE,QString,uint)),this,SLOT(slotRemovePort(XPort::PORTTYPE,QString,uint)),Qt::QueuedConnection);
            port->setText(QString("Port_%1").arg(i));
            port->nodefullname=nodefullname->text();
            port->portid=i;
            if(text=="Input")
            {
                port->porttype=XPort::InputPort;
                inputports->addWidget(port);
                inputportslist.push_back(port);
            }
            else if(text=="Output")
            {
                port->porttype=XPort::OutputPort;
                outputports->addWidget(port);
                outputportslist.push_back(port);
            }
        }
    }
    emit signalResetPortNum(nodefullname->text());
}

void XNode::slotChangeNodeExName()
{
    QString newnodefullname=QInputDialog::getText(NULL,"Input Node Full Name with New ExName","New Node Full Name",QLineEdit::Normal,nodefullname->text());
    if(newnodefullname.size()>0)
    {
        if(_graph->contains(nodefullname->text()))
        {
            tmpnewnodefullname=newnodefullname;
            emit signalChangeNodeExName(nodefullname->text(),newnodefullname);
        }
        else
        {
            QStringList oldnodefullnamelist=nodefullname->text().split(QString("::"),QString::SkipEmptyParts);
            QStringList newnodefullnamelist=newnodefullname.split(QString("::"),QString::SkipEmptyParts);
            if(newnodefullnamelist.size()>1&&newnodefullnamelist.size()<4)
            {
                if(oldnodefullnamelist.at(0)==newnodefullnamelist.at(0)&&oldnodefullnamelist.at(1)==newnodefullnamelist.at(1))
                {
                    emit signalUpdateNode(nodefullname->text(),newnodefullname);
                    nodefullname->setText(newnodefullname);
                }
            }
        }
    }
}

void XNode::slotChangeNodeLibrary()
{
#ifdef Q_OS_LINUX
    QString newlibrary=QFileDialog::getOpenFileName(NULL,"Open Library",QString(),QString("Shared Library (*.so)"));
#endif
#ifdef Q_OS_WIN32
    QString newlibrary=QFileDialog::getOpenFileName(NULL,"Open Library",QString(),QString("Shared Library (*.dll)"));
#endif
    if(newlibrary.size()>0)
    {
        if(_graph->contains(nodefullname->text()))
        {
            emit signalChangeNodeLibrary(nodefullname->text(),newlibrary);
        }
        else
        {
            emit signalAddNode(nodefullname->text(),newlibrary,configfilename->text());
        }
    }
}

void XNode::slotChangeNodeConfigFile()
{
    QString newconfigfile=QFileDialog::getOpenFileName(NULL,"Open Library",QString(),QString("XML File (*.xml)"));
    if(newconfigfile.size()>0)
    {
        if(_graph->contains(nodefullname->text()))
        {
            emit signalChangeNodeConfigFile(nodefullname->text(),newconfigfile);
        }
        configfilename->setText(newconfigfile);
        configfilename->setToolTip(newconfigfile);
    }
}

void XNode::slotAddNodeResult(bool successFlag, QString nodeFullName, const RobotSDK::Node *node)
{
    if(successFlag&&nodeFullName==nodefullname->text())
    {
        _node=node;
        connect(_node,SIGNAL(signalNodeState(bool,QString)),this,SLOT(slotNodeState(bool,QString)),Qt::QueuedConnection);
        libraryfilename->setText(node->_libraryfilename);
        libraryfilename->setToolTip(node->_libraryfilename);
    }
}

void XNode::slotChangeNodeExNameResult(bool successFlag, QString oldNodeFullName, const RobotSDK::Node *node)
{
    if(oldNodeFullName==nodefullname->text())
    {
        if(node!=NULL)
        {
            _node=node;
            connect(_node,SIGNAL(signalNodeState(bool,QString)),this,SLOT(slotNodeState(bool,QString)),Qt::QueuedConnection);
            if(successFlag)
            {
                emit signalUpdateNode(nodefullname->text(),node->_nodefullname);
                nodefullname->setText(node->_nodefullname);
                uint i;
                for(i=0;i<node->_inputportnum;i++)
                {
                    inputportslist.at(i)->nodefullname=node->_nodefullname;
                }
                for(i=0;i<node->_outputportnum;i++)
                {
                    outputportslist.at(i)->nodefullname=node->_nodefullname;
                }
            }
        }
    }
}

void XNode::slotChangeNodeLibraryResult(bool successFlag, QString nodeFullName, const RobotSDK::Node *node)
{
    if(nodeFullName==nodefullname->text())
    {
        if(node!=NULL)
        {
            _node=node;
            connect(_node,SIGNAL(signalNodeState(bool,QString)),this,SLOT(slotNodeState(bool,QString)),Qt::QueuedConnection);
            if(successFlag)
            {
                libraryfilename->setText(node->_libraryfilename);
                libraryfilename->setToolTip(node->_libraryfilename);
            }
        }
    }
}

void XNode::slotNodeFullNameMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction("Change ExName");
    menu.addAction("Change Library");
    menu.addAction("Change Config File");
    menu.addSeparator();
    menu.addAction("Generate Code");
    menu.addSeparator();
    menu.addAction("Delete Node");
    QAction * selecteditem=menu.exec(QCursor::pos());
    if(selecteditem)
    {
        if(selecteditem->text()==QString("Change ExName"))
        {
            slotChangeNodeExName();
        }
        else if(selecteditem->text()==QString("Change Library"))
        {
            slotChangeNodeLibrary();
        }
        else if(selecteditem->text()==QString("Change Config File"))
        {
            slotChangeNodeConfigFile();
        }
        else if(selecteditem->text()==QString("Generate Code"))
        {
            QString dir=QFileDialog::getExistingDirectory(NULL,QString("Generate Code for %1").arg(nodefullname->text()),QString(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if(dir.size()>0)
            {
                slotGenerateCode(dir);
            }
        }
        else if(selecteditem->text()==QString("Delete Node"))
        {
            emit signalRemoveNode(nodefullname->text());
        }
    }
}

void XNode::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    if(resizeFlag)
    {
        emit signalResize(nodefullname->text(),event->newSize());
    }
    QGraphicsProxyWidget::resizeEvent(event);
}

void XNode::slotGenerateCode(QString dir)
{
    QStringList namelist=nodefullname->text().split(QString("::"),QString::SkipEmptyParts);
    QString nodeclass=namelist.at(0);
    QString exname=QString();
    if(namelist.size()==3)
    {
            exname=namelist.at(2);
    }
    QString headerfile=QString("%1/%2.h").arg(dir).arg(nodeclass);
    QString cppfile=QString("%1/%2.cpp").arg(dir).arg(nodeclass);

    QFileInfo headerinfo(headerfile);
    if(!headerinfo.exists())
    {
        QFile file(headerfile);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream<<QString("#ifndef %1").arg(nodeclass.toUpper())<<"\n";
            stream<<QString("#define %1").arg(nodeclass.toUpper())<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Please add headers here:")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("#include<RobotSDK_Global.h>")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Port configuration")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("#undef NODE_CLASS")<<"\n";
            stream<<QString("#define NODE_CLASS %1").arg(nodeclass)<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("#undef INPUT_PORT_NUM")<<"\n";
            stream<<QString("#define INPUT_PORT_NUM %1").arg(_inputportnum)<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("#undef OUTPUT_PORT_NUM")<<"\n";
            stream<<QString("#define OUTPUT_PORT_NUM %1").arg(_outputportnum)<<"\n";
            stream<<QString("")<<"\n";
            if(_inputportnum>0)
            {
                stream<<QString("//Uncomment below PORT_DECL and set input node class name")<<"\n";
                uint i;
                for(i=0;i<_inputportnum;i++)
                {
                    stream<<QString("//PORT_DECL(%1, InputNodeClassName)").arg(i)<<"\n";
                }
                stream<<QString("")<<"\n";
            }
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Params types configuration")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you need refer params type of other node class, please uncomment below and comment its own params type.")<<"\n";
            stream<<QString("//NODE_PARAMS_TYPE_REF(RefNodeClassName)")<<"\n";
            stream<<QString("class NODE_PARAMS_TYPE : public NODE_PARAMS_BASE_TYPE")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("};")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Vars types configuration")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you need refer vars type of other node class, please uncomment below and comment its own vars type.")<<"\n";
            stream<<QString("//NODE_VARS_TYPE_REF(RefNodeClassName)")<<"\n";
            stream<<QString("class NODE_VARS_TYPE : public NODE_VARS_BASE_TYPE")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("};")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Data types configuration")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you need refer data type of other node class, please uncomment below and comment its own data type.")<<"\n";
            stream<<QString("//NODE_DATA_TYPE_REF(RefNodeClassName)")<<"\n";
            stream<<QString("class NODE_DATA_TYPE : public NODE_DATA_BASE_TYPE")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("};")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//You can declare functions here")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("#endif")<<"\n";
            file.close();
        }
    }

    QFileInfo cppinfo(cppfile);
    if(!cppinfo.exists())
    {
        QFile file(cppfile);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream<<QString("#include\"%1\"").arg(headerinfo.fileName())<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you need use extended node, please uncomment below and comment the using of default node")<<"\n";
            stream<<QString("//USE_EXTENDED_NODE(ExtendedNodeClass[,...])")<<"\n";
            stream<<QString("USE_DEFAULT_NODE")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Original node functions")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need initialize node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_FUNC_DEF_EXPORT(bool, initializeNode)")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need manually open node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_FUNC_DEF_EXPORT(bool, openNode)")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need manually close node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_FUNC_DEF_EXPORT(bool, closeNode)")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//This is original main function, you must keep it")<<"\n";
            stream<<QString("NODE_FUNC_DEF_EXPORT(bool, main)")<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";

            if(exname.size()>0)
            {
                    stream<<QString("")<<"\n";
                    stream<<QString("//=================================================")<<"\n";
                    stream<<QString("//Extended node functions ( %1 )").arg(exname)<<"\n";
                    stream<<QString("")<<"\n";
                    stream<<QString("//If you don't need initialize node, you can delete this code segment")<<"\n";
                    stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, initializeNode)").arg(exname)<<"\n";
                    stream<<QString("{")<<"\n";
                    stream<<QString("\treturn 1;")<<"\n";
                    stream<<QString("}")<<"\n";
                    stream<<QString("")<<"\n";
                    stream<<QString("//If you don't need manually open node, you can delete this code segment")<<"\n";
                    stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, openNode)").arg(exname)<<"\n";
                    stream<<QString("{")<<"\n";
                    stream<<QString("\treturn 1;")<<"\n";
                    stream<<QString("}")<<"\n";
                    stream<<QString("")<<"\n";
                    stream<<QString("//If you don't need manually close node, you can delete this code segment")<<"\n";
                    stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, closeNode)").arg(exname)<<"\n";
                    stream<<QString("{")<<"\n";
                    stream<<QString("\treturn 1;")<<"\n";
                    stream<<QString("}")<<"\n";
                    stream<<QString("")<<"\n";
                    stream<<QString("//As an extended main function, if you delete this code segment, original main function will be used")<<"\n";
                    stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, main)").arg(exname)<<"\n";
                    stream<<QString("{")<<"\n";
                    stream<<QString("\treturn 1;")<<"\n";
                    stream<<QString("}")<<"\n";
                    stream<<QString("")<<"\n";
            }
            file.close();
        }
    }
    else if(exname.size()>0)
    {
        QFile file(cppfile);
        if(file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream<<QString("//=================================================")<<"\n";
            stream<<QString("//Extended node functions ( %1 )").arg(exname)<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need initialize node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, initializeNode)").arg(exname)<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need manually open node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, openNode)").arg(exname)<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//If you don't need manually close node, you can delete this code segment")<<"\n";
            stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, closeNode)").arg(exname)<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            stream<<QString("//As an extended main function, if you delete this code segment, original main function will be used")<<"\n";
            stream<<QString("NODE_EXFUNC_DEF_EXPORT(bool, %1, main)").arg(exname)<<"\n";
            stream<<QString("{")<<"\n";
            stream<<QString("\treturn 1;")<<"\n";
            stream<<QString("}")<<"\n";
            stream<<QString("")<<"\n";
            file.close();
        }
    }
}
