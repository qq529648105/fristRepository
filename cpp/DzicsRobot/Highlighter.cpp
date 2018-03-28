#include "Highlighter.h"

#include <QtGui>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::blue);
    QStringList keywordPatterns1;
    keywordPatterns1 << "\\bBACKWARD\\b" << "\\bCONST\\b" << "\\bDIV\\b" << "\\bDO\\b"
		<< "\\bENDFOR\\b" << "\\bENDIF\\b" << "\\bENDTEST\\b" << "\\bENDWHILE\\b" << "\\bERROR\\b"
		<< "\\bFOR\\b" << "\\bFROM\\b" << "\\bGOTO\\b" << "\\bIF\\b" << "\\bINOUT\\b"
		<< "\\bMOD\\b" << "\\bNOSTEPIN\\b" << "\\bNOT\\b" << "\\bNOVIEW\\b" << "\\bPERS\\b"
		<< "\\bRAISE\\b" << "\\bREADONLY\\b" << "\\bRETRY\\b" << "\\bRETURN\\b" << "\\bSTEP\\b" << "\\bSYSMODULE\\b"
		<< "\\bTEST\\b" << "\\bTHEN\\b" << "\\bTO\\b" << "\\bTRYNEXT\\b"
		<< "\\bVAR\\b" << "\\bVIEWONLY\\b" << "\\bWHILE\\b" << "\\bWITH\\b"
        << "\\bCASE\\b" << "\\bDEFAULT\\b"<< "\\bELSE\\b"<< "\\bELSEIF\\b"
        << "\\bEND\\b" << "\\bSUB\\b" << "\\bDEF\\b" << "@" << "\\bDELAY\\b";  // Denso

    foreach (const QString &pattern, keywordPatterns1) {
        rule.pattern = QRegExp(pattern);
		rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
	keywordFormat2.setForeground(Qt::blue);
    QStringList keywordPatterns2;
    keywordPatterns2 << "\\bENDFUNC\\b" << "\\bENDMODULE\\b" << "\\bENDPROC\\b" << "\\bENDTRAP\\b"
		<< "\\bFUNC\\b" << "\\bMODULE\\b" << "\\bPROC\\b" << "\\bTRAP\\b";

    foreach (const QString &pattern, keywordPatterns2) {
        rule.pattern = QRegExp(pattern);
		rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat2;
        highlightingRules.append(rule);
    }

	functionFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	//rule.pattern = QRegExp("\\bPROC [A-Za-z0-9_]+");
    rule.format = functionFormat;
    highlightingRules.append(rule);

	keywordFormat3.setForeground(Qt::darkGreen);
    QStringList keywordPatterns3;
    keywordPatterns3 << "\\bbool\\b" << "\\bconfdata\\b" "\\bintnum\\b" << "\\bjointtarget\\b" << "\\bloaddata\\b" << "\\blocal\\b" << "\\bmecunit\\b" << "\\bnum\\b" << "\\borient\\b" << "\\bpos\\b" << "\\bpose\\b" << "\\bprogdisp\\b" << "\\brobjoint\\b" << "\\brobtarget\\b" << "\\bshapedata\\b" << "\\bsignalai\\b" << "\\bsignalao\\b" << "\\bsignaldi\\b" << "\\bsignaldo\\b" << "\\bsignalgi\\b" << "\\bsignalgo\\b" << "\\bspeeddata\\b" << "\\bstring\\b" << "\\btooldata\\b" << "\\btriggdata\\b" << "\\bwobjdata\\b" << "\\bzonedata\\b";
	keywordPatterns3 << "\\bAND\\b" << "\\bFALSE\\b" << "\\bOR\\b" << "\\bTRUE\\b" << "\\bXOR\\b";// << "\\b|\\b";
//    keywordPatterns3 << "\\bAccSet\\b" << "\\bActUnit\\b" << "\\bAdd\\b" << "\\bBreak\\b" << "\\bCallByVar\\b" << "\\bClear\\b" << "\\bClkReset\\b" << "\\bClkStart\\b" << "\\bClkStop\\b" << "\\bClose\\b" << "\\bConfJ\\b" << "\\bConfL\\b" << "\\bCONNECT\\b" << "\\bCorrClear\\b" << "\\bCorrCon\\b" << "\\bCorrDiscon\\b" << "\\bCorrWrite\\b" << "\\bDeactUnit\\b" << "\\bDecr\\b" << "\\bEoffsOff\\b" << "\\bEOffsOn\\b" << "\\bEoffsSet\\b" << "\\bERRNO\\b" << "\\bErrWrite\\b" << "\\bEXIT\\b" << "\\bExitCycle\\b" << "\\bGripLoad\\b" << "\\bIDelete\\b" << "\\bIdisable\\b" << "\\bIEnable\\b" << "\\bIncr\\b" << "\\bIndAMove\\b" << "\\bIndCMove\\b" << "\\bIndDMove\\b" << "\\bIndReset\\b" << "\\bIndRMove\\b" << "\\bInvertDO\\b" << "\\bIODisable\\b" << "\\bIOEnable\\b" << "\\bIsignalDI\\b" << "\\bISignalDO\\b" << "\\bIsleep\\b" << "\\bITimer\\b" << "\\bIVarValue\\b" << "\\bIWatch\\b";
	keywordPatterns3 << "\\bLoad\\b" << "\\bMoveAbsJ\\b" << "\\bMoveC\\b" << "\\bMoveJ\\b" << "\\bMoveL\\b" << "\\bOpen\\b" << "\\bPathResol\\b" << "\\bPdispOff\\b" << "\\bPDispOn\\b" << "\\bPDispSet\\b" << "\\bPulseDO\\b" << "\\bReset\\b" << "\\bRestoPath\\b" << "\\bRewind\\b" << "\\bSearchC\\b" << "\\bSearchL\\b" << "\\bSet\\b" << "\\bSetAO\\b" << "\\bSetDO\\b" << "\\bSetGO\\b" << "\\bSingArea\\b" << "\\bSoftAct\\b" << "\\bSoftDeact\\b" << "\\bStartMove\\b" << "\\bStop\\b" << "\\bStopMove\\b" << "\\bStorePath\\b" << "\\bTPErase\\b" << "\\bTPReadFK\\b" << "\\bTPReadNum\\b" << "\\bTPShow\\b" << "\\bTPWrite\\b" << "\\bTriggC\\b" << "\\bTriggEquip\\b" << "\\bTriggInt\\b" << "\\bTriggIO\\b" << "\\bTriggJ\\b" << "\\bTriggL\\b" << "\\bTuneReset\\b" << "\\bTuneServo\\b" << "\\bUnLoad\\b" << "\\bVelSet\\b" << "\\bWaitDI\\b" << "\\bWaitDO\\b" << "\\bWaitTime\\b" << "\\bWaitUntil\\b";// << "\\bWriteBin\\b" << "\\bWriteStrBin\\b" << "\\bWZBoxDef\\b" << "\\bWZCylDef\\b" << "\\bWZDisable\\b" << "\\bWZDOSet\\b" << "\\bWZEnable\\b" << "\\bWZFree\\b" << "\\bWZLimSup\\b" << "\\bWZSphDef\\b";
//	keywordPatterns3 << "\\bAbs\\b" << "\\bACos\\b" << "\\bAOutput\\b" << "\\bArgName\\b" << "\\bASin\\b" << "\\bATan\\b" << "\\bATan2\\b" << "\\bByteToStr\\b" << "\\bCDate\\b" << "\\bCjointT\\b" << "\\bClkRead\\b" << "\\bCorrRead\\b" << "\\bCos\\b" << "\\bCPos\\b" << "\\bCRobT\\b" << "\\bCTime\\b" << "\\bCTool\\b" << "\\bCWObj\\b" << "\\bDefDFrame\\b" << "\\bDefFrame\\b" << "\\bDim\\b" << "\\bDOutput\\b" << "\\bEulerZYX\\b" << "\\bExp\\b" << "\\bGetTime\\b" << "\\bGoutput\\b" << "\\bIndInpos\\b" << "\\bIndSpeed\\b" << "\\bIsPers\\b" << "\\bIsVar\\b" << "\\bMirPos\\b" << "\\bNumToStr\\b" << "\\bOffs\\b" << "\\bOpMode\\b" << "\\bOrientZYX\\b" << "\\bORobT\\b" << "\\bPoseInv\\b" << "\\bPoseMult\\b" << "\\bPoseVect\\b" << "\\bPow\\b" << "\\bPresent\\b" << "\\bReadBin\\b" << "\\bReadMotor\\b" << "\\bReadNum\\b" << "\\bReadStr\\b" << "\\bRelTool\\b" << "\\bRound\\b" << "\\bRunMode\\b" << "\\bSin\\b" << "\\bSqrt\\b" << "\\bStrFind\\b" << "\\bStrLen\\b" << "\\bStrMap\\b" << "\\bStrMatch\\b" << "\\bStrMemb\\b" << "\\bStrOrder\\b" << "\\bStrPart\\b" << "\\bStrToByte\\b" << "\\bStrToVal\\b" << "\\bTan\\b" << "\\bTestDI\\b" << "\\bTrunc\\b" << "\\bValToStr\\b";
//	keywordPatterns3 << "\\bAbsPosVergleich\\b" << "\\bCheckHomePos\\b" << "\\bCheckParity\\b" << "\\bEndSync\\b" << "\\bFahreZuPos\\b" << "\\bHigh\\b" << "\\bLow\\b" << "\\bMeldung\\b" << "\\bmv\\b" << "\\bPosVergleich\\b" << "\\bProgInfo\\b" << "\\bRobSync\\b" << "\\bSetParity\\b" << "\\bSMoveJ\\b" << "\\bSMoveL\\b" << "\\bSyncEvent\\b" << "\\bWaitBreakDI\\b" << "\\bWaitMsgDI\\b" << "\\bWaitTPBreakDI\\b";

    // Denso
    keywordPatterns3 << "\\bMove\\b" << "=";

    foreach (const QString &pattern, keywordPatterns3) {
        rule.pattern = QRegExp(pattern);
		rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat3;
        highlightingRules.append(rule);
    }
/*
	keywordFormat3.setForeground(Qt::darkGreen);
    QStringList keywordPatterns3;
    keywordPatterns3 << "\\bbool\\b" << "\\bclock\\b" << "\\bconfdata\\b" << "\\bcorrdescr\\b" << "\\bdionum\\b" << "\\berrnum\\b" << "\\bextjoint\\b" << "\\bintnum\\b" << "\\biodev\\b" << "\\bjointtarget\\b" << "\\bloaddata\\b" << "\\blocal\\b" << "\\bmecunit\\b" << "\\bmotsetdata\\b" << "\\bnum\\b" << "\\bo_jointtarget\\b" << "\\bo_robtarget\\b" << "\\borient\\b" << "\\bpos\\b" << "\\bpose\\b" << "\\bprogdisp\\b" << "\\brobjoint\\b" << "\\brobtarget\\b" << "\\bshapedata\\b" << "\\bsignalai\\b" << "\\bsignalao\\b" << "\\bsignaldi\\b" << "\\bsignaldo\\b" << "\\bsignalgi\\b" << "\\bsignalgo\\b" << "\\bspeeddata\\b" << "\\bstring\\b" << "\\bsymnum\\b" << "\\btooldata\\b" << "\\btpnum\\b" << "\\btriggdata\\b" << "\\btunetype\\b" << "\\bwobjdata\\b" << "\\bwzstationary\\b" << "\\bwztemporary\\b" << "\\bzonedata\\b";
	keywordPatterns3 << "\\[" << "\\]" << "\\+" << "\\-" << "\\*" << "\\/" << ":=" << "\\b#\\b" << "\\b%\\b" << "\\b&\\b" << "\\bAND\\b" << "\\bFALSE\\b" << "\\bOR\\b" << "\\bTRUE\\b" << "\\bXOR\\b";// << "\\b|\\b";
	//keywordPatterns3 <<  << "/\\b*\\b/" << "\/";// << "\\b/\\b" << "\\b:\\b" << "\\b<\\b" << "\\b=\\b" << "\\b>\\b" << "\\b^\\b";
    foreach (const QString &pattern, keywordPatterns3) {
        rule.pattern = QRegExp(pattern);
		rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat3;
        highlightingRules.append(rule);
    }

	keywordFormat4.setForeground(Qt::darkGreen);
	//keywordFormat4.setFontWeight(QFont::Bold);
    QStringList keywordPatterns4;
    keywordPatterns4 << "\\bAccSet\\b" << "\\bActUnit\\b" << "\\bAdd\\b" << "\\bBreak\\b" << "\\bCallByVar\\b" << "\\bClear\\b" << "\\bClkReset\\b" << "\\bClkStart\\b" << "\\bClkStop\\b" << "\\bClose\\b" << "\\bConfJ\\b" << "\\bConfL\\b" << "\\bCONNECT\\b" << "\\bCorrClear\\b" << "\\bCorrCon\\b" << "\\bCorrDiscon\\b" << "\\bCorrWrite\\b" << "\\bDeactUnit\\b" << "\\bDecr\\b" << "\\bEoffsOff\\b" << "\\bEOffsOn\\b" << "\\bEoffsSet\\b" << "\\bERRNO\\b" << "\\bErrWrite\\b" << "\\bEXIT\\b" << "\\bExitCycle\\b" << "\\bGripLoad\\b" << "\\bIDelete\\b" << "\\bIdisable\\b" << "\\bIEnable\\b" << "\\bIncr\\b" << "\\bIndAMove\\b" << "\\bIndCMove\\b" << "\\bIndDMove\\b" << "\\bIndReset\\b" << "\\bIndRMove\\b" << "\\bInvertDO\\b" << "\\bIODisable\\b" << "\\bIOEnable\\b" << "\\bIsignalDI\\b" << "\\bISignalDO\\b" << "\\bIsleep\\b" << "\\bITimer\\b" << "\\bIVarValue\\b" << "\\bIWatch\\b" << "\\bLoad\\b" << "\\bMoveAbsJ\\b" << "\\bMoveC\\b" << "\\bMoveJ\\b" << "\\bMoveL\\b" << "\\bOpen\\b" << "\\bPathResol\\b" << "\\bPdispOff\\b" << "\\bPDispOn\\b" << "\\bPDispSet\\b" << "\\bPulseDO\\b" << "\\bReset\\b" << "\\bRestoPath\\b" << "\\bRewind\\b" << "\\bSearchC\\b" << "\\bSearchL\\b" << "\\bSet\\b" << "\\bSetAO\\b" << "\\bSetDO\\b" << "\\bSetGO\\b" << "\\bSingArea\\b" << "\\bSoftAct\\b" << "\\bSoftDeact\\b" << "\\bStartMove\\b" << "\\bStop\\b" << "\\bStopMove\\b" << "\\bStorePath\\b" << "\\bTPErase\\b" << "\\bTPReadFK\\b" << "\\bTPReadNum\\b" << "\\bTPShow\\b" << "\\bTPWrite\\b" << "\\bTriggC\\b" << "\\bTriggEquip\\b" << "\\bTriggInt\\b" << "\\bTriggIO\\b" << "\\bTriggJ\\b" << "\\bTriggL\\b" << "\\bTuneReset\\b" << "\\bTuneServo\\b" << "\\bUnLoad\\b" << "\\bVelSet\\b" << "\\bWaitDI\\b" << "\\bWaitDO\\b" << "\\bWaitTime\\b" << "\\bWaitUntil\\b" << "\\bWriteBin\\b" << "\\bWriteStrBin\\b" << "\\bWZBoxDef\\b" << "\\bWZCylDef\\b" << "\\bWZDisable\\b" << "\\bWZDOSet\\b" << "\\bWZEnable\\b" << "\\bWZFree\\b" << "\\bWZLimSup\\b" << "\\bWZSphDef\\b";
	keywordPatterns4 << "\\bAbs\\b" << "\\bACos\\b" << "\\bAOutput\\b" << "\\bArgName\\b" << "\\bASin\\b" << "\\bATan\\b" << "\\bATan2\\b" << "\\bByteToStr\\b" << "\\bCDate\\b" << "\\bCjointT\\b" << "\\bClkRead\\b" << "\\bCorrRead\\b" << "\\bCos\\b" << "\\bCPos\\b" << "\\bCRobT\\b" << "\\bCTime\\b" << "\\bCTool\\b" << "\\bCWObj\\b" << "\\bDefDFrame\\b" << "\\bDefFrame\\b" << "\\bDim\\b" << "\\bDOutput\\b" << "\\bEulerZYX\\b" << "\\bExp\\b" << "\\bGetTime\\b" << "\\bGoutput\\b" << "\\bIndInpos\\b" << "\\bIndSpeed\\b" << "\\bIsPers\\b" << "\\bIsVar\\b" << "\\bMirPos\\b" << "\\bNumToStr\\b" << "\\bOffs\\b" << "\\bOpMode\\b" << "\\bOrientZYX\\b" << "\\bORobT\\b" << "\\bPoseInv\\b" << "\\bPoseMult\\b" << "\\bPoseVect\\b" << "\\bPow\\b" << "\\bPresent\\b" << "\\bReadBin\\b" << "\\bReadMotor\\b" << "\\bReadNum\\b" << "\\bReadStr\\b" << "\\bRelTool\\b" << "\\bRound\\b" << "\\bRunMode\\b" << "\\bSin\\b" << "\\bSqrt\\b" << "\\bStrFind\\b" << "\\bStrLen\\b" << "\\bStrMap\\b" << "\\bStrMatch\\b" << "\\bStrMemb\\b" << "\\bStrOrder\\b" << "\\bStrPart\\b" << "\\bStrToByte\\b" << "\\bStrToVal\\b" << "\\bTan\\b" << "\\bTestDI\\b" << "\\bTrunc\\b" << "\\bValToStr\\b";
	keywordPatterns4 << "\\bAbsPosVergleich\\b" << "\\bCheckHomePos\\b" << "\\bCheckParity\\b" << "\\bEndSync\\b" << "\\bFahreZuPos\\b" << "\\bHigh\\b" << "\\bLow\\b" << "\\bMeldung\\b" << "\\bmv\\b" << "\\bPosVergleich\\b" << "\\bProgInfo\\b" << "\\bRobSync\\b" << "\\bSetParity\\b" << "\\bSMoveJ\\b" << "\\bSMoveL\\b" << "\\bSyncEvent\\b" << "\\bWaitBreakDI\\b" << "\\bWaitMsgDI\\b" << "\\bWaitTPBreakDI\\b";

    foreach (const QString &pattern, keywordPatterns4) {
        rule.pattern = QRegExp(pattern);
		rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat4;
        highlightingRules.append(rule);
    }
*/

    //classFormat.setFontWeight(QFont::Bold);
    //classFormat.setForeground(Qt::darkMagenta);
    //rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    //rule.format = classFormat;
    //highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("![^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegExp("'[^\n]*");  // denso
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
//    rule.pattern = QRegExp("$[^\n]*");  // ur
//    rule.format = singleLineCommentFormat;
//    highlightingRules.append(rule);


    //multiLineCommentFormat.setForeground(Qt::red);

	quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //functionFormat.setFontItalic(true);
    //functionFormat.setForeground(Qt::blue);



    //commentStartExpression = QRegExp("/\\*");
    //commentEndExpression = QRegExp("\\*/");
}


void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    //int startIndex = 0;
    //if (previousBlockState() != 1)
    //    startIndex = commentStartExpression.indexIn(text);

    //while (startIndex >= 0) {
    //    int endIndex = commentEndExpression.indexIn(text, startIndex);
    //    int commentLength;
    //    if (endIndex == -1) {
    //        setCurrentBlockState(1);
    //        commentLength = text.length() - startIndex;
    //    } else {
    //        commentLength = endIndex - startIndex
    //                        + commentEndExpression.matchedLength();
    //    }
    //    setFormat(startIndex, commentLength, multiLineCommentFormat);
    //    startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    //}
}


Highlighter::~Highlighter()
{

}
