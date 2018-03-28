#include "MainWindow_all.h"

void MainWindow::processServiceMessage()
{

    while (serviceSocket[0]->hasPendingDatagrams()) {
      try {

        QByteArray result;
        result.resize(serviceSocket[0]->pendingDatagramSize());
        serviceSocket[0]->readDatagram(result.data(), result.size());


        if (serviceConnected[0] == false) {
            continue;
        }
        //qDebug()<<"abc"<<result;


        uchar len = result[0]-1;//移除一位,长度减1
        uchar command = result[1];
        uchar robotIndex=result[2];// robotIndex=1,2,3
        result.remove(2,1);//得到机器index,保持原数据不变


        //log

        if(fileState[0]&&!g_camera->recordLog)
        {

            if(command!=0x10&&command!=0x12)
            {
                QTextStream txtOutput(file[0]);
                txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" cmd:"<<QString::number((uchar)result[1],16)<<" robot:"<<QString::number(robotIndex)<<" data:"<<result.mid(2)<<"\r\n";
                file[0]->flush();
            }
        }
        else
        {

            QTextStream txtOutput(file[0]);
            txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" cmd:"<<QString::number((uchar)result[1],16)<<" robot:"<<QString::number(robotIndex)<<" data:"<<result.mid(2)<<"\r\n";
            file[0]->flush();
        }
        //end
        //qDebug()<<result;

        if (command == CMD_POSITION) {

            //uchar tt;
            //for (int i=2;i<len;i++){
            //	tt = result[i];
            //	tt = 0;
            //}
            //tt = result[2];
            //result[2] = result[5];
            //result[5] = result[2];
            //tt = result[3];
            //result[3] = result[4];
            //result[4] = result[3];

            float ax1, ax2, ax3, ax4, ax5, ax6;
            ax1 = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000000.0;
            ax2 = (((uchar)result[9] << 24) | ((uchar)result[8] << 16) | ((uchar)result[7] << 8) | ((uchar)result[6] << 0)) / 1000000.0;
            ax3 = (((uchar)result[13] << 24) | ((uchar)result[12] << 16) | ((uchar)result[11] << 8) | ((uchar)result[10] << 0)) / 1000000.0;
            ax4 = (((uchar)result[17] << 24) | ((uchar)result[16] << 16) | ((uchar)result[15] << 8) | ((uchar)result[14] << 0)) / 1000000.0;
            ax5 = (((uchar)result[21] << 24) | ((uchar)result[20] << 16) | ((uchar)result[19] << 8) | ((uchar)result[18] << 0)) / 1000000.0;
            ax6 = (((uchar)result[25] << 24) | ((uchar)result[24] << 16) | ((uchar)result[23] << 8) | ((uchar)result[22] << 0)) / 1000000.0;
            QStringList resultPos;
            resultPos.append( QString("%1").arg( ax1 ) );
            resultPos.append( QString("%1").arg( ax2 ) );
            resultPos.append( QString("%1").arg( ax3 ) );
            resultPos.append( QString("%1").arg( ax4 ) );
            resultPos.append( QString("%1").arg( ax5 ) );
            resultPos.append( QString("%1").arg( ax6 ) );
            //QStringList resultPos = QString::fromLocal8Bit(result.data() + 2, len - 2).split(',');
//            qDebug(resultPos.join("|").toLocal8Bit());
            emit syncPosition( resultPos, robotIndex );

            // debug
//            emit addLog(resultPos.join("|"), QString());
//            emit addLog(QString("%1").arg( ax3 ), QString());

        }else if (command == CMD_UNIT_DATA) {	// unit IO
            //QByteArray *cp = new QByteArray(result.data(), len);

            //update by zhugz
           // emit updateUnitData( result.mid(1) );
        } else if (command == CMD_PROGRAM_ROW) {	// ProgramRow
            //emit updateProgramRow( result );
            if(curRobotIndex!=robotIndex)
                return;
            int row = 0;


            //row = (int)(*(result + 1));
            row = ((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            //row = result[4];
            QString programName = "";
            if (len > 6) {
                programName = QString::fromUtf8(result.mid(6), len - 6);
            }
            curRow=row;
            setCodeRow(row, programName);
//        } else if (command == CMD_DISPLAY_STATE) {	// DisplayState
//            int displayState, pickCount;
//            displayState = ((uchar)result[2]<<8) | (uchar)result[3];
//            pickCount = ((uchar)result[4]<<8) | (uchar)result[5];
//            emit syncDisplayState( displayState, pickCount );
        } else if (command == CMD_PANEL) {
            int state;
            state = ((uchar)result[2]<<8) | (uchar)result[3];
            openPanel( state );
        } else if (command == CMD_LOG) {
            //qDebug(QString::fromUtf8(result.mid(2), len - 2).toLocal8Bit());
            if (len>10) {
                QString msg=QString::fromUtf8(result.mid(2), len - 2);
                if(msg.toLower().contains("out of memory"))
                {
                    emit addLog("机器人内存溢出请重启机器人","");
                }
                emit addLog(msg, QString());
            }
        } else if (command == CMD_OPERATE_MODE) {


            if (result.at(2) == 0){
                operatorStates[robotIndex]="自动";

            }else if(result.at(2) == 1){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 2){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 3){
                operatorStates[robotIndex]="手动全速";

            }else if(result.at(2) == 4){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 5){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 6){
                operatorStates[robotIndex]="示教";

            }else if(result.at(2) == 12){
                operatorStates[robotIndex]="力控";

            }else if(result.at(2) == 13){
                operatorStates[robotIndex]="扭矩";

            }else {
                operatorStates[robotIndex]="未知";

            }

            setOperatorState(robotIndex);


        } else if (command == CMD_EXECUTE_STATE) {


            if (result.at(2) == 1) {
                execStates[robotIndex]="运行";
            } else if(result.at(2) == 2) {

                execStates[robotIndex]="停止";

            } else if(result.at(2) == 10) {
                execStates[robotIndex]="暂停";

            } else if(result.at(2) == 20) {
                execStates[robotIndex]="告警";


            } else if(result.at(2) == 21) {
                execStates[robotIndex]="急停";


            } else if(result.at(2) == 30) {
                execStates[robotIndex]="空闲";


            } else if(result.at(2) == 40) {
                execStates[robotIndex]="断电";


            } else {
                execStates[robotIndex]="未知";
                addLog("operatorState:未知 "+ QString::number(result.at(2)),"");

            }

            setExecState(robotIndex);


        } else if (command == CMD_CONNECT_STATE) {

            if (result.at(2) == 1) {
                connStates[robotIndex]="联机";
               // emit addLog(QString::number(robotIndex),"");
            }
            else if(result.at(2) == 11) {
                connStates[robotIndex]="虚拟机";

            }
            else {
                connStates[robotIndex]="脱机";

            }

                setConnState(robotIndex);


        } else if (command == CMD_SPEED_RATIO) {

            //addLog(QString("改变速度%1").arg((int)result.at(2)),"");
            if (result.at(2) >= 0 && result.at(2) <= 100) {

                speedCache[robotIndex-1]=result.at(2);

                if(curRobotIndex==robotIndex)
                emit updateSpeedRatio(result.at(2));
            }

        }
        else if(command==CMD_POSITION_TRANSFORM)
         {


         }
        else if (command == CMD_PROCESS_TIME) {
            float sec;
            sec = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000.0;
            emit updateProcessTime( sec );
        } else if (command == CMD_SIGNAL_CHANGED) {
            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');

            if (signalTriggerMaps[robotIndex-1].contains(data[0])){
                signalTriggerCache[data[0]] = data[1];
                triggerValueChanged((signalTriggerMaps[robotIndex-1])[data[0]]);
            }


            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value = data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);
                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_VAR_CHANGED) {


            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');

           // varTriggerMaps[robotIndex-1]["Vision_1"].append("QWAY5j36SwSPZgOOBjcD");

            if (varTriggerMaps[robotIndex-1].contains(data[0])){


                varTriggerCache[data[0]] = data[1];
                triggerValueChanged((varTriggerMaps[robotIndex-1])[data[0]]);

            }


            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {

                axBase = axisMap[query.value(0).toString()];
                axBase->value =data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;

                    qDebug()<<"bbb"<<axBase->name<<axBase->moduleList.size();
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);


                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {

                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_LOGON_RESULT) {

            if (!currentStationId.isEmpty()) {
                QString signalList="?", varList="?";
                signalTriggerMaps[robotIndex-1].clear();
                varTriggerMaps[robotIndex-1].clear();
                QSqlQuery query,query2;
                query.exec(QString("SELECT id,type,code,extraTrigger,code2,extraTrigger2,code3 FROM StationAction WHERE stationId='%1' AND isUse=1").arg(currentStationId));


                while(query.next()) {

                    QSqlRecord record = query.record();
                    \
                    switch (record.value("type").toInt()) {
                    case 1: // 信号
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
                        }
                        break;
                    }
                    case 2: // 变量
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        //update zhugz

                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            varList += "|" + code;


                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        //end
                        break;
                    }
                    case 3: // 信号/变量
                    case 21: // 刀具寿命(信号/变量)
                    case 31: // 测量保存动作(信号/变量)
                    case 32: // 视觉分析动作(信号/变量)
                    {

                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        //update zhugz
                        // emit addLog("视觉分析注册"+code,"");
                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            varList += "|" + code;
//                            if (varTriggerMaps[robotIndex-1].contains("Vision_1"))
//                                qDebug()<<"OK视觉分析注册"<<varTriggerMaps[0].size();



                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        //end
                        break;
                    }
                    case 33: // 称重动作(信号/变量)
                    case 34: // 伺服电机
                    {

                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        //update zhugz
                        // emit addLog("伺服电机注册"+code,"");
                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            varList += "|" + code;

                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        //end
                        break;
                    }
                    case 35: // 通用注册
                    {

                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();

                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());


                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());

                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());

                        }

                        break;
                    }
                    case 100: // 复位动作(信号/变量)
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
//                            varList += "|" + code3;
                        }
                        break;
                    }
                    }
                }

                foreach (QString code, signalTriggerMaps[robotIndex-1].keys()) {
                   if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    signalList += "|" + code;
                }

                foreach (QString code, varTriggerMaps[robotIndex-1].keys()) {
                   if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    varList += "|" + code;

                }


                // 辅助轴变量注册
                //emit addLog(QString("变量注册%1").arg(robotIndex),"");
                query.exec(QString("select id from stationMotion where stationid='%1' and displayOrder=%2 order by displayOrder").arg(currentStationId).arg(robotIndex));
                QString r_Axis;
                if(query.next())
                    r_Axis=query.value(0).toString();

                query.exec(QString("SELECT DISTINCT sign FROM StationAxis WHERE motionId='%1' AND isUse=1 AND sign<>''").arg(r_Axis));

                while(query.next()) {
                    signalList += "|" + query.value(0).toString();
                    varList += "|" + query.value(0).toString();

                }

                //地轨注册
//                query.exec(QString("select id from stationMotion where stationid='%1' and type='101'").arg(currentStationId));
//                QString guideXY;
//                if(query.next())
//                    guideXY=query.value(0).toString();

//                query.exec(QString("SELECT DISTINCT sign FROM StationAxis WHERE motionId='%1' AND isUse=1 AND sign<>''").arg(guideXY));

//                while(query.next()) {
//                    signalList += "|" + query.value(0).toString();
//                    varList += "|" + query.value(0).toString();

//                }
                signalList += "|GUIDE_ServoA|GUIDE_ServoB" ;
                varList += "|GUIDE_ServoA|GUIDE_ServoB";

                //输出变量注册
                query.exec(QString("select id from StationAction where stationId='%1' and type >31").arg(currentStationId));
                QStringList v_actionId,listTmp;

                while(query.next())
                {
                   query2.exec(QString("select value from StationActionDetail where actionId='%1'").arg(query.value(0).toString()));

                   //qDebug()<<"aaaa"<<QString("select value from StationActionDetail where actionId='%1'").arg(query.value(0).toString());
                   while(query2.next())
                   {
                       listTmp=query2.value(0).toString().split("|");

                       if(listTmp.size()<4)
                           break;
                       if(listTmp[3]=="")
                           break;


                       v_actionId=listTmp[3].split(",");
                       varList += "|" +v_actionId.join("|");
                       //emit addLog("视觉变量注册"+v_actionId.join("|"),"");
                   }
                }



                if(signalList.length()==1)
                {
                    signalList="??";
                }

                QByteArray data = signalList.toLocal8Bit();
                data[0] = CMD_REG_SIGNAL_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,1);
                qDebug()<<"data sig"<<data;

                if (varList.length()==1) {
                    varList = "??";
                }
                data = varList.toLocal8Bit();
                data[0] = CMD_REG_VAR_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,1);
                qDebug()<<"data var"<<data;

                pageStation->safeAreaSend(robotIndex);

            }
        } else if (command == CMD_ERROR_NUMBER) {
            int errorNumber;
            errorNumber = ((uchar)result[2]<<8) | (uchar)result[3];

            if (errorNumber == 0)
            {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警已清除", QString());
            }
            else
            {
                QSqlQuery query;
                query.exec(QString("SELECT code,title,desc from AlarmLibrary where code=%1").arg(errorNumber));
                if (query.next())
                {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  "+query.value(1).toString()+": "+query.value(2).toString()+"[" + query.value(0).toString()+"]", QString());
                } else {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警：" + QString::number(errorNumber), QString());
                }
            }


//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
//            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
//            if (varTriggerMap.contains(data[0])){
//                varTriggerCache[data[0]] = data[1];
////                QStringList idList = varTriggerMap[data[0]];
//                triggerValueChanged(varTriggerMap[data[0]]);
//            }
        } else if (command == CMD_GET_PROGRAM_RESULT) {

            if ((uchar)result[2] == 0x01) {
                //	programCode = QString( SysUtils::readFile(robotPrg) );
                programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod";
                if (! QFile::exists( programFile )) {
                    programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\Module1.mod";
                    if (! QFile::exists( programFile )) {
                        programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod"; // 不存在module1就还原为mainmodule
                        QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+currentStationId);
                        if(programPath.exists()) {
                            QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                            if (files.count()>0) {
                                programFile = files.at(0);
                            }
                        } else {
                            programPath.mkpath(programPath.path());
                        }
                    }
                }
                programCode = QString( SysUtils::readFile( programFile ) );
                if(codeModel==NULL)//add by zhugz
                     codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                addLog("CMD_GET_PROGRAM_RESULT","");
                m_ui->codeView->setModel(codeModel);
                m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                m_ui->textEdit->setPlainText(programCode);
            } else {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  当前指令获取失败！", QString());
            }
            qApp->restoreOverrideCursor();
        } else if (command == CMD_PROGRAM_GENERATE_RESULT) {

            m_ui->textEdit->insertPlainText("    " + QString::fromUtf8(result.mid(2), len - 2) + "\n");
        }else if(command==CMD_KILL_SERVICE){


            if( t1->isActive()==false)
            {
                heartBeat=0;
                heartBeat2=-1000;

                t1->start(10000);

            }
            ++heartBeat;

        }else if(command==CMD_visual_001){
            QString value2=QString::fromLocal8Bit(result.mid(3));
            double value3;
            if(result.at(2)==1)
                value3=value2.toInt()*armRatio1;
            else
                value3=value2.toInt()*armRatio2;

            QString value=QString::number(value3,'f',3);
            QString str1=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
            QString str;
            if(result.at(2)==1)
            {
                emit addLog("圆孤检测:"+value,"");
                str1+=QString("圆孤波动值:%1 \n").arg(value);
                str+=outputName1;
                str+=value;
            }
            else
            {
                emit addLog("侧面检测:"+value,"");
                str1+=QString("侧面波动值:%1 \n").arg(12);
                str+=outputName2;
                str+=value;
            }
            writeTxt("d:/value.txt",str1);

                if (serviceSocket[0]) {
                    QString retStr;
                    if(result.at(2)==1)
                        retStr= "??" + armVarName1 + "|" +value;
                    else
                        retStr= "??" + armVarName2 + "|" +value;

                    QByteArray dataArray;
                    dataArray = retStr.toLocal8Bit();
                    dataArray[0] = CMD_WRITE_VAR;
                    dataArray[1] = 1;
                    mysendService(dataArray.data(),dataArray.length(),1);

                }

        }else if(command==CMD_KNIFE_STATE_RESULT)
        {
//            QByteArray b;
//            b.append(CMD_KNIFE_STATE_RESULT);
//            b.append(result.mid(2));
//            servoSocket->writeDatagram(b,  QHostAddress::LocalHost, 8905);
//            emit addLog("发送伺服状态","");
        }
        else if(command==CMD_WORKPIECE_COUNT){
            //统计工件个数
            //QString data1=  QString::fromUtf8(result.mid(2), len - 2);
            int num=0;
            num=((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            m_ui->maodingLabel->setText(QString::number(num));
            if(num==1)
            {
                writeXmlValue("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk","date",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }

        }else if(command==CMD_COMMON_DATA){
            //首页文字接收

            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
            if(data.size()<2)
                return;
            if(!textVar.contains(data[0]))
                return;
            int n=textVar[data[0]];
            QString str=data[1].left(8)+varUnit[data[0]];

            labelSetValue[n]->setText(str);

        }else if(command == CMD_GT2_RESULT){
            QString data1=  QString::fromUtf8(result);
            QStringList list1=data1.split("|");
            if(list1.size()<2)
                return;



            neddleValue[0]=list1[1].toDouble();
            m_ui->checkDataLabel->setText(QString::number(neddleValue[0],'f',3));


//            QString data1=  QString::fromUtf8(result.mid(2), len - 2);
//            QStringList error;
//            error<<"自动刀补开始写入"<<"机床开关查询失败"<<"机床开关无记录"<<"探针值超出范围"<<"差值超出0.002"<<"刀具磨损查询失败"<<"刀具磨损无记录"<<"磨损值超出范围"<<"磨损值超出0.3";
//            QStringList list=data1.split(",");
//            QStringList varList;
//            if(list.size()<6)
//            {
//                emit addLog("探针数据不完整1","");
//                return;
//            }

//            for(int i=0;i<6;i++)
//            {
//                varList=list[i].split("|");
//                if(varList.size()<2)
//                {
//                    emit addLog("探针数据不完整2","");
//                    return;
//                }
//                neddleValue[i]=varList[1].toDouble();

//                if(i==0)
//                      m_ui->labelMeasure1->setText(QString::number(neddleValue[i],'f',4));
//                else if(i==1)
//                      m_ui->labelMeasure2->setText(QString::number(neddleValue[i],'f',4));
//                else if(i==2)
//                      m_ui->labelMeasure3->setText(QString::number(neddleValue[i],'f',4));
//                else if(i==3)
//                      m_ui->labelMeasure4->setText(QString::number(neddleValue[i],'f',4));
//                else if(i==4)
//                      m_ui->labelMeasure5->setText(QString::number(neddleValue[i],'f',4));
//                else if(i==5)
//                      m_ui->labelMeasure6->setText(QString::number(neddleValue[i],'f',4));



//            }


  return;
//            if(1)//s_k
//            {
//                QString data="";

//                int ret= m_ui->page_4->autoKnife(list[0],dataValue,data);
//                emit addLog(error[ret]+data,"");
//                if(ret!=0)
//                    return;

//                QByteArray b1=data.toLocal8Bit();
//                b1[0]=NC_CMD_WRITE_VAR;

//               // sendKnife(b1);
//               // addLog("刀补数据发送成功","");
//            }

          }else if(command==CMD_DETECT_STATE){
            //探针气密


            static int gtIndex1=0,gtIndex2=0,gtIndex3=0;
            QString data1=  QString::fromUtf8(result.mid(2), len - 2);
            QStringList list=data1.split("|");

            if(list[0]!="Type")
                return;

            QString knifeType="";
            if(list.size()>2)
                knifeType=list[2];//精外南北

            bool log=true;
            //探针
            QFile f("d:/dzicsrobot/needle.txt");
            if(!f.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
            {
                qDebug() << "Open txt failed.";
                log=false;
            }
            QTextStream txtOutput(&f);



            double standard0,standard1,standard2,standard3,max0,max1,max2,max3;

            double needleStandard[3],needleMax[3];
            for(int i=0;i<Knife2List.size();i++)
            {
                if(Knife2List[i].name2=="A1")//探针均值
                {
                    needleStandard[0]=  Knife2List[i].v1;
                    needleMax[0]= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="A2")//直径
                {
                    needleStandard[1]=  Knife2List[i].v1;
                    needleMax[1]= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="A3")//总合
                {
                    needleStandard[2]=  Knife2List[i].v1;
                    needleMax[2]= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="B0")//气密均值设定
                {
                    standard0=  Knife2List[i].v1;
                    max0= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="B1")//气密1
                {
                    standard1=  Knife2List[i].v1;
                    max1= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="B2")//气密2
                {
                    standard2=  Knife2List[i].v1;
                    max2= Knife2List[i].v3;
                }
                else if(Knife2List[i].name2=="B3")//气密3
                {
                    standard3=  Knife2List[i].v1;
                    max3= Knife2List[i].v3;
                }
            }

            if(list[1][0]=='1')//第一台机器人
            {

//                emit addLog("开始气密1检测","");
                  m_ui->directLabel->setText(QString::number(neddleValue[3],'f',4));
//                //一台气密
//                if(gtIndex2>4)
//                    gtIndex2=0;
//                savedNeddleValue2[gtIndex2]=neddleValue[3];

//                ++gtIndex2;

//                //记录检测值

//                QString s1=QString("时间：%1  气密1值：%2  差值:%3 \n")
//                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
//                        .arg(neddleValue[3],0,'f',3)
//                        .arg(neddleValue[3]-standard1,0,'f',3);
//                writeTxt("d:/dzicsrobot/air.txt",s1);

//                //end

//                bool b3=true;
//                for(int i=0;i<5;i++)
//                {
//                    if(savedNeddleValue2[i]<0.002||savedNeddleValue2[i]>abs(standard1-max1))
//                    {
//                        b3=false;
//                    }

//                }

//                if(b3)
//                {

//                }
//                preNeddleValue[3]=neddleValue[3];
            }
            else if(list[1][0]=='2')//第三台机器人
            {

                emit addLog("开始探针检测","");
                m_ui->weightLabel->setText(QString::number((neddleValue[0]+neddleValue[1]+neddleValue[2])/3,'f',4));
                m_ui->yuanxinjuLabel->setText(QString::number((neddleValue[4]+neddleValue[5])/2,'f',4));
                //emit addLog(QString("%1 ,%1 ,%1").arg(neddleValue[0]).arg(neddleValue[1]).arg(neddleValue[2]),"");
                //3个探针


                if(gtIndex1>4)
                    gtIndex1=0;

                if(gtIndex2>4)
                    gtIndex2=0;

                if(gtIndex3>4)
                    gtIndex3=0;
                //保存探针,判断连续5次不合格值进行刀补[均值  直径  总合]
                if(knifeType=="A")
                    savedNeddleValueA[gtIndex1]=abs((needleStandard[2]-(neddleValue[0]+neddleValue[1]+neddleValue[2])/3)*2-needleStandard[1]);
                else if(knifeType=="B")
                    savedNeddleValueB[gtIndex2]=abs((needleStandard[2]-(neddleValue[0]+neddleValue[1]+neddleValue[2])/3)*2-needleStandard[1]);

                //两个气密
                savedNeddleValue3_avg[gtIndex3]=abs(standard0-(neddleValue[4]+neddleValue[5])/2);

                ++gtIndex3;




                preNeddleValue[0]=neddleValue[0];
                preNeddleValue[1]=neddleValue[1];
                preNeddleValue[2]=neddleValue[2];
                preNeddleValue[4]=neddleValue[4];
                preNeddleValue[5]=neddleValue[5];
                bool b1=true,b2=true;

                for(int i=0;i<5;i++)
                {

                    if(savedNeddleValueA[i]<0.002||savedNeddleValueA[i]>needleMax[0])
                    {
                        //b1=false;
                    }

                    if(savedNeddleValueB[i]<0.002||savedNeddleValueB[i]>needleMax[0])
                    {
                        //b2=false;
                    }


                }
                double avg1=(neddleValue[0]+neddleValue[1]+neddleValue[2])/3;
                if(b1&&knifeType=="A")
                {//61.315   5.256  6.053   8.352
                    //进行刀补

                    QString s1=QString("时间：%1 探针1:%2   探针2:%3   探针3:%4   探针均值:%5  半径:%6   直径:%7   探针均值差值:%8   直径差值:%9   刀补值：%10 \n")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(neddleValue[0],0,'f',4)
                            .arg(neddleValue[1],0,'f',4)
                            .arg(neddleValue[2],0,'f',4)
                            .arg(avg1,0,'f',4)
                            .arg(needleStandard[2]-avg1,0,'f',4)
                            .arg((needleStandard[2]-avg1)*2,0,'f',4)
                            .arg(avg1-needleStandard[0],0,'f',4)
                            .arg(needleStandard[2]-avg1-needleStandard[1],0,'f',4)
                            .arg(savedNeddleValueA[gtIndex1],0,'f',4)
                            ;



                    writeTxt("d:/dzicsrobot/knifeA.txt",s1);


                }
                if(b2&&knifeType=="B")
                {
                    //进行刀补
                    QString s1=QString("时间：%1 探针1:%2   探针2:%3   探针3:%4   探针均值:%5  半径:%6   直径:%7   探针均值差值:%8   直径差值:%9   刀补值：%10 \n")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(neddleValue[0],0,'f',4)
                            .arg(neddleValue[1],0,'f',4)
                            .arg(neddleValue[2],0,'f',4)
                            .arg(avg1,0,'f',4)
                            .arg(needleStandard[2]-avg1,0,'f',4)
                            .arg((needleStandard[2]-avg1)*2,0,'f',4)
                            .arg(avg1-needleStandard[0],0,'f',4)
                            .arg(needleStandard[2]-avg1-needleStandard[1],0,'f',4)
                            .arg(savedNeddleValueB[gtIndex2],0,'f',4)
                            ;
                    writeTxt("d:/dzicsrobot/knifeB.txt",s1);


                }


            }

            f.close();

            if(knifeType=="A")
                ++gtIndex1;
            else if(knifeType=="B")
                ++gtIndex2;
        }

        //try end
        } catch(...) {
            // Ignore
            emit addLog("通信数据错误","");
      }
    }
}


void MainWindow::processServiceMessage2()   // 第2机器人用
{
    while (service2Socket->hasPendingDatagrams()) {
      try {
        QByteArray result;
        result.resize(service2Socket->pendingDatagramSize());
        service2Socket->readDatagram(result.data(), result.size());

        if (service2Connected == false) {
            qDebug(result.data());  // debug
            continue;
        }

        uchar len = result[0];
        uchar command = result[1];
        if (command == CMD_POSITION) {
            //uchar tt;
            //for (int i=2;i<len;i++){
            //	tt = result[i];
            //	tt = 0;
            //}
            //tt = result[2];
            //result[2] = result[5];
            //result[5] = result[2];
            //tt = result[3];
            //result[3] = result[4];
            //result[4] = result[3];
            float ax1, ax2, ax3, ax4, ax5, ax6;
            ax1 = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000000.0;
            ax2 = (((uchar)result[9] << 24) | ((uchar)result[8] << 16) | ((uchar)result[7] << 8) | ((uchar)result[6] << 0)) / 1000000.0;
            ax3 = (((uchar)result[13] << 24) | ((uchar)result[12] << 16) | ((uchar)result[11] << 8) | ((uchar)result[10] << 0)) / 1000000.0;
            ax4 = (((uchar)result[17] << 24) | ((uchar)result[16] << 16) | ((uchar)result[15] << 8) | ((uchar)result[14] << 0)) / 1000000.0;
            ax5 = (((uchar)result[21] << 24) | ((uchar)result[20] << 16) | ((uchar)result[19] << 8) | ((uchar)result[18] << 0)) / 1000000.0;
            ax6 = (((uchar)result[25] << 24) | ((uchar)result[24] << 16) | ((uchar)result[23] << 8) | ((uchar)result[22] << 0)) / 1000000.0;
            QStringList resultPos;
            resultPos.append( QString("%1").arg( ax1 ) );
            resultPos.append( QString("%1").arg( ax2 ) );
            resultPos.append( QString("%1").arg( ax3 ) );
            resultPos.append( QString("%1").arg( ax4 ) );
            resultPos.append( QString("%1").arg( ax5 ) );
            resultPos.append( QString("%1").arg( ax6 ) );
            //QStringList resultPos = QString::fromLocal8Bit(result.data() + 2, len - 2).split(',');
//            qDebug(resultPos.join("|").toLocal8Bit());
            emit syncPosition( resultPos, 2 );

            // debug
//            emit addLog(resultPos.join("|"), QString());
//            emit addLog(QString("%1").arg( ax3 ), QString());

        } else if (command == CMD_UNIT_DATA) {	// unit IO
            //QByteArray *cp = new QByteArray(result.data(), len);

            emit updateUnitData( result.mid(1) );
        } else if (command == CMD_PROGRAM_ROW) {	// ProgramRow
            //emit updateProgramRow( result );
            int row = 0;
            //row = (int)(*(result + 1));
            row = ((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            //row = result[4];
            QString programName = "";
            if (len > 6) {
                programName = QString::fromUtf8(result.mid(6), len - 6);
            }
            setCodeRow(row, programName);
//        } else if (command == CMD_DISPLAY_STATE) {	// DisplayState
//            int displayState, pickCount;
//            displayState = ((uchar)result[2]<<8) | (uchar)result[3];
//            pickCount = ((uchar)result[4]<<8) | (uchar)result[5];
//            emit syncDisplayState( displayState, pickCount );
        } else if (command == CMD_PANEL) {
            int state;
            state = ((uchar)result[2]<<8) | (uchar)result[3];
            openPanel( state );
        } else if (command == CMD_LOG) {
//            qDebug(QString::fromUtf8(result.mid(2), len - 2).toLocal8Bit());
            if (len>10) {
                emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
            }
        } else if (command == CMD_OPERATE_MODE) {
            QPalette p;
            if (result.at(2) == 0){
                m_ui->tMode2->setText("自动");
                p.setColor(QPalette::WindowText, Qt::black);
            }else if(result.at(2) == 1){
                m_ui->tMode2->setText("手动");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 2){
                m_ui->tMode2->setText("手动");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 3){
                m_ui->tMode2->setText("手动全速");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 4){
                m_ui->tMode2->setText("手动");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 5){
                m_ui->tMode2->setText("手动");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 6){
                m_ui->tMode2->setText("示教");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 12){
                m_ui->tMode2->setText("力控");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else if(result.at(2) == 13){
                m_ui->tMode2->setText("扭矩");
                p.setColor(QPalette::WindowText, Qt::darkMagenta);
            }else {
                m_ui->tMode2->setText("未知");
                p.setColor(QPalette::WindowText, Qt::darkYellow);
            }
            m_ui->tMode2->setPalette(p);
        } else if (command == CMD_EXECUTE_STATE) {
            QPalette p;
            if (result.at(2) == 1) {
                m_ui->tMode3->setText("运行");
                p.setColor(QPalette::WindowText, Qt::blue);
                m_ui->numPad->showKey(KeyPauseProgram);
            } else if(result.at(2) == 2) {
                m_ui->tMode3->setText("停止");
                p.setColor(QPalette::WindowText, Qt::red);
                m_ui->numPad->showKey(KeyStartProgram);
            } else if(result.at(2) == 10) {
                m_ui->tMode3->setText("暂停");
                p.setColor(QPalette::WindowText, Qt::magenta);
                m_ui->numPad->showKey(KeyResumeProgram);
            } else if(result.at(2) == 20) {
                m_ui->tMode3->setText("告警");
                p.setColor(QPalette::WindowText, Qt::red);
                m_ui->numPad->showKey(KeyStartProgram);
            } else if(result.at(2) == 21) {
                m_ui->tMode3->setText("急停");
                p.setColor(QPalette::WindowText, Qt::red);
                m_ui->numPad->showKey(KeyStartProgram);
            } else if(result.at(2) == 30) {
                m_ui->tMode3->setText("空闲");
                p.setColor(QPalette::WindowText, Qt::red);
                m_ui->numPad->showKey(KeyStartProgram);
            } else if(result.at(2) == 40) {
                m_ui->tMode3->setText("断电");
                p.setColor(QPalette::WindowText, Qt::red);
                m_ui->numPad->showKey(KeyStartProgram);
            } else {
                m_ui->tMode3->setText("未知" + QString::number(result.at(2)));
                p.setColor(QPalette::WindowText, Qt::black);
            }
            m_ui->tMode3->setPalette(p);
        } else if (command == CMD_CONNECT_STATE) {
            QPalette p;
            if (result.at(2) == 1) {
                killTimer(reconnectTimerId);
                m_ui->tMode1->setText("联机");
                m_ui->tMode1->setEnabled(true);
                m_ui->tMode2->setEnabled(true);
                m_ui->tMode3->setEnabled(true);
                p.setColor(QPalette::WindowText, Qt::black);
                m_ui->numPad->enableKey(KeyOpenVM, false);
            }
            else if(result.at(2) == 11) {
                killTimer(reconnectTimerId);
                m_ui->tMode1->setText("虚拟机");
                m_ui->tMode1->setEnabled(true);
                m_ui->tMode2->setEnabled(true);
                m_ui->tMode3->setEnabled(true);
                p.setColor(QPalette::WindowText, Qt::darkGreen);
                m_ui->numPad->showKey(KeyCloseVM);
            }
            else {
//                if (m_ui->tMode1->text() == "联机") { // 真机状态下断开连接后每隔10秒尝试重连
//                    reconnectTimerId = startTimer( 10000 );
//                }
                m_ui->tMode1->setText("脱机");
                m_ui->tMode1->setEnabled(false);
                m_ui->tMode2->setEnabled(false);
                m_ui->tMode3->setEnabled(false);
                p.setColor(QPalette::WindowText, Qt::gray);
                m_ui->tMode1->setPalette(p);
                m_ui->numPad->showKey(KeyOpenVM);
                m_ui->numPad->enableKey(KeyOpenVM, true);
            }
            m_ui->tMode1->setPalette(p);
        } else if (command == CMD_SPEED_RATIO) {
            if (result.at(2) >= 0 && result.at(2) <= 100) {

                emit updateSpeedRatio(result.at(2));
            }
        } else if (command == CMD_PROCESS_TIME) {
            float sec;
            sec = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000.0;
            emit updateProcessTime( sec );
        } else if (command == CMD_SIGNAL_CHANGED) {
//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
//            emit addLog(data.join("|"),QString());
            if (signalTriggerMap2.contains(data[0])){
                signalTriggerCache[data[0]] = data[1];
                triggerValueChanged(signalTriggerMap2[data[0]]);
            }
            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value = data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);
                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_VAR_CHANGED) {
//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
            if (varTriggerMap2.contains(data[0])){
                varTriggerCache[data[0]] = data[1];
                triggerValueChanged(varTriggerMap2[data[0]]);
            }
            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value = data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);
                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_LOGON_RESULT) {

            if (!currentStationId.isEmpty()) {
                QString signalList="", varList="";
                signalTriggerMap2.clear();
                varTriggerMap2.clear();
                QSqlQuery query;
                query.exec(QString("SELECT id,type,code,extraTrigger,code2,extraTrigger2,code3 FROM StationAction WHERE stationId='%1' AND isUse=1").arg(currentStationId));
                while(query.next()) {
                    QSqlRecord record = query.record();
                    switch (record.value("type").toInt()) {
                    case 1: // 信号
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            signalTriggerMap2[code].append(record.value("id").toString());
//                            signalList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            signalTriggerMap2[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            signalTriggerMap2[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
                        }
                        break;
                    }
                    case 2: // 变量
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            varTriggerMap2[code].append(record.value("id").toString());
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            varTriggerMap2[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            varTriggerMap2[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        break;
                    }
                    case 3: // 信号/变量
                    case 21: // 刀具寿命(信号/变量)
                    case 100: // 复位动作(信号/变量)
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            signalTriggerMap2[code].append(record.value("id").toString());
                            varTriggerMap2[code].append(record.value("id").toString());
//                            signalList += "|" + code;
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            signalTriggerMap2[code2].append(record.value("id").toString());
                            varTriggerMap2[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            signalTriggerMap2[code3].append(record.value("id").toString());
                            varTriggerMap2[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
//                            varList += "|" + code3;
                        }
                        break;
                    }
                    }
                }

                foreach (QString code, signalTriggerMap2.keys()) {
                    signalList += "|" + code;
                }
                foreach (QString code, varTriggerMap2.keys()) {
                    varList += "|" + code;
                }


                // 辅助轴变量注册
                query.exec(QString("SELECT DISTINCT sign FROM StationAxis WHERE stationId='%1' AND isUse=1 AND sign<>''").arg(currentStationId));
                while(query.next()) {
                    signalList += "|" + query.value(0).toString();
                    varList += "|" + query.value(0).toString();
                }

                if (signalList.isEmpty()) {
                    signalList = "|";
                }
                QByteArray data = signalList.toLocal8Bit();
                data[0] = CMD_REG_SIGNAL_TRIGGER;
                sendService2(data);

                if (varList.isEmpty()) {
                    varList = "|";
                }
                data = varList.toLocal8Bit();
                data[0] = CMD_REG_VAR_TRIGGER;
                sendService2(data);
//                    qDebug("\r\n");
//                    qDebug(data2);
//                    qDebug(QString::number(strlen(data2)).toLocal8Bit().data());

            }
        } else if (command == CMD_ERROR_NUMBER) {
            int errorNumber;
            errorNumber = ((uchar)result[2]<<8) | (uchar)result[3];

            if (errorNumber == 0)
            {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警已清除", QString());
            }
            else
            {
                QSqlQuery query;
                query.exec(QString("SELECT code,title from AlarmLibrary where code=%1").arg(errorNumber));
                if (query.next())
                {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  "+query.value(1).toString()+"[" + query.value(0).toString()+"]", QString());
                } else {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警：" + QString::number(errorNumber), QString());
                }
            }


//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
//            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
//            if (varTriggerMap.contains(data[0])){
//                varTriggerCache[data[0]] = data[1];
////                QStringList idList = varTriggerMap[data[0]];
//                triggerValueChanged(varTriggerMap[data[0]]);
//            }
        } else if (command == CMD_GET_PROGRAM_RESULT) {
            if ((uchar)result[2] == 0x01) {
                //	programCode = QString( SysUtils::readFile(robotPrg) );
                programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod";
                if (! QFile::exists( programFile )) {
                    programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\Module1.mod";
                    if (! QFile::exists( programFile )) {
                        programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod"; // 不存在module1就还原为mainmodule
                        QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+currentStationId);
                        if(programPath.exists()) {
                            QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                            if (files.count()>0) {
                                programFile = files.at(0);
                            }
                        } else {
                            programPath.mkpath(programPath.path());
                        }
                    }
                }
                programCode = QString( SysUtils::readFile( programFile ) );
                codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                m_ui->codeView->setModel(codeModel);
                m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                m_ui->textEdit->setPlainText(programCode);
            } else {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  当前指令获取失败！", QString());
            }
            qApp->restoreOverrideCursor();
        } else if (command == CMD_PROGRAM_GENERATE_RESULT) {
            m_ui->textEdit->insertPlainText("    " + QString::fromUtf8(result.mid(2), len - 2) + "\n");
        }
      } catch(...) {
            // Ignore
      }
    }
}


void MainWindow::processServiceMessage_2()
{


    while (serviceSocket[1]->hasPendingDatagrams()) {
      try {

        QByteArray result;
        result.resize(serviceSocket[1]->pendingDatagramSize());
        serviceSocket[1]->readDatagram(result.data(), result.size());
       // qDebug()<<"conn data[]"<<QString::number( result[1],16);  // debug
        //test

//        log2<<result;
//        if(log2.size()==500)
//        {
//            int logn2=m_ui->checkDataLabel->text().toInt();
//            m_ui->checkDataLabel->setText(QString::number(logn2+500));
//            QFile f("d:/log2.txt");
//            if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
//            {
//                qDebug() << "Open failed.";
//                return;
//            }

//            QTextStream txtOutput(&f);

//            txtOutput << log2.join("\n");

//            f.close();

//        }
        //end
        if (serviceConnected[1] == false) {


            continue;
        }

        uchar len = result[0]-1;//移除一位,长度减1
        uchar command = result[1];
        uchar robotIndex=result[2];
        result.remove(2,1);//得到机器index,保持原数据不变

        //log

//        if(fileState[1]&&g_camera->recordLog)
//        {

//            QTextStream txtOutput(file[1]);
//            txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" cmd:"<<QString::number((uchar)result[1],16)<<" data:"<<result.mid(2)<<"\r\n";
//            file[1]->flush();


//        }
        //end

        if (command == CMD_POSITION) {

            //uchar tt;
            //for (int i=2;i<len;i++){
            //	tt = result[i];
            //	tt = 0;
            //}
            //tt = result[2];
            //result[2] = result[5];
            //result[5] = result[2];
            //tt = result[3];
            //result[3] = result[4];
            //result[4] = result[3];
            float ax1, ax2, ax3, ax4, ax5, ax6;
            ax1 = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000000.0;
            ax2 = (((uchar)result[9] << 24) | ((uchar)result[8] << 16) | ((uchar)result[7] << 8) | ((uchar)result[6] << 0)) / 1000000.0;
            ax3 = (((uchar)result[13] << 24) | ((uchar)result[12] << 16) | ((uchar)result[11] << 8) | ((uchar)result[10] << 0)) / 1000000.0;
            ax4 = (((uchar)result[17] << 24) | ((uchar)result[16] << 16) | ((uchar)result[15] << 8) | ((uchar)result[14] << 0)) / 1000000.0;
            ax5 = (((uchar)result[21] << 24) | ((uchar)result[20] << 16) | ((uchar)result[19] << 8) | ((uchar)result[18] << 0)) / 1000000.0;
            ax6 = (((uchar)result[25] << 24) | ((uchar)result[24] << 16) | ((uchar)result[23] << 8) | ((uchar)result[22] << 0)) / 1000000.0;
            QStringList resultPos;
            resultPos.append( QString("%1").arg( ax1 ) );
            resultPos.append( QString("%1").arg( ax2 ) );
            resultPos.append( QString("%1").arg( ax3 ) );
            resultPos.append( QString("%1").arg( ax4 ) );
            resultPos.append( QString("%1").arg( ax5 ) );
            resultPos.append( QString("%1").arg( ax6 ) );
            //QStringList resultPos = QString::fromLocal8Bit(result.data() + 2, len - 2).split(',');
//            qDebug(resultPos.join("|").toLocal8Bit());
            emit syncPosition( resultPos, robotIndex );

            // debug
//            emit addLog(resultPos.join("|"), QString());
//            emit addLog(QString("%1").arg( ax3 ), QString());

        } else if (command == CMD_UNIT_DATA) {	// unit IO
            //QByteArray *cp = new QByteArray(result.data(), len);

            emit updateUnitData( result.mid(1) );
        } else if (command == CMD_PROGRAM_ROW) {	// ProgramRow
            //emit updateProgramRow( result );
            if(curRobotIndex!=robotIndex)
                return;
            int row = 0;
            //row = (int)(*(result + 1));
            row = ((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            //row = result[4];
            QString programName = "";
            if (len > 6) {
                programName = QString::fromUtf8(result.mid(6), len - 6);
            }
            setCodeRow(row, programName);
//        } else if (command == CMD_DISPLAY_STATE) {	// DisplayState
//            int displayState, pickCount;
//            displayState = ((uchar)result[2]<<8) | (uchar)result[3];
//            pickCount = ((uchar)result[4]<<8) | (uchar)result[5];
//            emit syncDisplayState( displayState, pickCount );
        } else if (command == CMD_PANEL) {
            int state;
            state = ((uchar)result[2]<<8) | (uchar)result[3];
            openPanel( state );
        } else if (command == CMD_LOG) {
//            qDebug(QString::fromUtf8(result.mid(2), len - 2).toLocal8Bit());
            if (len>10) {
                emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
            }
        } else if (command == CMD_OPERATE_MODE) {


            if (result.at(2) == 0){
                operatorStates[robotIndex]="自动";

            }else if(result.at(2) == 1){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 2){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 3){
                operatorStates[robotIndex]="手动全速";

            }else if(result.at(2) == 4){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 5){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 6){
                operatorStates[robotIndex]="示教";

            }else if(result.at(2) == 12){
                operatorStates[robotIndex]="力控";

            }else if(result.at(2) == 13){
                operatorStates[robotIndex]="扭矩";

            }else {
                operatorStates[robotIndex]="未知";

            }


                setOperatorState(robotIndex);


        } else if (command == CMD_EXECUTE_STATE) {

            if (result.at(2) == 1) {
                execStates[robotIndex]="运行";
            } else if(result.at(2) == 2) {

                execStates[robotIndex]="停止";

            } else if(result.at(2) == 10) {
                execStates[robotIndex]="暂停";

            } else if(result.at(2) == 20) {
                execStates[robotIndex]="告警";


            } else if(result.at(2) == 21) {
                execStates[robotIndex]="急停";


            } else if(result.at(2) == 30) {
                execStates[robotIndex]="空闲";


            } else if(result.at(2) == 40) {
                execStates[robotIndex]="断电";


            } else {
                execStates[robotIndex]="未知";
                addLog("operatorState:未知 "+ QString::number(result.at(2)),"");

            }

            setExecState(robotIndex);


        } else if (command == CMD_CONNECT_STATE) {

            if (result.at(2) == 1) {
                connStates[robotIndex]="联机";
            }
            else if(result.at(2) == 11) {
                connStates[robotIndex]="虚拟机";

            }
            else {
                connStates[robotIndex]="脱机";

            }

                setConnState(robotIndex);


        } else if (command == CMD_SPEED_RATIO) {
            if (result.at(2) >= 0 && result.at(2) <= 100) {
                speedCache[robotIndex-1]=result.at(2);
                if(curRobotIndex==robotIndex)
                emit updateSpeedRatio(result.at(2));
            }

        }
        else if(command==CMD_POSITION_TRANSFORM)
         {


             emit positionTransform();
         }
        else if (command == CMD_PROCESS_TIME) {
            float sec;
            sec = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000.0;
            emit updateProcessTime( sec );
        } else if (command == CMD_SIGNAL_CHANGED) {
            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');

            if (signalTriggerMaps[robotIndex-1].contains(data[0])){
                signalTriggerCache[data[0]] = data[1];
                triggerValueChanged((signalTriggerMaps[robotIndex-1])[data[0]]);
            }


            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value = data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);
                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_VAR_CHANGED) {

            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
            if (varTriggerMaps[robotIndex-1].contains(data[0])){

                varTriggerCache[data[0]] = data[1];
                triggerValueChanged((varTriggerMaps[robotIndex-1])[data[0]]);

            }

            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value =data[0].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;

                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);


                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_LOGON_RESULT) {

            if (!currentStationId.isEmpty()) {
                QString signalList="?", varList="?";
                signalTriggerMaps[robotIndex-1].clear();
                varTriggerMaps[robotIndex-1].clear();
                QSqlQuery query,query2;
                query.exec(QString("SELECT id,type,code,extraTrigger,code2,extraTrigger2,code3 FROM StationAction WHERE stationId='%1' AND isUse=1").arg(currentStationId));
                while(query.next()) {
                    QSqlRecord record = query.record();
                    switch (record.value("type").toInt()) {
                    case 1: // 信号
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
                        }
                        break;
                    }
                    case 2: // 变量
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        //update zhugz

                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        //end
                        break;
                    }
                    case 3: // 信号/变量
                    case 21: // 刀具寿命(信号/变量)
                    case 31: // 测量保存动作(信号/变量)
                    case 32: // 视觉分析动作(信号/变量)
                    {


                    }
                    case 33: // 称重动作(信号/变量)
                    case 100: // 复位动作(信号/变量)
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
//                            varList += "|" + code3;
                        }
                        break;
                    }
                    }
                }

                foreach (QString code, signalTriggerMaps[robotIndex-1].keys()) {
                   if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    signalList += "|" + code;
                }

                foreach (QString code, varTriggerMaps[robotIndex-1].keys()) {
                    if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    varList += "|" + code;
                }


                // 辅助轴变量注册
                query.exec(QString("select id from stationMotion where stationid='%1' and displayOrder=%2 order by displayOrder").arg(currentStationId).arg(robotIndex));
                QString r_Axis;
                if(query.next())
                    r_Axis=query.value(0).toString();

                query.exec(QString("SELECT DISTINCT sign FROM StationAxis WHERE motionId='%1' AND isUse=1 AND sign<>''").arg(r_Axis));

                while(query.next()) {
                    signalList += "|" + query.value(0).toString();
                    varList += "|" + query.value(0).toString();

                }

                //视觉输出变量注册
//                query.exec(QString("select id from StationAction where stationId='%1'").arg(currentStationId));
//                QStringList v_actionId,listTmp;
//                QString temp1;
//                while(query.next())
//                {
//                   query2.exec(QString("select value from StationActionDetail where ationId='%1'").arg(query.value(0).toString()));
//                   while(query2.next())
//                   {
//                       listTmp=query2.value(0).toString().split("|");
//                       if(listTmp[3]=="")
//                           break;
//                       v_actionId=listTmp[3].split(",");
//                       varList += "|" +v_actionId.join("|");
//                       emit addLog("视觉变量注册"+v_actionId.join("|"),"");
//                   }
//                }

                if(signalList.length()==1)
                {
                    signalList="??";
                }

                QByteArray data = signalList.toLocal8Bit();
                data[0] = CMD_REG_SIGNAL_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,2);

                if (varList.length()==1) {
                    varList = "??";
                }
                data = varList.toLocal8Bit();
                data[0] = CMD_REG_VAR_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,2);




//                    qDebug("\r\n");
//                    qDebug(data2);
//                    qDebug(QString::number(strlen(data2)).toLocal8Bit().data());

            }
        } else if (command == CMD_ERROR_NUMBER) {
            int errorNumber;
            errorNumber = ((uchar)result[2]<<8) | (uchar)result[3];

            if (errorNumber == 0)
            {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警已清除", QString());
            }
            else
            {
                QSqlQuery query;
                query.exec(QString("SELECT code,title from AlarmLibrary where code=%1").arg(errorNumber));
                if (query.next())
                {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  "+query.value(1).toString()+": "+query.value(2).toString()+"[" + query.value(0).toString()+"]", QString());

                } else {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警：" + QString::number(errorNumber), QString());
                }
            }


//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
//            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
//            if (varTriggerMap.contains(data[0])){
//                varTriggerCache[data[0]] = data[1];
////                QStringList idList = varTriggerMap[data[0]];
//                triggerValueChanged(varTriggerMap[data[0]]);
//            }
        } else if (command == CMD_GET_PROGRAM_RESULT) {
            if ((uchar)result[2] == 0x01) {
                //	programCode = QString( SysUtils::readFile(robotPrg) );
                programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod";
                if (! QFile::exists( programFile )) {
                    programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\Module1.mod";
                    if (! QFile::exists( programFile )) {
                        programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod"; // 不存在module1就还原为mainmodule
                        QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+currentStationId);
                        if(programPath.exists()) {
                            QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                            if (files.count()>0) {
                                programFile = files.at(0);
                            }
                        } else {
                            programPath.mkpath(programPath.path());
                        }
                    }
                }
                programCode = QString( SysUtils::readFile( programFile ) );
                 if(codeModel==NULL)//add by zhugz
                codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                m_ui->codeView->setModel(codeModel);
                m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                m_ui->textEdit->setPlainText(programCode);
            } else {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  当前指令获取失败！", QString());
            }
            qApp->restoreOverrideCursor();
        } else if (command == CMD_PROGRAM_GENERATE_RESULT) {
            m_ui->textEdit->insertPlainText("    " + QString::fromUtf8(result.mid(2), len - 2) + "\n");
        }
      } catch(...) {
            // Ignore
      }
    }
}



void MainWindow::processServiceMessage_3()
{

    while (serviceSocket[2]->hasPendingDatagrams()) {
      try {

        QByteArray result;
        result.resize(serviceSocket[2]->pendingDatagramSize());
        serviceSocket[2]->readDatagram(result.data(), result.size());

        if (serviceConnected[2] == false) {


            continue;
        }

        uchar len = result[0]-1;//移除一位,长度减1
        uchar command = result[1];
        uchar robotIndex=result[2];
        result.remove(2,1);//得到机器index,保持原数据不变

        //log

//        if(fileState[2]&&g_camera->recordLog)
//        {

//            QTextStream txtOutput(file[2]);
//            txtOutput <<"time:"<<QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")<<" cmd:"<<QString::number((uchar)result[1],16)<<" data:"<<result.mid(2)<<"\r\n";
//            file[2]->flush();


//        }
        //end

        if (command == CMD_POSITION) {

            //uchar tt;
            //for (int i=2;i<len;i++){
            //	tt = result[i];
            //	tt = 0;
            //}
            //tt = result[2];
            //result[2] = result[5];
            //result[5] = result[2];
            //tt = result[3];
            //result[3] = result[4];
            //result[4] = result[3];
            float ax1, ax2, ax3, ax4, ax5, ax6;
            ax1 = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000000.0;
            ax2 = (((uchar)result[9] << 24) | ((uchar)result[8] << 16) | ((uchar)result[7] << 8) | ((uchar)result[6] << 0)) / 1000000.0;
            ax3 = (((uchar)result[13] << 24) | ((uchar)result[12] << 16) | ((uchar)result[11] << 8) | ((uchar)result[10] << 0)) / 1000000.0;
            ax4 = (((uchar)result[17] << 24) | ((uchar)result[16] << 16) | ((uchar)result[15] << 8) | ((uchar)result[14] << 0)) / 1000000.0;
            ax5 = (((uchar)result[21] << 24) | ((uchar)result[20] << 16) | ((uchar)result[19] << 8) | ((uchar)result[18] << 0)) / 1000000.0;
            ax6 = (((uchar)result[25] << 24) | ((uchar)result[24] << 16) | ((uchar)result[23] << 8) | ((uchar)result[22] << 0)) / 1000000.0;
            QStringList resultPos;
            resultPos.append( QString("%1").arg( ax1 ) );
            resultPos.append( QString("%1").arg( ax2 ) );
            resultPos.append( QString("%1").arg( ax3 ) );
            resultPos.append( QString("%1").arg( ax4 ) );
            resultPos.append( QString("%1").arg( ax5 ) );
            resultPos.append( QString("%1").arg( ax6 ) );
            //QStringList resultPos = QString::fromLocal8Bit(result.data() + 2, len - 2).split(',');
//            qDebug(resultPos.join("|").toLocal8Bit());
            emit syncPosition( resultPos, robotIndex );

            // debug
//            emit addLog(resultPos.join("|"), QString());
//            emit addLog(QString("%1").arg( ax3 ), QString());

        } else if (command == CMD_UNIT_DATA) {	// unit IO
            //QByteArray *cp = new QByteArray(result.data(), len);

            emit updateUnitData( result.mid(1) );
        } else if (command == CMD_PROGRAM_ROW) {	// ProgramRow
            //emit updateProgramRow( result );
            if(curRobotIndex!=robotIndex)
                return;
            int row = 0;
            //row = (int)(*(result + 1));
            row = ((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            //row = result[4];
            QString programName = "";
            if (len > 6) {
                programName = QString::fromUtf8(result.mid(6), len - 6);
            }
            setCodeRow(row, programName);
//        } else if (command == CMD_DISPLAY_STATE) {	// DisplayState
//            int displayState, pickCount;
//            displayState = ((uchar)result[2]<<8) | (uchar)result[3];
//            pickCount = ((uchar)result[4]<<8) | (uchar)result[5];
//            emit syncDisplayState( displayState, pickCount );
        } else if (command == CMD_PANEL) {
            int state;
            state = ((uchar)result[2]<<8) | (uchar)result[3];
            openPanel( state );
        } else if (command == CMD_LOG) {
//            qDebug(QString::fromUtf8(result.mid(2), len - 2).toLocal8Bit());
            if (len>10) {
                emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
            }
        } else if (command == CMD_OPERATE_MODE) {


            if (result.at(2) == 0){
                operatorStates[robotIndex]="自动";

            }else if(result.at(2) == 1){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 2){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 3){
                operatorStates[robotIndex]="手动全速";

            }else if(result.at(2) == 4){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 5){
                operatorStates[robotIndex]="手动";

            }else if(result.at(2) == 6){
                operatorStates[robotIndex]="示教";

            }else if(result.at(2) == 12){
                operatorStates[robotIndex]="力控";

            }else if(result.at(2) == 13){
                operatorStates[robotIndex]="扭矩";

            }else {
                operatorStates[robotIndex]="未知";

            }


                setOperatorState(robotIndex);


        } else if (command == CMD_EXECUTE_STATE) {

            if (result.at(2) == 1) {
                execStates[robotIndex]="运行";
            } else if(result.at(2) == 2) {

                execStates[robotIndex]="停止";

            } else if(result.at(2) == 10) {
                execStates[robotIndex]="暂停";

            } else if(result.at(2) == 20) {
                execStates[robotIndex]="告警";


            } else if(result.at(2) == 21) {
                execStates[robotIndex]="急停";


            } else if(result.at(2) == 30) {
                execStates[robotIndex]="空闲";


            } else if(result.at(2) == 40) {
                execStates[robotIndex]="断电";


            } else {
                execStates[robotIndex]="未知";
                addLog("operatorState:未知 "+ QString::number(result.at(2)),"");

            }

            setExecState(robotIndex);


        } else if (command == CMD_CONNECT_STATE) {

            if (result.at(2) == 1) {
                connStates[robotIndex]="联机";
            }
            else if(result.at(2) == 11) {
                connStates[robotIndex]="虚拟机";

            }
            else {
                connStates[robotIndex]="脱机";

            }

                setConnState(robotIndex);


        } else if (command == CMD_SPEED_RATIO) {
            if (result.at(2) >= 0 && result.at(2) <= 100) {
                speedCache[robotIndex-1]=result.at(2);
                if(curRobotIndex==robotIndex)
                emit updateSpeedRatio(result.at(2));
            }

        }
        else if(command==CMD_POSITION_TRANSFORM)
         {


             emit positionTransform();
         }
        else if (command == CMD_PROCESS_TIME) {
            float sec;
            sec = (((uchar)result[5] << 24) | ((uchar)result[4] << 16) | ((uchar)result[3] << 8) | ((uchar)result[2] << 0)) / 1000.0;
            emit updateProcessTime( sec );
        } else if (command == CMD_SIGNAL_CHANGED) {
            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');

            if (signalTriggerMaps[robotIndex-1].contains(data[0])){
                signalTriggerCache[data[0]] = data[1];
                triggerValueChanged((signalTriggerMaps[robotIndex-1])[data[0]]);
            }


            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value = data[1].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;
                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);
                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_VAR_CHANGED) {

            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
            if (varTriggerMaps[robotIndex-1].contains(data[0])){

                varTriggerCache[data[0]] = data[1];
                triggerValueChanged((varTriggerMaps[robotIndex-1])[data[0]]);

            }

            // 需要时更新外部轴位置
            QSqlQuery query;
            AxisObject *axBase = NULL;
            query.exec(QString("select id from StationAxis where stationId='%1' and sign='%2'").arg(currentStationId, data[0]));
            if(query.next()) {
                axBase = axisMap[query.value(0).toString()];
                axBase->value =data[0].toInt();
                while (!axBase->parentId.isEmpty()) {
                    axBase = axisMap[axBase->parentId];
                }
                TopLoc_Location locBase;
                Handle_AIS_Shape aisShape = NULL;
                while (true) {
                    gp_Trsf trsfBase;

                    if (axBase->type == 1) {
                        trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
                        locBase = locBase * TopLoc_Location(trsfBase);
                    } else if(axBase->type == 2) {
                        trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
                        locBase = locBase * TopLoc_Location(trsfBase);


                    }
                    for(int j = 0; j < axBase->moduleList.size(); ++j) {
                        aisShape = moduleMap[axBase->moduleList[j]]->aisShape;
                        aisShape->GetContext()->SetLocation(aisShape, locBase);
                    }
                    if (axBase->subAxisList.isEmpty()) {
                        break;
                    }
                    axBase = axisMap[axBase->subAxisList.at(0)];
                }
                if (aisShape) {
                    aisShape->GetContext()->UpdateCurrentViewer();
                }
            }
        } else if (command == CMD_LOGON_RESULT) {
qDebug()<<"aaaa";
            if (!currentStationId.isEmpty()) {
                qDebug()<<"bbbb";
                QString signalList="?", varList="?";
                signalTriggerMaps[robotIndex-1].clear();
                varTriggerMaps[robotIndex-1].clear();
                QSqlQuery query,query2;
                query.exec(QString("SELECT id,type,code,extraTrigger,code2,extraTrigger2,code3 FROM StationAction WHERE stationId='%1' AND isUse=1").arg(currentStationId));
                while(query.next()) {

                    QSqlRecord record = query.record();
                    switch (record.value("type").toInt()) {
                    case 1: // 信号
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
                        }
                        break;
                    }
                    case 2: // 变量
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        //update zhugz

                        if (!code.isEmpty()) {
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {

                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            varList += "|" + code3;
                        }
                        //end
                        break;
                    }
                    case 3: // 信号/变量
                    case 21: // 刀具寿命(信号/变量)
                    case 31: // 测量保存动作(信号/变量)
                    case 32: // 视觉分析动作(信号/变量)
                    {
                    }
                    case 33: // 称重动作(信号/变量)
                    case 100: // 复位动作(信号/变量)
                    {
                        QString code = record.value("code").toString();
                        QString code2 = record.value("code2").toString();
                        QString code3 = record.value("code3").toString();
                        if (!code.isEmpty()) {
                            (signalTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code].append(record.value("id").toString());
//                            signalList += "|" + code;
//                            varList += "|" + code;
                        }
                        if (!code2.isEmpty() && record.value("extraTrigger").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code2].append(record.value("id").toString());
//                            signalList += "|" + code2;
//                            varList += "|" + code2;
                        }
                        if (!code3.isEmpty() && record.value("extraTrigger2").toInt()>0) {
                            (signalTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
                            (varTriggerMaps[robotIndex-1])[code3].append(record.value("id").toString());
//                            signalList += "|" + code3;
//                            varList += "|" + code3;
                        }
                        break;
                    }
                    }
                }

                foreach (QString code, signalTriggerMaps[robotIndex-1].keys()) {
                    if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    signalList += "|" + code;
                }

                foreach (QString code, varTriggerMaps[robotIndex-1].keys()) {
                    if(code.at(code.size()-1).toAscii()==robotIndex+48)
                    varList += "|" + code;
                }


                // 辅助轴变量注册
                query.exec(QString("select id from stationMotion where stationid='%1' and displayOrder=%2 order by displayOrder").arg(currentStationId).arg(robotIndex));
                QString r_Axis;
                if(query.next())
                    r_Axis=query.value(0).toString();

                query.exec(QString("SELECT DISTINCT sign FROM StationAxis WHERE motionId='%1' AND isUse=1 AND sign<>''").arg(r_Axis));

                while(query.next()) {
                    signalList += "|" + query.value(0).toString();
                    varList += "|" + query.value(0).toString();

                }

                //视觉输出变量注册
//                query.exec(QString("select id from StationAction where stationId='%1'").arg(currentStationId));
//                QStringList v_actionId,listTmp;
//                QString temp1;
//                while(query.next())
//                {
//                   query2.exec(QString("select value from StationActionDetail where ationId='%1'").arg(query.value(0).toString()));
//                   while(query2.next())
//                   {
//                       listTmp=query2.value(0).toString().split("|");
//                       if(listTmp[3]=="")
//                           break;
//                       v_actionId=listTmp[3].split(",");
//                       varList += "|" +v_actionId.join("|");
//                       emit addLog("视觉变量注册"+v_actionId.join("|"),"");
//                   }
//                }

                if(signalList.length()==1)
                {
                    signalList="??";
                }

                QByteArray data = signalList.toLocal8Bit();
                data[0] = CMD_REG_SIGNAL_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,3);

                if (varList.length()==1) {
                    varList = "??";
                }
                data = varList.toLocal8Bit();
                data[0] = CMD_REG_VAR_TRIGGER;
                data[1] = robotIndex;
                mysendService(data,3);




//                    qDebug("\r\n");
//                    qDebug(data2);
//                    qDebug(QString::number(strlen(data2)).toLocal8Bit().data());

            }
        } else if (command == CMD_ERROR_NUMBER) {
            int errorNumber;
            errorNumber = ((uchar)result[2]<<8) | (uchar)result[3];

            if (errorNumber == 0)
            {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警已清除", QString());
            }
            else
            {
                QSqlQuery query;
                query.exec(QString("SELECT code,title from AlarmLibrary where code=%1").arg(errorNumber));
                if (query.next())
                {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  "+query.value(1).toString()+": "+query.value(2).toString()+"[" + query.value(0).toString()+"]", QString());
                } else {
                    emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  操作告警：" + QString::number(errorNumber), QString());
                }
            }


//            emit addLog(QString::fromUtf8(result.mid(2), len - 2), QString());
//            QStringList data = QString::fromUtf8(result.mid(2), len - 2).split('|');
//            if (varTriggerMap.contains(data[0])){
//                varTriggerCache[data[0]] = data[1];
////                QStringList idList = varTriggerMap[data[0]];
//                triggerValueChanged(varTriggerMap[data[0]]);
//            }
        } else if (command == CMD_GET_PROGRAM_RESULT) {
            if ((uchar)result[2] == 0x01) {
                //	programCode = QString( SysUtils::readFile(robotPrg) );
                programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod";
                if (! QFile::exists( programFile )) {
                    programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\Module1.mod";
                    if (! QFile::exists( programFile )) {
                        programFile = "D:\\DzicsRobot\\Program\\"+currentStationId+"\\MainModule.mod"; // 不存在module1就还原为mainmodule
                        QDir programPath = QDir("D:\\DzicsRobot\\Program\\"+currentStationId);
                        if(programPath.exists()) {
                            QStringList files = programPath.entryList(QStringList("*.mod"), QDir::Files | QDir::NoSymLinks);
                            if (files.count()>0) {
                                programFile = files.at(0);
                            }
                        } else {
                            programPath.mkpath(programPath.path());
                        }
                    }
                }
                programCode = QString( SysUtils::readFile( programFile ) );
                 if(codeModel==NULL)//add by zhugz
                codeModel = new QStringListModel(programCode.replace("\t", " ").split('\n'));
                m_ui->codeView->setModel(codeModel);
                m_ui->codeView->setCurrentIndex(codeModel->index(-1));

                m_ui->textEdit->setPlainText(programCode);
            } else {
                emit addLog(QTime::currentTime().toString("hh:mm:ss") + "  当前指令获取失败！", QString());
            }
            qApp->restoreOverrideCursor();
        } else if (command == CMD_PROGRAM_GENERATE_RESULT) {
            m_ui->textEdit->insertPlainText("    " + QString::fromUtf8(result.mid(2), len - 2) + "\n");
        } else if(command==CMD_WORKPIECE_COUNT){
            //统计工件个数
            //QString data1=  QString::fromUtf8(result.mid(2), len - 2);
            int num=0;
            num=((uchar)result[2]<<24) | ((uchar)result[3]<<16) | ((uchar)result[4]<<8) | (uchar)result[5];
            m_ui->maodingLabel->setText(QString::number(num));
            if(num==1)
            {
                writeXmlValue("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyy").toLocal8Bit()+"work.cpk","date",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            }
          }
       } catch(...) {
            // Ignore

      }
    }
}

void MainWindow::visionSendRet(QStringList &varName,params &value,int type)
{
    if(type==1000)
    {

    }
    else if(type==1001)
    {

    }
}
