#ifndef VIRTUALSCANVIEWER
#define VIRTUALSCANVIEWER

//=================================================
//Please add headers here:
#include<VirtualScanGenerator.h>
#include<QRgb>
#include<QScrollArea>
#include<QTabWidget>
#include<QLabel>
#include<QPainter>

//=================================================
#include<RobotSDK.h>
//=================================================
//Node configuration

#undef NODE_CLASS
#define NODE_CLASS VirtualScanViewer

#undef INPUT_PORT_NUM
#define INPUT_PORT_NUM 1

#undef OUTPUT_PORT_NUM
#define OUTPUT_PORT_NUM 0

//=================================================
//Params types configuration

//If you need refer params type of other node class, please uncomment below and comment its own params type.
//NODE_PARAMS_TYPE_REF(RefNodeClassName)
class NODE_PARAMS_TYPE : public NODE_PARAMS_BASE_TYPE
{
public:
    ADD_PARAM(int, maxrange, 80)
    ADD_PARAM(int, gridsize, 10)
    ADD_PARAM(int, imagesize, 600)
};

//=================================================
//Vars types configuration

//If you need refer vars type of other node class, please uncomment below and comment its own vars type.
//NODE_VARS_TYPE_REF(RefNodeClassName)
class NODE_VARS_TYPE : public NODE_VARS_BASE_TYPE
{
public:
    ADD_QLAYOUT(QHBoxLayout, layout)
    ADD_QWIDGET(QTabWidget, tabwidget)
    ADD_QWIDGET(QScrollArea, scrollarea)
    ADD_QWIDGET(QLabel, viewer, "Virtual Scan Viewer")
};

//=================================================
//Data types configuration

//If you need refer data type of other node class, please uncomment below and comment its own data type.
//NODE_DATA_TYPE_REF(RefNodeClassName)
class NODE_DATA_TYPE : public NODE_DATA_BASE_TYPE
{

};

//=================================================
//You can declare functions here


//=================================================

#endif
