#include "PageStation_all.h"
QMap<QString, AxisObject*> axisMap;
QMap<QString, ModuleObject*> moduleMap;
QMap<QString, RobotObject*> robotMap;

QMap<QString, ActionObject*> actionMap;
QMap<QString, ActionDetail*> actionDetailMap;
QMap<QString,int> txtSequence;
//rong lk
QMap<QString, TrajectoryObject*> trajectoryMap;
QMap<QString, TrajectoryDetail*> trajectoryDetailMap;
QMap<QString, CalibrationObject*> calibrationMap;
QMap<QString, CalibrationDetail*> calibrationDetailMap;
//end
QStringList modulNameList;
QStringList axisNameList;
QStringList knifeList;
QStringList moduleIdList;
QStringList axisIdList;
QStringList knifeIdList;

QStringList operList;
QStringList actionList;
QStringList exterList;
QStringList axisTypeList;
//rong lk
QStringList trajectoryList;
//end
#include "Global.h"
//#include "Viewer.h";
//extern Viewer *pageViewer;

PageStation::PageStation(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    lastSelectedPath = "";
//rong lk
    hModule = NULL;
//end
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(postOpenSignal()), this, SLOT(postOpen()), Qt::QueuedConnection);
//rong lk
    connect(this,SIGNAL(calibrationFlangeSuccess()), this, SLOT(updateCalibrationFlange()));
//end

    emit postOpenSignal();
    cur_areaIndex=0;
    areaX=areaY=areaZ=0;
    curArea=QString.null;
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui.label_areaInfo->setPalette(pa);

}

void PageStation::postOpen()
{
//    ui.tabWidget->removeTab(2);


    if (currentStationId.isEmpty()) {
        ui.lineEdit_station->setEnabled(false);
        ui.plainTextEdit_memo->setEnabled(false);
    }

//    moduleSql = new QSqlQueryModel( this );
//    moduleSql->setQuery( "SELECT id,name FROM Station ORDER BY name ASC" );
//    currentStationId = SysUtils::getSysParam( "lastStationId" ).toString();
//    int index = 0;
//    for (int i=0; i<moduleSql->rowCount(); i++) {
//        ui.comboStationName->addItem( moduleSql->data(moduleSql->index(i,1)).toString(), moduleSql->data(moduleSql->index(i,0)));
//        if (currentStationId == moduleSql->data(moduleSql->index(i,0)))
//            index = i;
//    }
    /***********update7 2014.5.27************************************/
        currentStationId = SysUtils::getSysParam( "lastStationId" ).toString();
        int index = 0, count =0;
        QSqlQuery query;
        query.exec(QString("SELECT id,name FROM Station ORDER BY name ASC"));
        while(query.next())
        {
            QSqlRecord record = query.record();
            ui.comboStationName->addItem( record.value("name").toString(), record.value("id").toString());
            if (currentStationId == record.value("id").toString())
            {
                index = count;
//                emit stationChanged(currentStationId);
            }
            count++;
        }
    /***********************end**********************************/

    /***********************update8 by cxs**********************************/
         readRobotInfo();
    /*********************************end******************************************************/

    connect(ui.comboStationName, SIGNAL(currentIndexChanged(int)), this, SLOT(stationListChanged(int)));
//    connect(this, SIGNAL(postStationListChanged(int)), this, SLOT(stationListChanged(int)), Qt::QueuedConnection);

    ui.comboStationName->setCurrentIndex( index );
    if (index == 0 ) {
//        emit postStationListChanged(index);
        this->stationListChanged( index );
//        qApp->postEvent(ui.comboStationName,new QEvent(QEvent::ModifiedChange));
    }

}

PageStation::~PageStation()
{
//ronglk
    if(hModule != NULL)
    {
        ::FreeLibrary(hModule);
        hModule = NULL;
    }
//end

}

int PageStation::robotNum()
{

    return  ui.cur_robot->count();
}

void PageStation::showEvent( QShowEvent * /*event*/ )
{

    static bool showInited = false;
    if (showInited) return;
    showInited = true;

    // Module
    variantManagerModule = new QtVariantPropertyManager();
    topPropertyModule = variantManagerModule->addProperty(QtVariantPropertyManager::groupTypeId(), tr("组件属性"));


    QtVariantProperty *item = variantManagerModule->addProperty(QVariant::String, tr("名称"));
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Bool, tr("隐藏"));
    item->setValue(false);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QtVariantPropertyManager::enumTypeId(), tr("关联轴"));
//    QStringList axisNameList = QStringList("无");
//    foreach (AxisObject *axis, axisMap.values()) {
//        axisNameList << axis->name;
//    }
//    axisNameList.sort();
    item->setAttribute(QLatin1String("enumNames"), axisNameList);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("X偏移"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("Y偏移"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("Z偏移"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("X轴旋转"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("Y轴旋转"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("Z轴旋转"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Double, tr("旋转角度"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QtVariantPropertyManager::enumTypeId(), tr("材质"));
    QStringList enumNames;
    enumNames << "黄铜" << "青铜" << "铜" << "金" << "锡" << "石膏" << "塑料" << "银" << "钢" << "石" << "高光塑料" << "色丁" << "灰" << "铬" << "铝" << "黑曜石" << "管桩" << "玉" << "默认" << "自定义";
    item->setAttribute(QLatin1String("enumNames"), enumNames);
//    QMap<int, QIcon> enumIcons;
//    enumIcons[0] = QIcon(":/images/resource/images/F1.png");
//    enumIcons[1] = QIcon(":/images/resource/images/F2.png");
//    enumIcons[2] = QIcon(":/images/resource/images/F3.png");
//    item->setAttribute(QLatin1String("enumIcons"), QVariant::fromValue( enumIcons ));
    item->setValue(18);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Color, tr("颜色"));
    item->setValue(Qt::black);
    topPropertyModule->addSubProperty(item);

    item = variantManagerModule->addProperty(QVariant::Int, tr("透明度"));
    item->setValue(0);
    topPropertyModule->addSubProperty(item);

    if (ui.treeWidgetModule->selectedItems().count()>0){
        moduleSelectionChanged();
    }

    connect(variantManagerModule, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(modulePropertyChanged(QtProperty *, const QVariant &)));



    QtVariantEditorFactory *variantFactoryModule = new QtVariantEditorFactory();
    ui.propertyModule->setFactoryForManager(variantManagerModule, variantFactoryModule);
    ui.propertyModule->addProperty(topPropertyModule);
    ui.propertyModule->setPropertiesWithoutValueMarked(true);
    ui.propertyModule->setRootIsDecorated(false);

    QtIntPropertyManager *intMangerModule = new QtIntPropertyManager();
    QtSliderFactory *sliderFactory = new QtSliderFactory();
    ui.propertyModule->setFactoryForManager(intMangerModule, sliderFactory);

    QAction *treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("导入新组件");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleImport()));
    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("替换原组件");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleReplace()));
    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setSeparator(true);
    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("新建分组");
//    connect(treeAction, SIGNAL(triggered()), SLOT(moduleNewGroup()));
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("新建运动机构");
//    connect(treeAction, SIGNAL(triggered()), SLOT(moduleNewMotion()));
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setSeparator(true);
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("平移");
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("旋转");
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("重置");
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setSeparator(true);
//    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("重命名");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleRename()));
    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("复制");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleDuplicate()));
    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("阵列");
//    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("删除");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleRemove()));
    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("移动");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleTranslate()));
    ui.treeWidgetModule->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("旋转");
    connect(treeAction, SIGNAL(triggered()), SLOT(moduleTranslate()));
    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("隐藏");
//    treeAction->setCheckable(true);
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setSeparator(true);
//    ui.treeWidgetModule->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetModule);
//    treeAction->setText("属性");
//    connect(treeAction, SIGNAL(triggered()), SLOT(moduleProperty()));
//    ui.treeWidgetModule->addAction(treeAction);

/******************update13*******************/
    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("查找动作");
    connect(treeAction, SIGNAL(triggered()), SLOT(showModuleAction()));
    ui.treeWidgetModule->addAction(treeAction);
/*********************end******************/

//rong lk
/******************by ronglk*******************/
    treeAction = new QAction(ui.treeWidgetModule);
    treeAction->setText("增加标签");
    connect(treeAction, SIGNAL(triggered()), SLOT(AddModuleLable()));
    ui.treeWidgetModule->addAction(treeAction);

    QAction *treeTrajectory = new QAction(ui.treeWidgetTrajectory);
    treeTrajectory->setText("添加路径");

    variantManagerTrajectory = new QtVariantPropertyManager(this);
    topPropertyTrajectory = variantManagerTrajectory->addProperty(QtVariantPropertyManager::groupTypeId(), tr("轨迹属性"));

   item = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("刀具类型"));
   axisTypeList<<"钻头"<<"铣刀";
   item->setAttribute(QLatin1String("enumNames"), axisTypeList);
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);


   item = variantManagerTrajectory->addProperty(QTCMMF,tr("刀具方向"));
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("X方向"));
   item->setValue(1);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("Y方向"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("Z方向"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);


   item = variantManagerTrajectory->addProperty(QTCMMF,tr("刀具位置"));
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("X位置"));
   item->setValue(1);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("Y位置"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("Z位置"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);

   item = variantManagerTrajectory->addProperty(QTCMMF,tr("刀具截面"));
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("上边"));
   item->setValue(1);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("高"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);
   item = variantManagerTrajectory->addProperty(QVariant::Double, tr("下边"));
   item->setValue(0);
   topPropertyTrajectory->addSubProperty(item);


/*********************end******************/
//end
    //Axis
    /******************add by cxs 2014 4.16****/
       variantManagerAxis = new QtVariantPropertyManager(this);
       topPropertyAxis = variantManagerAxis->addProperty(QtVariantPropertyManager::groupTypeId(), tr("轴属性"));

       item = variantManagerAxis->addProperty(QVariant::String, tr("名称"));
       topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("轴类型"));
      axisTypeList<<"平移轴"<<"旋转轴"<<"运动机构轴群组"<<"用户轴群组"<<"无限平移轴"<<"无限旋转轴";
      item->setAttribute(QLatin1String("enumNames"), axisTypeList);
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QVariant::String, tr("数据来源")); //标识
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("上级轴"));
      topPropertyAxis->addSubProperty(item);


      /******************update5 by cxs 2014 5.6******************/
      /*
      item = variantManagerAxis->addProperty(QVariant::Double, tr("X起始"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QVariant::Double, tr("Y起始"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QVariant::Double, tr("Z起始"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      */

      item = variantManagerAxis->addProperty(QTCMMF,tr("起始坐标"));
      topPropertyAxis->addSubProperty(item);
/*
      item = variantManagerAxis->addProperty(QVariant::Double, tr("X结束"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QVariant::Double, tr("Y结束"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
      item = variantManagerAxis->addProperty(QVariant::Double, tr("Z结束"));
      item->setValue(0);
      topPropertyAxis->addSubProperty(item);
*/
      item = variantManagerAxis->addProperty(QTCMMF,tr("结束坐标"));
      topPropertyAxis->addSubProperty(item);
      /******************************end***************************************************/

      item = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("符号"));
       QStringList enumSymbol;
       enumSymbol<<"正"<<"负";
       item->setAttribute(QLatin1String("enumNames"),enumSymbol);
        topPropertyAxis->addSubProperty(item);
         item = variantManagerAxis->addProperty(QVariant::Double, tr("最大值"));
         item->setValue(0);
         topPropertyAxis->addSubProperty(item);
         item = variantManagerAxis->addProperty(QVariant::Double, tr("最小值"));
         item->setValue(0);
         topPropertyAxis->addSubProperty(item);
         item = variantManagerAxis->addProperty(QVariant::Double, tr("原点偏移"));
         item->setValue(0);
         topPropertyAxis->addSubProperty(item);

//         item = variantManagerAxis->addProperty(QVariant::String, tr("当前机器"));
//         topPropertyAxis->addSubProperty(item);



         topPropertyAxis->setEnabled(false);
        QtVariantEditorFactory *variantFactoryAxis = new QtVariantEditorFactory();
          ui.propertyAxis->setFactoryForManager(variantManagerAxis, variantFactoryAxis);
         ui.propertyAxis->addProperty(topPropertyAxis);
          ui.propertyAxis->setPropertiesWithoutValueMarked(true);
         ui.propertyAxis->setRootIsDecorated(false);
   /************************* 4.16  ********************************/

    treeAction = new QAction(ui.treeWidgetAxis);
    treeAction->setText("新建运动轴");
    connect(treeAction, SIGNAL(triggered()), SLOT(axisNew()));
    ui.treeWidgetAxis->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetAxis);
    treeAction->setText("重命名");
    connect(treeAction, SIGNAL(triggered()), SLOT(axisRename()));
    ui.treeWidgetAxis->addAction(treeAction);

    /************update4 by cxs *******2014.4.27************/
        treeAction = new QAction(ui.treeWidgetModule);
        treeAction->setText("复制");
        connect(treeAction, SIGNAL(triggered()), SLOT(axisDuplicate()));
        ui.treeWidgetAxis->addAction(treeAction);
    /*********************end*************/

    treeAction = new QAction(ui.treeWidgetAxis);
    treeAction->setText("删除");
    connect(treeAction, SIGNAL(triggered()), SLOT(axisRemove()));
    ui.treeWidgetAxis->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetAxis);
    treeAction->setText("位置设定");
    connect(treeAction, SIGNAL(triggered()), SLOT(axisSetPosition()));
    ui.treeWidgetAxis->addAction(treeAction);

    //update zhugz
//    treeAction = new QAction(ui.treeWidgetAxis);
//    treeAction->setText("设为当前机器");
//    connect(treeAction, SIGNAL(triggered()), SLOT(axisSetCurRobot()));
//    ui.treeWidgetAxis->addAction(treeAction);
    //end

//    treeAction = new QAction(ui.treeWidgetAxis);
//    treeAction->setText("属性");
////    connect(treeAction, SIGNAL(triggered()), SLOT(axisProperty()));
//    ui.treeWidgetAxis->addAction(treeAction);


    //Action

    /**********************************
     *
     *add by cxs 2014 4.15
     *
     *********************************/
QuerKnifeName();

    /****************add by cxs 2014 4.15*********/

       variantManagerAction = new QtVariantPropertyManager();
       topPropertyAction = variantManagerAction->addProperty(QtVariantPropertyManager::groupTypeId(), tr("动作属性"));
       item = variantManagerModule->addProperty(QVariant::String, tr("名称"));
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("主动作类型"));
       actionList << "信号动作" <<"变量动作"<<"信号和变量组合动作"<<"定时动作"<<"始终动作"<<"刀具寿命动作"<<"测量保存动作"<<"视觉分析动作"<<"称重动作"<<"复位动作"<<"伺服电机"<<"通用";  //<<"测量换页动作"
       item->setAttribute(QLatin1String("enumNames"), actionList);
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QVariant::String, tr("信号名"));
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符"));
       //QStringList enumOpt;
       operList<<"="<<">"<<">="<<"<"<<"<="<<"<>";     //<<">"<<"<"<<"="<<"<>";
       item->setAttribute(QLatin1String("enumNames"), operList);
       item->setValue(2);
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QVariant::String, tr("信号值"));
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("附加条件操作符"));
       //QStringList addMove;
       exterList<< "无" <<"并且"<<"或者"<<"取反";
       item->setAttribute(QLatin1String("enumNames"), exterList);
       item->setValue(0);
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QVariant::Int, tr("动作执行次数"));
       item->setValue(0);
       topPropertyAction->addSubProperty(item);
       item = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("更新方式"));
       QStringList updateName;
       updateName<<"绝对动作"<<"相对动作";
       item->setAttribute(QLatin1String("enumNames"), updateName);
       item->setValue(1);
       topPropertyAction->addSubProperty(item);
       topPropertyAction->setEnabled(false);
   /**********************end****************************/

    if (ui.treeWidgetAction->selectedItems().count()>0){
         actionSelectionChanged();
    }
//rong lk
    /****************add by ronglk*********/

       variantManagerTrajectory = new QtVariantPropertyManager();
       topPropertyTrajectory = variantManagerTrajectory->addProperty(QtVariantPropertyManager::groupTypeId(), tr("轨迹属性"));
       item = variantManagerTrajectory->addProperty(QVariant::String, tr("名称"));
       topPropertyTrajectory->addSubProperty(item);
       item = variantManagerTrajectory->addProperty(QVariant::String, tr("轨迹分类号"));
       topPropertyTrajectory->addSubProperty(item);

       QtVariantEditorFactory *variantFactoryTrajectory = new QtVariantEditorFactory();
       ui.propertyTrajectory->setFactoryForManager(variantManagerTrajectory, variantFactoryTrajectory);
       trajectoryItemParent = ui.propertyTrajectory->addProperty(topPropertyTrajectory);
       itemPos = 0;
       trajectoryNum = 0;

           ui.propertyTrajectory->setPropertiesWithoutValueMarked(true);
          ui.propertyTrajectory->setRootIsDecorated(false);

       treeAction = new QAction(ui.treeWidgetTrajectory);
       treeAction->setText("新建轨迹");
       connect(treeAction, SIGNAL(triggered()), SLOT(trajectoryNew()));
       ui.treeWidgetTrajectory->addAction(treeAction);

       treeAction = new QAction(ui.treeWidgetTrajectory);
       treeAction->setText("选择路径");
       connect(treeAction, SIGNAL(triggered()), SLOT(selectPath()));
       ui.treeWidgetTrajectory->addAction(treeAction);

       treeAction = new QAction(ui.treeWidgetTrajectory);
       treeAction->setText("重命名");
       connect(treeAction, SIGNAL(triggered()), SLOT(trajectoryRename()));
       ui.treeWidgetTrajectory->addAction(treeAction);

       /************update4 by cxs *******2014.4.27************/
       treeAction = new QAction(ui.treeWidgetTrajectory);
       treeAction->setText("复制");
       //connect(treeAction, SIGNAL(triggered()), SLOT(trajectoryDuplicate()));
       ui.treeWidgetTrajectory->addAction(treeAction);
       /*********************end*************/

       treeAction = new QAction(ui.treeWidgetTrajectory);
       treeAction->setText("删除");
       connect(treeAction, SIGNAL(triggered()), SLOT(trajectoryRemove()));
       ui.treeWidgetTrajectory->addAction(treeAction);


       variantManagerCalibration = new QtVariantPropertyManager();
       topPropertyCalibration = variantManagerCalibration->addProperty(QtVariantPropertyManager::groupTypeId(), tr("工件校准属性"));

//       item = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QVariant::String, tr("校准分类号"));
//       topPropertyCalibration->addSubProperty(item);

//       item = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
//       topPropertyCalibration->addSubProperty(item);

//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位1"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位2"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位3"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位4"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位5"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位6"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位7"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位8"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("点位9"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTCMMF,tr("法兰盘位置"));
//       topPropertyCalibration->addSubProperty(item);
//       item = variantManagerCalibration->addProperty(QTPOINT4F,tr("法兰盘方向"));
//       topPropertyCalibration->addSubProperty(item);

//       topPropertyCalibration->setEnabled(false);
       QtVariantEditorFactory *variantFactoryCalibration = new QtVariantEditorFactory();
       ui.propertyCalibration->setFactoryForManager(variantManagerCalibration, variantFactoryCalibration);
       calibrationItemParent = ui.propertyCalibration->addProperty(topPropertyCalibration);
       itemPos = 0;
       calibrationNum = 0;

       ui.propertyCalibration->setPropertiesWithoutValueMarked(true);
       ui.propertyCalibration->setRootIsDecorated(false);



       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("新建工具校准");
       connect(treeAction, SIGNAL(triggered()), SLOT(calibrationNew()));
       ui.treeWidgetCalibration->addAction(treeAction);

       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("标定法兰盘");
       connect(treeAction, SIGNAL(triggered()), SLOT(calibrationFlange()));
       ui.treeWidgetCalibration->addAction(treeAction);

       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("生成工具rapid指令");
       connect(treeAction, SIGNAL(triggered()), SLOT(generateRapidToolData()));
       ui.treeWidgetCalibration->addAction(treeAction);

       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("重命名");
       connect(treeAction, SIGNAL(triggered()), SLOT(calibrationRename()));
       ui.treeWidgetCalibration->addAction(treeAction);

       /************update4 by cxs *******2014.4.27************/
       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("复制数据");
       connect(treeAction, SIGNAL(triggered()), SLOT(calibrationDuplicate()));
       ui.treeWidgetCalibration->addAction(treeAction);
       /*********************end*************/

       treeAction = new QAction(ui.treeWidgetCalibration);
       treeAction->setText("清除数据");
       connect(treeAction, SIGNAL(triggered()), SLOT(calibrationRemove()));
       ui.treeWidgetCalibration->addAction(treeAction);
   /**********************end****************************/
//emd
//    QtVariantPropertyManager *variantManagerAction = new QtVariantPropertyManager();
//    QtProperty *topItemAction = variantManagerAction->addProperty(QtVariantPropertyManager::groupTypeId(), tr("动作属性"));
//    QtVariantEditorFactory *variantFactoryAction = new QtVariantEditorFactory();
//    ui.propertyAction->setFactoryForManager(variantManagerAction, variantFactoryAction);
//    ui.propertyAction->addProperty(topItemAction);
//    ui.propertyAction->setPropertiesWithoutValueMarked(true);
//    ui.propertyAction->setRootIsDecorated(false);

    QtVariantEditorFactory *variantFactoryAction = new QtVariantEditorFactory();
        ui.propertyAction->setFactoryForManager(variantManagerAction, variantFactoryAction);
         actItemParent = ui.propertyAction->addProperty(topPropertyAction);
          itemPos = 0;

        ui.propertyAction->setPropertiesWithoutValueMarked(true);
       ui.propertyAction->setRootIsDecorated(false);




    treeAction = new QAction(ui.treeWidgetAction);
    treeAction->setText("新建动作");
    connect(treeAction, SIGNAL(triggered()), SLOT(actionNew()));
    ui.treeWidgetAction->addAction(treeAction);

    treeAction = new QAction(ui.treeWidgetAction);
    treeAction->setText("重命名");
    connect(treeAction, SIGNAL(triggered()), SLOT(actionRename()));
    ui.treeWidgetAction->addAction(treeAction);

    /************update4 by cxs *******2014.4.27************/
    treeAction = new QAction(ui.treeWidgetAction);
    treeAction->setText("复制");
    connect(treeAction, SIGNAL(triggered()), SLOT(actionDuplicate()));
    ui.treeWidgetAction->addAction(treeAction);
    /*********************end*************/

    treeAction = new QAction(ui.treeWidgetAction);
    treeAction->setText("删除");
    connect(treeAction, SIGNAL(triggered()), SLOT(actionRemove()));
    ui.treeWidgetAction->addAction(treeAction);

//    treeAction = new QAction(ui.treeWidgetAction);
//    treeAction->setText("属性");
////    connect(treeAction, SIGNAL(triggered()), SLOT(actionProperty()));
//    ui.treeWidgetAction->addAction(treeAction);



}

void PageStation::currentTabChanged(int index)
{
    //动作
    if(index == 3)
    {
        actionSelectionChanged();
    }
}

void PageStation::textChanged(const QString &/*text*/)
{
    ui.comboStationName->setItemText(ui.comboStationName->currentIndex(), ui.lineEdit_station->text());
    QSqlQuery query;
    if ( query.exec(QString("UPDATE Station SET name='%1' WHERE id='%2'").arg(ui.lineEdit_station->text(), currentStationId)) ) {
        qDebug("设置成功！");
    } else {
        qDebug("设置失败！", true);
        return;
    }

}

void PageStation::memoTextChanged()
{
    QSqlQuery query;
    if ( query.exec(QString("UPDATE Station SET memo='%1' WHERE id='%2'").arg(ui.plainTextEdit_memo->toPlainText(), currentStationId)) ) {
        qDebug("设置成功！");
    } else {
        qDebug("设置失败！", true);
        return;
    }

}

#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
void PageStation::stationListChanged(int index)
{

    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

    ui.comboStationName->setEnabled(false);
    ui.tabWidget->setEnabled(false);

    QString stationId = ui.comboStationName->itemData(index).toString();
    currentStationId = stationId;

    disconnect(ui.lineEdit_station, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    disconnect(ui.plainTextEdit_memo, SIGNAL(textChanged()), this, SLOT(memoTextChanged()));

    moduleSql = new QSqlQueryModel( this );
    moduleSql->setQuery( QString("SELECT id,name,memo FROM Station WHERE id='%1'").arg(stationId) );
    ui.lineEdit_station->setText(moduleSql->data(moduleSql->index(0,1)).toString());
    ui.plainTextEdit_memo->setPlainText(moduleSql->data(moduleSql->index(0,2)).toString());
    ui.lineEdit_station->setEnabled(true);
    ui.plainTextEdit_memo->setEnabled(true);

    connect(ui.lineEdit_station, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(ui.plainTextEdit_memo, SIGNAL(textChanged()), this, SLOT(memoTextChanged()));

    disconnect(ui.treeWidgetModule, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));


    /********************update7 by cxs ************************************/


           disconnect(ui.comboRobot,SIGNAL(currentIndexChanged(int)),this,SLOT(robotChanged(int)));
           disconnect(ui.comboRobot2,SIGNAL(currentIndexChanged(int)),this,SLOT(robot2Changed(int)));
           disconnect(ui.comboRobot3,SIGNAL(currentIndexChanged(int)),this,SLOT(robot3Changed(int)));
           ui.comboRobot->setCurrentIndex(0);
           ui.comboRobot2->setCurrentIndex(0);
           ui.comboRobot3->setCurrentIndex(0);
           robotStationId = "";
           robot2StationId ="";
           robot3StationId = "";
           //update zhugz
           ui.cur_robot->clear();
           //end
           QSqlQuery query;

           query.exec(QString("SELECT libraryId,name,type,id,displayOrder FROM StationMotion WHERE stationId ='%1' ORDER BY displayOrder ASC").arg(currentStationId));
           //static int countRobot = 0;
           while(query.next())
           {
               //countRobot++;
               RobotObject *robotObject = new RobotObject(this);
               robotObject->id = query.value(0).toString();
               robotObject->name = query.value(1).toString();
               robotObject->type = query.value(2).toInt();
               robotObject->displayOrder = query.value(4).toInt();
               //updated by zhugz
               if(robotMap[robotObject->id]==NULL)
               {
                   emit addLog("机器人添加失败","");
                   emit addLog("请导入该类型机器人"+robotObject->id,"");
                   continue;
               }

               int robotIndex = robotMap[robotObject->id]->comboIndex;
               if(!robotIndex)
                   continue;
                //update zhugz
               ui.cur_robot->addItem(robotObject->name,robotObject->displayOrder);

               //end
               switch( robotObject->displayOrder)
               {
               case 0:
               case 1:
                   ui.comboRobot->setCurrentIndex(robotIndex);
                   robotStationId = query.value(3).toString();
                   break;
               case 2:
                   ui.comboRobot2->setCurrentIndex(robotIndex);
                   robot2StationId = query.value(3).toString();
                   break;
               case 3:
                   ui.comboRobot3->setCurrentIndex(robotIndex);
                   robot3StationId = query.value(3).toString();
                   break;
               default:
                   break;
               }
   //            if(countRobot>=3){

   //                break;
   //            }

           }
           //countRobot =0;
           connect(ui.comboRobot,SIGNAL(currentIndexChanged(int)),this,SLOT(robotChanged(int)));
           connect(ui.comboRobot2,SIGNAL(currentIndexChanged(int)),this,SLOT(robot2Changed(int)));
           connect(ui.comboRobot3,SIGNAL(currentIndexChanged(int)),this,SLOT(robot3Changed(int)));


    /**************end****************************************************/



//    foreach (AxisObject *axis, axisMap) {
//        axis->deleteLater();
//    }
    axisMap.clear();
    foreach (ModuleObject *module, moduleMap) {
        if (!module) {
            continue;
        }
        try{
            module->aisShape->GetContext()->Clear(module->aisShape, false);
            module->aisShape->Delete();
        }catch(...){}
//        delete module->aisShape;
//        module->deleteLater();
    }
    moduleMap.clear();

    ui.treeWidgetAxis->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(ui.treeWidgetAxis);
    root->setText(0, "轴列表");
    root->setText(1, "");
    root->setExpanded(true);

//    moduleSql = new QSqlQueryModel( this );
//    moduleSql->setQuery( QString("SELECT id,name,type,parentId FROM StationAxis WHERE stationId='%1' AND isUse=1 ORDER BY name ASC" ).arg(currentStationId) );
//    for (int i=0; i<moduleSql->rowCount(); i++) {
//        QTreeWidgetItem *item = new QTreeWidgetItem();
//        item->setText(0, moduleSql->data(moduleSql->index(i,1)).toString());
//        item->setText(1, moduleSql->data(moduleSql->index(i,0)).toString());
//        item->setFlags(item->flags()|Qt::ItemIsEditable);
//        item->setExpanded(true);
//        root->addChild(item);
//    }

    axisNameList.clear();
    axisNameList << tr("无");

//    QSqlQuery query;
    QString axisId;
    query.exec(QString("SELECT * FROM StationAxis WHERE stationId='%1' AND isUse=1 ORDER BY name ASC").arg(currentStationId));
    while(query.next()) {
        QSqlRecord record = query.record();

        axisId = record.value("id").toString();
        AxisObject *axis = axisMap[axisId];
        if (!axis){
            axis = new AxisObject();
            //axisMap.insert(axisId, axis);
            axisMap[axisId] = axis;
        }
        //axis->cur_robotindex=record.value("cur_robotindex").toInt();
        axis->id = axisId;
        axis->sign = record.value("sign").toString();
        axis->name = record.value("name").toString();
        axis->parentId = record.value("parentId").toString();
        axis->type = record.value("type").toInt();
        axis->start[X] = record.value("startX").toDouble();
        axis->start[Y] = record.value("startY").toDouble();
        axis->start[Z] = record.value("startZ").toDouble();
        axis->end[X] = record.value("endX").toDouble();
        axis->end[Y] = record.value("endY").toDouble();
        axis->end[Z] = record.value("endZ").toDouble();
        axis->symbol = record.value("symbol").toInt();

        if (axis->symbol == 0) axis->symbol = 1;
        axis->minValue = record.value("minValue").toDouble();
        axis->maxValue = record.value("maxValue").toDouble();

        double length = axis->symbol * sqrt((axis->end[X]-axis->start[X])*(axis->end[X]-axis->start[X]) + (axis->end[Y]-axis->start[Y])*(axis->end[Y]-axis->start[Y]) + (axis->end[Z]-axis->start[Z])*(axis->end[Z]-axis->start[Z]));
        axis->stepValue[X] = (axis->end[X]-axis->start[X]) / length;
        axis->stepValue[Y] = (axis->end[Y]-axis->start[Y]) / length;
        axis->stepValue[Z] = (axis->end[Z]-axis->start[Z]) / length;

        axis->originOffset = record.value("originOffset").toDouble();

        if (!axis->parentId.isEmpty()) {
            AxisObject *parentAxis = axisMap[axis->parentId];
            if (!parentAxis) {
                parentAxis = new AxisObject();
                axisMap[axis->parentId] = parentAxis;
            }
            parentAxis->subAxisList.append(axis->id);
        }


//        if (axis->type == 1 && !axis->sign.isEmpty()) {
            axis->value = 0.0;
////            axisSignMap[axis->sign] = axis->id;
//         //   emit updateAxisValue(axis->sign, axis->value);
//        } else {
////            axis->value = axis->originOffset;
//        }

        axisMap[axisId] = axis;

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, axis->name);
        item->setText(1, axisId);
        item->setFlags(item->flags()|Qt::ItemIsEditable);
        item->setExpanded(true);
        root->addChild(item);

        axisNameList << axis->name;
    }

    connect(ui.treeWidgetAxis, SIGNAL(itemSelectionChanged()), this, SLOT(axisSelectionChanged()));
    connect(ui.treeWidgetAxis, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));


    ui.treeWidgetModule->clear();
    root = new QTreeWidgetItem(ui.treeWidgetModule);
    root->setText(0, "组件列表");
    root->setText(1, "");
    root->setExpanded(true);

//    moduleSql = new QSqlQueryModel( this );
//    moduleSql->setQuery( QString("SELECT id,name,type,parentId FROM StationModule WHERE stationId='%1' AND isUse=1 ORDER BY name ASC" ).arg(currentStationId) );
//    for (int i=0; i<moduleSql->rowCount(); i++) {
//        QTreeWidgetItem *item = new QTreeWidgetItem();
//        item->setText(0, moduleSql->data(moduleSql->index(i,1)).toString());
//        item->setText(1, moduleSql->data(moduleSql->index(i,0)).toString());
//        item->setFlags(item->flags()|Qt::ItemIsEditable);
//        item->setExpanded(true);
//        root->addChild(item);
//    }

    query.exec(QString("SELECT * FROM StationModule WHERE stationId='%1' AND isUse=1 ORDER BY name ASC").arg(stationId));
    while(query.next()) {
        QSqlRecord record = query.record();
        ModuleObject *module = new ModuleObject();
        module->id = record.value("id").toString();
//      module->code = record.value("code").toString();
        module->name = record.value("name").toString();
        module->type = record.value("type").toInt();
        module->filename = record.value("filename").toString();
        module->bindAxis = record.value("bindAxis").toString();
        module->pos[X] = record.value("x").toDouble();
        module->pos[Y] = record.value("y").toDouble();
        module->pos[Z] = record.value("z").toDouble();

        module->rotateDirection[X] = record.value("rotateX").toDouble();
        module->rotateDirection[Y] = record.value("rotateY").toDouble();
        module->rotateDirection[Z] = record.value("rotateZ").toDouble();
        module->rotateAngle = record.value("initAngle").toDouble();
        module->material = record.value("material").toInt();
        module->color = record.value("color").toInt();
        module->transparency = record.value("transparent").toDouble();
//        qDebug(QString::number(module->transparency).toLocal8Bit());
        module->isHidden = (record.value("isShow").toInt() == 1);
//rong lk
        module->yaw = record.value("yaw").toDouble();
        module->pitch = record.value("pitch").toDouble();
        module->roll = record.value("roll").toDouble();
        //qDebug()<<module->name<<module->pos[X]<<module->pos[Y]<<module->pos[Z]<<module->rotateAngle<<module->rotateDirection[X]<<module->rotateDirection[Y]<<module->rotateDirection[Z]<<module->yaw<<module->pitch<<module->roll;
//end
        if (!module->bindAxis.isEmpty()) {
            if (axisMap.contains(module->bindAxis)) {
                axisMap[module->bindAxis]->moduleList.append(module->id);
            }
        }

        moduleMap[module->id] = module;

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, module->name);
        item->setText(1, module->id);
        item->setFlags(item->flags()|Qt::ItemIsEditable);
        item->setExpanded(true);
        root->addChild(item);
    }

    connect(ui.treeWidgetModule, SIGNAL(itemSelectionChanged()), this, SLOT(moduleSelectionChanged()));
    connect(ui.treeWidgetModule, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));



    /************************************
     *
     *add by cxs 2014 4.15
     *
     ************************************/

        disconnect(ui.treeWidgetAction, SIGNAL(itemSelectionChanged()), this, SLOT(actionSelectionChanged()));
        disconnect(ui.treeWidgetAction, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));
        actionMap.clear();
        actionDetailMap.clear();

        ui.treeWidgetAction->clear();
        root = new QTreeWidgetItem(ui.treeWidgetAction);
        root->setText(0, "动作列表");
        root->setText(1, "");
        root->setExpanded(true);

        /*************************update6 by cxs *******************************/
//        moduleSql = new QSqlQueryModel( this );
//        moduleSql->setQuery( QString("SELECT * FROM StationAction WHERE stationId='%1' AND isUse=1 ORDER BY name ASC" ).arg(currentStationId) );
//        while(moduleSql->canFetchMore())
//        {
//            moduleSql->fetchMore();
//        }
        query.exec(QString("SELECT * FROM StationAction WHERE stationId='%1' AND isUse=1 ORDER BY name ASC" ).arg(currentStationId));
        while(query.next()) {
            QSqlRecord record = query.record();
//        for (int i=0; i<moduleSql->rowCount(); i++) {
            ActionObject *actionObject = new ActionObject();
            actionObject->id = record.value("id").toString();
            actionObject->stationId = record.value("stationId").toString();
            actionObject->name = record.value("name").toString();
            actionObject->type = record.value("type").toInt();
            actionObject->code =  record.value("code").toString();
            actionObject->oper = record.value("operator").toString();
            actionObject->value = record.value("value").toString();
            actionObject->extTrg = record.value("extraTrigger").toInt();
            actionObject->code2 = record.value("code2").toString();
            actionObject->oper2 = record.value("operator2").toString();
            actionObject->value2 = record.value("value2").toString();
            actionObject->extTrg2 = record.value("extraTrigger2").toInt();
            actionObject->code3 =  record.value("code3").toString();
            actionObject->oper3 =  record.value("operator3").toString();
            actionObject->value3 = record.value("value3").toString();
            actionObject->count = record.value("count").toInt();
            actionObject->updateType = record.value("updateType").toInt();
            actionMap.insert(actionObject->id,actionObject);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, actionObject->name);
            item->setText(1, actionObject->id);
            item->setFlags(item->flags()|Qt::ItemIsEditable);
            item->setExpanded(true);
            root->addChild(item);
            QSqlQuery queryChild;
            queryChild.exec(QString("SELECT id,actionId,name,type,moduleId,value FROM StationActionDetail WHERE actionId= '%1' AND isUse=1 ORDER BY name ASC").arg(actionObject->id));
            while(queryChild.next())
            {
                QSqlRecord recordChild = queryChild.record();
                ActionDetail  *actionDetail  = new ActionDetail();
                actionDetail->id = recordChild.value("id").toString();
                actionDetail->actionId = recordChild.value("actionId").toString();
                actionDetail->name = recordChild.value("name").toString();
                actionDetail->type = recordChild.value("type").toInt();
                actionDetail->moduleId = recordChild.value("moduleId").toString();
                actionDetail->value = recordChild.value("value").toString();
                actionDetailMap.insert(actionDetail->id,actionDetail);
                QTreeWidgetItem *itemChild = new QTreeWidgetItem();
                itemChild->setText(0,actionDetail->name);
                itemChild->setText(1,actionDetail->id);
                itemChild->setFlags(item->flags()|Qt::ItemIsEditable);
                itemChild->setExpanded(true);
                item->addChild(itemChild);
            }

        }
        /******************end************************************/

        connect(ui.treeWidgetAction, SIGNAL(itemSelectionChanged()), this, SLOT(actionSelectionChanged()));
        connect(ui.treeWidgetAction, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));
//rong lk
        /******************by ronglk************************************/
        disconnect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
        disconnect(ui.treeWidgetTrajectory, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));
        trajectoryMap.clear();
        trajectoryDetailMap.clear();

        ui.treeWidgetTrajectory->clear();
        root = new QTreeWidgetItem(ui.treeWidgetTrajectory);
        root->setText(0, "轨迹列表");
        root->setText(1, "");
        root->setExpanded(true);
        query.exec(QString("SELECT * FROM TrajectoryCategory WHERE stationId='%1'ORDER BY name ASC" ).arg(currentStationId));
        while(query.next()) {
            QSqlRecord record = query.record();
            TrajectoryObject *trajectoryObject = new TrajectoryObject();

            trajectoryObject->id = record.value("id").toString();
            trajectoryObject->stationId = record.value("stationId").toString();
            trajectoryObject->trajectoryNum = record.value("trajectoryNum").toInt();
            trajectoryObject->name = record.value("name").toString();

            trajectoryMap.insert(trajectoryObject->id,trajectoryObject);

            QTreeWidgetItem *item = new QTreeWidgetItem();
            //char trajectoryNumName[128];
            //sprintf_s(trajectoryNumName, "轨迹%d", trajectoryObject->trajectoryNum);
            //trajectoryObject->name = QString::fromLocal8Bit(trajectoryNumName,128);

            item->setText(0, trajectoryObject->name);
            item->setText(1, trajectoryObject->id);
            item->setFlags(item->flags()|Qt::ItemIsEditable);
            item->setExpanded(true);
            root->addChild(item);
            QSqlQuery queryChild;
            queryChild.exec(QString("SELECT * FROM TrajectoryDetail WHERE trajectoryId = '%1' ORDER BY name ASC").arg(trajectoryObject->id));
            while(queryChild.next())
            {
                QSqlRecord recordChild = queryChild.record();

                TrajectoryDetail *trajectoryDetail = new TrajectoryDetail();

                trajectoryDetail->id = recordChild.value("id").toString();
                trajectoryDetail->trajectoryId = recordChild.value("trajectoryId").toString();
                trajectoryDetail->name = recordChild.value("name").toString();
                trajectoryDetail->pathType = recordChild.value("pathType").toInt();
                //trajectoryObject->trajectoryNum = record.value("trajectoryNum").toInt();
                trajectoryDetail->pathFile = recordChild.value("pathFile").toString();

                trajectoryDetail->pointIndensity = recordChild.value("pointIndensity").toDouble();
                trajectoryDetail->FalaX = recordChild.value("FalaX").toDouble();
                trajectoryDetail->FalaY = recordChild.value("FalaY").toDouble();
                trajectoryDetail->FalaZ = recordChild.value("FalaZ").toDouble();

                trajectoryDetail->cutRadius = recordChild.value("cutRadius").toDouble();
                trajectoryDetail->knifeX = recordChild.value("knifeX").toDouble();
                trajectoryDetail->knifeY = recordChild.value("knifeY").toDouble();
                trajectoryDetail->knifeZ = recordChild.value("knifeZ").toDouble();

                trajectoryDetail->longBase = recordChild.value("longBase").toDouble();
                trajectoryDetail->height = recordChild.value("height").toDouble();
                trajectoryDetail->shortBase = recordChild.value("shortBase").toDouble();

                trajectoryDetail->cutDepth = recordChild.value("cutDepth").toDouble();
                trajectoryDetail->others1 = recordChild.value("others1").toDouble();
                trajectoryDetail->others2 = recordChild.value("others2").toDouble();
                trajectoryDetail->others3 = recordChild.value("others3").toDouble();

                trajectoryDetail->FalaRX = recordChild.value("FalaRX").toDouble();
                trajectoryDetail->FalaRY = recordChild.value("FalaRY").toDouble();
                trajectoryDetail->FalaRZ = recordChild.value("FalaRZ").toDouble();

                trajectoryDetail->knifeDirectionX = recordChild.value("knifeDirectionX").toDouble();
                trajectoryDetail->knifeDirectionY = recordChild.value("knifeDirectionY").toDouble();
                trajectoryDetail->knifeDirectionZ = recordChild.value("knifeDirectionZ").toDouble();

                trajectoryDetail->precision1 = recordChild.value("precision1").toDouble();
                trajectoryDetail->precision2 = recordChild.value("precision2").toDouble();
                trajectoryDetail->cutAngle = recordChild.value("cutAngle").toDouble();
                trajectoryDetail->in1 = recordChild.value("in1").toInt();
                trajectoryDetail->out1 = recordChild.value("out1").toInt();

                trajectoryDetailMap.insert(trajectoryDetail->id,trajectoryDetail);
                QTreeWidgetItem *itemChild = new QTreeWidgetItem();
                itemChild->setText(0,trajectoryDetail->name);
                itemChild->setText(1,trajectoryDetail->id);
                itemChild->setFlags(item->flags()|Qt::ItemIsEditable);
                itemChild->setExpanded(true);
                item->addChild(itemChild);
            }

        }
        connect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
        connect(ui.treeWidgetTrajectory, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));


        disconnect(ui.treeWidgetCalibration, SIGNAL(itemSelectionChanged()), this, SLOT(calibrationSelectionChanged()));
        disconnect(ui.treeWidgetCalibration, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));
        calibrationMap.clear();
        calibrationDetailMap.clear();

        ui.treeWidgetCalibration->clear();
        root = new QTreeWidgetItem(ui.treeWidgetCalibration);

        qDebug()<<currentStationId;
        query.exec(QString("SELECT * FROM CalibrationCategory WHERE stationId='%1'ORDER BY name ASC" ).arg(currentStationId));
        root->setText(0, "工件校准列表");
        root->setText(1, "");
        root->setExpanded(true);
        while(query.next()) {
                    QSqlRecord record = query.record();
                    CalibrationObject *calibrationObject = new CalibrationObject();

                    calibrationObject->id = record.value("id").toString();
                    calibrationObject->stationId = record.value("stationId").toString();
                    calibrationObject->calibrationNum = record.value("calibrationNum").toInt();
                    calibrationObject->name = record.value("name").toString();

                    calibrationMap.insert(calibrationObject->id,calibrationObject);

                    QTreeWidgetItem *item = new QTreeWidgetItem();
                    //char trajectoryNumName[128];
                    //sprintf_s(trajectoryNumName, "轨迹%d", trajectoryObject->trajectoryNum);
                    //trajectoryObject->name = QString::fromLocal8Bit(trajectoryNumName,128);

                    item->setText(0, calibrationObject->name);
                    item->setText(1, calibrationObject->id);
                    qDebug()<<calibrationObject->name<<calibrationObject->id;
                    item->setFlags(item->flags()|Qt::ItemIsEditable);
                    item->setExpanded(true);
                    root->addChild(item);
                    QSqlQuery queryChild;
                    queryChild.exec(QString("SELECT * FROM CalibrationDetail WHERE calibrationId = '%1' ORDER BY name ASC").arg(calibrationObject->id));
                    while(queryChild.next()) {

                        QSqlRecord recordChild = queryChild.record();

                        CalibrationDetail *calibrationDetail = new CalibrationDetail();

                        calibrationDetail->id = recordChild.value("id").toString();
                        calibrationDetail->calibrationId = recordChild.value("calibrationId").toString();

//                        QSqlRecord record = query.record();

//        calibrationId = record.value("id").toString();
//        CalibrationObject *workCalbration = calibrationMap[calibrationId];
//                        if (!workCalibration){
//                            workCalibration = new CalibrationObject();
//                            //axisMap.insert(axisId, axis);
//                            calibrationMap[calibrationId] = workCalibration;
//                        }
                        //axis->cur_robotindex=record.value("cur_robotindex").toInt();

                        calibrationDetail->name = recordChild.value("name").toString();
                        calibrationDetail->point[0] = recordChild.value("point0").toString();
                        calibrationDetail->point[1] = recordChild.value("point1").toString();
                        calibrationDetail->point[2] = recordChild.value("point2").toString();
                        calibrationDetail->point[3] = recordChild.value("point3").toString();
                        calibrationDetail->point[4] = recordChild.value("point4").toString();
                        calibrationDetail->point[5] = recordChild.value("point5").toString();
                        calibrationDetail->point[6] = recordChild.value("point6").toString();
                        calibrationDetail->point[7] = recordChild.value("point7").toString();
                        calibrationDetail->point[8] = recordChild.value("point8").toString();

                        for(int i = 0; i < 9; i++)
                        {
                            QStringList tmpList = calibrationDetail->point[i].split("|");
                            if (tmpList.length()>0) {
                                double pointX = tmpList[0].toDouble();
                                calibrationDetail->pointX[i] = pointX;
                            }
                            if (tmpList.length()>1) {
                                double pointY = tmpList[1].toDouble();
                                calibrationDetail->pointY[i] = pointY;
                            }
                            if (tmpList.length()>2) {
                                double pointZ = tmpList[2].toDouble();
                                calibrationDetail->pointZ[i] = pointZ;
                            }
                        }


                        calibrationDetail->falaX = recordChild.value("falaX").toDouble();
                        calibrationDetail->falaY = recordChild.value("falaY").toDouble();
                        calibrationDetail->falaZ = recordChild.value("falaZ").toDouble();
                        calibrationDetail->falaQ1 = recordChild.value("falaQ1").toDouble();
                        calibrationDetail->falaQ2 = recordChild.value("falaQ2").toDouble();
                        calibrationDetail->falaQ3 = recordChild.value("falaQ3").toDouble();
                        calibrationDetail->falaQ4 = recordChild.value("falaQ4").toDouble();
                        calibrationDetail->toolPosX = recordChild.value("toolPosX").toDouble();
                        calibrationDetail->toolPosY = recordChild.value("toolPosY").toDouble();
                        calibrationDetail->toolPosZ = recordChild.value("toolPosZ").toDouble();

                        calibrationDetail->toolQ1 = recordChild.value("toolQ1").toDouble();
                        calibrationDetail->toolQ2 = recordChild.value("toolQ2").toDouble();
                        calibrationDetail->toolQ3 = recordChild.value("toolQ3").toDouble();
                        calibrationDetail->toolQ4 = recordChild.value("toolQ4").toDouble();
                        calibrationDetail->toolKg = recordChild.value("toolKg").toDouble();
                        calibrationDetail->toolMassX = recordChild.value("toolMassX").toDouble();
                        calibrationDetail->toolMassY = recordChild.value("toolMassY").toDouble();
                        calibrationDetail->toolMassZ = recordChild.value("toolMassZ").toDouble();

                        calibrationDetail->sensorPosX = recordChild.value("sensorPosX").toDouble();
                        calibrationDetail->sensorPosY = recordChild.value("sensorPosY").toDouble();
                        calibrationDetail->sensorPosZ = recordChild.value("sensorPosZ").toDouble();
                        calibrationDetail->sensorDirectionX = recordChild.value("sensorDirectionX").toDouble();
                        calibrationDetail->sensorDirectionY = recordChild.value("sensorDirectionY").toDouble();
                        calibrationDetail->sensorDirectionZ = recordChild.value("sensorDirectionZ").toDouble();
                        calibrationDetail->sensorReads = recordChild.value("sensorReads").toDouble();
                        calibrationDetail->sensorDistance = recordChild.value("sensorDistance").toDouble();

                        calibrationDetail->pnt_index = -1;

                        calibrationDetailMap.insert(calibrationDetail->id, calibrationDetail);
                        QTreeWidgetItem *itemChild1 = new QTreeWidgetItem();
                        itemChild1->setText(0, "校准点位设置");
                        itemChild1->setText(1, calibrationDetail->id);
                        itemChild1->setFlags(item->flags()|Qt::ItemIsEditable);
                        itemChild1->setExpanded(true);
                        item->addChild(itemChild1);

                        QTreeWidgetItem *itemChild2 = new QTreeWidgetItem();
                        itemChild2->setText(0, "工具设定");
                        itemChild2->setText(1, calibrationDetail->id);
                        itemChild2->setFlags(itemChild2->flags()|Qt::ItemIsEditable);
                        itemChild2->setExpanded(true);
                        item->addChild(itemChild2);

                        QTreeWidgetItem *itemChild3 = new QTreeWidgetItem();
                        itemChild3->setText(0, "传感器设定");
                        itemChild3->setText(1, calibrationDetail->id);
                        itemChild3->setFlags(itemChild3->flags()|Qt::ItemIsEditable);
                        itemChild3->setExpanded(true);
                        item->addChild(itemChild3);
                    }

                }

        connect(ui.treeWidgetCalibration, SIGNAL(itemSelectionChanged()), this, SLOT(calibrationSelectionChanged()));
        connect(ui.treeWidgetCalibration, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(moduleItemChanged(QTreeWidgetItem*,int)));



                /******************end************************************/
//end
    SysUtils::setSysParam( "lastStationId", currentStationId );
//    SysUtils::setSysParam( "lastStationName", ui.comboStationName->currentText() );

    qApp->restoreOverrideCursor();

    emit stationChanged(currentStationId);

}

void PageStation::on_pushButton_add_clicked()
{
    currentStationId = SysUtils::uuid();
    ui.comboStationName->addItem("新方案", currentStationId);
    ui.comboStationName->setCurrentIndex(ui.comboStationName->count() - 1);

    ui.lineEdit_station->setEnabled(true);
//    ui.lineEdit_station->clear();
    ui.lineEdit_station->setText("新方案");
    ui.plainTextEdit_memo->setEnabled(true);
    ui.plainTextEdit_memo->clear();


    QSqlQuery query;
    if ( query.exec(QString("INSERT INTO Station(id,name,type) VALUES ('%1','新方案',1)").arg(currentStationId)) ) {
        qDebug("增加成功！");
    } else {
        qDebug("增加失败！", true);
        return;
    }

    ui.lineEdit_station->setFocus();
}

void PageStation::on_pushButton_del_clicked()
{
    if(0==QMessageBox::information(this,tr("系统提示"),tr("确认要删除当前选中的方案吗？"), tr("是"), tr("否"))) {
        QSqlQuery query,query2;
        if ( query.exec(QString("DELETE FROM Station WHERE id='%1'").arg(currentStationId)) ) {
            qDebug("删除成功！");
        } else {
            qDebug("删除失败！", true);
            return;
        }
        //updated by zhugz
        query.exec(QString("select name , filename from StationModule where  stationId='%1'").arg(currentStationId));
        while(query.next())
        {
            QString name=query.value(0).toString();
            QString fileName=query.value(1).toString();
            if(name.left(3)!="机器人")
            {
                query2.exec(QString("select id from StationModule where name='%1' and  stationId<>'%2'").arg(name).arg(currentStationId));
                if(!query2.next())
                {
                    QFile::remove("D:\\DzicsRobot\\" + fileName+ ".dzm");

                }


            }


        }

        query.exec(QString("delete from StationModule where stationId='%1'").arg(currentStationId));
        query.exec(QString("delete from StationMotion where stationId='%1'").arg(currentStationId));

        query.exec(QString("select id from StationAction where stationId='%1'").arg(currentStationId));
        while(query.next())
        {
            QString id=query.value(0).toString();
            query2.exec(QString("delete from StationAction where  ationId='%1'").arg(id));
        }
        query.exec(QString("delete from StationAction where stationId='%1'").arg(currentStationId));


        query.exec(QString("select id from CameraProperty where solutionId='%1'").arg(currentStationId));
        while(query.next())
        {
            QString id=query.value(0).toString();
            query2.exec(QString("delete from CameraTemplate where  cameraId='%1'").arg(id));
        }
        query.exec(QString("delete from CameraProperty where solutionId='%1'").arg(currentStationId));
        query.exec(QString("delete from safeArea where solutionId='%1'").arg(currentStationId));


        //end
        currentStationId = "";
        ui.comboStationName->removeItem(ui.comboStationName->currentIndex());

    }
}

void PageStation::moduleNewGroup()
{
    if (currentStationId.isEmpty())
        return;

    QTreeWidgetItem *cItem = ui.treeWidgetModule->currentItem();
    if (!cItem)
        return;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, "新分组");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    ui.treeWidgetModule->setCurrentItem(item);
    ui.treeWidgetModule->editItem(item);

    QSqlQuery query;
    if ( query.exec(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,isUse) VALUES ('%1','%2','%3',3,'%4',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1))) ) {
        qDebug("增加成功！");
    } else {
        qDebug("增加失败！", true);
        return;
    }

    qDebug(item->text(1).toLocal8Bit().constData());


}

void PageStation::moduleNewMotion()
{
    if (currentStationId.isEmpty())
        return;

    QTreeWidgetItem *cItem = ui.treeWidgetModule->currentItem();
    if (!cItem)
        return;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, "新运动机构");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    ui.treeWidgetModule->setCurrentItem(item);
    ui.treeWidgetModule->editItem(item);

    QSqlQuery query;
    if ( query.exec(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,isUse) VALUES ('%1','%2','%3',2,'%4',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1))) ) {
        qDebug("增加成功！");
    } else {
        qDebug("增加失败！", true);
        return;
    }

    qDebug(item->text(1).toLocal8Bit().constData());

}


void PageStation::moduleImport()
{
//    static QString lastSelectedPath = "";
    QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr(" 请选择需要新增的三维组件"),
                                lastSelectedPath,
                                tr("所有支持的组件 (*.stp *.igs *.stl *.dzm *.step *.iges);;STEP文件 (*.stp *.step);;IGES文件 (*.igs *.iges);;STL文件 (*.stl);;大正组件 (*.dzm);;所有文件 (*.*)"));

    if (fileNameList.isEmpty()) return;

    QFileInfo fileInfo(fileNameList.at(0));
    lastSelectedPath = fileInfo.absolutePath();

    QTreeWidgetItem *cItem = ui.treeWidgetModule->currentItem();
    if (!cItem)
        return;

    QString sql;
    QStringList ids,stationIds,names,filenames,parentIds;
    foreach (QString filename, fileNameList) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFileInfo fileInfo(filename);
        int sp = fileInfo.fileName().lastIndexOf('.');
        item->setText(0, fileInfo.fileName().left(sp));
        item->setText(1, SysUtils::uuid());
        item->setFlags(item->flags()|Qt::ItemIsEditable);
        qDebug(item->text(1).toLocal8Bit().constData());
        cItem->addChild(item);

//        QSqlQuery query;
//        qDebug(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,filename,isUse) VALUES ('%1','%2','%3',1,'%4','%5',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1), "Module\\" + item->text(1)).toLocal8Bit()) ;
//        if ( query.exec(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,filename,isUse) VALUES ('%1','%2','%3',1,'%4','%5',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1), "Module\\" + item->text(1))) ) {
//            qDebug("增加成功A！");
//        } else {
//            qDebug("增加失败A！", true);
//            return;
//        }

        ids << item->text(1);
        stationIds << currentStationId;
        names << item->text(0);
        filenames << "Module\\" + item->text(1);
        parentIds << cItem->text(1);

        emit importModuleFile(fileInfo.filePath(), item->text(1), item->text(0));

    }

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.prepare("INSERT INTO StationModule(id,stationId,name,type,filename,parentId,material,color,rotateZ,isUse) VALUES (?,?,?,1,?,?,14,0,1,1)");
    query.addBindValue(ids);
    query.addBindValue(stationIds);
    query.addBindValue(names);
    query.addBindValue(filenames);
    query.addBindValue(parentIds);
    if ( query.execBatch() ) {
        QSqlDatabase::database().commit();
        qDebug("增加成功！");
    } else {
        QSqlDatabase::database().rollback();
        qDebug("增加失败！", true);
        return;
    }

    cItem->setExpanded(true);

}

void PageStation::moduleReplace()
{
    QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr(" 请选择需要替换的三维组件"),
                                lastSelectedPath,
                                tr("所有支持的组件 (*.stp *.igs *.stl *.dzm *.step *.iges);;STEP文件 (*.stp *.step);;IGES文件 (*.igs *.iges);;STL文件 (*.stl);;大正组件 (*.dzm);;所有文件 (*.*)"));
    if (fileNameList.isEmpty()) return;

    QFileInfo fileInfo(fileNameList.at(0));
    lastSelectedPath = fileInfo.absolutePath();

    QTreeWidgetItem *cItem = ui.treeWidgetModule->currentItem();
    if (!cItem)
        return;

//    QString sql;
//    QStringList ids,stationIds,names,parentIds;
    foreach (QString filename, fileNameList) {
//        QTreeWidgetItem *item = new QTreeWidgetItem();
        QFileInfo fileInfo(filename);
//        item->setText(0, fileInfo.fileName().split('.').at(0));
//        item->setText(1, SysUtils::uuid());
//        item->setFlags(item->flags()|Qt::ItemIsEditable);
//        qDebug(item->text(1).toLocal8Bit().constData());
//        cItem->addChild(item);

//        QSqlQuery query;
//        if ( query.exec(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,isUse) VALUES ('%1','%2','%3',1,'%4',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1))) ) {
//            qDebug("增加成功！");
//        } else {
//            qDebug("增加失败！", true);
//            return;
//        }

//        ids << item->text(1);
//        stationIds << currentStationId;
//        names << item->text(0);
//        parentIds << cItem->text(1);

//        sql += QString("INSERT INTO StationModule(id,stationId,name,type,parentId,isUse) VALUES ('%1','%2','%3',1,'%4',1);").arg(item->text(1), currentStationId, item->text(0), cItem->text(1));

        emit importModuleFile(fileInfo.filePath(), cItem->text(1), cItem->text(0));

//        ModuleLoadThread *loadThread = new ModuleLoadThread(filename, module->aisShape, module->transform, module->isShow);
//        connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this->parentWidget(), SLOT(moduleLoadFinish(QString)));
//        loadThread->start();

//        TopoDS_Shape shape;
//        shape = MeshTranslate::loadSTEP(filename);
//        Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
//        sps->Append( shape );
//        MeshTranslate::exportModule(MeshTranslate::FormatCSFDB, "resource/" + fileInfo.fileName().split('.').at(0) + ".dzm", sps);

    }
//    QSqlQuery query;
//    query.prepare("INSERT INTO StationModule(id,stationId,name,type,parentId,isUse) VALUES (?,?,?,1,?,1)");
//    query.addBindValue(ids);
//    query.addBindValue(stationIds);
//    query.addBindValue(names);
//    query.addBindValue(parentIds);
//    if ( query.execBatch() ) {
//        qDebug("增加成功！");
//    } else {
//        qDebug("增加失败！", true);
//        return;
//    }
//    cItem->setExpanded(true);

}

void PageStation::moduleRename()
{
    ui.treeWidgetModule->edit(ui.treeWidgetModule->currentIndex());
}

void PageStation::moduleDuplicate()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetModule->selectedItems();
    if (items.isEmpty())
        return;\
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    foreach (QTreeWidgetItem *item, items) {
        QString id = item->text(1);
        if (!id.isEmpty()) {
            if (moduleMap[id]) {
//                moduleMap[id]->aisShape->GetContext()->Clear(moduleMap[id]->aisShape, true);
//                moduleMap[id]->aisShape->Delete();
//                moduleMap.remove(id);

                QString newId = SysUtils::uuid();
                moduleMap[newId] = new ModuleObject();
                moduleMap[newId]->id = newId;
                moduleMap[newId]->name = moduleMap[id]->name + "_2";

                moduleMap[newId]->type=moduleMap[id]->type;
                moduleMap[newId]->pos[0]=moduleMap[id]->pos[0];
                moduleMap[newId]->pos[1]=moduleMap[id]->pos[1];
                moduleMap[newId]->pos[2]=moduleMap[id]->pos[2];
                moduleMap[newId]->rotatePoint[0]=moduleMap[id]->rotatePoint[0];
                moduleMap[newId]->rotatePoint[1]=moduleMap[id]->rotatePoint[1];
                moduleMap[newId]->rotatePoint[2]=moduleMap[id]->rotatePoint[2];
                moduleMap[newId]->rotateDirection[0]=moduleMap[id]->rotateDirection[0];
                moduleMap[newId]->rotateDirection[1]=moduleMap[id]->rotateDirection[0];
                moduleMap[newId]->rotateDirection[2]=moduleMap[id]->rotateDirection[0];
                moduleMap[newId]->rotateAngle=moduleMap[id]->rotateAngle;
                moduleMap[newId]->transparency=moduleMap[id]->transparency;
                moduleMap[newId]->material=moduleMap[id]->material;
                moduleMap[newId]->color=moduleMap[id]->color;
                moduleMap[newId]->isHidden=moduleMap[id]->isHidden;
                moduleMap[newId]->isShapeLoaded=moduleMap[id]->isShapeLoaded;

                moduleMap[newId]->aisShape = new AIS_Shape(moduleMap[id]->aisShape->Shape());
                moduleMap[id]->aisShape->GetContext()->Display(moduleMap[newId]->aisShape, Standard_True);

                QTreeWidgetItem *newItem = item->clone();
                newItem->setText(1, newId);
                newItem->setText(0, moduleMap[newId]->name);//item->text(0) + "_2";

                item->parent()->addChild(newItem);

                item->setSelected(false);
                newItem->setSelected(true);

                QSqlQuery query;
                if ( query.exec(QString("INSERT INTO StationModule(id,stationId,name,type,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder,createTime,filename) select '"+newId+"',stationId,'"+moduleMap[newId]->name+"',type,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder,createTime,filename FROM StationModule WHERE id='%1'").arg(id)) ) {
                    qDebug("复制成功！");
                } else {
                    qDebug("复制失败！", true);
                    continue;
                }
            }
        }
    };

    qApp->restoreOverrideCursor();
}

void PageStation::moduleRemove()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetModule->selectedItems();
    if (items.isEmpty())
        return;
    if(0==QMessageBox::information(this,tr("系统提示"),tr("确认要删除选中的项目吗？"), tr("是"), tr("否"))) {
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);
        foreach (QTreeWidgetItem *item, items) {
            QString id = item->text(1);
            if (!id.isEmpty()) {
                QSqlQuery query;
                if ( query.exec(QString("DELETE FROM StationModule WHERE id='%1'").arg(id)) ) {
                    qDebug("删除成功！");
                } else {
                    qDebug("删除失败！", true);
                    continue;
                }
                QFile::remove("D:\\DzicsRobot\\Module\\" + id + ".dzm");
//                emit moduleChanged(id, true);
                if (moduleMap[id]) {
                    moduleMap[id]->aisShape->GetContext()->Clear(moduleMap[id]->aisShape, true);
                    moduleMap[id]->aisShape->Delete();
                    moduleMap.remove(id);
                }
            }
            item->parent()->removeChild(item);
        };
        qApp->restoreOverrideCursor();
    }
}

//#include <MoveTool.h>
//MoveTool *moveToolWidget;
void PageStation::moduleTranslate() {
    QList<QTreeWidgetItem*> items = ui.treeWidgetModule->selectedItems();
    if (items.isEmpty())
        return;
    pageViewer->showToolWidget(1);


//    moveToolWidget = new MoveTool(this->parentWidget(), moduleMap[items.first()->text(1)]->aisShape->GetContext()) ;
//    moveToolWidget->show();
}

void PageStation::moduleItemChanged(QTreeWidgetItem *item, int /*column*/)
{
    if (item->text(1).isEmpty())
        return;

    QString name = item->treeWidget()->objectName();
    if ( name == "treeWidgetModule") {
        name = "StationModule";
        if (moduleMap[item->text(1)]) {
            moduleMap[item->text(1)]->name = item->text(0);
        }
    } else if (name == "treeWidgetAxis") {
        name = "StationAxis";
        if (axisMap[item->text(1)]) {
            axisMap[item->text(1)]->name = item->text(0);
        }
        /*************update4 by cxs 2014 4.28********/
               QtVariantProperty *property = ( QtVariantProperty *)topPropertyAxis->subProperties().at(0);
               property->setValue(item->text(0));
        /*****************end************************/
    } else if (name == "treeWidgetAction") {
        /*************update4 by cxs 2014 4.28********/
        //name = "StationAction";
        if(!item->parent()->parent())
        {
             name = "StationAction";
             if(actionMap[item->text(1)]){
                 actionMap[item->text(1)]->name = item->text(0);
             }
        }
        else
        {
            //item->setText(0,actionMap[item->text(1)]->name );
             return;
         //    name = "StationActionDetail";
        }
        QtVariantProperty *property = ( QtVariantProperty *)topPropertyAction->subProperties().at(0);
        property->setValue(item->text(0));
//rong lk
/*******************by ronglk***********************/
    } else if (name == "treeWidgetTrajectory") {
        //name = "StationTrajectory";
        //if(!item->parent()->parent())
        //{
             //name = "StationTrajectory";
             //if(trajectoryMap[item->text(1)]){
                 //trajectoryMap[item->text(1)]->name = item->text(0);
             //}
        //}
        //else
        //{
          //   return;
        //}
        //QtVariantProperty *property = ( QtVariantProperty *)topPropertyTrajectory->subProperties().at(0);
        //property->setValue(item->text(0));
        return;
/*******************end***********************/
    }else if (name == "treeWidgetCalibration") {
        //name = "treeWidgetCalibration";
        //if (calibrationMap[item->text(1)]) {
            //calibrationMap[item->text(1)]->name = item->text(0);
        //}
        /*************update4 by cxs 2014 4.28********/
               //QtVariantProperty *property = ( QtVariantProperty *)topPropertyCalibration->subProperties().at(0);
               //property->setValue(item->text(0));
        /*****************end************************/
        return;
    }
//end
    QSqlQuery query;
    if ( query.exec(QString("UPDATE %1 SET name='%2' WHERE id='%3'").arg(name, item->text(0), item->text(1))) ) {
        qDebug("修改成功！");
    } else {
        qDebug("修改失败！", true);
        return;
    }

}

void PageStation::on_treeWidgetModule_itemActivated(QTreeWidgetItem * /*item*/, int /*column*/)
{

}

void PageStation::on_treeWidgetModule_currentItemChanged(QTreeWidgetItem * /*current*/, QTreeWidgetItem * /*previous*/)
{

}

void PageStation::axisNew()
{
    if (currentStationId.isEmpty())
        return;

    /****************update11**************************/
        QTreeWidgetItem *cItem = ui.treeWidgetAxis->currentItem();
        if (!cItem)
            return;
        if(cItem->parent())
            return;
        disconnect(ui.treeWidgetAxis, SIGNAL(itemSelectionChanged()), this, SLOT(axisSelectionChanged()));
    /**********************end******************************/
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, "新运动轴");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    ui.treeWidgetAxis->setCurrentItem(item);
    ui.treeWidgetAxis->editItem(item);

/* ==cxs== */
    AxisObject *axis = new AxisObject(this);
    axis->id = item->text(1);
    axis->sign = "Axis";
    axis->name ="新运动轴";
    axis->parentId = "";
    axis->type = 3;
    axis->start[X] =0.00;

    axis->start[Y] =0.00;
    axis->start[Z] =0.00;
    axis->end[X] = 0.00;
    axis->end[Y] = 0.00;
    axis->end[Z] =0.00;
    axis->symbol = 1;
    axis->originOffset =0.00;
    axis->maxValue = 0.00;
    axis->minValue = 0.00;
    axisMap.insert(axis->id,axis);
/* ===== */

    QSqlQuery query;
    if ( query.exec(QString("INSERT INTO StationAxis(id,stationId,sign,name,type,parentId,isUse) VALUES ('%1','%2','Axis','%3',3,'%4',1)").arg(item->text(1), currentStationId, item->text(0), cItem->text(1))) ) {
        qDebug("增加成功！");
    } else {
        qDebug("增加失败！", true);
        return;
    }

    /* ==cxs== */
    connect(ui.treeWidgetAxis, SIGNAL(itemSelectionChanged()), this, SLOT(axisSelectionChanged()));
    axisSelectionChanged();
    /* ===== */

    qDebug(item->text(1).toLocal8Bit().constData());


}

void PageStation::axisRename()
{
    ui.treeWidgetAxis->edit(ui.treeWidgetAxis->currentIndex());

}

/****************update 4  by cxs 2014  4.28***********/
void PageStation::axisDuplicate()
{

    QList<QTreeWidgetItem*> items = ui.treeWidgetAxis->selectedItems();
    if (items.isEmpty())
        return;
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    foreach (QTreeWidgetItem *item, items) {
        QString id = item->text(1);
        if (!id.isEmpty()) {
            if (axisMap[id]) {
                QString newId = SysUtils::uuid();
                QTreeWidgetItem *newItem = item->clone();
                newItem->setText(0,item->text(0)+"_2");
                newItem->setText(1,newId);
                item->parent()->addChild(newItem);

                AxisObject *axis = new AxisObject(this);
                axis->id = newItem->text(1);
                axis->sign = axisMap[id]->sign;
                axis->name = newItem->text(0);
                axis->parentId = axisMap[id]->parentId;
                axis->type = axisMap[id]->type;
                axis->start[X] = axisMap[id]->start[X];
                axis->start[Y] = axisMap[id]->start[Y];
                axis->start[Z] = axisMap[id]->start[Z];
                axis->end[X] = axisMap[id]->end[X];
                axis->end[Y] = axisMap[id]->end[Y];
                axis->end[Z] = axisMap[id]->end[Z];
                axis->symbol = axisMap[id]->symbol;
                axis->originOffset =axisMap[id]->originOffset;
                axis->maxValue = axisMap[id]->maxValue;
                axis->minValue = axisMap[id]->minValue;
                axisMap.insert(axis->id,axis);
               QSqlQuery query;
               QString sql = "INSERT INTO StationAxis(id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse) VALUES(";
               QString valueSql ="'"+ axis->id  + "','" + currentStationId +"','" + axis->sign +"','" +axis->name +"'," +QString::number(axis->type)+",'" + axis->parentId +"'," +QString::number(axis->start[X],'g',13)+","  \
                       +QString::number(axis->start[Y],'g',13)+","+QString::number(axis->start[Z],'g',13)+","+QString::number(axis->end[X],'g',13)+"," +QString::number(axis->end[Y],'g',13) +"," +QString::number(axis->end[Z],'g',13)+","  \
                       +QString::number(axis->symbol)+"," + QString::number(axis->minValue,'g',13)+","+QString::number(axis->maxValue,'g',13)+","+QString::number(axis->originOffset,'g',13)+"," + "1)";
               sql.append(valueSql);
//               qDebug()<<sql;
                if ( query.exec(sql))
                {
                    qDebug("复制成功！");
                } else {
                    qDebug("复制失败！", true);
                    continue;
                }
            }
        }
    };
    qApp->restoreOverrideCursor();



}
/***************end*****************************/\
//********************update zhugz***************
void PageStation::axisSetCurRobot()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetAxis->selectedItems();
    if (items.isEmpty())
        return;

        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);
        int c_robotIndex=-1;
        foreach (QTreeWidgetItem *item, items) {
            if (!item->text(1).isEmpty()) {
                QSqlQuery query;
                c_robotIndex=ui.cur_robot->itemData(ui.cur_robot->currentIndex()).toInt();
                query.exec(QString("UPDATE StationAxis SET cur_robotindex = %1 WHERE id= '%2'").arg(c_robotIndex).arg(item->text(1)));

                // variantManagerAxis->setProperty("cur_robotindex",QString::number(c_robotIndex));

                //                if ( query.exec(QString("DELETE FROM StationAxis WHERE id='%1'").arg(item->text(1))) ) {
//                    qDebug("删除成功！");
//                } else {
//                    qDebug("删除失败！", true);
//                    continue;
//                }

            }

        };
        qApp->restoreOverrideCursor();

    axisSelectionChanged();
}

void PageStation::axisSetPosition()
{
    try {
        QList<QTreeWidgetItem*> items = ui.treeWidgetAxis->selectedItems();
        if (items.isEmpty())
            return;
        QString id = items.last()->text(1);
        if (id.isEmpty()) {
            return;
        }

        bool ok;
        double d = QInputDialog::getDouble(this, tr("请输入目标位置"),
                                        tr("目标位置:"), axisMap.value(id)->value, -2147483647, 2147483647, 7, &ok);
        if (ok) {

            AxisObject *axis = NULL;
            foreach (QTreeWidgetItem *item, items) {
                axis = axisMap.value(item->text(1));
                axis->value = d;
            }

            // 需要时更新外部轴位置
            while (axis && !axis->parentId.isEmpty()) {
                axis = axisMap[axis->parentId];
            }
            Handle_AIS_Shape aisShape = NULL;
            TopLoc_Location locBase;
            while (true) {
                gp_Trsf trsfBase;
                if (axis->type == 1) {
                    trsfBase.SetTranslation((axis->value - axis->originOffset) * axis->symbol * gp_Vec(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]));
                    locBase = locBase * TopLoc_Location(trsfBase);
                } else if(axis->type == 2) {
                    trsfBase.SetRotation(gp_Ax1(gp_Pnt(axis->start[X] + moduleMap[axis->moduleList.last()]->pos[X], axis->start[Y] + moduleMap[axis->moduleList.last()]->pos[Y], axis->start[Z] + moduleMap[axis->moduleList.last()]->pos[Z]), gp_Dir( axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), (axis->value - axis->originOffset) * axis->symbol * M_PI / 180);
                    locBase = locBase * TopLoc_Location(trsfBase);
                }
                for(int j = 0; j < axis->moduleList.size(); ++j) {
                    aisShape = moduleMap[axis->moduleList[j]]->aisShape;
                    aisShape->GetContext()->SetLocation(aisShape, locBase);
                }
                if (axis->subAxisList.isEmpty()) {
                    break;
                }
                axis = axisMap[axis->subAxisList.at(0)];
            }
            if (aisShape) {
                aisShape->GetContext()->UpdateCurrentViewer();
            }

        }
    } catch(...) {

    }
}

void PageStation::axisSetPosition_servo(double d)
{
    try {



         {

            AxisObject *axis = NULL;

            axis = axisMap.value("p0sLs5klRbyKDkEk31sD");
            axis->value = d;



            // 需要时更新外部轴位置
            while (axis && !axis->parentId.isEmpty()) {
                axis = axisMap[axis->parentId];
            }
            Handle_AIS_Shape aisShape = NULL;
            TopLoc_Location locBase;
            while (true) {
                gp_Trsf trsfBase;
                if (axis->type == 1) {
                    trsfBase.SetTranslation((axis->value - axis->originOffset) * axis->symbol * gp_Vec(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]));
                    locBase = locBase * TopLoc_Location(trsfBase);
                } else if(axis->type == 2) {
                    trsfBase.SetRotation(gp_Ax1(gp_Pnt(axis->start[X] + moduleMap[axis->moduleList.last()]->pos[X], axis->start[Y] + moduleMap[axis->moduleList.last()]->pos[Y], axis->start[Z] + moduleMap[axis->moduleList.last()]->pos[Z]), gp_Dir( axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), (axis->value - axis->originOffset) * axis->symbol * M_PI / 180);
                    locBase = locBase * TopLoc_Location(trsfBase);
                }
                for(int j = 0; j < axis->moduleList.size(); ++j) {
                    aisShape = moduleMap[axis->moduleList[j]]->aisShape;
                    aisShape->GetContext()->SetLocation(aisShape, locBase);
                }
                if (axis->subAxisList.isEmpty()) {
                    break;
                }
                axis = axisMap[axis->subAxisList.at(0)];
            }
            if (aisShape) {
                aisShape->GetContext()->UpdateCurrentViewer();
            }

        }
    } catch(...) {

    }
}

void PageStation::axisRemove()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetAxis->selectedItems();
    if (items.isEmpty())
        return;
    if(QMessageBox::Yes==QMessageBox::information(this,tr("系统提示"),tr("确认要删除选中的项目吗？"), QMessageBox::Yes|QMessageBox::No)) {
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);
        foreach (QTreeWidgetItem *item, items) {
            if (!item->text(1).isEmpty()) {
                QSqlQuery query;
                if ( query.exec(QString("DELETE FROM StationAxis WHERE id='%1'").arg(item->text(1))) ) {
                    qDebug("删除成功！");
                } else {
                    qDebug("删除失败！", true);
                    continue;
                }
            }
            axisMap.remove(item->text(1));
            item->parent()->setSelected(true);
            item->parent()->removeChild(item);
        };
        qApp->restoreOverrideCursor();
    }
    axisSelectionChanged();
}


void PageStation::actionNew()
{
    if (currentStationId.isEmpty())
        return;

    disconnect(ui.treeWidgetAction, SIGNAL(itemSelectionChanged()), this, SLOT(actionSelectionChanged()));
    QTreeWidgetItem *cItem = ui.treeWidgetAction->currentItem();
    if (!cItem)
        return;

    if(cItem->parent())
    {
        if(cItem->parent()->parent())
            return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
     if(!cItem->parent())
         item->setText(0, "新动作");
     else
         item->setText(0, "");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    ui.treeWidgetAction->setCurrentItem(item);
   // ui.treeWidgetAction->editItem(item);

    QSqlQuery query;
    if(!cItem->parent())
    {
        ui.treeWidgetAction->editItem(item);

        /************update3 by cxs 2014.4.21********/
                if ( query.exec(QString("INSERT INTO StationAction(id,stationId,name,type,operator,updateType,isUse,extraTrigger,extraTrigger2,count) VALUES ('%1','%2','%3',1,'=', 2,1,0,0,0)").arg(item->text(1), currentStationId, item->text(0))) ) {
        /********************end***********************/
            qDebug("增加成功！");
            ActionObject *actionObject = new ActionObject();
            actionObject->id = item->text(1);
            actionObject->stationId = currentStationId;
            actionObject->name = item->text(0);
            actionObject->type = 1;
            actionObject->extTrg = 0;
            /*****************update by cxs 2014.4.18******/
            actionObject->count = 0;
            /**************update2 by  cxs 2014.4.18******/
            actionObject->oper ="=";
            actionObject->extTrg2 = 0;
            /*****************end*************************/
            actionObject->updateType = 2;
            actionMap.insert(actionObject->id,actionObject);

        } else {
            qDebug("增加失败！", true);
            return;
        }

    }
    else
    {
        ActionDetail *actionDetail = new ActionDetail();
        actionDetail->id = item->text(1);
        actionDetail->actionId = cItem->text(1);
        actionDetail->name = "";
         actionDetail->type = 1;
         /**************update2 by  cxs 2014.4.18******/
                actionDetail->value = "0";
        /******************end********************/
        actionDetailMap.insert(actionDetail->id,actionDetail);
        /**************update2 by  cxs 2014.4.18******/
              if(query.exec(QString("INSERT INTO StationActionDetail(id,actionId,name,type,moduleId,value,isUse) VALUES ('%1','%2','%3',1,'','0',1)").
                            arg(item->text(1),cItem->text(1),item->text(0))))
       /*******************end************/
        {

            qDebug("增加成功");
        }
        else
        {
            qDebug("增加失败");
            return;
        }

    }
    connect(ui.treeWidgetAction, SIGNAL(itemSelectionChanged()), this, SLOT(actionSelectionChanged()));
    actionSelectionChanged();
    qDebug(item->text(1).toLocal8Bit().constData());

}

void PageStation::actionRename()
{
    if(ui.treeWidgetAction->currentItem()->parent())
    {
        if(ui.treeWidgetAction->currentItem()->parent()->parent())
            return;
    }
    ui.treeWidgetAction->edit(ui.treeWidgetAction->currentIndex());
}

void PageStation::actionRemove()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetAction->selectedItems();
    if (items.isEmpty())
        return;
    if (QMessageBox::Yes==QMessageBox::information(this, tr("系统提示"), tr("确认要删除选中的项目吗？"), QMessageBox::Yes | QMessageBox::No )) {
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);
        foreach (QTreeWidgetItem *item, items) {
            if (!item->text(1).isEmpty()) {
                QSqlQuery query;
                if(!item->parent()->parent())
                {

                if ( query.exec(QString("DELETE FROM StationAction WHERE id='%1'").arg(item->text(1))) ) {
                    qDebug("删除成功！");
                } else {
                    qDebug("删除失败！", true);
                    continue;
                }
                query.exec(QString("DELETE FROM  StationActionDetail WHERE  actionId ='%1'").arg(item->text(1)));

                }
                else
                {
                    if ( query.exec(QString("DELETE FROM StationActionDetail WHERE id='%1'").arg(item->text(1))) ) {
                        qDebug("删除成功！");
                    } else {
                        qDebug("删除失败！", true);
                        continue;
                    }
                }
            }
            item->parent()->removeChild(item);
        };
        qApp->restoreOverrideCursor();
    }
}

/*************update 4 by cxs 2014 4.28**********/
void PageStation::actionDuplicate()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetAction->selectedItems();
    if (items.isEmpty())
        return;
    QSqlQuery query;
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    foreach (QTreeWidgetItem *item, items) {
        if(!item->parent())
        {
            continue;
        }

        QString id = item->text(1);
        QString newId = SysUtils::uuid();
        QTreeWidgetItem *newItem = item->clone();
        newItem->setText(0,item->text(0)+"_2");
        newItem->setText(1,newId);
        item->parent()->addChild(newItem);

        if(!item->parent()->parent()){
         ActionObject *actionObject = new ActionObject();
         actionObject->id = newId;
         actionObject->stationId = currentStationId;
         actionObject->name = item->text(0)+"_2";
         actionObject->type = actionMap[id]->type;
         actionObject->code = actionMap[id]->code;
         actionObject->oper = actionMap[id]->oper;
         actionObject->value = actionMap[id]->value;
         actionObject->extTrg = actionMap[id]->extTrg;
         actionObject->code2 = actionMap[id]->code2;
         actionObject->oper2 = actionMap[id]->oper2;
         actionObject->value2 = actionMap[id]->value2;
         actionObject->extTrg2 = actionMap[id]->extTrg;
         actionObject->code3 = actionMap[id]->code3;
         actionObject->oper3 = actionMap[id]->oper3;
         actionObject->value3 = actionMap[id]->value3;
         actionObject->extTrg2 = actionMap[id]->extTrg2;
         actionObject->count = actionMap[id]->count;
         actionObject->updateType = actionMap[id]->updateType;
         actionMap.insert(newId,actionObject);
                  QString sql = "INSERT INTO StationAction(id,stationId,name,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3,count,updateType,isUse) Values('";
                  QString valuesql = actionObject->id +"','" +currentStationId +"','" + actionObject->name+"'," + QString::number(actionObject->type) +",'" +actionObject->code  +"','"  \
                          +actionObject->oper +"','" + actionObject->value +"'," +QString::number(actionObject->extTrg)+",'"+ actionObject->code2 +"','" +actionObject->oper2+"','"  \
                          +actionObject->value2 +"'," +QString::number(actionObject->extTrg2) +",'" +actionObject->code3 +"','"+actionObject->oper3 +"','" +actionObject->value3+"'," \
                          +QString::number(  actionObject->count) +"," +QString::number(actionObject->updateType)+"," +"1)";
                  sql.append(valuesql);

                  if(query.exec(sql))
                      qDebug("成功");
                  else
                      qDebug("失败");
                  if(newItem->childCount() != 0)
                  {
                      QTreeWidgetItem *itemChild;
                      for(int i = 0; i< newItem->childCount(); i++)
                      {

                          itemChild = newItem->child(i);
                          QString oldId = itemChild->text(1);
                          QString newId = SysUtils::uuid();
                          itemChild->setText(1,newId);
                          ActionDetail* actionDetail = new  ActionDetail(this);
                          actionDetail->id = newId;
                          actionDetail->actionId = newItem->text(1);
                          actionDetail->name = itemChild->text(0);
                          actionDetail->type = actionDetailMap[oldId]->type;
                          actionDetail->moduleId = actionDetailMap[oldId]->moduleId;
                          actionDetail->value = actionDetailMap[oldId]->value;
                          actionDetailMap.insert(actionDetail->id,actionDetail);
                          if(query.exec(QString("INSERT INTO StationActionDetail (id,actionId,name,type,moduleId,value,isUse) VALUES('%1','%2','%3',%4,'%5','%6',1)")
                                        .arg( actionDetail->id,actionDetail->actionId,actionDetail->name,QString::number(actionDetail->type),actionDetail->moduleId,actionDetail->value)))
                          {
                              qDebug("复制成功");
                          }
                          else
                              qDebug("复制失败");
                  }



                 }



        }
        else{
            newItem->setText(0,item->text(0));
            ActionDetail* actionDetail = new  ActionDetail(this);
            actionDetail->id = newId;
            actionDetail->actionId = actionDetailMap[id]->actionId;
            actionDetail->name = actionDetailMap[id]->name;
            actionDetail->type = actionDetailMap[id]->type;
            actionDetail->moduleId = actionDetailMap[id]->moduleId;
            actionDetail->value = actionDetailMap[id]->value;
            actionDetailMap.insert(newId,actionDetail);
            if(query.exec(QString("INSERT INTO StationActionDetail (id,actionId,name,type,moduleId,value,isUse) VALUES('%1','%2','%3',%4,'%5','%6',1)")
                          .arg( actionDetail->id,actionDetail->actionId,actionDetail->name,QString::number(actionDetail->type),actionDetail->moduleId,actionDetail->value)))
            {
                qDebug("复制成功");
            }
            else
                qDebug("复制失败");

        }


    };
    qApp->restoreOverrideCursor();




}
/********************end************************/

void PageStation::moduleSelectionChanged()
{
    if (!this->isVisible()) return;

    disconnect(variantManagerModule, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(modulePropertyChanged(QtProperty *, const QVariant &)));

    if ( ui.treeWidgetModule->selectedItems().count()>0 ) {
//rong lk
        QList<QTreeWidgetItem*> selectedItemsList = ui.treeWidgetModule->selectedItems();
        QTreeWidgetItem *item = selectedItemsList.last();
        if(moduleMap.contains(item->text(1)))
        {
            moduleMap[item->text(1)]->aisShape->GetContext()->ClearSelected(false);
        }else
        {
            //根节点返回
            return;
        }


        for(int i=0; i<selectedItemsList.count(); i++)
        {
            QTreeWidgetItem *item = selectedItemsList[i];
            if (!item->parent()) {
                topPropertyModule->setEnabled(false);
                foreach (QtProperty *property, topPropertyModule->subProperties()) {
                    QtVariantProperty *p = (QtVariantProperty *)property;
                    if(p->valueType() == QVariant::String) {
                        p->setValue("");
                    }else if(p->valueType() == QVariant::Bool) {
                        p->setValue(false);
                    }else if(p->valueType() == QVariant::Int) {
                        p->setValue(NULL);
                    }else if(p->valueType() == QVariant::Double) {
                        p->setValue(NULL);
                    }else if(p->valueType() == QVariant::Color) {
                        p->setValue(QColor(Qt::black));
                    }else if(p->valueType() == QtVariantPropertyManager::enumTypeId()) {
                        p->setValue(0);
                    }
        //            p->setEnabled(false);
                }
                if ( moduleMap.count()>0 ) {
                    moduleMap.values().at(0)->aisShape->GetContext()->ClearSelected(true);
                }
                return;
            }

            QtVariantProperty *property = (QtVariantProperty *)topPropertyModule->subProperties().at(0);
            property->setValue(item->text(0));

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(1);
            property->setValue(moduleMap[item->text(1)]->isHidden);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(2);
            if (!moduleMap[item->text(1)]->bindAxis.isEmpty()) {
                if (axisMap.contains(moduleMap[item->text(1)]->bindAxis)) {
                    property->setValue(axisNameList.indexOf(axisMap[moduleMap[item->text(1)]->bindAxis]->name));
                } else {
                    property->setValue("");
                }
            } else {
                property->setValue(0);
            }

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(3);
            property->setValue(moduleMap[item->text(1)]->pos[X]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(4);
            property->setValue(moduleMap[item->text(1)]->pos[Y]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(5);
            property->setValue(moduleMap[item->text(1)]->pos[Z]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(6);
            property->setValue(moduleMap[item->text(1)]->rotateDirection[X]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(7);
            property->setValue(moduleMap[item->text(1)]->rotateDirection[Y]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(8);
            property->setValue(moduleMap[item->text(1)]->rotateDirection[Z]);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(9);
            property->setValue(moduleMap[item->text(1)]->rotateAngle * 180 / M_PI);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(10);
            property->setValue(moduleMap[item->text(1)]->material);

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(11);
            property->setValue(moduleMap[item->text(1)]->material);

            int c = moduleMap[item->text(1)]->color;
    //        property->setValue( QColor( ((c & 0xFF0000) >> 16), ((c & 0xFF00) >> 8), (c & 0xFF) ) );
            property->setValue( QColor::fromRgb( ((c & 0xFF0000) >> 16), ((c & 0xFF00) >> 8), (c & 0xFF) ) );
            topPropertyModule->setEnabled(true);
            if (!moduleMap[item->text(1)]->isHidden) {
                //moduleMap[item->text(1)]->aisShape->GetContext()->SetSelected(moduleMap[item->text(1)]->aisShape, false);
                moduleMap[item->text(1)]->aisShape->GetContext()->AddOrRemoveSelected(moduleMap[item->text(1)]->aisShape, false);
                //moduleMap[item->text(1)]->aisShape->GetContext()->HilightSelected(true);

            }

            property = (QtVariantProperty *)topPropertyModule->subProperties().at(12);
            property->setValue(moduleMap[item->text(1)]->transparency * 100.0);
        }
        moduleMap[item->text(1)]->aisShape->GetContext()->HilightSelected(true);
//end
    } else {
        topPropertyModule->setEnabled(false);
        foreach (QtProperty *property, topPropertyModule->subProperties()) {
            QtVariantProperty *p = (QtVariantProperty *)property;
            if(p->valueType() == QVariant::String) {
                p->setValue("");
            }else if(p->valueType() == QVariant::Bool) {
                p->setValue(false);
            }else if(p->valueType() == QVariant::Int) {
                p->setValue(NULL);
            }else if(p->valueType() == QVariant::Double) {
                p->setValue(NULL);
            }else if(p->valueType() == QVariant::Color) {
                p->setValue(QColor(Qt::black));
            }else if(p->valueType() == QtVariantPropertyManager::enumTypeId()) {
                p->setValue(0);
            }
//            p->setEnabled(false);
        }
        if ( moduleMap.count()>0 ) {
            moduleMap.values().at(0)->aisShape->GetContext()->ClearSelected(true);
        }
    }
    connect(variantManagerModule, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(modulePropertyChanged(QtProperty *, const QVariant &)));

}

/*********************update4 by cxs 4.28.2014****************/
void PageStation::axisSelectionChanged()
{
    if (!this->isVisible()) return;
    disconnect(variantManagerAxis, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
               this, SLOT(axisPropertyChanged(QtProperty *, const QVariant &)));
    if(ui.treeWidgetAxis->selectedItems().count() > 0)
    {

        QTreeWidgetItem *item = ui.treeWidgetAxis->selectedItems().last();
        if (!item->parent()){
            topPropertyAxis->setEnabled(false);
            return;
        }
//        foreach (QtProperty *property, topPropertyAxis->subProperties())
//            topPropertyAxis->removeSubProperty(property);
//        QtVariantProperty *itemPro = variantManagerAxis->addProperty(QVariant::String, tr("轴名称"));
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro->setValue(axisMap[item->text(1)]->name);
//        itemPro = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("轴类型"));
//        itemPro->setAttribute(QLatin1String("enumNames"), axisTypeList);
//        topPropertyAxis->addSubProperty(itemPro);

//        switch(axisMap[item->text(1)]->type)
//        {
//        case 1:
//            itemPro->setValue(0);
//            break;
//        case 2:
//            itemPro->setValue(1);
//            break;

//        case 3:
//            itemPro->setValue(2);
//            break;
//        case 4:
//            itemPro->setValue(3);
//            break;
//        case 11:
//            itemPro->setValue(4);
//            break;
//        case 12:
//            itemPro->setValue(5);
//            break;

//        }
//        itemPro = variantManagerAxis->addProperty(QVariant::String, tr("数据来源"));  // 标识
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro->setValue(axisMap[item->text(1)]->sign);
//        itemPro = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("上级轴"));
//        axisIdList.clear();
//        axisIdList.append("");
//        axisNameList.clear();
//        axisNameList.append("");
//        QMap<QString, AxisObject*>::iterator it;
//        for ( it = axisMap.begin(); it != axisMap.end(); ++it ) {
//            axisIdList<<it.value()->id;
//            axisNameList<<it.value()->name;
//            //qDebug()<<it.value()->parentId << it.value()->name <<axisMap.size();
//        }
//        itemPro->setAttribute(QLatin1String("enumNames"),axisNameList);
//        int  index = axisIdList.indexOf(axisMap[item->text(1)]->parentId);
//        if(index == -1)
//        {
//            axisMap[item->text(1)]->parentId = "";
//            index = 0;
//        }
//        itemPro->setValue(index);

//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("X起始"));
//        itemPro->setValue(axisMap[item->text(1)]->start[X]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("Y起始"));
//        itemPro->setValue(axisMap[item->text(1)]->start[Y]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("Z起始"));
//        itemPro->setValue(axisMap[item->text(1)]->start[Z]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("X结束"));
//        itemPro->setValue(axisMap[item->text(1)]->end[X]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("Y结束"));
//        itemPro->setValue(axisMap[item->text(1)]->end[Y]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("Z结束"));
//        itemPro->setValue(axisMap[item->text(1)]->end[Z]);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QtVariantPropertyManager::enumTypeId(), tr("符号"));
//        QStringList enumSymbol;
//        enumSymbol<<"正"<<"负";
//        itemPro->setAttribute(QLatin1String("enumNames"),enumSymbol);
//        if(axisMap[item->text(1)]->symbol == 1)
//            itemPro->setValue(0);
//        else if(axisMap[item->text(1)]->symbol == -1)
//            itemPro->setValue(1);
//        else
//            itemPro->setValue(0);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("最大值"));
//        itemPro->setValue(axisMap[item->text(1)]->maxValue);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("最小值"));
//        itemPro->setValue(axisMap[item->text(1)]->minValue);
//        topPropertyAxis->addSubProperty(itemPro);
//        itemPro = variantManagerAxis->addProperty(QVariant::Double, tr("原点偏移"));
//        itemPro->setValue(axisMap[item->text(1)]->originOffset);
//        topPropertyAxis->addSubProperty(itemPro);
//        topPropertyAxis->setEnabled(true);

        QtVariantProperty *itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(0);
        itemPro->setValue(axisMap[item->text(1)]->name);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(1);
        switch(axisMap[item->text(1)]->type)
        {
        case 1:
            itemPro->setValue(0);
            break;
        case 2:
            itemPro->setValue(1);
            break;

        case 3:
            itemPro->setValue(2);
            break;
        case 4:
            itemPro->setValue(3);
            break;
        case 11:
            itemPro->setValue(4);
            break;
        case 12:
            itemPro->setValue(5);
            break;

        }
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(2);
        itemPro->setValue(axisMap[item->text(1)]->sign);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(3);
        axisIdList.clear();
        axisIdList.append("");
        axisNameList.clear();
        axisNameList.append("");
//        QMap<QString, AxisObject*>::iterator it;
//        for ( it = axisMap.begin(); it != axisMap.end(); ++it ) {
//            axisIdList<<it.value()->id;
//            axisNameList<<it.value()->name;
//            //qDebug()<<it.value()->parentId << it.value()->name <<axisMap.size();
//        }
        QSqlQuery query;
        query.exec(QString("select id, name from StationAxis where  stationId = '%1' and isUse =1 ORDER BY name ASC").arg(currentStationId));
         while(query.next())
         {
             axisIdList.append(query.value(0).toString());
             axisNameList.append(query.value(1).toString());
         }

        itemPro->setAttribute(QLatin1String("enumNames"),axisNameList);
        int  index = axisIdList.indexOf(axisMap[item->text(1)]->parentId);
        if(index == -1)
        {
            axisMap[item->text(1)]->parentId = "";
            index = 0;
        }
        itemPro->setValue(index);
        /*************************update5 by cxs 2014 5.6*************************************/
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(4);
//        itemPro->setValue(axisMap[item->text(1)]->start[X]);
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(5);
//        itemPro->setValue(axisMap[item->text(1)]->start[Y]);
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(6);
//        itemPro->setValue(axisMap[item->text(1)]->start[Z]);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(4);
        QCmmF value; value.x = axisMap[item->text(1)]->start[X]; value.y = axisMap[item->text(1)]->start[Y]; value.z = axisMap[item->text(1)]->start[Z];
        itemPro->setValue(QVariant::fromValue(value));

//        itemPro =(QtVariantProperty *) topPropertyAxis->subProperties().at(7);
//        itemPro->setValue(axisMap[item->text(1)]->end[X]);
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(8);
//        itemPro->setValue(axisMap[item->text(1)]->end[Y]);
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(9);
//        itemPro->setValue(axisMap[item->text(1)]->end[Z]);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(5);
        value.x = axisMap[item->text(1)]->end[X]; value.y = axisMap[item->text(1)]->end[Y]; value.z = axisMap[item->text(1)]->end[Z];
        itemPro->setValue(QVariant::fromValue(value));

        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(6);
        if(axisMap[item->text(1)]->symbol == 1)
            itemPro->setValue(0);
        else if(axisMap[item->text(1)]->symbol == -1)
            itemPro->setValue(1);
        else
            itemPro->setValue(0);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(7);
        itemPro->setValue(axisMap[item->text(1)]->maxValue);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(8);
        itemPro->setValue(axisMap[item->text(1)]->minValue);
        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(9);
        itemPro->setValue(axisMap[item->text(1)]->originOffset);
        //update zhugz
//        itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(10);
//        itemPro->setValue(axisMap[item->text(1)]->cur_robotindex);
        topPropertyAxis->setEnabled(true);
        /**************************************end**************************/

    }

    connect(variantManagerAxis, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(axisPropertyChanged(QtProperty *, const QVariant &)));

}
/*************end*******************************************************************/



/*********************add by cxs 4.16.2014****************/
void PageStation::axisPropertyChanged(QtProperty *property, const QVariant &value)
{

    if (ui.treeWidgetAxis->selectedItems().count()==0)
        return;

    /*******update4 by cxs 2014 4.25*****************/
    //    QTreeWidgetItem *item = ui.treeWidgetAxis->selectedItems().last();
    //    if (!item->parent())
    //        return;
    /*****************end****************/
    QSqlQuery query;
    QString tableName = "StationAxis";
    QString field;
    /*******update4 by cxs 2014 4.25*************/

      foreach (QTreeWidgetItem *item, ui.treeWidgetAxis->selectedItems())
      {

         if(!item->parent())
          continue;
     /**************end*************/
    if(property->propertyName() == "轴名称")
    {
        axisMap[item->text(1)]->name = value.toString();
        item->setText(0,value.toString());

        //query.exec(QString("update %1 set "))
    }
    else if(property->propertyName() == "轴类型" )
    {
        switch(value.toInt())
        {
        case 0:
            axisMap[item->text(1)]->type =1 ;
            break;
        case 1:
            axisMap[item->text(1)]->type = 2;
            break;
        case 2:
            axisMap[item->text(1)]->type = 3;
            break;
        case 3:
            axisMap[item->text(1)]->type = 4;
            break;
        case 4:
            axisMap[item->text(1)]->type = 11;
            break;
        case 5:
            axisMap[item->text(1)]->type  = 12;
            break;
        }
        field = "type";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->type) ,item->text(1)));
    }
    else if(property->propertyName() == "数据来源") // 标识
    {
        axisMap[item->text(1)]->sign = value.toString();
        field = "sign";
        query.exec(QString("update %1 set %2 = '%3' where id = '%4'").arg(tableName,field, axisMap[item->text(1)]->sign ,item->text(1)));
    }
    else if(property->propertyName() == "上级轴")
    {
        int index = value.toInt();
        field = "parentId";
        axisMap[item->text(1)]->parentId = axisIdList.value(index);
        if(axisMap[item->text(1)]->parentId == item->text(1))
        {
            axisMap[item->text(1)]->parentId ="";
            axisSelectionChanged();
        }

        query.exec(QString("update %1 set %2 = '%3' where id = '%4'").arg(tableName,field, axisMap[item->text(1)]->parentId ,item->text(1)));

    }
//    else if(property->propertyName() == "X起始")
    /**********************update5 by cxs  2014 5.6*********************/
    else if(property->propertyName() == "X" && property == topPropertyAxis->subProperties().at(4)->subProperties().at(0) )
   /*******************************end***********************************/
    {
        axisMap[item->text(1)]->start[X] = value.toDouble();
        field = "startX";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->start[X],'g',13) ,item->text(1)));
    }
//    else if(property->propertyName() == "Y起始")
    /**********************update5 by cxs  2014 5.6*********************/
    else if(property->propertyName() == "Y" && property == topPropertyAxis->subProperties().at(4)->subProperties().at(1))
      /***********************end**********************/
    {
        axisMap[item->text(1)]->start[Y] = value.toDouble();
        field = "startY";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->start[Y],'g',13) ,item->text(1)));
    }

//    else if(property->propertyName() == "Z起始")
    /********************update5 by cxs 2014 5.6****************************/
        else if(property->propertyName() == "Z" && property == topPropertyAxis->subProperties().at(4)->subProperties().at(2))
    /**********************************end***********************************/
    {
        axisMap[item->text(1)]->start[Z] = value.toDouble();
        field = "startZ";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->start[Z],'g',13) ,item->text(1)));
    }
    /********************update5 by cxs 2014 5.6****************************/
        else if(property->propertyName() == "X" && property == topPropertyAxis->subProperties().at(5)->subProperties().at(0) )
    /*******************************end*********************************/
//    else if(property->propertyName() == "X结束")
    {
        axisMap[item->text(1)]->end[X] = value.toDouble();
        field = "endX";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->end[X],'g',13) ,item->text(1)));
    }
//    else if(property->propertyName() == "Y结束")
    /********************update5 by cxs 2014 5.6****************************/
        else if(property->propertyName() == "Y" && property == topPropertyAxis->subProperties().at(5)->subProperties().at(1) )
    /*******************************end*******************************************/
    {
        axisMap[item->text(1)]->end[Y] = value.toDouble();
        field = "endY";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->end[Y],'g',13) ,item->text(1)));
    }

//    else if(property->propertyName() == "Z结束")
    /********************update5 by cxs 2014 5.6****************************/
        else if(property->propertyName() == "Z" && property == topPropertyAxis->subProperties().at(5)->subProperties().at(2))
    /*******************************end****************************************/
    {
        axisMap[item->text(1)]->end[Z] = value.toDouble();
        field = "endZ";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->end[Z],'g',13) ,item->text(1)));
    }
    else if(property->propertyName() == "符号")
    {
        if(value.toInt() == 0)
            axisMap[item->text(1)]->symbol = 1;
        else if(value.toInt() ==1 )
            axisMap[item->text(1)]->symbol = -1;
        field = "symbol";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->symbol) ,item->text(1)));
    }
    else if(property->propertyName() == "最大值")
    {
        axisMap[item->text(1)]->maxValue = value.toDouble();
        field = "maxValue";
        query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->maxValue,'g',13) ,item->text(1)));

    }
    else if(property->propertyName() == "最小值")
    {
        axisMap[item->text(1)]->minValue = value.toDouble();
        field = "minValue";
        query.exec(QString("UPDATE %1 set %2 = %3 WHERE id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->minValue,'g',13) ,item->text(1)));

    }
    else if(property->propertyName() == "原点偏移")
    {
        axisMap[item->text(1)]->originOffset = value.toDouble();
        field ="originOffset ";
        query.exec(QString("UPDATE %1 set %2 = %3 WHERE id = '%4'").arg(tableName,field, QString::number(axisMap[item->text(1)]->originOffset,'g',13) ,item->text(1)));


    }
    /**************update4 by cxs 2014.4.25**/
      }
    /*****end*************/

}
/**********************end*****************************************/
//rong lk
  /*********************************
   *add by cxs 2014 4.15
   *******************************/
void PageStation::actionSelectionChanged()
{
    if (!this->isVisible()) return;

    disconnect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
               this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
    if(ui.treeWidgetAction->selectedItems().count()>0)
    {

        QTreeWidgetItem *item = ui.treeWidgetAction->selectedItems().last();
        if (!item->parent()){
            topPropertyAction->setEnabled(false);
            return;

        }
        foreach (QtProperty *property, topPropertyAction->subProperties())
            topPropertyAction->removeSubProperty(property);

        //动作部分
        if(!item->parent()->parent()){

            QtVariantProperty *itemPro = variantManagerAction->addProperty(QVariant::String, tr("名称"));
            topPropertyAction->addSubProperty(itemPro);
            itemPro->setValue(actionMap[item->text(1)]->name);
            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("主动作类型"));
            itemPro->setAttribute(QLatin1String("enumNames"), actionList);
            topPropertyAction->addSubProperty(itemPro);

           QString codeName, valueName;
            switch(actionMap[item->text(1)]->type)
            {
            case 1:
                codeName = "信号名";
                valueName = "信号值";
                itemPro->setValue(0);
                break;
            case 2:
                codeName = "变量名";
                valueName = "变量值";
                itemPro->setValue(1);
                break;
            case 3:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(2);
                break;
            case 11:
                codeName = "定时次数";
                valueName = "定时时间";
                itemPro->setValue(3);
                break;
            case 12:
                codeName = "始终动作";
                //valueName = "默认值";
                itemPro->setValue(4);
                break;
            case 21:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(5);
                break;
            case 31:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(6);
                break;
            case 32:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(7);
                break;
            case 33:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(8);
                break;
            case 100:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(9);  //7 //6 // 8
                break;
            case 34:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(10);  //7 //6 // 8
                break;
            case 35:
                codeName = "信号/变量名";
                valueName = "信号/变量值";
                itemPro->setValue(11);  //7 //6 // 8
                break;
            }
//end

            if(actionMap[item->text(1)]->type != 12)
            {
                itemPro = variantManagerAction->addProperty(QVariant::String, codeName);
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionMap[item->text(1)]->code);

                if(actionMap[item->text(1)]->type == 11)
                {
                    itemPro = variantManagerAction->addProperty(QVariant::String, valueName);
                    topPropertyAction->addSubProperty(itemPro);
                    itemPro->setValue(actionMap[item->text(1)]->value);
                }
                else
                {
                    itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符"));
                    topPropertyAction->addSubProperty(itemPro);
                    itemPro->setAttribute(QLatin1String("enumNames"), operList);
                    int index = operList.indexOf(actionMap[item->text(1)]->oper);
                    if(index < 0)
                        index = 0;
                    itemPro->setValue(index);
                    itemPro = variantManagerAction->addProperty(QVariant::String, valueName);
                    topPropertyAction->addSubProperty(itemPro);
                    itemPro->setValue(actionMap[item->text(1)]->value);
                    itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("附加条件操作符"));
                    itemPro->setAttribute(QLatin1String("enumNames"), exterList);
                    index = actionMap[item->text(1)]->extTrg;
                    itemPro->setValue(index);
                    topPropertyAction->addSubProperty(itemPro);
                    if(actionMap[item->text(1)]->extTrg != 0)
                    {
                        itemPro = variantManagerAction->addProperty(QVariant::String, codeName +"2");
                        topPropertyAction->addSubProperty(itemPro);
                        itemPro->setValue(actionMap[item->text(1)]->code2);
                        itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符2"));
                        topPropertyAction->addSubProperty(itemPro);
                        itemPro->setAttribute(QLatin1String("enumNames"), operList);
                        int index = operList.indexOf(actionMap[item->text(1)]->oper2);
                        if(index < 0)
                            index = 0;
                        itemPro->setValue(index);
                        itemPro = variantManagerAction->addProperty(QVariant::String, valueName+"2");
                        topPropertyAction->addSubProperty(itemPro);
                        itemPro->setValue(actionMap[item->text(1)]->value2);
                        itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("附加条件2操作符"));
                        itemPro->setAttribute(QLatin1String("enumNames"), exterList);
                        index = actionMap[item->text(1)]->extTrg2;
                        itemPro->setValue(index);
                        topPropertyAction->addSubProperty(itemPro);
                        if(actionMap[item->text(1)]->extTrg2 != 0){
                            itemPro = variantManagerAction->addProperty(QVariant::String, codeName+"3");
                            topPropertyAction->addSubProperty(itemPro);
                            itemPro->setValue(actionMap[item->text(1)]->code3);
                            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符3"));
                            topPropertyAction->addSubProperty(itemPro);
                            itemPro->setAttribute(QLatin1String("enumNames"), operList);
                            int index = operList.indexOf(actionMap[item->text(1)]->oper3);
                            if(index < 0)
                                index = 0;
                            itemPro->setValue(index);
                            itemPro = variantManagerAction->addProperty(QVariant::String, valueName +"3");
                            topPropertyAction->addSubProperty(itemPro);
                            itemPro->setValue(actionMap[item->text(1)]->value3);



                        }
                    }
                }
                itemPro = variantManagerAction->addProperty(QVariant::Int, tr("动作执行次数"));
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionMap[item->text(1)]->count);
            }
            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("更新方式"));
            QStringList updateMove;
            updateMove<<"绝对动作"<<"相对动作";
            itemPro->setAttribute(QLatin1String("enumNames"), updateMove);
            topPropertyAction->addSubProperty(itemPro);
            /***update4 by cxs 2014.4.25****/
                       if(actionMap[item->text(1)]->updateType != 2 && actionMap[item->text(1)]->updateType != 1 )
           /**************end*******************/
                actionMap[item->text(1)]->updateType = 2;
            itemPro->setValue(actionMap[item->text(1)]->updateType-1);
            topPropertyAction->setEnabled(true);


         //  int index =  actItemParent->children().indexOf(ui.propertyAction);
//           qDebug()<<index <<actItemParent->children().count()<<" ee"<<ui.propertyAction->currentItem();
//                   index++;

//           if(index >= actItemParent->children().count())
//               index =0;
//             ui.propertyAction->setCurrentItem(actItemParent->children().at(index));
//             ui.propertyAction->editItem(actItemParent->children().at(index));
            // return;
        }

        //动作明细部分
        else
        {
//            // topPropertyAction = variantManagerAction->addProperty(QtVariantPropertyManager::groupTypeId(), tr("动作明细"));
            QtVariantProperty *itemPro;// = variantManagerAction->addProperty(QVariant::String, tr("名称"));
//            itemPro->setValue(actionDetailMap[item->text(1)]->name);
//            topPropertyAction->addSubProperty(itemPro);


            //qDebug()<<actionMap[item->parent()->text(1)]->type ;
            int index;
            if(actionMap[item->parent()->text(1)]->type == 21 ) //刀具
            {
                 QuerKnifeName();
                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("刀具"));
                itemPro->setAttribute(QLatin1String("enumNames"),knifeList);
                topPropertyAction->addSubProperty(itemPro);
                index = knifeIdList.indexOf(actionDetailMap[item->text(1)]->moduleId);
                itemPro->setValue(index);
                topPropertyAction->setEnabled(true);
                 connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                 return;
            } else if(actionMap[item->parent()->text(1)]->type == 31 ) {    // 测量保存
//                itemPro = variantManagerAction->addProperty(QVariant::String, tr("保存文件名"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setValue(actionDetailMap[item->text(1)]->value);
//                topPropertyAction->setEnabled(true);
//                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
//                return;
//            } else if(actionMap[item->parent()->text(1)]->type == 32 ) {    // 测量换页
                itemPro = variantManagerAction->addProperty(QVariant::Int, tr("测量页号"));
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionDetailMap[item->text(1)]->value.toInt());
                topPropertyAction->setEnabled(true);
                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                return;
            } else if(actionMap[item->parent()->text(1)]->type == 32 ) {    // 视觉分析
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("视觉相机编号"));
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionDetailMap[item->text(1)]->moduleId);

                QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");

                itemPro = variantManagerAction->addProperty(QVariant::String, tr("视觉模板编号"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>0) {
                    itemPro->setValue(tmpList[0]);
                }

                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("视觉算法"));
                QStringList visionModes;
                visionModes<<"工件A线角度"<<"工件A线反转"<<"圆孔检测"<<"风扇圆孤"<<"风扇圆孤侧面"<<"工件B线"<<"工件C线"<<"大垫片检测"<<"铆接角度检测"<<"铆接有无检测"<<"小垫片检测"<<"hd裂缝检测"<<"工件D线"<<"垫片环形光"<<"工件E线"<<"wx检测1"<<"wx缺陷检测"<<"铆钉检测算法"<<"铆钉位置1检测"<<"铆钉位置2检测"<<"间隙检测"<<"工件F线"<<"模板匹配"<<"工件G线"<<"间隙检测2"<<"预留"<<"wx检测2"<<"灰度值检测"<<"工件I线"<<"雪龙异物"<<"HD角度检测2"<<"自定义检测"<<"HD角度检测3"<<"arm检测"<<"毛刺检测";
                itemPro->setAttribute(QLatin1String("enumNames"),visionModes);
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>1) {
                    itemPro->setValue(tmpList[1].toInt());
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输入参数列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>2) {
                    itemPro->setValue(tmpList[2]);
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输出变量列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>3) {
                    itemPro->setValue(tmpList[3]);
                }

                topPropertyAction->setEnabled(true);
                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                return;
            } else if(actionMap[item->parent()->text(1)]->type == 33 ) {    // 称重
//                itemPro = variantManagerAction->addProperty(QVariant::Int, tr("重量范围"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setValue(actionDetailMap[item->text(1)]->value.toInt());
//                topPropertyAction->setEnabled(true);
//                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                return;
            }  else if(actionMap[item->parent()->text(1)]->type == 34 ) {    // 伺服电机
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("属性1"));
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionDetailMap[item->text(1)]->moduleId);

                QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");

                itemPro = variantManagerAction->addProperty(QVariant::String, tr("属性2"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>0) {
                    itemPro->setValue(tmpList[0]);
                }

                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("配置方法"));
                QStringList visionModes;
                visionModes<<"当前位置"<<"参数配置"<<"伺服动作获取"<<"冲压动作获取";
                itemPro->setAttribute(QLatin1String("enumNames"),visionModes);
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>1) {
                    itemPro->setValue(tmpList[1].toInt());
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输入参数列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>2) {
                    itemPro->setValue(tmpList[2]);
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输出变量列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>3) {
                    itemPro->setValue(tmpList[3]);
                }

                topPropertyAction->setEnabled(true);
                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                return;
            }
            else if(actionMap[item->parent()->text(1)]->type == 35 ) {    // 通用
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("属性1"));
                topPropertyAction->addSubProperty(itemPro);
                itemPro->setValue(actionDetailMap[item->text(1)]->moduleId);

                QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");

                itemPro = variantManagerAction->addProperty(QVariant::String, tr("属性2"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>0) {
                    itemPro->setValue(tmpList[0]);
                }

                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("配置方法"));
                QStringList visionModes;
                visionModes<<"探针检测"<<"数据转发"<<"预留";
                itemPro->setAttribute(QLatin1String("enumNames"),visionModes);
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>1) {
                    itemPro->setValue(tmpList[1].toInt());
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输入参数列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>2) {
                    itemPro->setValue(tmpList[2]);
                }
                itemPro = variantManagerAction->addProperty(QVariant::String, tr("输出变量列表"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>3) {
                    itemPro->setValue(tmpList[3]);
                }

                topPropertyAction->setEnabled(true);
                connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                        this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
                return;
            }

            else {
                QuerModuleId();
                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("模型组件"));
                itemPro->setAttribute(QLatin1String("enumNames"),modulNameList);
                index = moduleIdList.indexOf(actionDetailMap[item->text(1)]->moduleId);
            }
            topPropertyAction->addSubProperty(itemPro);
            if(index == -1)
            {
                actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->moduleId ="";
                index =0;
                QtVariantProperty *itemName = (QtVariantProperty *)topPropertyAction->subProperties().at(0);
                itemName->setValue(actionDetailMap[item->text(1)]->name);
                // QSqlQuery query;
                //query.exec(QString("insert into StationActionDetail (moduleId ) values('') where id = '%1' and isUse =1 ").arg(item->text(1)));
            }
            itemPro->setValue(index);
            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("动作类型"));
            QStringList actionMove;
            actionMove<<"显示设定"<<"显示切换"<<"轴运动";
            itemPro->setAttribute(QLatin1String("enumNames"), actionMove);
            itemPro->setValue(actionDetailMap[item->text(1)]->type-1);
            topPropertyAction->addSubProperty(itemPro);

            if(actionDetailMap[item->text(1)]->type == 1)
            {

                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("隐藏"));
                QStringList boolMove;
                boolMove<<"隐藏"<<"显示";
                itemPro->setAttribute(QLatin1String("enumNames"),boolMove);
                topPropertyAction->addSubProperty(itemPro);
                if(actionDetailMap[item->text(1)]->value == "1")
                    itemPro->setValue(1);
                else if(actionDetailMap[item->text(1)]->value == "0")
                    itemPro->setValue(0);
                else
                {
                    itemPro->setValue(0);
                    //  actionDetailMap[item->text(1)]->value == "1";
                    //QSqlQuery query;
                    //query.exec(QString("insert into StationActionDetail (value) values('1') where id = '%1' and isUse =1 ").arg(item->text(1)));
                }

            } else if(actionDetailMap[item->text(1)]->type == 2)
            {

                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("隐藏"));
                QStringList boolMove;
                boolMove<<"隐藏"<<"显示";
                itemPro->setAttribute(QLatin1String("enumNames"),boolMove);
                topPropertyAction->addSubProperty(itemPro);
                if(actionDetailMap[item->text(1)]->value == "1")
                    itemPro->setValue(1);
                else if(actionDetailMap[item->text(1)]->value == "0")
                    itemPro->setValue(0);
                else
                {
                    itemPro->setValue(0);
                    //  actionDetailMap[item->text(1)]->value == "1";
                    //QSqlQuery query;
                    //query.exec(QString("insert into StationActionDetail (value) values('1') where id = '%1' and isUse =1 ").arg(item->text(1)));
                }

            }
//            else if(actionDetailMap[item->text(1)]->type == 2 && actionMap[item->parent()->text(1)]->type != 21 )
//            {

//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("绑定轴"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setAttribute(QLatin1String("enumNames"), axisNameList);
//                int i =  axisIdList.indexOf(actionDetailMap[item->text(1)]->value);

//                if(i == -1)
//                {
//                    i =0;
//                    actionDetailMap[item->text(1)]->value ="";
//                }
//                itemPro->setValue(i);
//            }
            else if(actionDetailMap[item->text(1)]->type == 3 )
            {
                // 轴用组件所绑定的轴代替轴选择
//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("运动轴"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setAttribute(QLatin1String("enumNames"), axisNameList);
//                int i =  axisIdList.indexOf(actionDetailMap[item->text(1)]->value);

//                if(i == -1)
//                {
//                    i =0;
//                    actionDetailMap[item->text(1)]->value ="";
//                }
//                itemPro->setValue(i);

                QStringList tmpList = actionDetailMap[item->text(1)]->value.split(",");

                itemPro = variantManagerAction->addProperty(QVariant::Double, tr("目标位置"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>0) {
                    itemPro->setValue(tmpList[0]);
                }

                itemPro = variantManagerAction->addProperty(QVariant::Double, tr("运动时间(秒)"));
                topPropertyAction->addSubProperty(itemPro);
                if (tmpList.length()>1) {
                    itemPro->setValue(tmpList[1]);
                }
            }



            topPropertyAction->setEnabled(true);
        }
    }

    if(isActionChange){
    if(itemPos >= actItemParent->children().count())
        itemPos = 0;
//    ui.propertyAction->setCurrentItem(actItemParent->children().at(itemPos));
//    ui.propertyAction->editItem(actItemParent->children().at(itemPos));
    isActionChange = false;
    }
//    else
//      ui.propertyAction->editItem(actItemParent->children().at(0));
  connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));


}




  void PageStation::actionPropertyChanged(QtProperty *property, const QVariant &value)
  {


      for(int i = 0; i<actItemParent->children().count(); ++i)
      {
           //qDebug()<<property->propertyName()<<actItemParent->children().at(i)->property()->propertyName() ;
          if(actItemParent->children().at(i)->property()->propertyName() == property->propertyName())
          {
              itemPos = i;
              itemPos++;
              isActionChange = true;
              break;
          }

      }
      if (ui.treeWidgetAction->selectedItems().count()==0)
          return;

      /**************update4 by cxs 2014 4.25*******************/
      //    QTreeWidgetItem *item = ui.treeWidgetAction->selectedItems().last();
      //    if (!item->parent())
      //        return;
       /********************end**************/

      QSqlQuery query;
      QString tableName = "StationAction";
      QString tableNameD = "StationActionDetail";
      QString field;

      /**************update4 by cxs 2014 4.25*******************/
          foreach (QTreeWidgetItem *item, ui.treeWidgetAction->selectedItems())
          {
              if(!item->parent())
                  continue;

      /**********************end*************************/
      if(!item->parent()->parent())
      {

          //ActionObject *actionObject  = actionMap[item->text(1)];
          // int subCout = topPropertyAction->subProperties().count();
          if(property->propertyName() == "名称" )
          {

              actionMap[item->text(1)]->name = value.toString();
              item->setText(0,value.toString());
              // field = "name";
              //query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field,actionMap[item->text(1)]->name, item->text(1)));
          }
          if(property->propertyName() == "主动作类型"  )
          {
              static int sType = 0;
              if(value.toInt() == 0)
                  actionMap[item->text(1)]->type = 1;
              else if(value.toInt() == 1)
                  actionMap[item->text(1)]->type = 2;
              else if(value.toInt() == 2)
                  actionMap[item->text(1)]->type = 3;
              else if(value.toInt() == 3)
              {
                  actionMap[item->text(1)]->type = 11;

              }
              else if(value.toInt() == 4)
                  actionMap[item->text(1)]->type = 12;
              else if(value.toInt() == 5)
              {
                  actionMap[item->text(1)]->type = 21;
              }
              else if(value.toInt() == 6)
              {
                  actionMap[item->text(1)]->type = 31;
              }
              else if(value.toInt() == 7)
              {
                  actionMap[item->text(1)]->type = 32;
              }
              else if(value.toInt() == 8)
              {
                  actionMap[item->text(1)]->type = 33;
              }
              else if(value.toInt() == 9) //7  // 6    // 8
              {
                  actionMap[item->text(1)]->type = 100;
              }
              else if(value.toInt() == 10)
              {
                  actionMap[item->text(1)]->type = 34;
              }
              else if(value.toInt() == 11)
              {
                  actionMap[item->text(1)]->type = 35;
              }
              if(value.toInt() == 3 ||value.toInt() == 4 )
              {
                  actionMap[item->text(1)]->code = "";
                  actionMap[item->text(1)]->oper ="";
                  actionMap[item->text(1)]->value ="";
                  actionMap[item->text(1)]->extTrg = 0;
                  actionMap[item->text(1)]->code2 = "";
                  actionMap[item->text(1)]->oper2 ="";
                  actionMap[item->text(1)]->value2 ="";
                  actionMap[item->text(1)]->extTrg2 = 0;
                  actionMap[item->text(1)]->code3 = "";
                  actionMap[item->text(1)]->oper3 ="";
                  actionMap[item->text(1)]->value3 ="";
                  actionMap[item->text(1)]->count = 0;
                  query.exec(QString("UPDATE %1 set type = %2, code ='' ,operator ='',value ='',extraTrigger= 0 ,code2 ='',   \
                                     operator2 ='', value2 ='', extraTrigger2=0, code3 ='' ,operator3 ='',value3 ='',count =0    \
                          where id = '%3'").arg(tableName,QString::number(actionMap[item->text(1)]->type),item->text(1)));

              }
              else
              {
                  field = "type";
                  query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(actionMap[item->text(1)]->type), item->text(1)));
              }
  //            qDebug()<<"sType" <<sType;
              if(sType == 21 ||actionMap[item->text(1)]->type == 21 ||
                      sType == 31 ||actionMap[item->text(1)]->type == 31
                       || sType == 32 ||actionMap[item->text(1)]->type == 32
                       || sType == 33 ||actionMap[item->text(1)]->type == 33) // 清除非组件明细
              {
                  QTreeWidgetItem *itemc;
                  while(item->childCount())
                  {

                      itemc = item->child(0);
                      item->removeChild(itemc);
                  }
                   query.exec(QString("DELETE FROM  StationActionDetail WHERE  actionId ='%1'").arg(item->text(1)));
              }
              sType = actionMap[item->text(1)]->type;

              actionSelectionChanged();
          }
          else if(property->propertyName() == "信号名"  || property->propertyName() == "变量名" || property->propertyName() == "信号/变量名"
                  || property->propertyName()== "定时次数"|| property->propertyName()=="默认名"  )
          {
              actionMap[item->text(1)]->code = value.toString();
              field = "code";
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));
              /***********end************************/

          }
          else if(property->propertyName() == "条件操作符" )
          {
  //            if(value.toInt() == 0)
  //                actionMap[item->text(1)]->oper = ">";
  //            else if(value.toInt() == 1)
  //                actionMap[item->text(1)]->oper = "<";
  //            else if(value.toInt() == 2)
  //                actionMap[item->text(1)]->oper = "=";
  //            else if(value.toInt() == 3)
  //                actionMap[item->text(1)]->oper = "<>";
              actionMap[item->text(1)]->oper = operList.value(value.toInt());
              field = "operator";
               actionSelectionChanged();
               /*********update by cxs 2014 .4.17******/
                           query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper, item->text(1)));
               /*****************end*****************/
          }
          /**********update3 by cxs 2014 .4.21************
           变量名 改 变量值
           *******************************/
          else if(property->propertyName()== "信号值" || property->propertyName() == "变量值"  ||  property->propertyName() == "信号/变量值"  \
                  || property->propertyName() == "定时时间" ||   property->propertyName() == "默认值" )
          {
              actionMap[item->text(1)]->value = value.toString();
              field = "value";
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value, item->text(1)));
              /**************end*******************/
          }
          else if(property->propertyName() == "附加条件操作符")
          {
              actionMap[item->text(1)]->extTrg  = value.toInt();
              field = "extraTrigger";
              if(value.toInt() == 0)
              {
                  actionMap[item->text(1)]->code2 = "";
                  actionMap[item->text(1)]->oper2 ="";
                  actionMap[item->text(1)]->value2 ="";
                  actionMap[item->text(1)]->extTrg2 = 0;
                  actionMap[item->text(1)]->code3 = "";
                  actionMap[item->text(1)]->oper3 ="";
                  actionMap[item->text(1)]->value3 ="";
                  query.exec(QString("UPDATE %1 set %2 = %3, code2 ='', operator2 ='', value2 ='', extraTrigger2=0, code3 ='' ,  \
                                     operator3 ='',value3 ='' where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
              }
              else
              {
   /*****************update2  by cxs 2014.4.18********/
                  if(actionMap[item->text(1)]->oper2 == "")
                  {
                      actionMap[item->text(1)]->oper2 = "=";
                      query.exec(QString("UPDATE %1 set %2 = %3, operator2 ='%4' where id = '%5'").arg(tableName, field, QString::number(value.toInt()),actionMap[item->text(1)]->oper2, item->text(1)));
                  }
                  else
                      query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
                  /******************end********************/
              }
              actionSelectionChanged();
          }
          else if(property->propertyName() == "信号名2"  || property->propertyName() == "变量名2" || property->propertyName() == "信号/变量名2"
                  ||  property->propertyName()== "定时次数2"|| property->propertyName()=="默认名2" )
          {
              actionMap[item->text(1)]->code2 = value.toString();
              field = "code2";
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));
              /********************end****************/

          }
          else if(property->propertyName() == "条件操作符2" )
          {
  //            if(value.toInt() == 0)
  //                actionMap[item->text(1)]->oper2 = ">";
  //            else if(value.toInt() == 1)
  //                actionMap[item->text(1)]->oper2 = "<";
  //            else if(value.toInt() == 2)
  //                actionMap[item->text(1)]->oper2 = "=";
  //            else if(value.toInt() == 3)
  //                actionMap[item->text(1)]->oper2 = "<>";
              actionMap[item->text(1)]->oper2 = operList.value(value.toInt());

              field = "operator2";
               actionSelectionChanged();
               /*********update by cxs 2014 .4.17******/
                           query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper2, item->text(1)));
               /***************************end*********/
          }
          /**********update3 by cxs 2014 .4.21************
                           变量名2 改 变量值2
           *******************************/
          else if(property->propertyName()== "信号值2" || property->propertyName() == "变量值2" || property->propertyName() == "信号/变量值2"    \
                  || property->propertyName() == "定时时间2" ||    property->propertyName() == "默认值2")
          {
              actionMap[item->text(1)]->value2 = value.toString();
              field = "value2";
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value2, item->text(1)));
               /*****************end**********/
          }
          else if(property->propertyName() == "附加条件2操作符")
          {
              actionMap[item->text(1)]->extTrg2  = value.toInt();
              field = "extraTrigger2";
              if(value.toInt() == 0)
              {
                  actionMap[item->text(1)]->code3 = "";
                  actionMap[item->text(1)]->oper3 ="";
                  actionMap[item->text(1)]->value3 ="";
                  query.exec(QString("UPDATE %1 set %2 = %3, code3 ='',operator3 ='',value3 ='' where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));

              }
              /***************update2 by cxs 2014.4.18*******/
              else
              {
                  if(actionMap[item->text(1)]->oper3 == "")
                  {
                     actionMap[item->text(1)]->oper3 = "=";
                     query.exec(QString("UPDATE %1 set %2 = %3, operator3 ='%4' where id = '%5'").arg(tableName, field, QString::number(value.toInt()),actionMap[item->text(1)]->oper3, item->text(1)));
                  }

                  else
                      query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
              }
              /*****************end**********/
              actionSelectionChanged();
          }
          else if(property->propertyName() == "信号名3"  || property->propertyName() == "变量名3" || property->propertyName() == "信号/变量名3"
                  || property->propertyName()== "定时次数3"|| property->propertyName()=="默认名3")
          {
              actionMap[item->text(1)]->code3 = value.toString();
              field = "code3";
              /*********update by cxs 2014 .4.17******/
                        query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));
             /***************end***************/
          }
          else if(property->propertyName() == "条件操作符3" )
          {
  //            if(value.toInt() == 0)
  //                actionMap[item->text(1)]->oper3 = ">";
  //            else if(value.toInt() == 1)
  //                actionMap[item->text(1)]->oper3 = "<";
  //            else if(value.toInt() == 2)
  //                actionMap[item->text(1)]->oper3 = "=";
  //            else if(value.toInt() == 3)
  //                actionMap[item->text(1)]->oper3 = "<>";
             actionMap[item->text(1)]->oper3 = operList.value(value.toInt());

             field = "operator3";
              actionSelectionChanged();
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper3, item->text(1)));
              /****************end***************/

          }
          /******************updata3  by cxs 2014.4.21*************************
                  变量名3 改为 变量值3
           ************************end************/
          else if(property->propertyName()== "信号值3" || property->propertyName() == "变量值3"  ||  property->propertyName() == "信号/变量值3"   \
                  ||  property->propertyName() == "定时时间3" ||    property->propertyName() == "默认值3" )
          {
              actionMap[item->text(1)]->value3 = value.toString();
              field = "value3";
              /*********update by cxs 2014 .4.17******/
                          query.exec(QString("UPDATE %1 set %2 = '%3'  where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value3, item->text(1)));
               /********************end**************/
          }
          else if(property->propertyName()== "动作执行次数")
          {
              actionMap[item->text(1)]->count = value.toInt();
              field = "count";
              query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
          }
          else if(property->propertyName()== "更新方式")
          {
              actionMap[item->text(1)]->updateType = value.toInt()+1;
              field = "updateType";

              query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()+1), item->text(1)));
          }
      }


      //明细动作
      else{
          if(property->propertyName() == "名称")
          {
              actionSelectionChanged();
              // actionDetailMap[item->text(1)]->name = value.toString();
              //item->setText(0,value.toString());
          }
          else if(property->propertyName() == "动作类型")
          {
              actionDetailMap[item->text(1)]->type = value.toInt()+1;
              field = "type";
  //            QString fvalue;
              if(value.toInt() == 0)
              {
                  actionDetailMap[item->text(1)]->value ="0";
              }
              else
                  actionDetailMap[item->text(1)]->value= " ";

              query.exec(QString("UPDATE %1 set %2 = %3, value = '%4' where id = '%5'").arg(tableNameD, field, QString::number( actionDetailMap[item->text(1)]->type),actionDetailMap[item->text(1)]->value, item->text(1)));
              actionSelectionChanged();

          }
          else if(property->propertyName() == "刀具"  || property->propertyName() == "模型组件")
          {
              //QString id;
              if(property->propertyName() == "刀具" )
              {
                  actionDetailMap[item->text(1)]->moduleId  = knifeIdList.value(value.toInt());
                  actionDetailMap[item->text(1)]->name =  knifeList.value(value.toInt());
                  // id = knifeIdList.value(value.toInt());
              }
              else if(property->propertyName() == "模型组件" )
              {
                  actionDetailMap[item->text(1)]->moduleId = moduleIdList.value(value.toInt());
                  actionDetailMap[item->text(1)]->name =  modulNameList.value(value.toInt());
                  //id = moduleIdList.value(value.toInt());
              }

              item->setText(0, actionDetailMap[item->text(1)]->name);
              actionSelectionChanged();
              field = "moduleId";
              query.exec(QString("UPDATE %1 set %2 = '%3' ,name ='%4'  where id = '%5'").arg(tableNameD, field,actionDetailMap[item->text(1)]->moduleId ,
                         actionDetailMap[item->text(1)]->name, item->text(1)));

          }
  //        else if(property->propertyName() == "保存文件名" || property->propertyName() == "测量页号")
  //        {
  //            //QString id;
  //            actionDetailMap[item->text(1)]->name = value.toString();
  //            item->setText(0, actionDetailMap[item->text(1)]->name);
  ////            actionSelectionChanged();
  //            field = "moduleId";
  //            query.exec(QString("UPDATE %1 set %2 = '%3' ,name ='%4'  where id = '%5'").arg(tableNameD, field,actionDetailMap[item->text(1)]->moduleId ,
  //                       actionDetailMap[item->text(1)]->name, item->text(1)));

  //        }
          else if(property->propertyName().contains("隐藏"))
          {
              if(value.toInt() == 0)
                  actionDetailMap[item->text(1)]->value = "0";
              else if(value.toInt() ==1)
                  actionDetailMap[item->text(1)]->value ="1";
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field,actionDetailMap[item->text(1)]->value  , item->text(1)));
          }
          else if(property->propertyName().contains("绑定轴"))
          {
              actionDetailMap[item->text(1)]->value = axisIdList.value(value.toInt());
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, axisIdList.value(value.toInt()) , item->text(1)));
          }
          else if(property->propertyName().contains("目标位置"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split(",");
              tmpList[0] = QString::number(value.toDouble(), 'g', 13);
              actionDetailMap[item->text(1)]->value = tmpList.join(",");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
          }
          else if(property->propertyName().contains("运动时间"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split(",");
              if (tmpList.length()<2){
                  tmpList.append(QString::number(value.toDouble(), 'g', 13));
              } else {
                  tmpList[1] = QString::number(value.toDouble(), 'g', 13);
              }

              actionDetailMap[item->text(1)]->value = tmpList.join(",");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
          }
  //        else if(property->propertyName().contains("保存文件名"))
  //        {
  //            actionDetailMap[item->text(1)]->value = value.toString();
  //            actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->value;
  //            item->setText(0, actionDetailMap[item->text(1)]->name);
  //            field = "value";
  //            query.exec(QString("UPDATE %1 set name='%3',%2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  ////            actionSelectionChanged();
  //        }
          else if(property->propertyName().contains("测量页号"))
          {
              actionDetailMap[item->text(1)]->value = QString::number(value.toInt());
              actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->value;
              item->setText(0, actionDetailMap[item->text(1)]->name);
              field = "value";
              query.exec(QString("UPDATE %1 set name='%3',%2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("视觉相机编号"))
          {
              actionDetailMap[item->text(1)]->moduleId = value.toString();
              actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->moduleId;
              item->setText(0, actionDetailMap[item->text(1)]->name);
              field = "moduleId";
              query.exec(QString("UPDATE %1 set name='%3',%2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->moduleId, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("视觉模板编号"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[0] = value.toString();  //QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
          }
          else if(property->propertyName().contains("属性1"))
          {
              actionDetailMap[item->text(1)]->moduleId = value.toString();
              actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->moduleId;
              item->setText(0, actionDetailMap[item->text(1)]->name);
              field = "moduleId";
              query.exec(QString("UPDATE %1 set name='%3',%2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->moduleId, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("属性2"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[0] = value.toString();  //QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
          }
          else if(property->propertyName().contains("视觉算法"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[1] = QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("配置方法"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[1] = QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("输入参数列表"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[2] = value.toString();  //QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }
          else if(property->propertyName().contains("输出变量列表"))
          {
              QStringList tmpList = actionDetailMap[item->text(1)]->value.split("|");
              while (tmpList.length()<4) tmpList.append("");
              tmpList[3] = value.toString();  //QString::number(value.toInt());
              actionDetailMap[item->text(1)]->value = tmpList.join("|");
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }
          else
          {
              actionDetailMap[item->text(1)]->value = value.toString();
              field = "value";
              query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, actionDetailMap[item->text(1)]->value, item->text(1)));
  //            actionSelectionChanged();
          }


      }

      /***************update4 by cxs *********/
       }
      /***************end************/
  }

void PageStation::modulePropertyChanged(QtProperty *property, const QVariant &value)
{
    if (!property->isEnabled())
        return;

    if (ui.treeWidgetModule->selectedItems().count()==0)
        return;

    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

try{
    QSqlQuery query;

    if(!QSqlDatabase::database().transaction())
            qDebug()<<"事务开启失败";

//    QTreeWidgetItem *item = ui.treeWidgetModule->selectedItems().last();
    foreach (QTreeWidgetItem *item, ui.treeWidgetModule->selectedItems()) {
        if (!item->parent())
            continue;
        ModuleObject *module = moduleMap[item->text(1)];
        if (property->propertyName() == tr("隐藏")) {
            if (value.toBool() == module->isHidden)
                continue;
            module->isHidden = value.toBool();
            if (value.toBool()) {
                module->aisShape->GetContext()->Erase(module->aisShape, true);
            } else {
                module->aisShape->GetContext()->Display(module->aisShape, true);
            }

            if ( query.exec(QString("UPDATE StationModule SET isShow=%1 WHERE id='%2'").arg(QString::number(module->isHidden ? 1 : 0), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("名称")) {
            if (value.toString() == module->name)
                continue;
            module->name = value.toString();
            item->setText(0, module->name);
        } else if (property->propertyName() == tr("关联轴")) {
            if (value.toInt() > 0) {
                foreach (AxisObject *axis, axisMap.values()) {
                    if (axisNameList.at(value.toInt()) == axis->name) {
                        if (axis->id == module->bindAxis)
                            continue;
                        module->bindAxis = axis->id;
                        break;
                    }
                }
                if (!module->bindAxis.isEmpty()) {
                    axisMap[module->bindAxis]->moduleList.removeOne(module->id);
                    axisMap[module->bindAxis]->moduleList.append(module->id);
                }
            } else {
                if (!module->bindAxis.isEmpty()) {
                    axisMap[module->bindAxis]->moduleList.removeOne(module->id);
                    module->bindAxis = "";
                } else {
                    continue;
                }
            }
            if ( query.exec(QString("UPDATE StationModule SET bindAxis='%1' WHERE id='%2'").arg(module->bindAxis, module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("X偏移")) {
            if (value.toDouble() == module->pos[X])
                continue;
            module->pos[X] = value.toDouble();
            //            if (module->rotateAngle!=0.0) {
            //                gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
            //                module->transform = gp_Trsf();
            //                module->transform.SetRotation(ax, module->rotateAngle);
            //                module->currentLoc = TopLoc_Location(module->transform);
            //            }
            //            module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
                        //module->returnModouleOriginalLocation();
            module->updateModouleMatrix();
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);

            if ( query.exec(QString("UPDATE StationModule SET x=%1 WHERE id='%2'").arg(QString::number(module->pos[X],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("Y偏移")) {
            if (value.toDouble() == module->pos[Y])
                continue;
            module->pos[Y] = value.toDouble();
            module->updateModouleMatrix();
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET y=%1 WHERE id='%2'").arg(QString::number(module->pos[Y],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("Z偏移")) {
            if (value.toDouble() == module->pos[Z])
                continue;
            module->pos[Z] = value.toDouble();
            module->updateModouleMatrix();
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET z=%1 WHERE id='%2'").arg(QString::number(module->pos[Z],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败! ", true);
                continue;
            }
        } else if (property->propertyName() == tr("X轴旋转")) {
            if (value.toDouble() == module->rotateDirection[X])
                continue;
            module->rotateDirection[X] = value.toDouble();
            if (module->rotateAngle!=0.0) {
                gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
                module->transform = gp_Trsf();
                module->transform.SetRotation(ax, module->rotateAngle);
                module->currentLoc = TopLoc_Location(module->transform);
            }
            module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);

            if ( query.exec(QString("UPDATE StationModule SET rotateX=%1 WHERE id='%2'").arg(QString::number(module->rotateDirection[X],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("Y轴旋转")) {
            if (value.toDouble() == module->rotateDirection[Y])
                continue;
            module->rotateDirection[Y] = value.toDouble();
            if (module->rotateAngle!=0.0) {
                gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
                module->transform = gp_Trsf();
                module->transform.SetRotation(ax, module->rotateAngle);
                module->currentLoc = TopLoc_Location(module->transform);
            }
            module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET rotateY=%1 WHERE id='%2'").arg(QString::number(module->rotateDirection[Y],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("Z轴旋转")) {
            if (value.toDouble() == module->rotateDirection[Z])
                continue;
            module->rotateDirection[Z] = value.toDouble();
            if (module->rotateAngle!=0.0) {
                gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
                module->transform = gp_Trsf();
                module->transform.SetRotation(ax, module->rotateAngle);
                module->currentLoc = TopLoc_Location(module->transform);
            }
            module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET rotateZ=%1 WHERE id='%2'").arg(QString::number(module->rotateDirection[Z],'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("旋转角度")) {
            double angle = value.toDouble() * M_PI / 180;
            if (angle == module->rotateAngle)
                continue;
            module->rotateAngle = angle;
//            if (module->rotateAngle!=0.0) {
                gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
                module->transform = gp_Trsf();
                module->transform.SetRotation(ax, module->rotateAngle);
//            }
            module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
            TopoDS_Shape shape = module->aisShape->Shape();
            shape.Location(TopLoc_Location(module->transform));
            module->currentLoc = TopLoc_Location(module->transform);
            module->aisShape->Set(shape);
            module->aisShape->Redisplay();
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET initAngle=%1 WHERE id='%2'").arg(QString::number(module->rotateAngle,'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("材质")) {
            if (value.toInt() == module->material)
                continue;
            module->material = value.toInt();
            if (value.toInt()>18) {
                Quantity_Color color;
                Quantity_Color::Argb2color(module->color, color);
                module->aisShape->SetColor( color );
            } else {
                module->aisShape->UnsetColor();
            }
            module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)value.toInt() );
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET material=%1 WHERE id='%2'").arg(QString::number(module->material), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("颜色")) {
            QColor cc = qVariantValue<QColor>(value);
            int t = cc.red() << 16 | cc.green()<<8 | cc.blue();
            if (t == module->color)
                continue;
            module->color = t;
            if (module->material>18) {
                Quantity_Color color;
                Quantity_Color::Argb2color(module->color, color);
                module->aisShape->SetColor( color );
            } else {
                module->aisShape->UnsetColor();
            }
            module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET color=%1 WHERE id='%2'").arg(QString::number(module->color), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        } else if (property->propertyName() == tr("透明度")) {
            double transRate = value.toInt() * 0.01;
            if (transRate == module->transparency)
                continue;
            module->transparency = transRate;
            module->aisShape->SetTransparency(transRate);
            module->aisShape->GetContext()->Update(module->aisShape, true);
            if ( query.exec(QString("UPDATE StationModule SET transparent=%1 WHERE id='%2'").arg(QString::number(transRate,'g',13), module->id)) ) {
                qDebug("修改成功！");
            } else {
                qDebug("修改失败！", true);
                continue;
            }
        }

    }
    }catch(...){
        // Ignore
        qDebug("属性修改失败！", true);
    }

    QSqlDatabase::database().commit();
    qApp->restoreOverrideCursor();
}

void PageStation::shapeSelected(QStringList currentModuleIdList, bool isMulti)
{
    bool isSelected = false;
    //ui.treeWidgetModule->setSelectionMode(QAbstractItemView.SelectionMode.MultiSelection);
    QTreeWidgetItemIterator it(ui.treeWidgetModule);
    while (*it) {
        for ( int i=0; i!=currentModuleIdList.size(); ++i )
        {
           if ( currentModuleIdList.at(i) == (*it)->text(1))
           {
               if(isMulti)
               {
                   ui.treeWidgetModule->setItemSelected(*it,true);
               }else
               {
                   ui.treeWidgetModule->setCurrentItem(*it);
               }
               isSelected = true;
           }

        }
        ++it;
    }
    if(!isSelected)
    {
        ui.treeWidgetModule->clearSelection();
    }

//    QTreeWidgetItemIterator it(ui.treeWidgetModule);
//    while (*it) {
//         if ((*it)->text(1) == currentModuleId) {
//                ui.treeWidgetModule->setCurrentItem(*it);
//             return;
//         }
//        ++it;
//    }
//    ui.treeWidgetModule->clearSelection();
}

void PageStation::changeStationTab(int index)
{
    ui.tabWidget->setCurrentIndex( index );
}

void PageStation::moduleAllLoaded( int count )
{
    showAllArea();
    ui.comboStationName->setEnabled(true);
    ui.tabWidget->setEnabled(true);
}

/*********************************
 *add by cxs 2014 4.15
 *******************************/
//void PageStation::actionSelectionChanged()
//{
//    if (!this->isVisible()) return;

//    disconnect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//               this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
//    if(ui.treeWidgetAction->selectedItems().count()>0)
//    {

//        QTreeWidgetItem *item = ui.treeWidgetAction->selectedItems().last();
//        if (!item->parent()){
//            topPropertyAction->setEnabled(false);
//            return;

//        }
//        foreach (QtProperty *property, topPropertyAction->subProperties())
//            topPropertyAction->removeSubProperty(property);

//        //动作部分
//        if(!item->parent()->parent()){

//            QtVariantProperty *itemPro = variantManagerAction->addProperty(QVariant::String, tr("名称"));
//            topPropertyAction->addSubProperty(itemPro);
//            itemPro->setValue(actionMap[item->text(1)]->name);
//            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("主动作类型"));
//            itemPro->setAttribute(QLatin1String("enumNames"), actionList);
//            topPropertyAction->addSubProperty(itemPro);
//            //ui.propertyAction->setCurrentItem();
//            QString codeName, valueName;
//            switch(actionMap[item->text(1)]->type)
//            {
//            case 1:
//                codeName = "信号名";
//                valueName = "信号值";
//                itemPro->setValue(0);
//                break;
//            case 2:
//                codeName = "变量名";
//                valueName = "变量值";
//                itemPro->setValue(1);
//                break;
//            case 3:
//                codeName = "信号/变量名";
//                valueName = "信号/变量值";
//                itemPro->setValue(2);
//                break;
//            case 11:
//                codeName = "定时次数";
//                valueName = "定时时间";
//                itemPro->setValue(3);
//                break;
//            case 12:
//                codeName = "始终动作";
//                //valueName = "默认值";
//                itemPro->setValue(4);
//                break;
//            case 21:
//                codeName = "信号/变量名";
//                valueName = "信号/变量值";
//                itemPro->setValue(5);
//                break;
//            }


//            if(actionMap[item->text(1)]->type != 12)
//            {
//                itemPro = variantManagerAction->addProperty(QVariant::String, codeName);
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setValue(actionMap[item->text(1)]->code);

//                if(actionMap[item->text(1)]->type == 11)
//                {
//                    itemPro = variantManagerAction->addProperty(QVariant::String, valueName);
//                    topPropertyAction->addSubProperty(itemPro);
//                    itemPro->setValue(actionMap[item->text(1)]->value);
//                }
//                else
//                {
//                    itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符"));
//                    topPropertyAction->addSubProperty(itemPro);
//                    itemPro->setAttribute(QLatin1String("enumNames"), operList);
//                    int index = operList.indexOf(actionMap[item->text(1)]->oper);
//                    if(index == -1)
//                        index = 2;
//                    itemPro->setValue(index);
//                    itemPro = variantManagerAction->addProperty(QVariant::String, valueName);
//                    topPropertyAction->addSubProperty(itemPro);
//                    itemPro->setValue(actionMap[item->text(1)]->value);
//                    itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("附加条件操作符"));
//                    itemPro->setAttribute(QLatin1String("enumNames"), exterList);
//                    index = actionMap[item->text(1)]->extTrg;
//                    itemPro->setValue(index);
//                    topPropertyAction->addSubProperty(itemPro);
//                    if(actionMap[item->text(1)]->extTrg != 0)
//                    {
//                        itemPro = variantManagerAction->addProperty(QVariant::String, codeName +"2");
//                        topPropertyAction->addSubProperty(itemPro);
//                        itemPro->setValue(actionMap[item->text(1)]->code2);
//                        itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符2"));
//                        topPropertyAction->addSubProperty(itemPro);
//                        itemPro->setAttribute(QLatin1String("enumNames"), operList);
//                        int index = operList.indexOf(actionMap[item->text(1)]->oper2);
//                        if(index == -1)
//                            index = 2;
//                        itemPro->setValue(index);
//                        itemPro = variantManagerAction->addProperty(QVariant::String, valueName+"2");
//                        topPropertyAction->addSubProperty(itemPro);
//                        itemPro->setValue(actionMap[item->text(1)]->value2);
//                        itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("附加条件2操作符"));
//                        itemPro->setAttribute(QLatin1String("enumNames"), exterList);
//                        index = actionMap[item->text(1)]->extTrg2;
//                        itemPro->setValue(index);
//                        topPropertyAction->addSubProperty(itemPro);
//                        if(actionMap[item->text(1)]->extTrg2 != 0){
//                            itemPro = variantManagerAction->addProperty(QVariant::String, codeName+"3");
//                            topPropertyAction->addSubProperty(itemPro);
//                            itemPro->setValue(actionMap[item->text(1)]->code3);
//                            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("条件操作符3"));
//                            topPropertyAction->addSubProperty(itemPro);
//                            itemPro->setAttribute(QLatin1String("enumNames"), operList);
//                            int index = operList.indexOf(actionMap[item->text(1)]->oper3);
//                            if(index == -1)
//                                index = 2;
//                            itemPro->setValue(index);
//                            itemPro = variantManagerAction->addProperty(QVariant::String, valueName +"3");
//                            topPropertyAction->addSubProperty(itemPro);
//                            itemPro->setValue(actionMap[item->text(1)]->value3);



//                        }
//                    }
//                }
//                itemPro = variantManagerAction->addProperty(QVariant::Int, tr("动作执行次数"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setValue(actionMap[item->text(1)]->count);
//            }
//            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("更新方式"));
//            QStringList updateMove;
//            updateMove<<"绝对动作"<<"相对动作";
//            itemPro->setAttribute(QLatin1String("enumNames"), updateMove);
//            topPropertyAction->addSubProperty(itemPro);
//            if(actionMap[item->text(1)]->updateType != 2 || actionMap[item->text(1)]->updateType != 1 )
//                actionMap[item->text(1)]->updateType = 2;
//            itemPro->setValue(actionMap[item->text(1)]->updateType-1);
//            topPropertyAction->setEnabled(true);

//            // return;
//        }

//        //动作明细部分
//        else
//        {
//            // topPropertyAction = variantManagerAction->addProperty(QtVariantPropertyManager::groupTypeId(), tr("动作明细"));
//            QtVariantProperty *itemPro = variantManagerAction->addProperty(QVariant::String, tr("名称"));
//            topPropertyAction->addSubProperty(itemPro);
//            itemPro->setValue(actionDetailMap[item->text(1)]->name);


//            //qDebug()<<actionMap[item->parent()->text(1)]->type ;
//            int index;
//            if(actionMap[item->parent()->text(1)]->type == 21 )
//            {
//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("刀具"));
//                itemPro->setAttribute(QLatin1String("enumNames"),knifeList);
//                topPropertyAction->addSubProperty(itemPro);
//                index = knifeIdList.indexOf(actionDetailMap[item->text(1)]->moduleId);
//                itemPro->setValue(index);
//                topPropertyAction->setEnabled(true);
//                 connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//                         this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));
//                 return;
//            }
//            else
//            {
//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("模型组件"));
//                itemPro->setAttribute(QLatin1String("enumNames"),modulNameList);
//                index = moduleIdList.indexOf(actionDetailMap[item->text(1)]->moduleId);
//            }
//            topPropertyAction->addSubProperty(itemPro);
//            if(index == -1)
//            {
//                actionDetailMap[item->text(1)]->name = actionDetailMap[item->text(1)]->moduleId ="";
//                index =0;
//                QtVariantProperty *itemName = (QtVariantProperty *)topPropertyAction->subProperties().at(0);
//                itemName->setValue(actionDetailMap[item->text(1)]->name);
//                // QSqlQuery query;
//                //query.exec(QString("insert into StationActionDetail (moduleId ) values('') where id = '%1' and isUse =1 ").arg(item->text(1)));
//            }
//            itemPro->setValue(index);
//            itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("动作类型"));
//            QStringList actionMove;
//            actionMove<<"显示切换"<<"显示设定";
//            itemPro->setAttribute(QLatin1String("enumNames"), actionMove);
//            itemPro->setValue(actionDetailMap[item->text(1)]->type-1);
//            topPropertyAction->addSubProperty(itemPro);

//            if(actionDetailMap[item->text(1)]->type == 1)
//            {

//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("隐藏"));
//                QStringList boolMove;
//                boolMove<<"隐藏"<<"显示";
//                itemPro->setAttribute(QLatin1String("enumNames"),boolMove);
//                topPropertyAction->addSubProperty(itemPro);
//                if(actionDetailMap[item->text(1)]->value == "1")
//                    itemPro->setValue(1);
//                else if(actionDetailMap[item->text(1)]->value == "0")
//                    itemPro->setValue(0);
//                else
//                {
//                    itemPro->setValue(0);
//                    //  actionDetailMap[item->text(1)]->value == "1";
//                    //QSqlQuery query;
//                    //query.exec(QString("insert into StationActionDetail (value) values('1') where id = '%1' and isUse =1 ").arg(item->text(1)));
//                }

//            }
//            else if(actionDetailMap[item->text(1)]->type == 2 && actionMap[item->parent()->text(1)]->type != 21 )
//            {

//                itemPro = variantManagerAction->addProperty(QtVariantPropertyManager::enumTypeId(), tr("绑定轴"));
//                topPropertyAction->addSubProperty(itemPro);
//                itemPro->setAttribute(QLatin1String("enumNames"), axisNameList);
//                int i =  axisIdList.indexOf(actionDetailMap[item->text(1)]->value);

//                if(i == -1)
//                {
//                    i =0;
//                    actionDetailMap[item->text(1)]->value ="";
//                }
//                itemPro->setValue(i);
//            }



//            topPropertyAction->setEnabled(true);
//        }
//    }
//    connect(variantManagerAction, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
//            this, SLOT(actionPropertyChanged(QtProperty *, const QVariant &)));


//}


//void PageStation::actionPropertyChanged(QtProperty *property, const QVariant &value)
//{

//    if (ui.treeWidgetAction->selectedItems().count()==0)
//        return;

//    QTreeWidgetItem *item = ui.treeWidgetAction->selectedItems().last();
//    if (!item->parent())
//        return;
//    QSqlQuery query;
//    QString tableName = "StationAction";
//    QString tableNameD = "StationActionDetail";
//    QString field;
//    if(!item->parent()->parent())
//    {

//        //ActionObject *actionObject  = actionMap[item->text(1)];
//        // int subCout = topPropertyAction->subProperties().count();
//        if(property->propertyName() == "名称" )
//        {

//            actionMap[item->text(1)]->name = value.toString();
//            item->setText(0,value.toString());
//            // field = "name";
//            //query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field,actionMap[item->text(1)]->name, item->text(1)));
//        }
//        if(property->propertyName() == "主动作类型"  )
//        {
//            static int sType = 0;
//            if(value.toInt() == 0)
//                actionMap[item->text(1)]->type = 1;
//            else if(value.toInt() == 1)
//                actionMap[item->text(1)]->type = 2;
//            else if(value.toInt() == 2)
//                actionMap[item->text(1)]->type = 3;
//            else if(value.toInt() == 3)
//            {
//                actionMap[item->text(1)]->type = 11;

//            }
//            else if(value.toInt() == 4)
//                actionMap[item->text(1)]->type = 12;
//            else if(value.toInt() == 5)
//            {
//                actionMap[item->text(1)]->type = 21;

//            }
//            if(value.toInt() == 3 ||value.toInt() == 4 )
//            {
//                actionMap[item->text(1)]->code = "";
//                actionMap[item->text(1)]->oper ="";
//                actionMap[item->text(1)]->value ="";
//                actionMap[item->text(1)]->extTrg = 0;
//                actionMap[item->text(1)]->code2 = "";
//                actionMap[item->text(1)]->oper2 ="";
//                actionMap[item->text(1)]->value2 ="";
//                actionMap[item->text(1)]->extTrg2 = 0;
//                actionMap[item->text(1)]->code3 = "";
//                actionMap[item->text(1)]->oper3 ="";
//                actionMap[item->text(1)]->value3 ="";
//                actionMap[item->text(1)]->count = 0;
//                query.exec(QString("UPDATE %1 set type = %2, code ='' ,operator ='',value ='',extraTrigger= 0 ,code2 ='',   \
//                                   operator2 ='', value2 ='', extraTrigger2=0, code3 ='' ,operator3 ='',value3 ='',count =0    \
//                        where id = '%3'").arg(tableName,QString::number(actionMap[item->text(1)]->type),item->text(1)));

//            }
//            else
//            {
//                field = "type";
//                query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(actionMap[item->text(1)]->type), item->text(1)));
//            }
////            qDebug()<<"sType" <<sType;
//            if(sType == 21 ||actionMap[item->text(1)]->type == 21)
//            {
//                QTreeWidgetItem *itemc;
//                while(item->childCount())
//                {

//                    itemc = item->child(0);
//                    item->removeChild(itemc);
//                }
//                 query.exec(QString("DELETE FROM  StationActionDetail WHERE  actionId ='%1'").arg(item->text(1)));
//            }
//            sType = actionMap[item->text(1)]->type;

//            actionSelectionChanged();
//        }
//        else if(property->propertyName() == "信号名"  || property->propertyName() == "变量名" || property->propertyName() == "信号/变量名"
//                || property->propertyName()== "定时次数"|| property->propertyName()=="默认名"  )
//        {
//            actionMap[item->text(1)]->code = value.toString();
//            field = "code";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));

//        }
//        else if(property->propertyName() == "条件操作符" )
//        {
//            if(value.toInt() == 0)
//                actionMap[item->text(1)]->oper = ">";
//            else if(value.toInt() == 1)
//                actionMap[item->text(1)]->oper = "<";
//            else if(value.toInt() == 2)
//                actionMap[item->text(1)]->oper = "=";
//            else if(value.toInt() == 3)
//                actionMap[item->text(1)]->oper = "<>";
//            field = "operator";
//            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper, item->text(1)));
//        }
//        else if(property->propertyName()== "信号值" || property->propertyName() == "变量名"  ||  property->propertyName() == "信号/变量值"  \
//                || property->propertyName() == "定时时间" ||   property->propertyName() == "默认值" )
//        {
//            actionMap[item->text(1)]->value = value.toString();
//            field = "value";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value, item->text(1)));
//        }
//        else if(property->propertyName() == "附加条件操作符")
//        {
//            actionMap[item->text(1)]->extTrg  = value.toInt();
//            field = "extraTrigger";
//            if(value.toInt() == 0)
//            {
//                actionMap[item->text(1)]->code2 = "";
//                actionMap[item->text(1)]->oper2 ="";
//                actionMap[item->text(1)]->value2 ="";
//                actionMap[item->text(1)]->extTrg2 = 0;
//                actionMap[item->text(1)]->code3 = "";
//                actionMap[item->text(1)]->oper3 ="";
//                actionMap[item->text(1)]->value3 ="";
//                query.exec(QString("UPDATE %1 set %2 = %3, code2 ='', operator2 ='', value2 ='', extraTrigger2=0, code3 ='' ,  \
//                                   operator3 ='',value3 ='' where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
//            }
//            else
//                query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
//            actionSelectionChanged();
//        }
//        else if(property->propertyName() == "信号名2"  || property->propertyName() == "变量名2" || property->propertyName() == "信号/变量名2"
//                ||  property->propertyName()== "定时次数2"|| property->propertyName()=="默认名2" )
//        {
//            actionMap[item->text(1)]->code2 = value.toString();
//            field = "code2";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));

//        }
//        else if(property->propertyName() == "条件操作符2" )
//        {

//            if(value.toInt() == 0)
//                actionMap[item->text(1)]->oper2 = ">";
//            else if(value.toInt() == 1)
//                actionMap[item->text(1)]->oper2 = "<";
//            else if(value.toInt() == 2)
//                actionMap[item->text(1)]->oper2 = "=";
//            else if(value.toInt() == 3)
//                actionMap[item->text(1)]->oper2 = "<>";
//            field = "operator2";
//            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper2, item->text(1)));

//        }
//        else if(property->propertyName()== "信号值2" || property->propertyName() == "变量名2" || property->propertyName() == "信号/变量值2"    \
//                || property->propertyName() == "定时时间2" ||    property->propertyName() == "默认值2")
//        {
//            actionMap[item->text(1)]->value2 = value.toString();
//            field = "value2";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value2, item->text(1)));
//        }
//        else if(property->propertyName() == "附加条件2操作符")
//        {
//            actionMap[item->text(1)]->extTrg2  = value.toInt();
//            field = "extraTrigger2";
//            if(value.toInt() == 0)
//            {
//                actionMap[item->text(1)]->code3 = "";
//                actionMap[item->text(1)]->oper3 ="";
//                actionMap[item->text(1)]->value3 ="";
//                query.exec(QString("UPDATE %1 set %2 = %3, code3 ='',operator3 ='',value3 ='' where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));

//            }
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
//            actionSelectionChanged();
//        }
//        else if(property->propertyName() == "信号名3"  || property->propertyName() == "变量名3" || property->propertyName() == "信号/变量名3"
//                || property->propertyName()== "定时次数3"|| property->propertyName()=="默认名3")
//        {
//            actionMap[item->text(1)]->code3 = value.toString();
//            field = "code3";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, value.toString(), item->text(1)));

//        }
//        else if(property->propertyName() == "条件操作符3" )
//        {
//            if(value.toInt() == 0)
//                actionMap[item->text(1)]->oper3 = ">";
//            else if(value.toInt() == 1)
//                actionMap[item->text(1)]->oper3 = "<";
//            else if(value.toInt() == 2)
//                actionMap[item->text(1)]->oper3 = "=";
//            else if(value.toInt() == 3)
//                actionMap[item->text(1)]->oper3 = "<>";
//           field = "operator3";
//            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->oper3, item->text(1)));

//        }
//        else if(property->propertyName()== "信号值3" || property->propertyName() == "变量名3"  ||  property->propertyName() == "信号/变量值3"   \
//                ||  property->propertyName() == "定时时间3" ||    property->propertyName() == "默认值3" )
//        {
//            actionMap[item->text(1)]->value3 = value.toString();
//            field = "value3";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, actionMap[item->text(1)]->value3, item->text(1)));
//        }
//        else if(property->propertyName()== "动作执行次数")
//        {
//            actionMap[item->text(1)]->count = value.toInt();
//            field = "count";
//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()), item->text(1)));
//        }
//        else if(property->propertyName()== "更新方式")
//        {
//            actionMap[item->text(1)]->updateType = value.toInt()+1;
//            field = "updateType";

//            query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableName, field, QString::number(value.toInt()+1), item->text(1)));
//        }
//    }


//    //明细动作
//    else{
//        if(property->propertyName() == "名称")
//        {
//            actionSelectionChanged();
//            // actionDetailMap[item->text(1)]->name = value.toString();
//            //item->setText(0,value.toString());
//        }
//        else if(property->propertyName() == "动作类型")
//        {
//            actionDetailMap[item->text(1)]->type = value.toInt()+1;
//            field = "type";
//            QString fvalue;
//            if(value.toInt() == 0)
//            {
//                actionDetailMap[item->text(1)]->value ="0";
//            }
//            else
//                 actionDetailMap[item->text(1)]->value= " ";

//            query.exec(QString("UPDATE %1 set %2 = %3, value = '%4' where id = '%5'").arg(tableNameD, field, QString::number( actionDetailMap[item->text(1)]->type),actionDetailMap[item->text(1)]->value, item->text(1)));
//            actionSelectionChanged();

//        }
//        else if(property->propertyName() == "刀具"  || property->propertyName() == "模型组件")
//        {
//            //QString id;
//            if(property->propertyName() == "刀具" )
//            {
//                actionDetailMap[item->text(1)]->moduleId  = knifeIdList.value(value.toInt());
//                actionDetailMap[item->text(1)]->name =  knifeList.value(value.toInt());
//                // id = knifeIdList.value(value.toInt());
//            }
//            else
//            {
//                actionDetailMap[item->text(1)]->moduleId = moduleIdList.value(value.toInt());
//                actionDetailMap[item->text(1)]->name =  modulNameList.value(value.toInt());
//                //id = moduleIdList.value(value.toInt());
//            }


//            item->setText(0, actionDetailMap[item->text(1)]->name);
//            actionSelectionChanged();
//            field = "moduleId";
//            query.exec(QString("UPDATE %1 set %2 = '%3' ,name ='%4'  where id = '%5'").arg(tableNameD, field,actionDetailMap[item->text(1)]->moduleId ,
//                       actionDetailMap[item->text(1)]->name, item->text(1)));

//        }

//        else if(property->propertyName().contains("隐藏"))
//        {
//            if(value.toInt() == 0)
//                actionDetailMap[item->text(1)]->value = "0";
//            else if(value.toInt() ==1)
//                actionDetailMap[item->text(1)]->value ="1";
//            field = "value";
//            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field,actionDetailMap[item->text(1)]->value  , item->text(1)));
//        }
//        else if(property->propertyName().contains("绑定轴"))
//        {
//            actionDetailMap[item->text(1)]->value = axisIdList.value(value.toInt());
//            field = "value";
//            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, axisIdList.value(value.toInt()) , item->text(1)));
//        }


//    }


//}


void PageStation::QuerModuleId()
{

    modulNameList.clear();
    axisNameList.clear();
    moduleIdList.clear();
    axisIdList.clear();
    modulNameList.append("");
    axisNameList.append("");
    moduleIdList.append("");
    axisIdList.append("");
    QSqlQuery query;
    query.exec(QString("select id, name from StationModule where  stationId = '%1' and isUse = 1 ORDER BY name ASC").arg(currentStationId));

    while(query.next())
    {
        moduleIdList.append(query.value(0).toString());
        modulNameList.append(query.value(1).toString());
    }
    query.exec(QString("select id, name from StationAxis where  stationId = '%1' and isUse =1 ORDER BY name ASC").arg(currentStationId));

     while(query.next())
     {
         axisIdList.append(query.value(0).toString());
         axisNameList.append(query.value(1).toString());
     }
}

void PageStation::QuerKnifeName()
{
    knifeList.clear();
    knifeIdList.clear();
    knifeList.append("");
    knifeIdList.append("");
    QSqlQuery query;
    query.exec(QString("select id, name from KnifeLibrary ORDER BY name ASC" ));
    while(query.next())
    {

        knifeIdList.append(query.value(0).toString());
        knifeList.append(query.value(1).toString());
    }

}

/*************update6 by cxs ************************/

void PageStation::on_pushButton_export_clicked()
{
    QString dbpath = QFileDialog::getSaveFileName(this, tr("保存方案"), ui.comboStationName->currentText(), tr("*.sol"));
    if(dbpath == "")
            return;
////    qDebug()<<dbpath;
//    QStringList list;
////    list.clear();
////    list<<"./module1/test.avi";
////    dbpath.chop(4);

//    QSqlQuery query;
//    query.exec(QString("SELECT filename FROM StationModule WHERE stationId='%1'" ).arg(currentStationId));
//    while(query.next())
//    {
//        list.append("D:\\DzicsRobot\\" + query.value(0).toString() + ".dzm");
//    }

//    exportSolThread = new SolutionSet();
//    exportSolThread->setTaskExport(this->currentStationId,dbpath);
    exportSolThread = new SolutionSet(TASKEXPORT, dbpath, currentStationId);
    connect(exportSolThread,SIGNAL(finished()),this,SLOT(GetExportData()));
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    exportSolThread->start();
}

void PageStation::GetExportData()
{
    qApp->restoreOverrideCursor();
    if(exportSolThread->isTaskSuccess)
        emit addLog("方案导出成功", QString());
//        QMessageBox::information(NULL,"系统提示","导出成功",QMessageBox::Ok);
   else
        emit addLog("方案导出失败", QString());

//       QMessageBox::information(NULL,"系统提示","导出失败",QMessageBox::Ok);
   exportSolThread->deleteLater();

}

void PageStation::on_pushButton_import_clicked()
{
    QString filePath  = QFileDialog::getOpenFileName(NULL,"导入方案","./","*.sol");
    if(filePath == "")
        return;

    ui.comboStationName->setEnabled(false);
    ui.tabWidget->setEnabled(false);

//    importSolThread = new SolutionSet();
//    importSolThread->setTaskImport(filePath);
    importSolThread = new SolutionSet(TASKIMPORT, filePath);
    connect(importSolThread,SIGNAL(finished()),this,SLOT(GetImportData()));
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    importSolThread->start();

}


void PageStation::GetImportData()
{

    qApp->restoreOverrideCursor();
    if(importSolThread->isTaskSuccess)
    {
        ui.comboStationName->addItem(importSolThread->newStationName, importSolThread->newStationId);
        ui.comboStationName->setCurrentIndex(ui.comboStationName->count() - 1);
        ui.lineEdit_station->setEnabled(true);
    //    stationListChanged(ui.comboStationName->count() - 1);
//        QMessageBox::information(NULL,"系统提示","导入成功",QMessageBox::Ok);
        emit addLog("方案导入成功", QString());
    }
    else
    {
//        QMessageBox::information(NULL,"系统提示","导入失败",QMessageBox::Ok);
        emit addLog("方案导入失败", QString());

        ui.comboStationName->setEnabled(true);
        ui.tabWidget->setEnabled(true);
    }
    importSolThread->deleteLater();

}
/*******************end******************************t****/






/**********************update8 by cxs ***************************/
void PageStation::robotChanged(int index)
{

    bool tranFlag = true;
    if(!QSqlDatabase::database().transaction())
        qDebug()<<"事务开启失败";
    QSqlQuery query;
/******************************update10***********************/
    QString robotId=  ui.comboRobot->itemData(index).toString();
    if(!robotId.isEmpty()){
        QString robotLibId = robotMap[robotId]->id;
        query.exec(QString("SELECT libraryId FROM StationMotion WHERE libraryId = '%1' and stationId = '%2'").arg(robotLibId,currentStationId));
        if(query.next()){
           tranFlag = false;
        }
    }
/*******************end****************************/
    if( robotStationId == ""){
        robotStationId = SysUtils::uuid();
       // robotId = ui.comboRobot->itemData(index).toString();

        if(!query.exec(QString("INSERT INTO StationMotion (id,stationId,name,type,libraryId,displayOrder) VALUES('%1', '%2', '%3',%4,'%5',1)").arg(robotStationId ,currentStationId,robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id))){
            tranFlag = false;
            qDebug()<<QString("INSERT INTO StationMotion (id,stationId,name,type,libraryId,displayOrder) VALUES('%1', '%2', '%3',%4,'%5',1)").arg(robotStationId ,currentStationId,robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id);
        }



    }
    else if(index == 0){
        if(!query.exec(QString("DELETE FROM StationMotion WHERE id ='%1'").arg(robotStationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg( robotStationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg( robotStationId)))
            tranFlag = false;
          robotStationId = "";
    }
    else{
        robotId = ui.comboRobot->itemData(index).toString();
       // QString robotLibId = robotMap[robotId]->id;
        if(!query.exec(QString("UPDATE StationMotion SET name ='%1',type='%2',libraryId='%3' WHERE id='%4'").arg(robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id,robotStationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg( robotStationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg(robotStationId)))
            tranFlag = false;

    }

    /*************update9*****************/
    if(index !=0){
        /*******update11**********************/
        QMap<QString,QString>oldToNewMap;
        QStringList newAxisIdList,oldAxisIdList,newParentIdList;
        oldToNewMap.clear();
        oldAxisIdList.clear();
        newAxisIdList.clear();
        newParentIdList.clear();
        query.exec(QString("SELECT id,parentId FROM RobotAxis WHERE robotId ='%1'").arg(robotId));
        while(query.next())
        {
          QString newId;
          QString oldId = query.value(0).toString();
          if(!oldToNewMap.contains(oldId)){
              newId = SysUtils::uuid();
              newAxisIdList<<newId;
              oldAxisIdList<<oldId;
              oldToNewMap.insert(query.value(0).toString(),newId);
          }
          QString parentId = query.value(1).toString();
          if(parentId.isEmpty()){
              QSqlQuery queryRoot;
              QString rootAxis = newId;
              queryRoot.exec(QString("UPDATE StationMotion SET rootAxis = '%1' WHERE libraryId = '%2' and stationId ='%3'").arg(rootAxis, robotId,currentStationId));

              newParentIdList<<"";
              continue;
          }
          if(oldToNewMap.contains(parentId)){
              newParentIdList<<oldToNewMap[parentId];
          }else{
              newId = SysUtils::uuid();
              oldToNewMap.insert(parentId,newId);
              oldAxisIdList<<parentId;
              newAxisIdList<<newId;
              newParentIdList<<newId;
          }

        }
        /*************end***************/
        query.prepare(QString("INSERT INTO StationAxis (id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, motionId,isLock,disPlayOrder) SELECT ?,'%1',sign,name,type,?,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, '%2',isLock,disPlayOrder FROM RobotAxis WHERE id= ? ").arg(currentStationId,robotStationId));
        query.addBindValue(newAxisIdList);
        query.addBindValue(newParentIdList);
        query.addBindValue(oldAxisIdList);
        if(!query.execBatch()){
            tranFlag = false;
        }
//        parentIdList.clear(); // update 11
        newParentIdList.clear();
        QStringList oldModuleIdList,newModuleIdList,bindAxisIdList;
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
        query.exec(QString("SELECT id,bindAxis FROM RobotModule WHERE robotId ='%1'").arg(robotId));
        /******************update11*******************/
          while(query.next())
          {
              oldModuleIdList<<query.value(0).toString();
              QString bindAxisId = query.value(1).toString();
              if(oldToNewMap.contains(bindAxisId)){
                  bindAxisIdList<<oldToNewMap[bindAxisId];
              }else
                  bindAxisIdList<<"";
              newModuleIdList<<SysUtils::uuid();

          }
       /*****************end***********************************/
        query.prepare(QString("INSERT INTO StationModule (id,stationId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse, motionId,isLock,isShow,disPlayOrder) SELECT ?,'%1',name,type,filename,parentId,?,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,'%2',isLock,isShow,disPlayOrder FROM RobotModule WHERE id = ? ").arg(currentStationId,robotStationId));
        query.addBindValue(newModuleIdList);
        query.addBindValue(bindAxisIdList);
        query.addBindValue(oldModuleIdList);
        if(!query.execBatch()){
            tranFlag = false;
        }
        oldAxisIdList.clear();
        newAxisIdList.clear();
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
    }
    if(tranFlag)
        QSqlDatabase::database().commit();
    else
        QSqlDatabase::database().rollback();
    /*******************end**************/
    this->stationListChanged( ui.comboStationName->currentIndex());
}


void  PageStation::robot2Changed(int index)
{
    bool tranFlag = true;
    if(!QSqlDatabase::database().transaction())
        qDebug()<<"事务开启失败";
    QSqlQuery query;
    /******************************update10***********************/
    QString robotId=  ui.comboRobot2->itemData(index).toString();
    if(!robotId.isEmpty()){
        QString robotLibId = robotMap[robotId]->id;
        query.exec(QString("SELECT libraryId FROM StationMotion WHERE libraryId = '%1' and stationId = '%2'").arg(robotLibId,currentStationId));
        if(query.next()){
            tranFlag = false;
        }
    }
    /*******************end****************************/
    if( robot2StationId == ""){
        robot2StationId = SysUtils::uuid();
      //  robotId = ui.comboRobot2->itemData(index).toString();
        if(!query.exec(QString("INSERT INTO StationMotion (id,stationId,name,type,libraryId,displayOrder) VALUES('%1', '%2', '%3',%4,'%5',2)").arg(robot2StationId ,currentStationId,robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id)))
            tranFlag = false;
    }
    else if(index == 0){
        if(!query.exec(QString("DELETE FROM StationMotion WHERE id ='%1'").arg(robot2StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg(robot2StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg(robot2StationId)))
            tranFlag = false;
        robot2StationId = "";
    }
    else{
       // robotId = ui.comboRobot2->itemData(index).toString();
        if(!query.exec(QString("UPDATE StationMotion SET name ='%1',type='%2',libraryId='%3' WHERE id='%4'").arg(robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id,robot2StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg( robot2StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg(robot2StationId)))
            tranFlag = false;

    }
    /*******update9************************/
    if(index !=0){
        /******************update11*********************/
             QMap<QString,QString>oldToNewMap;
             QStringList newAxisIdList,oldAxisIdList,newParentIdList;
             oldToNewMap.clear();
             oldAxisIdList.clear();
             newAxisIdList.clear();
             newParentIdList.clear();
             query.exec(QString("SELECT id,parentId FROM RobotAxis WHERE robotId ='%1'").arg(robotId));
             while(query.next())
             {
                 QString newId;
                 QString oldId = query.value(0).toString();
                 if(!oldToNewMap.contains(oldId)){
                     newId = SysUtils::uuid();
                     newAxisIdList<<newId;
                     oldAxisIdList<<oldId;
                     oldToNewMap.insert(query.value(0).toString(),newId);
                 }
                 QString parentId = query.value(1).toString();
                 if(parentId.isEmpty()){
                     QSqlQuery queryRoot;
                     QString rootAxis = newId;
                     queryRoot.exec(QString("UPDATE StationMotion SET rootAxis = '%1' WHERE libraryId = '%2' and  stationId ='%3'").arg(rootAxis, robotId,currentStationId));
                     newParentIdList<<"";
                     continue;
                 }
                 if(oldToNewMap.contains(parentId)){
                     newParentIdList<<oldToNewMap[parentId];
                 }else{
                     newId = SysUtils::uuid();
                     oldToNewMap.insert(parentId,newId);
                     oldAxisIdList<<parentId;
                     newAxisIdList<<newId;
                     newParentIdList<<newId;
                 }

             }
        /************************end*********************************/
        query.prepare(QString("INSERT INTO StationAxis (id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, motionId,isLock,disPlayOrder)  SELECT ?,'%1',sign,name,type,?,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, '%2',isLock,disPlayOrder FROM RobotAxis WHERE id = ? ").arg(currentStationId,robot2StationId));
        query.addBindValue(newAxisIdList);
        query.addBindValue(newParentIdList);
        query.addBindValue(oldAxisIdList);
        if(!query.execBatch()){
            tranFlag = false;
            qDebug()<<query.lastError().text()<<"failed";
        }
//        parentIdList.clear();
        newParentIdList.clear();
        QStringList oldModuleIdList,newModuleIdList,bindAxisIdList;
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
        query.exec(QString("SELECT id,bindAxis FROM RobotModule WHERE robotId ='%1'").arg(robotId));
        /***************update11*******************/
             while(query.next())
             {
                 oldModuleIdList<<query.value(0).toString();
                 QString bindAxisId = query.value(1).toString();
                 if(oldToNewMap.contains(bindAxisId)){
                     bindAxisIdList<<oldToNewMap[bindAxisId];
                 }else
                     bindAxisIdList<<"";
                 newModuleIdList<<SysUtils::uuid();

             }
       /******************end*************************/
        query.prepare(QString("INSERT INTO StationModule (id,stationId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse, motionId,isLock,isShow,disPlayOrder) SELECT ?,'%1',name,type,filename,parentId,?,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,'%2',isLock,isShow,disPlayOrder FROM RobotModule WHERE id = ? ").arg(currentStationId,robot2StationId));
        query.addBindValue(newModuleIdList);
        query.addBindValue(bindAxisIdList);
        query.addBindValue(oldModuleIdList);
        if(!query.execBatch()){
            tranFlag = false;
            qDebug()<<query.lastError();
        }
        oldAxisIdList.clear();
        newAxisIdList.clear();
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
    }
    if(tranFlag)
        QSqlDatabase::database().commit();
    else
        QSqlDatabase::database().rollback();

    /******************end*****************************/
    this->stationListChanged(ui.comboStationName->currentIndex());
}


void PageStation::robot3Changed(int index)
{
    bool tranFlag = true;
    if(!QSqlDatabase::database().transaction())
        qDebug()<<"事务开启失败";
    QSqlQuery query;
    /******************************update10***********************/
    QString robotId=  ui.comboRobot3->itemData(index).toString();
    if(!robotId.isEmpty()){
        QString robotLibId = robotMap[robotId]->id;
        query.exec(QString("SELECT libraryId FROM StationMotion WHERE libraryId = '%1' and stationId = '%2'").arg(robotLibId,currentStationId));
        if(query.next()){
            tranFlag = false;
        }
    }
    /*******************end****************************/
    if( robot3StationId == ""){
        robot3StationId = SysUtils::uuid();
       // robotId = ui.comboRobot3->itemData(index).toString();
        if(!query.exec(QString("INSERT INTO StationMotion (id,stationId,name,type,libraryId,displayOrder) VALUES('%1', '%2', '%3',%4,'%5',3)").arg(robot3StationId ,currentStationId,robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id)))
            tranFlag = false;

    }
    else if(index == 0){
        if(!query.exec(QString("DELETE FROM StationMotion WHERE id ='%1'").arg(robot3StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg(robot3StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg(robot3StationId)))
            tranFlag = false;
        robot3StationId = "";
    }
    else{
       // robotId = ui.comboRobot3->itemData(index).toString();
        if(!query.exec(QString("UPDATE StationMotion SET name ='%1',type='%2',libraryId='%3' WHERE id='%4'").arg(robotMap[robotId]->name,QString::number(robotMap[robotId]->type),robotMap[robotId]->id,robot3StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationAxis WHERE motionId= '%1'").arg( robot3StationId)))
            tranFlag = false;
        if(!query.exec(QString("DELETE FROM StationModule WHERE motionId= '%1'").arg(robot3StationId)))
            tranFlag = false;


    }
    /********************update9*****************/
    if(index !=0){
        /******************update11*********************/
        QMap<QString,QString>oldToNewMap;
        QStringList newAxisIdList,oldAxisIdList,newParentIdList;
        oldToNewMap.clear();
        oldAxisIdList.clear();
        newAxisIdList.clear();
        newParentIdList.clear();
        query.exec(QString("SELECT id,parentId FROM RobotAxis WHERE robotId ='%1'").arg(robotId));
        while(query.next())
        {
            QString newId;
            QString oldId = query.value(0).toString();
            if(!oldToNewMap.contains(oldId)){
                newId = SysUtils::uuid();
                newAxisIdList<<newId;
                oldAxisIdList<<oldId;
                oldToNewMap.insert(query.value(0).toString(),newId);
            }
            QString parentId = query.value(1).toString();
            if(parentId.isEmpty()){
                QSqlQuery queryRoot;
                QString rootAxis = newId;
                queryRoot.exec(QString("UPDATE StationMotion SET rootAxis = '%1' WHERE libraryId = '%2' and  stationId ='%3'").arg(rootAxis, robotId,currentStationId));

                newParentIdList<<"";
                continue;
            }
            if(oldToNewMap.contains(parentId)){
                newParentIdList<<oldToNewMap[parentId];
            }else{
                newId = SysUtils::uuid();
                oldToNewMap.insert(parentId,newId);
                oldAxisIdList<<parentId;
                newAxisIdList<<newId;
                newParentIdList<<newId;
            }

        }
        /************************end*********************************/
        query.prepare(QString("INSERT INTO StationAxis (id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, motionId,isLock,disPlayOrder) SELECT ?,'%1',sign,name,type,?,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse, '%2',isLock,disPlayOrder FROM RobotAxis WHERE id = ? ").arg(currentStationId,robot3StationId));
        query.addBindValue(newAxisIdList);
        query.addBindValue(newParentIdList);
        query.addBindValue(oldAxisIdList);
        if(!query.execBatch()){
            tranFlag = false;
        }
        // oldIdList.clear();
        // newAxisIdList.clear();
//        parentIdList.clear();
        newParentIdList.clear();
        QStringList oldModuleIdList,newModuleIdList,bindAxisIdList;
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
        query.exec(QString("SELECT id, bindAxis FROM RobotModule WHERE robotId ='%1'").arg(robotId));
        /***************update11*******************/
        while(query.next())
        {
            oldModuleIdList<<query.value(0).toString();
            QString bindAxisId = query.value(1).toString();
            if(oldToNewMap.contains(bindAxisId)){
                bindAxisIdList<<oldToNewMap[bindAxisId];
            }else
                bindAxisIdList<<"";
            newModuleIdList<<SysUtils::uuid();

        }
       /******************end*************************/
        query.prepare(QString("INSERT INTO StationModule (id,stationId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse, motionId,isLock,isShow,disPlayOrder) SELECT ?,'%1',name,type,filename,parentId,?,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,'%2',isLock,isShow,disPlayOrder FROM RobotModule WHERE id = ? ").arg(currentStationId,robot3StationId));
        query.addBindValue(newModuleIdList);
        query.addBindValue(bindAxisIdList);
        query.addBindValue(oldModuleIdList);
        if(!query.execBatch()){
            tranFlag = false;
        }
        oldAxisIdList.clear();
        newAxisIdList.clear();
        oldModuleIdList.clear();
        newModuleIdList.clear();
        bindAxisIdList.clear();
    }


    if(tranFlag)
        QSqlDatabase::database().commit();
    else
        QSqlDatabase::database().rollback();
    /*********************end*************************/
    this->stationListChanged( ui.comboStationName->currentIndex());
}


void PageStation::readRobotInfo()
{
//    /************update7 by cxs ********************************************/

//       QPixmap pixmap(30,30);
//       pixmap.fill(Qt::transparent);
//       QIcon icon(pixmap);
//       ui.comboRobot->setIconSize(QSize(30,30));
//       ui.comboRobot2->setIconSize(QSize(30,30));
//       ui.comboRobot3->setIconSize(QSize(30,30));

//       ui.comboRobot->addItem(icon,"无");
//       ui.comboRobot2->addItem(icon,"无");
//       ui.comboRobot3->addItem(icon,"无");
//       query.exec("SELECT id,name,type,preview FROM RobotLibrary ORDER BY type ASC");
//       int combIndex =0;
//       while(query.next())
//       {
//               combIndex++;
//               QSqlRecord record = query.record();
//               RobotObject *robotObject = new RobotObject(this);
//               robotObject->id =record.value("id").toString();
//               robotObject->name = record.value("name").toString();
//               robotObject->type = record.value("type").toInt();
//               robotObject->preview = record.value("preview").toString();
//               robotObject->comboIndex = combIndex;
//               robotMap.insert(robotObject->id,robotObject);
////                   QString type;
////                   if(robotObject->type == 1)
////                       type = "ABB六轴机器人";
////                   else if(robotObject->type == 2)
////                       type = "FANUC六轴机器人";
////                   else if(robotObject->type = 101)
////                       type = "大正六轴机器人";

//               ui.comboRobot->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//               ui.comboRobot2->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//               ui.comboRobot3->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//      }


//         connect(ui.comboRobot,SIGNAL(currentIndexChanged(int)),this,SLOT(robotChanged(int)));
//         connect(ui.comboRobot2,SIGNAL(currentIndexChanged(int)),this,SLOT(robot2Changed(int)));
//         connect(ui.comboRobot3,SIGNAL(currentIndexChanged(int)),this,SLOT(robot3Changed(int)));

//   /*********************************end******************************************************/


    disconnect(ui.comboRobot,SIGNAL(currentIndexChanged(int)),this,SLOT(robotChanged(int)));
    disconnect(ui.comboRobot2,SIGNAL(currentIndexChanged(int)),this,SLOT(robot2Changed(int)));
    disconnect(ui.comboRobot3,SIGNAL(currentIndexChanged(int)),this,SLOT(robot3Changed(int)));
    ui.comboRobot->clear();
    ui.comboRobot2->clear();
    ui.comboRobot3->clear();
    QPixmap pixmap(30,30);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    ui.comboRobot->setIconSize(QSize(30,30));
    ui.comboRobot2->setIconSize(QSize(30,30));
    ui.comboRobot3->setIconSize(QSize(30,30));
    ui.comboRobot->addItem(icon,"无");
    ui.comboRobot2->addItem(icon,"无");
    ui.comboRobot3->addItem(icon,"无");
    QSqlQuery query;
    query.exec("SELECT id ,name,type,preview FROM RobotLibrary ORDER BY type ASC, name ASC");
    int comboIndex =0;
    while(query.next())
    {
            comboIndex++;
            QSqlRecord record = query.record();
            RobotObject *robotObject = new RobotObject(this);
            robotObject->id =record.value("id").toString();
            robotObject->name = record.value("name").toString();
            robotObject->type = record.value("type").toInt();
            robotObject->preview = record.value("preview").toString();
            robotObject->comboIndex = comboIndex;



            robotMap.insert(robotObject->id,robotObject);
//            QString type;
//            if(robotObject->type == 1)
//                type = "ABB六轴机器人";
//            else if(robotObject->type == 2)
//                type = "FANUC六轴机器人";
//            else if(robotObject->type = 101)
//                type = "大正六轴机器人";

           ui.comboRobot->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
           ui.comboRobot2->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
           ui.comboRobot3->addItem(robotObject->preview.isEmpty()?icon:QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//            ui.comboRobot->addItem(QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//            ui.comboRobot2->addItem(QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
//            ui.comboRobot3->addItem(QIcon(robotObject->preview), robotObject->name,robotObject->id);//+"-" + type,robotObject->id);
   }

      connect(ui.comboRobot,SIGNAL(currentIndexChanged(int)),this,SLOT(robotChanged(int)));
      connect(ui.comboRobot2,SIGNAL(currentIndexChanged(int)),this,SLOT(robot2Changed(int)));
      connect(ui.comboRobot3,SIGNAL(currentIndexChanged(int)),this,SLOT(robot3Changed(int)));
}

void PageStation::on_pushButton_importRobot_clicked()
{
      QString filePath  = QFileDialog::getOpenFileName(NULL,"导入机器人","./","*.rob");
      if(filePath == "")
          return;
//      importRobotThread = new SolutionSet();
//      importRobotThread->setTaskRobotImport(filePath);
      importRobotThread = new SolutionSet(ROBOTIMPORT, filePath);
      connect(importRobotThread,SIGNAL(finished()),this,SLOT(importRobotResult()));
      qApp->restoreOverrideCursor();
      qApp->setOverrideCursor(Qt::WaitCursor);
      importRobotThread->start();
}

void PageStation::importRobotResult()
{
    qApp->restoreOverrideCursor();
    if(importRobotThread->isTaskSuccess){
        readRobotInfo();
        this->stationListChanged(ui.comboStationName->currentIndex());
//        QMessageBox::information(NULL,"系统提示","导入机器人成功",QMessageBox::tr("确认"));
        emit addLog("机器人导入成功", QString());
//        emit robotRefresSignal();
    }
    else
        emit addLog("机器人导入失败", QString());
//        QMessageBox::information(NULL,"系统错误","导入机器人失败",QMessageBox::tr("确认"));

}

void PageStation::exportRobotResult()
{
    qApp->restoreOverrideCursor();
    if(exportRobotThread->isTaskSuccess)
        emit addLog("机器人导出成功", QString());
//        QMessageBox::information(NULL,"系统提示","导出机器人成功",QMessageBox::tr("确认"));

    else
        emit addLog("机器人导出失败", QString());
//        QMessageBox::information(NULL,"系统提示","导出机器人失败",QMessageBox::tr("确认"));
    exportRobotThread->deleteLater();
}

/*****************************************end******************/


/**************update12**********************/

void PageStation::on_pushButton_exportRobot_clicked()
{
//    int iTitleBarHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);
    RobotExport *robotExport = new RobotExport(this);
//    robotExport->setGeometry(this->parentWidget()->x()+8,this->parentWidget()->y()+iTitleBarHeight*2,640,408);
    robotExport->setAttribute(Qt::WA_DeleteOnClose);
    robotExport->exec();
    if(RobotExport::getChoseRobot().isEmpty())
        return;
    QString path = QFileDialog::getSaveFileName(NULL, tr("导出机器人"),robotMap.value(RobotExport::getChoseRobot())->name,tr("*.rob"));
    if(path== "")
        return;
//    exportRobotThread = new SolutionSet();
//    exportRobotThread->setTaskRobotExport(RobotExport::getChoseRobot(),path);
    exportRobotThread = new SolutionSet(ROBOTEXPORT, path, RobotExport::getChoseRobot());
    connect(exportRobotThread,SIGNAL(finished()),this,SLOT(exportRobotResult()));
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
    exportRobotThread->start();



}
/***************end************************************/


/******************update13*******************/

void PageStation::showModuleAction()
{
    QStringList detailIdList;
    ui.treeWidgetAction->collapseAll();
    ui.treeWidgetAction->clearSelection();
   // QMap<Qstring, actionMap;
    ui.tabWidget->setCurrentWidget(ui.tabAction);
    QString moduleId = ui.treeWidgetModule->selectedItems().last()->text(1);
  //  qDebug()<<moduleId;
    QSqlQuery query;
    query.exec(QString("SELECT id FROM StationActionDetail WHERE moduleId ='%1' ORDER BY name ASC").arg(moduleId));
    while(query.next()){

        QString detailId = query.value(0).toString();
        detailIdList<<detailId;
    }
    QTreeWidgetItemIterator it(ui.treeWidgetAction);
        while (*it) {
            QTreeWidgetItem *item = *(it);
            qDebug()<<item->text(1)<<item->text(0);
            if(detailIdList.contains(item->text(1))){
               // ui.treeWidgetAction->expandItem(item);
                ui.treeWidgetAction->setCurrentItem(item,0,QItemSelectionModel::Select);
                //ui.treeWidgetAction->setItemSelected(item,true);

                }
         // (*it)->text(0);
            ++it;
        }

        ui.treeWidgetAction->expandItem(ui.treeWidgetAction->topLevelItem(0));
}

/*********************end******************/

void PageStation::moveEnd(double x, double y, double z) {
    try{
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);

        QSqlQuery query;

        if(!QSqlDatabase::database().transaction())
              qDebug()<<"事务开启失败";

        Handle_AIS_InteractiveContext context;
        foreach (QTreeWidgetItem *item, ui.treeWidgetModule->selectedItems()) {
            if (!item->parent())
                continue;

            ModuleObject *module = moduleMap[item->text(1)];
            if (x != 0.0 || y != 0.0 || z != 0.0) {
                module->pos[X] += x;
                module->pos[Y] += y;
                module->pos[Z] += z;
//                if (module->rotateAngle!=0.0) {
//                    gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
//                    module->transform = gp_Trsf();
//                    module->transform.SetRotation(ax, module->rotateAngle);
//                }
                module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
                TopoDS_Shape shape = module->aisShape->Shape();
                shape.Location(TopLoc_Location(module->transform));
                module->aisShape->Set(shape);
                QTime time;
                time.start();
                module->aisShape->Redisplay();
                context = module->aisShape->GetContext();
                context->Update(module->aisShape, false);

                if ( query.exec(QString("UPDATE StationModule SET x=%1,y=%2,z=%3 WHERE id='%4'").arg(QString::number(module->pos[X],'g',13), QString::number(module->pos[Y],'g',13), QString::number(module->pos[Z],'g',13), module->id)) ) {
                    qDebug("移动成功！");
                } else {
                    qDebug("移动失败！", true);
                    continue;
                }
            }
        }

        if (!context.IsNull()) {
            context->CurrentViewer()->Update();
        }

    }catch(...){
        // Ignore
        qDebug("移动失败！", true);
    }
    QSqlDatabase::database().commit();
    qApp->restoreOverrideCursor();


}

#include <qdebug.h>
void PageStation::rotateEnd(double x, double y, double z, double rx, double ry,double rz, double angle) {
    try{
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);

        QSqlQuery query;

        if(!QSqlDatabase::database().transaction())
              qDebug()<<"事务开启失败";

        foreach (QTreeWidgetItem *item, ui.treeWidgetModule->selectedItems()) {
            if (!item->parent())
                continue;
            ModuleObject *module = moduleMap[item->text(1)];
            angle = angle * M_PI / 180;

            qDebug()<<x<<y<<z<<rx<<ry<<rz<<angle;
            TopoDS_Shape shape = module->aisShape->Shape();
            //shape.Location(TopLoc_Location(module->transform));
            //module->aisShape->Set(shape);
            //module->aisShape->Redisplay();
            //module->aisShape->GetContext()->Update(module->aisShape, true);

            if (x != 0.0 || y != 0.0 || z != 0.0 || rx != 0.0 || ry != 0.0 || rz != 0.0 || angle != 0.0) {
                module->rotatePoint[X] = x;
                module->rotatePoint[Y] = y;
                module->rotatePoint[Z] = z;
                module->rotateDirection[X] = rx;
                module->rotateDirection[Y] = ry;
                module->rotateDirection[Z] = rz;
                module->rotateAngle = angle;
                //TopLoc_Location locRotation;
                if (module->rotateAngle!=0.0) {
                    gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));

                    //显示轴位置
                    //gp_Ax1 axe = gp_Ax1(gp_Pnt(module->rotatePoint[X],module->rotatePoint[Y],module->rotatePoint[Z]),gp_Dir(0.,1.,0.));
                    Handle(Geom_Axis1Placement) Gax1 = new Geom_Axis1Placement(ax);
                    Handle (AIS_Axis) ax1 = new AIS_Axis(Gax1);
                    module->aisShape->GetContext()->Display(ax1,Standard_False);

                    //by ronglk 修正模型有偏移时，旋转不正确
                    //模型置初始位置

                    TopoDS_Shape shape = module->aisShape->Shape();
                                        shape.Location(TopLoc_Location(module->originalTransform));
                                        module->aisShape->Set( shape );
                                        //ax.Location();


                                        //shape = module->aisShape->Shape();
                    //                    module->transform = gp_Trsf();
                    //                    module->transform.SetRotation(ax, module->rotateAngle);
                    //                    module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
                    //                    module->currentLoc = TopLoc_Location(module->transform);

                    gp_Trsf trsfRotation;
                    gp_Trsf trsfTransf;
                    //gp_Trsf trsfFinal;
                    trsfRotation.SetRotation(ax, module->rotateAngle);
                    //trsfTransf.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
                    //locBase = locBase * TopLoc_Location(trsfBase);

                    module->transform = trsfRotation*trsfTransf*module->transform;

                      //将变换矩阵转成欧拉角和平移，并保存到数据库
                      double a11 = module->transform.Value(1,1);
                      double a12 = module->transform.Value(1,2);
                      double a13 = module->transform.Value(1,3);
                      double a14 = module->transform.Value(1,4);

                      double a21 = module->transform.Value(2,1);
                      double a22 = module->transform.Value(2,2);
                      double a23 = module->transform.Value(2,3);
                      double a24 = module->transform.Value(2,4);

                      double a31 = module->transform.Value(3,1);
                      double a32 =  module->transform.Value(3,2);
                      double a33 =  module->transform.Value(3,3);
                      double a34 = module->transform.Value(3,4);
                      qDebug()<<"rotation:"<<a11<<a12<<a13<<a14<<a21<<a22<<a23<<a24<<a31<<a32<<a33<<a34;

                      gp_XYZ Col1 = gp_XYZ(a11, a21, a31);
                      gp_XYZ Col2 = gp_XYZ(a12, a22, a32);
                      gp_XYZ Col3 = gp_XYZ(a13, a23, a33);
                      gp_Mat rotationMatrix = gp_Mat(Col1, Col2, Col3);

                      gp_Quaternion finalQuaternion= gp_Quaternion(rotationMatrix);
                      finalQuaternion.GetEulerAngles(gp_YawPitchRoll, module->yaw, module->pitch, module->roll);

                      module->pos[0] = a14;
                      module->pos[1] = a24;
                      module->pos[2] = a34;

//                      //验证RT矩阵正确性
//                      module->quaternion.SetEulerAngles(gp_YawPitchRoll, module->yaw, module->pitch, module->roll) ;
//                      gp_Mat rotateMatrix = module->quaternion.GetMatrix();
//                      a11 = rotateMatrix.Value(1, 1);
//                      a12 = rotateMatrix.Value(1, 2);
//                      a13 = rotateMatrix.Value(1, 3);

//                      a21 = rotateMatrix.Value(2, 1);
//                      a22 = rotateMatrix.Value(2, 2);
//                      a23 = rotateMatrix.Value(2, 3);

//                      a31 = rotateMatrix.Value(3, 1);
//                      a32 = rotateMatrix.Value(3, 2);
//                      a33 = rotateMatrix.Value(3, 3);

//                      qDebug()<<"2:"<<a11<<a12<<a13<<module->translation[0]<<a21<<a22<<a23<<module->translation[1]<<a31<<a32<<a33<<module->translation[2];

//                      module->transform.SetValues(a11, a12, a13, module->translation[0], a21, a22, a23, module->translation[1], a31, a32, a33, module->translation[2],0.01,0.001);

                    shape.Location(TopLoc_Location(module->transform));
                    module->aisShape->Set(shape);
                }
                module->aisShape->Redisplay();
                module->aisShape->GetContext()->Update(module->aisShape, true);


                if ( query.exec(QString("UPDATE StationModule SET rotateX=%1,rotateY=%2,rotateZ=%3,initAngle=%4 WHERE id='%5'").arg(QString::number(module->rotateDirection[X],'g',13), QString::number(module->rotateDirection[Y],'g',13), QString::number(module->rotateDirection[Z],'g',13), QString::number(module->rotateAngle,'g',13), module->id)) ){
                    qDebug()<<module->pos[X]<<module->pos[Y]<<module->pos[Z]<<module->yaw<<module->pitch<<module->roll;
                    qDebug("旋转成功！");
                } else {
                    qDebug("旋转失败！", true);
                    continue;
                }
                if ( query.exec(QString("UPDATE StationModule SET yaw=%1, pitch=%2, roll=%3, x=%4, y=%5, z=%6 WHERE id='%7'").arg(QString::number(module->yaw,'g',13),QString::number(module->pitch,'g',13),QString::number(module->roll,'g',13),QString::number(module->pos[0],'g',13),QString::number(module->pos[1],'g',13),QString::number(module->pos[2],'g',13),module->id)) ){
                    qDebug()<<module->pos[0]<<module->pos[1]<<module->pos[2]<<module->yaw<<module->pitch<<module->roll;
                    qDebug("存储欧拉角和平移向量成功！");
                } else {
                    qDebug("存储欧拉角和平移向量失败！", true);
                    continue;
                }

            }
        }

    } catch(...) {
        // Ignore
        qDebug("旋转失败！", true);
    }
    QSqlDatabase::database().commit();
    qApp->restoreOverrideCursor();
}
//end


void PageStation::on_cur_robot_currentIndexChanged(int index)
{

    qDebug()<<"index :"<<index<<"udpIndex:"<<mainWindow->currentUdpIndex;
    if(mainWindow->curRobotIndex==-1)
    {
      mainWindow->curRobotIndex=ui.cur_robot->itemData(index).toInt();
      return;
    }else
    {
        mainWindow->curRobotIndex=ui.cur_robot->itemData(index).toInt();

        //mainWindow->currentUdpIndex=index;
    }
    emit signalVMState();
    qDebug()<<mainWindow->curRobotIndex;
    mainWindow->setConnState(mainWindow->curRobotIndex);

    mainWindow->setExecState(mainWindow->curRobotIndex);

    mainWindow->setOperatorState(mainWindow->curRobotIndex);
    mainWindow->setSpeedState(index);

    querySafeArea();


}



void PageStation::on_pushButton_3_clicked()
{
    mainWindow->pauseRobot();
}

void PageStation::on_pushButton_4_clicked()
{

    QTimer::singleShot(0,mainWindow,SLOT(resumeRobot()));


}

void PageStation::on_pushButton_5_clicked()
{


    mainWindow->clearWorkNum();
}



/*********************by ronglk*****************/
void PageStation::AddModuleLable()
{
//    QList<QTreeWidgetItem*> items = ui.treeWidgetModule->selectedItems();
//    if (items.isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        qApp->restoreOverrideCursor();
//        qApp->setOverrideCursor(Qt::WaitCursor);
//        foreach (QTreeWidgetItem *item, items) {
//            QString id = item->text(1);
//            if (!id.isEmpty()) {
////                QSqlQuery query;
////                if ( query.exec(QString("DELETE FROM StationModule WHERE id='%1'").arg(id)) ) {
////                    qDebug("删除成功！");
////                } else {
////                    qDebug("删除失败！", true);
////                    continue;
////                }
////                QFile::remove("D:\\DzicsRobot\\Module\\" + id + ".dzm");
////                emit moduleChanged(id, true);
//                if (moduleMap[id]) {
//                    Handle(ISession_Text) aGraphicText = new ISession_Text("aTextWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",moduleMap[id]->pos[0],moduleMap[id]->pos[1],moduleMap[id]->pos[2], Aspect_TOT_SOLID,0.0,0.0,1,1,0.1);
//                    moduleMap[id]->aisShape->GetContext()->Display(aGraphicText, 1);
////                    moduleMap[id]->aisShape->GetContext()->Clear(moduleMap[id]->aisShape, true);
////                    moduleMap[id]->aisShape->Delete();
////                    moduleMap.remove(id);
//                }
//            }
//            //item->parent()->removeChild(item);
//        };
//        qApp->restoreOverrideCursor();
//    }
}



/*********************************
 *by ronglk
 *******************************/
void PageStation::calibrationSelectionChanged()
{
    if (!this->isVisible()) return;

    disconnect(variantManagerCalibration, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
               this, SLOT(calibrationPropertyChanged(QtProperty *, const QVariant &)));
    if(ui.treeWidgetCalibration->selectedItems().count()>0)
    {

        QTreeWidgetItem *item = ui.treeWidgetCalibration->selectedItems().last();
        if (!item->parent()){
            topPropertyCalibration->setEnabled(false);
            return;
        }
        foreach (QtProperty *property, topPropertyCalibration->subProperties())
            topPropertyCalibration->removeSubProperty(property);
        //校准分类部分
        if(!item->parent()->parent())
        {
            QtVariantProperty *itemPro = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
            //QtVariantProperty *itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(0);
            topPropertyCalibration->addSubProperty(itemPro);
            itemPro->setValue(calibrationMap[item->text(1)]->name);

            itemPro = variantManagerCalibration->addProperty(QVariant::Int, tr("校准分类号"));
            //itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(1);
            topPropertyCalibration->addSubProperty(itemPro);

            itemPro->setValue(calibrationMap[item->text(1)]->calibrationNum);
            topPropertyCalibration->setEnabled(true);
            qDebug()<<calibrationMap[item->text(1)]->name<<calibrationMap[item->text(1)]->calibrationNum;
        }

        //校准明细部分
        else
        {
            int indexOfChild = item->parent()->indexOfChild(item);
            if(indexOfChild == 0)
            {
                QtVariantProperty *itemPro = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
                //QtVariantProperty *itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(0);
                topPropertyCalibration->addSubProperty(itemPro);
                qDebug()<<item->text(1);
                itemPro->setValue("校准点设定");

                itemPro = variantManagerCalibration->addProperty(QVariant::Int, tr("选取点位"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->pnt_index);
                topPropertyCalibration->addSubProperty(itemPro);

                QCmmF value;
                for(int i = 0; i < 9; i++)
                {
                    QString property_name = QString("点位%1").arg(i);
                    itemPro = variantManagerCalibration->addProperty(QTCMMF, property_name);

                    //解析calibrationPoint
                    QStringList tmpList = calibrationDetailMap[item->text(1)]->point[i].split("|");
                    if (tmpList.length()>0) {
                        value.x = tmpList[0].toDouble();
                        calibrationDetailMap[item->text(1)]->pointX[i] = value.x;
                    }
                    if (tmpList.length()>1) {
                        value.y = tmpList[1].toDouble();
                        calibrationDetailMap[item->text(1)]->pointY[i] = value.y;
                    }
                    if (tmpList.length()>2) {
                        value.z = tmpList[2].toDouble();
                        calibrationDetailMap[item->text(1)]->pointZ[i] = value.z;
                    }
                    itemPro->setValue(QVariant::fromValue(value));
                    topPropertyCalibration->addSubProperty(itemPro);
                }

                itemPro = variantManagerCalibration->addProperty(QTCMMF, tr("法兰盘位置"));
                //itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(10);

                value.x = calibrationDetailMap[item->text(1)]->falaX;
                value.y = calibrationDetailMap[item->text(1)]->falaY;
                value.z = calibrationDetailMap[item->text(1)]->falaZ;
                qDebug()<<value.x<<value.y<<value.z;
                itemPro->setValue(QVariant::fromValue(value));
                topPropertyCalibration->addSubProperty(itemPro);

                QtVariantProperty *groupItem =variantManagerCalibration->addProperty(QtVariantPropertyManager::groupTypeId(),tr("法兰盘方向"));
                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q1"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->falaQ1);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q2"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->falaQ2);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q3"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->falaQ3);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q4"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->falaQ4);
                groupItem->addSubProperty(itemPro);
                topPropertyCalibration->addSubProperty(groupItem);
            }else if(indexOfChild == 1)
            {
                QtVariantProperty *itemPro = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
                //QtVariantProperty *itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(0);
                topPropertyCalibration->addSubProperty(itemPro);
                itemPro->setValue("工具设定");

                itemPro = variantManagerCalibration->addProperty(QTCMMF, tr("工具坐标"));
                //itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(1);
                QCmmF value;
                //解析calibrationPoint
                //QStringList tmpList = calibrationDetailMap[item->text(1)]->point1.split("|");
                value.x = calibrationDetailMap[item->text(1)]->toolPosX;
                value.y = calibrationDetailMap[item->text(1)]->toolPosY;
                value.z = calibrationDetailMap[item->text(1)]->toolPosZ;
                itemPro->setValue(QVariant::fromValue(value));
                topPropertyCalibration->addSubProperty(itemPro);

                QtVariantProperty *groupItem =variantManagerCalibration->addProperty(QtVariantPropertyManager::groupTypeId(),tr("工具方向"));
                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q1"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolQ1);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q2"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolQ2);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q3"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolQ3);
                groupItem->addSubProperty(itemPro);
                //topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("Q4"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolQ4);
                groupItem->addSubProperty(itemPro);
                topPropertyCalibration->addSubProperty(groupItem);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("重心设定X"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolMassX);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("重心设定Y"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolMassY);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("重心设定Z"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolMassZ);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("重心设定Kg"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->toolKg);
                topPropertyCalibration->addSubProperty(itemPro);
            }else if(indexOfChild == 2)
            {
                QtVariantProperty *itemPro = variantManagerCalibration->addProperty(QVariant::String, tr("名称"));
                //QtVariantProperty *itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(0);
                topPropertyCalibration->addSubProperty(itemPro);
                itemPro->setValue("传感器设定");

                itemPro = variantManagerCalibration->addProperty(QTCMMF, tr("传感器位置"));
                //itemPro = (QtVariantProperty *)topPropertyCalibration->subProperties().at(1);
                QCmmF value;
                //解析calibrationPoint
                //QStringList tmpList = calibrationDetailMap[item->text(1)]->point1.split("|");
                value.x = calibrationDetailMap[item->text(1)]->sensorPosX;
                value.y = calibrationDetailMap[item->text(1)]->sensorPosY;
                value.z = calibrationDetailMap[item->text(1)]->sensorPosZ;
                itemPro->setValue(QVariant::fromValue(value));
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("传感器方向Vx"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->sensorDirectionX);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("传感器方向Vy"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->sensorDirectionY);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("传感器方向Vz"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->sensorDirectionZ);
                topPropertyCalibration->addSubProperty(itemPro);

                itemPro = variantManagerCalibration->addProperty(QVariant::Double, tr("传感器距离"));
                itemPro->setValue(calibrationDetailMap[item->text(1)]->sensorDistance);
                topPropertyCalibration->addSubProperty(itemPro);
            }



//            QStringList tmpList = calibrationMap[item->text(1)]->calibrationPoint.split("|");
//            itemPro = (QtVariantProperty *)topPropertyAxis->subProperties().at(5);
//            value.x = calibrationMap[item->text(1)]->end[X];
//            value.y = calibrationMap[item->text(1)]->end[Y];
//            value.z = calibrationMap[item->text(1)]->end[Z];
//            itemPro->setValue(QVariant::fromValue(value));
            topPropertyCalibration->setEnabled(true);
        }
    }

    if(isCalibrationChange)
    {
        if(itemPos >= calibrationItemParent->children().count())
            itemPos = 0;
        isCalibrationChange = false;
    }
    connect(variantManagerCalibration, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(calibrationPropertyChanged(QtProperty *, const QVariant &)));
}

/*********************add by ronglk****************/
void PageStation::updateCalibrationPoint(gp_Pnt pnt)
{
    QSqlQuery query;
    QString tableName = "CalibrationCategory";
    QString tableNameD = "CalibrationDetail";
    QString field;

    foreach (QTreeWidgetItem *item, ui.treeWidgetCalibration->selectedItems())
    {
        qDebug()<<ui.treeWidgetCalibration->selectedItems().count()<<item->text(0);
        if(!item->parent())
                continue;

        if(!item->parent()->parent())
        {

        }
        else    //明细校准
        {
            int pnt_index = topPropertyCalibration->subProperties().at(1)->valueText().toInt();
            if(pnt_index>=0 && pnt_index<9)
            {
                QStringList tmpList;
                tmpList<<QString("%1").arg(pnt.X())<<QString("%1").arg(pnt.Y())<<QString("%1").arg(pnt.Z());
                calibrationDetailMap[item->text(1)]->point[pnt_index] = tmpList.join("|");
                calibrationDetailMap[item->text(1)]->pnt_index = pnt_index;
                field = QString("point%1").arg(pnt_index);
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, calibrationDetailMap[item->text(1)]->point[pnt_index], item->text(1)));
                calibrationSelectionChanged();
            }
        }
    }
}

void PageStation::updateCalibrationFlange()
{
    QSqlQuery query;
    QString tableName = "CalibrationCategory";
    QString tableNameD = "CalibrationDetail";
    QString field;

    foreach (QTreeWidgetItem *item, ui.treeWidgetCalibration->selectedItems())
    {
        qDebug()<<ui.treeWidgetCalibration->selectedItems().count()<<item->text(0);
        if(!item->parent())
                continue;

        if(!item->parent()->parent())
        {

        }
        else    //明细校准
        {
            //else if(property->propertyName() == "法拉盘位置")
            //qDebug()<<calibrationDetailMap[item->text(1)]->falaX;
            field = "falaX";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaX,'g',13) ,item->text(1)));

            field = "falaY";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaY,'g',13) ,item->text(1)));

            field = "falaZ";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaZ,'g',13) ,item->text(1)));

            field = "falaQ1";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ1,'g',13) ,item->text(1)));

            field = "faaQ2";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ2,'g',13) ,item->text(1)));

            field = "falaQ3";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ3,'g',13) ,item->text(1)));

            field = "falaQ4";
            query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ4,'g',13) ,item->text(1)));

            calibrationSelectionChanged();
        }
    }
}

void PageStation::calibrationPropertyChanged(QtProperty *property, const QVariant &value)
{
    for(int i = 0; i<calibrationItemParent->children().count(); ++i)
    {
         //qDebug()<<property->propertyName()<<actItemParent->children().at(i)->property()->propertyName() ;
        if(calibrationItemParent->children().at(i)->property()->propertyName() == property->propertyName())
        {
            itemPos = i;
            itemPos++;
            isCalibrationChange = true;
            break;
        }
    }
    if (ui.treeWidgetCalibration->selectedItems().count()==0)
        return;

    QSqlQuery query;
    QString tableName = "CalibrationCategory";
    QString tableNameD = "CalibrationDetail";
    QString field;

    /**************update4 by cxs 2014 4.25*******************/
    qDebug()<<222222<<ui.treeWidgetCalibration->selectedItems().count()<<property->propertyName();
    foreach (QTreeWidgetItem *item, ui.treeWidgetCalibration->selectedItems())
    {
        qDebug()<<ui.treeWidgetCalibration->selectedItems().count()<<property->propertyName();
        if(!item->parent())
                continue;

        if(!item->parent()->parent())
        {

            //ActionObject *actionObject  = actionMap[item->text(1)];
            // int subCout = topPropertyAction->subProperties().count();
            if(property->propertyName() == "名称" )
            {
                calibrationMap[item->text(1)]->name = value.toString();
                item->setText(0,value.toString());
                field = "name";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, calibrationMap[item->text(1)]->name, item->text(1)));
                calibrationSelectionChanged();
            }
            if(property->propertyName() == "校准分类号" )
            {
                calibrationMap[item->text(1)]->calibrationNum = value.toInt();
                item->setText(0,value.toString());
                field = "calibrationNum";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, QString::number(calibrationMap[item->text(1)]->calibrationNum), item->text(1)));
                calibrationSelectionChanged();
            }
        }
        else    //明细校准
        {
            int indexOfChild = item->parent()->indexOfChild(item);
            if(indexOfChild == 0)
            {
                if(property->propertyName() == "选取点位")
                {
                    pageViewer->showToolWidget(3);
                    value.toInt();
                }
                if(property->propertyName() == "X")
                {
                    for(int i = 0; i < 9; ++i)
                    {
                        qDebug()<<topPropertyCalibration->subProperties().at(i+2)->propertyName() ;
                        if(property == topPropertyCalibration->subProperties().at(i+2)->subProperties().at(0))
                        {
                            QStringList tmpList = calibrationDetailMap[item->text(1)]->point[i].split("|");

                            while (tmpList.length()<3) tmpList.append("");
                            tmpList[0] = value.toString();
                            calibrationDetailMap[item->text(1)]->point[i] = tmpList.join("|");
                            field = QString("point%1").arg(i);
                            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, calibrationDetailMap[item->text(1)]->point[i], item->text(1)));
                            break;
                        }
                    }
                }
                if(property->propertyName() == "Y"/* && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(1)*/)
                {
                    for(int i = 0; i < 9; ++i)
                    {
                         //qDebug()<<property->propertyName()<<actItemParent->children().at(i+2)->property()->propertyName();
                        qDebug()<<topPropertyCalibration->subProperties().at(i+2)->subProperties().at(1);
                        if(property == topPropertyCalibration->subProperties().at(i+2)->subProperties().at(1))
                        {
                            QStringList tmpList = calibrationDetailMap[item->text(1)]->point[i].split("|");
                            while (tmpList.length()<3) tmpList.append("");
                            tmpList[1] = value.toString();
                            calibrationDetailMap[item->text(1)]->point[i] = tmpList.join("|");
                            field = QString("point%1").arg(i);
                            qDebug()<<"point%1"<<field;
                            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, calibrationDetailMap[item->text(1)]->point[i], item->text(1)));
                            break;
                        }
                    }
                   //actionSelectionChanged();
                }
                if(property->propertyName() == "Z"/* && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(2)*/)
                {
                    for(int i = 0; i < 9; ++i)
                    {
                        //qDebug()<<property->propertyName()<<actItemParent->children().at(i)->property()->propertyName() ;
                        if(property == topPropertyCalibration->subProperties().at(i+2)->subProperties().at(2))
                        {
                            QStringList tmpList = calibrationDetailMap[item->text(1)]->point[i].split("|");
                            while (tmpList.length()<3) tmpList.append("");
                            tmpList[2] = value.toString();
                            calibrationDetailMap[item->text(1)]->point[i] = tmpList.join("|");
                            field = QString("point%1").arg(i);
                            qDebug()<<"point%1"<<field;
                            query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, calibrationDetailMap[item->text(1)]->point[i], item->text(1)));
                            break;
                        }
                    }
                   //actionSelectionChanged();
                }

                //else if(property->propertyName() == "法拉盘位置")
                else if(property->propertyName() == "X" && property == topPropertyCalibration->subProperties().at(9+2)->subProperties().at(0))
                {
                    calibrationDetailMap[item->text(1)]->falaX = value.toDouble();
                    field = "falaX";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaX,'g',13) ,item->text(1)));
                }else if(property->propertyName() == "Y" && property == topPropertyCalibration->subProperties().at(9+2)->subProperties().at(1))
                {
                    calibrationDetailMap[item->text(1)]->falaY = value.toDouble();
                    field = "falaY";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaY,'g',13) ,item->text(1)));
                }else if(property->propertyName() == "Z" && property == topPropertyCalibration->subProperties().at(9+2)->subProperties().at(2))
                {
                    calibrationDetailMap[item->text(1)]->falaZ = value.toDouble();
                    field = "falaZ";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaZ,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q1")
                {
                    calibrationDetailMap[item->text(1)]->falaQ1 = value.toDouble();
                    field = "falaQ1";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ1,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q2")
                {
                    calibrationDetailMap[item->text(1)]->falaQ2 = value.toDouble();
                    field = "falaQ2";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ2,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q3")
                {
                    calibrationDetailMap[item->text(1)]->falaQ3 = value.toDouble();
                    field = "falaQ3";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ3,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q4")
                {
                    calibrationDetailMap[item->text(1)]->falaQ4 = value.toDouble();
                    field = "falaQ4";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->falaQ4,'g',13) ,item->text(1)));
                }
            }else if(indexOfChild == 1)
            {
                if(property->propertyName() == "X"&& property == topPropertyCalibration->subProperties().at(1)->subProperties().at(0))
                {
                    calibrationDetailMap[item->text(1)]->toolPosX = value.toDouble();
                    field = "toolPosX";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolPosX,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "Y" && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(1))
                {
                    calibrationDetailMap[item->text(1)]->toolPosY = value.toDouble();
                    field = "toolPosY";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolPosY,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "Z" && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(2))
                {
                    calibrationDetailMap[item->text(1)]->toolPosZ = value.toDouble();
                    field = "toolPosZ";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolPosZ,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "Q1")
                {
                    calibrationDetailMap[item->text(1)]->toolQ1 = value.toDouble();
                    field = "toolQ1";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolQ1,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q2")
                {
                    calibrationDetailMap[item->text(1)]->toolQ2 = value.toDouble();
                    field = "toolQ2";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolQ2,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q3")
                {
                    calibrationDetailMap[item->text(1)]->toolQ3 = value.toDouble();
                    field = "toolQ3";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolQ3,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "Q4")
                {
                    calibrationDetailMap[item->text(1)]->toolQ4 = value.toDouble();
                    field = "toolQ4";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolQ4,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "重心设定X")
                {
                    calibrationDetailMap[item->text(1)]->toolMassX = value.toDouble();
                    field = "toolMassX";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolMassX,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "重心设定Y")
                {
                    calibrationDetailMap[item->text(1)]->toolMassY = value.toDouble();
                    field = "toolMassY";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolMassY,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "重心设定Z")
                {
                    calibrationDetailMap[item->text(1)]->toolMassZ = value.toDouble();
                    field = "toolMassZ";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolMassZ,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "重心设定Kg")
                {
                    calibrationDetailMap[item->text(1)]->toolKg = value.toDouble();
                    field = "toolKg";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->toolKg,'g',13) ,item->text(1)));
                }

            }else if(indexOfChild == 2)
            {
                if(property->propertyName() == "X"&& property == topPropertyCalibration->subProperties().at(1)->subProperties().at(0))
                {
                    calibrationDetailMap[item->text(1)]->sensorPosX = value.toDouble();
                    field = "sensorPosX";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorPosX,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "Y" && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(1))
                {
                    calibrationDetailMap[item->text(1)]->sensorPosY = value.toDouble();
                    field = "sensorPosY";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorPosY,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "Z" && property == topPropertyCalibration->subProperties().at(1)->subProperties().at(2))
                {
                    calibrationDetailMap[item->text(1)]->sensorPosZ = value.toDouble();
                    field = "sensorPosZ";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorPosZ,'g',13) ,item->text(1)));
                }
                if(property->propertyName() == "传感器方向Vx")
                {
                    calibrationDetailMap[item->text(1)]->sensorDirectionX = value.toDouble();
                    field = "sensorDirectionX";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorDirectionX,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "传感器方向Vy")
                {
                    calibrationDetailMap[item->text(1)]->sensorDirectionY = value.toDouble();
                    field = "sensorDirectionY";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorDirectionY,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "传感器方向Vz")
                {
                    calibrationDetailMap[item->text(1)]->sensorDirectionZ = value.toDouble();
                    field = "sensorDirectionZ";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorDirectionZ,'g',13) ,item->text(1)));
                }
                else if(property->propertyName() == "传感器距离")
                {
                    calibrationDetailMap[item->text(1)]->sensorDistance = value.toDouble();
                    field = "sensorDistance";
                    query.exec(QString("update %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number(calibrationDetailMap[item->text(1)]->sensorDistance,'g',13) ,item->text(1)));
                }
            }

            qDebug()<<property->propertyName();
        }
    }
}

void PageStation::calibrationNew()
{
    if (currentStationId.isEmpty())
        return;

    disconnect(ui.treeWidgetCalibration, SIGNAL(itemSelectionChanged()), this, SLOT(calibrationSelectionChanged()));
    QTreeWidgetItem *cItem = ui.treeWidgetCalibration->currentItem();
    if (!cItem)
        return;

    if(cItem->parent())
    {
        QMessageBox::warning(NULL,"","请选择第一级列表",QMessageBox::tr("确认"));
        return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
     if(!cItem->parent())
         item->setText(0, "工件校准");
     else
         item->setText(0, "");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    QTreeWidgetItem *subItem1 = new QTreeWidgetItem();
    QTreeWidgetItem *subItem2 = new QTreeWidgetItem();
    QTreeWidgetItem *subItem3 = new QTreeWidgetItem();

    subItem1->setText(0, "校准点位设置");
    subItem2->setText(0, "工具设定");
    subItem3->setText(0, "传感器设定");

    item->addChild(subItem1);
    item->addChild(subItem2);
    item->addChild(subItem3);

    ui.treeWidgetCalibration->setCurrentItem(item);
    //ui.treeWidgetCalibration->editItem(item);

    QSqlQuery query;
    if(!cItem->parent())
    {

        ui.treeWidgetCalibration->editItem(item);
        if ( query.exec(QString("INSERT INTO CalibrationCategory(id,stationId,name, calibrationNum) VALUES ('%1','%2','%3','%4')").arg(item->text(1), currentStationId, item->text(0), QString::number(calibrationNum))) )
        {
            qDebug("增加成功！");
            CalibrationObject *calibrationObject = new CalibrationObject();
            calibrationObject->id = item->text(1);
            calibrationObject->stationId = currentStationId;
            calibrationObject->name = item->text(0);
            calibrationObject->calibrationNum = 0;

            calibrationMap.insert(calibrationObject->id, calibrationObject);

        } else {
            qDebug("增加失败！", true);
            return;
        }

        CalibrationDetail *calibrationDetail = new CalibrationDetail();
        calibrationDetail->id = SysUtils::uuid();
        calibrationDetail->calibrationId = item->text(1);
        qDebug()<<calibrationDetail->id<<item->text(1);
        calibrationDetail->name = item->text(0);
        calibrationDetail->point[0] = "1|0|0";
        calibrationDetail->point[1] = "0|0|0";
        calibrationDetail->point[2] = "0|0|0";
        calibrationDetail->point[3] = "0|0|0";

        calibrationDetail->point[4] = "0|0|0";
        calibrationDetail->point[5] = "0|0|0";
        calibrationDetail->point[6] = "0|0|0";
        calibrationDetail->point[7] = "0|0|0";
        calibrationDetail->point[8] = "0|0|0";

        calibrationDetail->falaX = 0;
        calibrationDetail->falaY = 0;
        calibrationDetail->falaZ = 0;
        calibrationDetail->falaQ1 = 0;
        calibrationDetail->falaQ2 = 0;
        calibrationDetail->falaQ3 = 0;
        calibrationDetail->falaQ4 = 0;

        calibrationDetail->toolPosX = 0;
        calibrationDetail->toolPosY = 0;
        calibrationDetail->toolPosZ = 0;
        calibrationDetail->toolQ1 = 0;
        calibrationDetail->toolQ2 = 0;
        calibrationDetail->toolQ3 = 0;
        calibrationDetail->toolQ4 = 0;
        calibrationDetail->toolKg= 0;
        calibrationDetail->toolMassX = 0;
        calibrationDetail->toolMassY = 0;
        calibrationDetail->toolMassZ = 0;

        calibrationDetail->sensorPosX = 0;
        calibrationDetail->sensorPosY = 0;
        calibrationDetail->sensorPosZ = 0;
        calibrationDetail->sensorDirectionX = 0;
        calibrationDetail->sensorDirectionY = 0;
        calibrationDetail->sensorDirectionZ = 0;
        calibrationDetail->sensorReads = 0;
        calibrationDetail->sensorDistance = 0;
        calibrationDetail->pnt_index = -1;

        calibrationDetailMap.insert(calibrationDetail->id, calibrationDetail);
        qDebug()<<cItem->text(1);
        query.prepare(QString("INSERT INTO CalibrationDetail(id, calibrationId, name, point0, point1, point2, point3, point4, point5, point6, point7, point8, falaX, falaY, falaZ,falaQ1,"\
                                      "falaQ2,falaQ3, falaQ4, toolPosX, toolPosY, toolPosZ, toolQ1,toolQ2,toolQ3,toolQ4,"\
                                      "toolKg,toolMassX,toolMassY,toolMassZ,sensorPosX,sensorPosY,sensorPosZ,sensorDirectionX,sensorDirectionY,"\
                                      "sensorDirectionZ,sensorReads, sensorDistance)VALUES(?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?,  ?,?,?)"));
                        query.addBindValue(QString(calibrationDetail->id));
                        query.addBindValue(QString(calibrationDetail->calibrationId));
                        query.addBindValue(item->text(0));
                        query.addBindValue(QString(calibrationDetail->point[0]));
                        query.addBindValue(QString(calibrationDetail->point[1]));
                        query.addBindValue(QString(calibrationDetail->point[2]));
                        query.addBindValue(QString(calibrationDetail->point[3]));
                        query.addBindValue(QString(calibrationDetail->point[4]));
                        query.addBindValue(QString(calibrationDetail->point[5]));
                        query.addBindValue(QString(calibrationDetail->point[6]));
                        query.addBindValue(QString(calibrationDetail->point[7]));
                        query.addBindValue(QString(calibrationDetail->point[8]));
                        query.addBindValue(QString::number(calibrationDetail->falaX));
                        query.addBindValue(QString::number(calibrationDetail->falaY));
                        query.addBindValue(QString::number(calibrationDetail->falaZ));
                        query.addBindValue(QString::number(calibrationDetail->falaQ1));
                        query.addBindValue(QString::number(calibrationDetail->falaQ2));
                        query.addBindValue(QString::number(calibrationDetail->falaQ3));
                        query.addBindValue(QString::number(calibrationDetail->falaQ4));
                        query.addBindValue(QString::number(calibrationDetail->toolPosX));
                        query.addBindValue(QString::number(calibrationDetail->toolPosY));
                        query.addBindValue(QString::number(calibrationDetail->toolPosZ));

                        query.addBindValue(QString::number(calibrationDetail->toolQ1));
                        query.addBindValue(QString::number(calibrationDetail->toolQ2));
                        query.addBindValue(QString::number(calibrationDetail->toolQ3));
                        query.addBindValue(QString::number(calibrationDetail->toolQ4));
                        query.addBindValue(QString::number(calibrationDetail->toolKg));
                        query.addBindValue(QString::number(calibrationDetail->toolMassX));
                        query.addBindValue(QString::number(calibrationDetail->toolMassY));
                        query.addBindValue(QString::number(calibrationDetail->toolMassZ));

                        query.addBindValue(QString::number(calibrationDetail->sensorPosX));
                        query.addBindValue(QString::number(calibrationDetail->sensorPosY));
                        query.addBindValue(QString::number(calibrationDetail->sensorPosZ));
                        query.addBindValue(QString::number(calibrationDetail->sensorDirectionX));
                        query.addBindValue(QString::number(calibrationDetail->sensorDirectionY));
                        query.addBindValue(QString::number(calibrationDetail->sensorDirectionZ));
                        query.addBindValue(QString::number(calibrationDetail->sensorReads));
                        query.addBindValue(QString::number(calibrationDetail->sensorDistance));

        if(!query.exec()){
            qDebug()<<query.lastError().text()<<query.lastError().number()<<query.lastQuery();
            return;
        }
        subItem1->setText(1, calibrationDetail->id);
        subItem2->setText(1, calibrationDetail->id);
        subItem3->setText(1, calibrationDetail->id);
    }

    connect(ui.treeWidgetCalibration, SIGNAL(itemSelectionChanged()), this, SLOT(calibrationSelectionChanged()));
    calibrationSelectionChanged();
    qDebug(item->text(1).toLocal8Bit().constData());
}


void PageStation::sendCalibrationData() //发送信息
{

    QString key;
    foreach (QTreeWidgetItem *item, ui.treeWidgetCalibration->selectedItems())
    {
        if(!item->parent())
                continue;


        if(!item->parent()->parent())
        {
            qDebug()<<item->text(0)<<item->child(0)->text(1);
            qDebug()<<calibrationDetailMap.size();
            key = item->child(0)->text(1);
        }
        else    //明细校准
        {
            qDebug()<<item->text(0)<<item->text(1);
            qDebug()<<calibrationDetailMap.size();
            key = item->text(1);
        }
    }

    if(key.isEmpty())
    {
        return;
    }

    QString str = "?point" + QString::number(calibrationDetailMap[key]->pointX[0],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[0],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[0],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[1],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[1],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[1],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[2],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[2],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[2],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[3],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[3],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[3],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[4],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[4],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[4],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[5],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[5],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[5],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[6],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[6],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[6],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[7],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[7],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[7],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointX[8],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointY[8],'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->pointZ[8],'f',7);

    str+="fala" + QString::number(calibrationDetailMap[key]->falaX,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaY,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaZ,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaQ1,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaQ2,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaQ3,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->falaQ4,'f',7);

    str+="pSensor" + QString::number(calibrationDetailMap[key]->sensorPosX,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->sensorPosY,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->sensorPosZ,'f',7);


    str+="vecSensor" + QString::number(calibrationDetailMap[key]->sensorDistance,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->sensorDirectionX,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->sensorDirectionY,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->sensorDirectionZ,'f',7);

    str+="tool" + QString::number(calibrationDetailMap[key]->toolPosX,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolPosY,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolPosZ,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolQ1,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolQ2,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolQ3,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolQ4,'f',7);

    str+="mass" + QString::number(calibrationDetailMap[key]->toolMassX,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolMassY,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolMassZ,'f',7);
    str+="|" + QString::number(calibrationDetailMap[key]->toolKg,'f',7);

    str+="dP" + QString::number(0.0,'f',7);
    str+="|" + QString::number(0.0,'f',7);
    str+="|" + QString::number(0.0,'f',7);

    QByteArray dataArray = str.toLocal8Bit();
    dataArray[0] = 0x35;
    int res = mainWindow->sendToPythonService(dataArray.data(),dataArray.length());
    if(res>-1)
    {
        qDebug()<<"-------- 发送数据给python程序成功 --------\n";
        emit addLog("发送数据给workObjCal.exe程序成功","");
    }else
    {
        qDebug()<<"-------- 发送数据给python程序失败 --------\n";
        emit addLog("发送数据给workObjCal.exe程序失败","");
    }
}
int PageStation::generateRapidToolData()
{

    sendCalibrationData();


    return 1;
}


void PageStation::calibrationFlange()
{
    // TODO: Add your control notification handler code here
    if (NULL == hModule)
    {
        hModule = ::LoadLibraryA("RobotSolutionDLL.dll");
    }

    QSqlQuery query;
    QString tableName = "CalibrationCategory";
    QString tableNameD = "CalibrationDetail";
    QString field;

    positiveSolutionDLL positiveSolution;
    positiveSolution = positiveSolutionDLL(GetProcAddress(hModule, "positiveSolution"));

    angleToQuaternionDLL angleToQuaternion;
    angleToQuaternion = angleToQuaternionDLL(GetProcAddress(hModule, "angleToQuaternion"));

    foreach (QTreeWidgetItem *item, ui.treeWidgetCalibration->selectedItems())
    {
        //qDebug()<<ui.treeWidgetCalibration->selectedItems().count()<<item->text(0)<<item->parent()->indexOfChild(item);
        if(!item->parent())
                continue;


        QString cur_robotName = ui.cur_robot->currentText();
        QString calibrationDetailId;
        if(!item->parent()->parent())
        {
            calibrationDetailId = item->child(0)->text(1);
        }
        else    //明细校准
        {
            calibrationDetailId = item->text(1);
        }

        //ABB1600 10/1.45运动学参数
        int RobotType;
        double RobotParamsX[6];
        double RobotParamsZ[6];
        double RobotParamsAS[6];
        double RobotParamsAE[6];

        //取数据数据
        QSqlQuery query;
        QSqlQuery queryRootAxis;
        robotMap;
        qDebug()<<ui.cur_robot->currentText();

        query.exec(QString("SELECT * FROM RobotParameter WHERE name = '%1'").arg(cur_robotName));
        int countRobot = 0;
        while(query.next())
        {
            countRobot++;
            RobotType = query.value(2).toDouble();
            RobotParamsX[0] = query.value(3).toDouble();
            RobotParamsX[1] = query.value(4).toDouble();
            RobotParamsX[2] = query.value(5).toDouble();
            RobotParamsX[3] = query.value(6).toDouble();
            RobotParamsX[4] = query.value(7).toDouble();
            RobotParamsX[5] = query.value(8).toDouble();
            RobotParamsZ[0] = query.value(9).toDouble();
            RobotParamsZ[1] = query.value(10).toDouble();
            RobotParamsZ[2] = query.value(11).toDouble();
            RobotParamsZ[3] = query.value(12).toDouble();
            RobotParamsZ[4] = query.value(13).toDouble();
            RobotParamsZ[5] = query.value(14).toDouble();
            RobotParamsAS[0] = query.value(15).toDouble();
            RobotParamsAS[1] = query.value(16).toDouble();
            RobotParamsAS[2] = query.value(17).toDouble();
            RobotParamsAS[3] = query.value(18).toDouble();
            RobotParamsAS[4] = query.value(19).toDouble();
            RobotParamsAS[5] = query.value(20).toDouble();
            RobotParamsAE[0] = query.value(21).toDouble();
            RobotParamsAE[1] = query.value(22).toDouble();
            RobotParamsAE[2] = query.value(23).toDouble();
            RobotParamsAE[3] = query.value(24).toDouble();
            RobotParamsAE[4] = query.value(25).toDouble();
            RobotParamsAE[5] = query.value(26).toDouble();
        }

        if(countRobot > 0)
        {
            double Solution[6];
            double Angles[6] = {0,};

            AxisObject *ax1 = NULL;
            AxisObject *ax2 = NULL;
            AxisObject *ax3 = NULL;
            AxisObject *ax4 = NULL;
            AxisObject *ax5 = NULL;
            AxisObject *ax6 = NULL;
            QString axisId;
            AxisObject *axBase = NULL;
            int index = ui.cur_robot->itemData(ui.cur_robot->currentIndex()).toInt();
            queryRootAxis.exec(QString("select rootAxis from StationMotion, RobotLibrary where libraryId=RobotLibrary.id and stationId='%1' and displayOrder=%2 order by displayOrder ASC limit 1").arg(currentStationId).arg(index));

            qDebug()<<currentStationId<<cur_robotName;
            if(queryRootAxis.next()) {
        //        QSqlRecord record = query.record();
        //        axisId = record.value("rootAxis").toString();
                axisId = queryRootAxis.value(0).toString();
                axBase = axisMap[axisId];
                if (!axBase || axBase->subAxisList.count()==0)
                    return;
                ax1 = axisMap[axBase->subAxisList.at(0)];
                if (!ax1 || ax1->subAxisList.count()==0)
                    return;
                ax2 = axisMap[ax1->subAxisList.at(0)];
                if (!ax2 || ax2->subAxisList.count()==0)
                    return;
                ax3 = axisMap[ax2->subAxisList.at(0)];
                if (!ax3 || ax3->subAxisList.count()==0)
                    return;
                ax4 = axisMap[ax3->subAxisList.at(0)];
                if (!ax4 || ax4->subAxisList.count()==0)
                    return;
                ax5 = axisMap[ax4->subAxisList.at(0)];
                if (!ax5 || ax5->subAxisList.count()==0)
                    return;
                ax6 = axisMap[ax5->subAxisList.at(0)];
            } else {
                return;
            }
            Angles[0] = ax1->value;
            Angles[1] = ax2->value;
            Angles[2] = ax3->value;
            Angles[3] = ax4->value;
            Angles[4] = ax5->value;
            Angles[5] = ax6->value;
            double WPR[3];
            double Quaternion[4];
            positiveSolution(RobotType, RobotParamsX, RobotParamsZ, RobotParamsAS, RobotParamsAE, Angles, Solution);
            WPR[0] = Solution[0];
            WPR[1] = Solution[1];
            WPR[2] = Solution[2];
            angleToQuaternion(WPR, Quaternion);
            calibrationDetailMap[calibrationDetailId]->falaX = Solution[3];
            calibrationDetailMap[calibrationDetailId]->falaY = Solution[4];
            calibrationDetailMap[calibrationDetailId]->falaZ = Solution[5];
            calibrationDetailMap[calibrationDetailId]->falaQ1 = Quaternion[0];
            calibrationDetailMap[calibrationDetailId]->falaQ2 = Quaternion[1];
            calibrationDetailMap[calibrationDetailId]->falaQ3 = Quaternion[2];
            calibrationDetailMap[calibrationDetailId]->falaQ4 = Quaternion[3];
            emit calibrationFlangeSuccess();
        }else
        {
             QMessageBox::warning(NULL,"","未找到"+cur_robotName+"型号机械参数",QMessageBox::tr("确认"));
        }
    }
}


void PageStation::trajectorySelectionChanged()
{
    if (!this->isVisible()) return;

    disconnect(variantManagerTrajectory, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
               this, SLOT(trajectoryPropertyChanged(QtProperty *, const QVariant &)));
    if(ui.treeWidgetTrajectory->selectedItems().count()>0)
    {

        QTreeWidgetItem *item = ui.treeWidgetTrajectory->selectedItems().last();
        if (!item->parent()){
            topPropertyTrajectory->setEnabled(false);
            return;
        }
        foreach (QtProperty *property, topPropertyTrajectory->subProperties())
            topPropertyTrajectory->removeSubProperty(property);

        //轨迹分类部分
        if(!item->parent()->parent())
        {
            QtVariantProperty *itemPro = variantManagerTrajectory->addProperty(QVariant::String, tr("名称"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryMap[item->text(1)]->name);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Int, tr("轨迹分类号"));
            topPropertyTrajectory->addSubProperty(itemPro);

            itemPro->setValue(trajectoryMap[item->text(1)]->trajectoryNum);
            topPropertyTrajectory->setEnabled(true);
        }

        //轨迹明细部分
        else
        {
            QtVariantProperty *itemPro = variantManagerTrajectory->addProperty(QVariant::String, tr("路径名称"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->name);

            itemPro = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("路径类型"));
            QStringList pathType;
            pathType<<"MoveNbs"<<"MoveLine";
            itemPro->setAttribute(QLatin1String("enumNames"), pathType);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->pathType-1);
            topPropertyTrajectory->addSubProperty(itemPro);

            itemPro = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("选择曲线"));
            QStringList selectCurve;
            selectCurve<<"未选定"<<"已选定";
            itemPro->setAttribute(QLatin1String("enumNames"), selectCurve);
            if(trajectoryDetailMap[item->text(1)]->pathFile.isEmpty())
            {
                itemPro->setValue(0);
            }else
            {
                itemPro->setValue(1);
            }
            topPropertyTrajectory->addSubProperty(itemPro);



            itemPro = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("刀具类型"));
            QStringList knifeType;
            knifeType<<"刀具1"<<"刀具2"<<"刀具3"<<"刀具4"<<"刀具5";
            itemPro->setAttribute(QLatin1String("enumNames"), knifeType);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeType-1);
            topPropertyTrajectory->addSubProperty(itemPro);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("切割半径"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->cutRadius);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("切割深度"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->cutDepth);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("切割角度"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->cutAngle);

            itemPro = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("切割方向1"));
            QStringList cutDirection1;
            cutDirection1<<"内"<<"外";
            itemPro->setAttribute(QLatin1String("enumNames"), cutDirection1);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->in1-1);
            topPropertyTrajectory->addSubProperty(itemPro);

            itemPro = variantManagerTrajectory->addProperty(QtVariantPropertyManager::enumTypeId(), tr("切割方向2"));
            QStringList cutDirection2;
            cutDirection2<<"内"<<"外";
            itemPro->setAttribute(QLatin1String("enumNames"), cutDirection2);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->out1-1);
            topPropertyTrajectory->addSubProperty(itemPro);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("精度1"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->precision1);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("精度2"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->precision2);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具坐标X"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeX);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具坐标Y"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeY);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具坐标Z"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeZ);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具方向X"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeDirectionX);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具方向Y"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeDirectionY);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具方向Z"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->knifeDirectionZ);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具长边"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->longBase);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具高"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->height);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("刀具短边"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->shortBase);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘坐标X"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaX);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘坐标Y"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaY);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘坐标Z"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaZ);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘姿态角RX"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaRX);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘姿态角RY"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaRY);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("法兰盘姿态角RZ"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->FalaRZ);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("备用1"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->others1);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("备用2"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->others2);

            itemPro = variantManagerTrajectory->addProperty(QVariant::Double, tr("备用3"));
            topPropertyTrajectory->addSubProperty(itemPro);
            itemPro->setValue(trajectoryDetailMap[item->text(1)]->others3);

            topPropertyTrajectory->setEnabled(true);
        }
    }

    if(isTrajectoryChange)
    {
        if(itemPos >= trajectoryItemParent->children().count())
            itemPos = 0;
        isTrajectoryChange = false;
    }
    connect(variantManagerTrajectory, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(trajectoryPropertyChanged(QtProperty *, const QVariant &)));
}

void PageStation::selectPath()
{
    if (currentStationId.isEmpty())
        return;

//    disconnect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
//    QTreeWidgetItem *cItem = ui.treeWidgetTrajectory->currentItem();
//    if (!cItem)
//        return;
    //emit selectionEdge();
//    if(cItem->parent())
//    {
//        if(cItem->parent()->parent())
//            return;
//    }

//    connect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
//    trajectorySelectionChanged();
//    qDebug(item->text(1).toLocal8Bit().constData());

}

void PageStation::trajectoryNew()
{
    if (currentStationId.isEmpty())
        return;

    disconnect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
    QTreeWidgetItem *cItem = ui.treeWidgetTrajectory->currentItem();
    if (!cItem)
        return;

    if(cItem->parent())
    {
        if(cItem->parent()->parent())
            return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem();
     if(!cItem->parent())
         item->setText(0, "新轨迹");
     else
         item->setText(0, "");
    item->setText(1, SysUtils::uuid());
    item->setFlags(item->flags()|Qt::ItemIsEditable);

    cItem->addChild(item);

    ui.treeWidgetTrajectory->setCurrentItem(item);
   // ui.treeWidgetAction->editItem(item);

    QSqlQuery query;
    if(!cItem->parent())
    {
        ui.treeWidgetTrajectory->editItem(item);
        if ( query.exec(QString("INSERT INTO TrajectoryCategory(id,stationId,name,trajectoryNum) VALUES ('%1','%2','%3','%4')").arg(item->text(1), currentStationId, item->text(0), QString::number(trajectoryNum))) )
        {
            qDebug("增加成功！");
            TrajectoryObject *trajectoryObject = new TrajectoryObject();
            trajectoryObject->id = item->text(1);
            trajectoryObject->stationId = currentStationId;
            trajectoryObject->name = item->text(0);
            trajectoryObject->trajectoryNum = 0;

            trajectoryMap.insert(trajectoryObject->id,trajectoryObject);

        } else {
            qDebug("增加失败！", true);
            return;
        }

    }
    else
    {
        TrajectoryDetail *trajectoryDetail = new TrajectoryDetail();
        trajectoryDetail->id = item->text(1);
        trajectoryDetail->trajectoryId = cItem->text(1);
        trajectoryDetail->name = "Curve_10";
        trajectoryDetail->pathType = 1;
        trajectoryDetail->pathFile = "";

        trajectoryDetail->knifeType = 1;
        trajectoryDetail->pointIndensity = 0;
        trajectoryDetail->cutRadius = 0;
        trajectoryDetail->cutDepth = 0;
        trajectoryDetail->cutAngle = 0;
        trajectoryDetail->in1 = 1;
        trajectoryDetail->out1 = 1;

        trajectoryDetail->precision1 = 0;
        trajectoryDetail->precision2 = 0;

        trajectoryDetail->knifeX = 0;
        trajectoryDetail->knifeY = 0;
        trajectoryDetail->knifeZ = 0;
        trajectoryDetail->knifeDirectionX = 0;
        trajectoryDetail->knifeDirectionY = 0;
        trajectoryDetail->knifeDirectionZ = 0;
        trajectoryDetail->longBase = 0;
        trajectoryDetail->height = 0;
        trajectoryDetail->shortBase = 0;

        trajectoryDetail->FalaX = 0;
        trajectoryDetail->FalaY = 0;
        trajectoryDetail->FalaZ = 0;
        trajectoryDetail->FalaRX = 0;
        trajectoryDetail->FalaRY = 0;
        trajectoryDetail->FalaRZ = 0;

        trajectoryDetail->others1 = 0;
        trajectoryDetail->others2 = 0;
        trajectoryDetail->others3 = 0;

        trajectoryDetailMap.insert(trajectoryDetail->id,trajectoryDetail);

        query.prepare(QString("INSERT INTO TrajectoryDetail(id, trajectoryId, name, pathType, pathFile, knifeType, knifeType,"\
                                      "pointIndensity, cutRadius,cutDepth, cutAngle,in1,out1,precision1,knifeX,knifeY,knifeZ,knifeDirectionX,knifeDirectionY,"\
                                      "knifeDirectionZ,longBase,height,shortBase,FalaX,FalaY,FalaZ,FalaRX,FalaRY,FalaRZ,others1,"\
                                      "others2,others3)VALUES(?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,? ,?,?,?)"));
                        query.addBindValue(item->text(1));
                        query.addBindValue(cItem->text(1));
                        query.addBindValue(item->text(0));
                        query.addBindValue(trajectoryDetail->pathType);
                        query.addBindValue(QString(trajectoryDetail->pathFile));
                        query.addBindValue(QString::number(trajectoryDetail->knifeType));
                        query.addBindValue(QString::number(trajectoryDetail->pointIndensity));
                        query.addBindValue(QString::number(trajectoryDetail->cutRadius));
                        query.addBindValue(QString::number(trajectoryDetail->cutDepth));
                        query.addBindValue(QString::number(trajectoryDetail->cutAngle));
                        query.addBindValue(QString::number(trajectoryDetail->in1));
                        query.addBindValue(QString::number(trajectoryDetail->out1));
                        query.addBindValue(QString::number(trajectoryDetail->precision1));
                        query.addBindValue(QString::number(trajectoryDetail->precision2));
                        query.addBindValue(QString::number(trajectoryDetail->knifeX));
                        query.addBindValue(QString::number(trajectoryDetail->knifeY));
                        query.addBindValue(QString::number(trajectoryDetail->knifeZ));
                        query.addBindValue(QString::number(trajectoryDetail->knifeDirectionX));
                        query.addBindValue(QString::number(trajectoryDetail->knifeDirectionY));
                        query.addBindValue(QString::number(trajectoryDetail->knifeDirectionZ));
                        query.addBindValue(QString::number(trajectoryDetail->longBase));
                        query.addBindValue(QString::number(trajectoryDetail->height));
                        query.addBindValue(QString::number(trajectoryDetail->shortBase));
                        query.addBindValue(QString::number(trajectoryDetail->FalaX));
                        query.addBindValue(QString::number(trajectoryDetail->FalaY));
                        query.addBindValue(QString::number(trajectoryDetail->FalaZ));
                        query.addBindValue(QString::number(trajectoryDetail->FalaRX));
                        query.addBindValue(QString::number(trajectoryDetail->FalaRY));
                        query.addBindValue(QString::number(trajectoryDetail->FalaRZ));
                        query.addBindValue(QString::number(trajectoryDetail->others1));
                        query.addBindValue(QString::number(trajectoryDetail->others2));
                        query.addBindValue(QString::number(trajectoryDetail->others3));
               if(!query.exec()){
                   qDebug()<<query.lastError().text()<<query.lastError().number()<<query.lastQuery();
                   return;
               }
    }
    connect(ui.treeWidgetTrajectory, SIGNAL(itemSelectionChanged()), this, SLOT(trajectorySelectionChanged()));
    trajectorySelectionChanged();
    qDebug(item->text(1).toLocal8Bit().constData());

}

void PageStation::trajectoryRename()
{
    if(ui.treeWidgetTrajectory->currentItem()->parent())
    {
        if(ui.treeWidgetTrajectory->currentItem()->parent()->parent())
            return;
    }
    ui.treeWidgetTrajectory->edit(ui.treeWidgetTrajectory->currentIndex());
}

void PageStation::trajectoryRemove()
{
    QList<QTreeWidgetItem*> items = ui.treeWidgetAction->selectedItems();
    if (items.isEmpty())
        return;
    if (QMessageBox::Yes==QMessageBox::information(this, tr("系统提示"), tr("确认要删除选中的项目吗？"), QMessageBox::Yes | QMessageBox::No )) {
        qApp->restoreOverrideCursor();
        qApp->setOverrideCursor(Qt::WaitCursor);
        foreach (QTreeWidgetItem *item, items) {
            if (!item->text(1).isEmpty()) {
                QSqlQuery query;
                if(!item->parent()->parent())
                {

                if ( query.exec(QString("DELETE FROM StationAction WHERE id='%1'").arg(item->text(1))) ) {
                    qDebug("删除成功！");
                } else {
                    qDebug("删除失败！", true);
                    continue;
                }
                query.exec(QString("DELETE FROM  StationActionDetail WHERE  actionId ='%1'").arg(item->text(1)));

                }
                else
                {
                    if ( query.exec(QString("DELETE FROM StationActionDetail WHERE id='%1'").arg(item->text(1))) ) {
                        qDebug("删除成功！");
                    } else {
                        qDebug("删除失败！", true);
                        continue;
                    }
                }
            }
            item->parent()->removeChild(item);
        };
        qApp->restoreOverrideCursor();
    }
}



void PageStation::trajectoryPropertyChanged(QtProperty *property, const QVariant &value)
{
    for(int i = 0; i<trajectoryItemParent->children().count(); ++i)
    {
         //qDebug()<<property->propertyName()<<actItemParent->children().at(i)->property()->propertyName() ;
        if(trajectoryItemParent->children().at(i)->property()->propertyName() == property->propertyName())
        {
            itemPos = i;
            itemPos++;
            isTrajectoryChange = true;
            break;
        }
    }
    if (ui.treeWidgetTrajectory->selectedItems().count()==0)
        return;

    QSqlQuery query;
    QString tableName = "TrajectoryCategory";
    QString tableNameD = "TrajectoryDetail";
    QString field;

    foreach (QTreeWidgetItem *item, ui.treeWidgetTrajectory->selectedItems())
    {
        if(!item->parent())
                continue;

        if(!item->parent()->parent())
        {
            if(property->propertyName() == "名称" )
            {
                trajectoryMap[item->text(1)]->name = value.toString();
                item->setText(0,value.toString());
                field = "name";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, trajectoryMap[item->text(1)]->name, item->text(1)));
                trajectorySelectionChanged();
            }
            if(property->propertyName() == "轨迹分类号")
            {
                trajectoryMap[item->text(1)]->trajectoryNum = value.toInt();
                item->setText(0,value.toString());
                field = "trajectoryNum";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableName, field, QString::number(trajectoryMap[item->text(1)]->trajectoryNum), item->text(1)));
                trajectorySelectionChanged();
            }
        }
        else    //明细动作
        {

            if(property->propertyName() == "路径名称")
            {
                trajectoryDetailMap[item->text(1)]->name = value.toString();
                field = "name";
                if(!query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, trajectoryDetailMap[item->text(1)]->name, item->text(1))))
                {
                    qDebug()<<"falied";
                }
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "路径类型")
            {
                trajectoryDetailMap[item->text(1)]->pathType = value.toInt()+1;
                field = "pathType";

                query.exec(QString("UPDATE %1 set %2 = %3 where id = '%4'").arg(tableNameD, field, QString::number( trajectoryDetailMap[item->text(1)]->pathType), item->text(1)));
                trajectorySelectionChanged();

            }
            else if(property->propertyName() == "选择曲线")
            {
                trajectoryDetailMap[item->text(1)]->pathFile = value.toString();
                field = "pathFile";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, trajectoryDetailMap[item->text(1)]->pathFile, item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具类型")
            {
                trajectoryDetailMap[item->text(1)]->knifeType = value.toInt();
                field = "knifeType";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeType), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "切割半径")
            {
                trajectoryDetailMap[item->text(1)]->cutRadius = value.toDouble();
                field = "cutRadius";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->cutRadius), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "切割深度")
            {
                trajectoryDetailMap[item->text(1)]->cutDepth = value.toDouble();
                field = "cutDepth";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->cutDepth), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "切割角度")
            {
                trajectoryDetailMap[item->text(1)]->cutAngle = value.toDouble();
                field = "cutAngle";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->cutAngle), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "切割方向1")
            {
                trajectoryDetailMap[item->text(1)]->in1 = value.toInt();
                field = "in1";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->in1), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "切割方向2")
            {
                trajectoryDetailMap[item->text(1)]->out1 = value.toInt();
                field = "out1";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->out1), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "精度1")
            {
                trajectoryDetailMap[item->text(1)]->precision1 = value.toDouble();
                field = "precision1";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->precision1), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "精度2")
            {
                trajectoryDetailMap[item->text(1)]->precision2 = value.toDouble();
                field = "precision2";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->precision2), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具坐标X")
            {
                trajectoryDetailMap[item->text(1)]->knifeX = value.toDouble();
                field = "knifeX";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeX), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具坐标Y")
            {
                trajectoryDetailMap[item->text(1)]->knifeY = value.toDouble();
                field = "knifeY";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeY), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具坐标Z")
            {
                trajectoryDetailMap[item->text(1)]->knifeZ = value.toDouble();
                field = "knifeZ";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeZ), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具方向X")
            {
                trajectoryDetailMap[item->text(1)]->knifeDirectionX = value.toDouble();
                field = "knifeDirectionX";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeDirectionX), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具方向Y")
            {
                trajectoryDetailMap[item->text(1)]->knifeDirectionX = value.toDouble();
                field = "knifeDirectionY";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeDirectionY), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具方向Z")
            {
                trajectoryDetailMap[item->text(1)]->knifeDirectionX = value.toDouble();
                field = "knifeDirectionZ";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->knifeDirectionZ), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具长边")
            {
                trajectoryDetailMap[item->text(1)]->longBase = value.toDouble();
                field = "longBase";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->longBase), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具高")
            {
                trajectoryDetailMap[item->text(1)]->height = value.toDouble();
                field = "height";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->height), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "刀具短边")
            {
                trajectoryDetailMap[item->text(1)]->shortBase = value.toDouble();
                field = "shortBase";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->shortBase), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘坐标X")
            {
                trajectoryDetailMap[item->text(1)]->FalaX = value.toDouble();
                field = "FalaX";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaX), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘坐标Y")
            {
                trajectoryDetailMap[item->text(1)]->FalaY = value.toDouble();
                field = "FalaY";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaY), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘坐标Z")
            {
                trajectoryDetailMap[item->text(1)]->FalaZ = value.toDouble();
                field = "FalaZ";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaZ), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘姿态角X")
            {
                trajectoryDetailMap[item->text(1)]->FalaRX = value.toDouble();
                field = "FalaRX";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaRX), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘姿态角Y")
            {
                trajectoryDetailMap[item->text(1)]->FalaRY = value.toDouble();
                field = "FalaRY";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaRY), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "法兰盘姿态角Z")
            {
                trajectoryDetailMap[item->text(1)]->FalaRX = value.toDouble();
                field = "FalaRZ";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->FalaRZ), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "备用1")
            {
                trajectoryDetailMap[item->text(1)]->others1 = value.toDouble();
                field = "others1";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->others1), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "备用2")
            {
                trajectoryDetailMap[item->text(1)]->FalaRY = value.toDouble();
                field = "others2";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->others2), item->text(1)));
                trajectorySelectionChanged();
            }
            else if(property->propertyName() == "备用3")
            {
                trajectoryDetailMap[item->text(1)]->FalaRX = value.toDouble();
                field = "others3";
                query.exec(QString("UPDATE %1 set %2 = '%3' where id = '%4'").arg(tableNameD, field, QString::number(trajectoryDetailMap[item->text(1)]->others3), item->text(1)));
                trajectorySelectionChanged();
            }
        }
    }
}
void PageStation::calibrationRename()
{

}

void PageStation::calibrationDuplicate()
{

}

void PageStation::calibrationRemove()
{

}

