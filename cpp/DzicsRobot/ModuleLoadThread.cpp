#include "ModuleLoadThread.h"

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

#include <MgtBRep.hxx>
#include <PTopoDS_HShape.hxx>
#include <PTColStd_TransientPersistentMap.hxx>
#include <Storage_Data.hxx>
#include <ShapeSchema.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <Storage_Root.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <AIS_InteractiveContext.hxx>

#include "FSD_BinaryFile.hxx"
#include "mesh/MeshTranslate.h"

#include <QApplication.h>
#include <QFile>

#include <BRepTools_ShapeSet.hxx>
#include "SysUtils.h"

//ModuleLoadThread::ModuleLoadThread(QString filename, Handle_AIS_Shape aisShape, gp_Trsf transform, bool isHidden)
//{
//	this->filename = filename;
//	this->aisShape = aisShape;
//	this->transform = transform;
//    this->isHidden = isHidden;
//}

ModuleLoadThread::ModuleLoadThread(QString filename, QList<ModuleObject *> moduleList)
{
    this->filename = filename;
    this->moduleList = moduleList;
}

ModuleLoadThread::~ModuleLoadThread(void)
{
}

QMutex mutex;
//void ModuleLoadThread::run()
//{
////    BRep_Builder *builder = new BRep_Builder();
////    BRepTools_ShapeSet *SS = new BRepTools_ShapeSet(*builder);
////        QByteArray file = SysUtils::readFile(filename);
////        qDebug(filename.toLocal8Bit().constData());

////        TopoDS_Shape shapeT;

////        std::stringbuf sbuf;
////        sbuf.sputn(file.constData(), file.size());
////        istream in(&sbuf);
////        SS->Read(in);
////        SS->Read(shapeT, in);
//////        shapeT.Location(TopLoc_Location(this->transform));
////        this->aisShape->Set( shapeT );
////    //	this->aisShape->Redisplay();
//////        if (!this->isHidden) {
////            try {
////                this->aisShape->GetContext()->Display(this->aisShape, Standard_False);
////            } catch (...) { }

////            //this->aisShape->GetContext()->Select();
////            //this->aisShape->GetContext()->HilightWithColor(this->aisShape, Quantity_NOC_GRAY30, Standard_False);	//Quantity_NOC_GRAY25
//////        }
////        emit moduleLoadFinish( "this->id" );
////        return;

////	BRep_Builder aBuilder;

////	TopoDS_Shape shape;// = this->aisShape->Shape();
////	BRepTools::Read( shape, filename.toAscii().data(), aBuilder );
////Handle_TopTools_HSequenceOfShape sps = MeshTranslate::importModel( MeshTranslate::FormatCSFDB, filename + ".dzm" );

////qApp->restoreOverrideCursor();
////qApp->setOverrideCursor(Qt::BusyCursor);

////	if (!QFile::exists(filename+".dzm") && QFile::exists(filename+".stp")) {
////		mutex.lock();
////		TopoDS_Shape shape;
//////		BRep_Builder aBuilder;
//////		BRepTools::Read(  shape, filename.toAscii().data(), aBuilder );
////        shape = MeshTranslate::loadSTEP(filename+".stp");
////		Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
////		sps->Append( shape );
////        MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, filename + ".dzm", sps);
////		mutex.unlock();
////	} else if (QFile::exists(filename+".brep.dzm")) {
////		QFile::rename(filename+".brep.dzm", filename+".dzm");
////	}

////	        Handle_TopTools_HSequenceOfShape aSequence;
////    if (filename == "Module\\yLnC2hCfSWuC476xRTwD") {
////        qDebug("yLnC2hCfSWuC476xRTwD");
////    }
//         TCollection_AsciiString aName( ("D:\\DzicsRobot\\" + filename + ".dzm").toAscii().data() );
//       // Check file type
//         mutex.lock();
//            if ( FSD_BinaryFile::IsGoodFileType( aName ) != Storage_VSOk ) {
//                mutex.unlock();
////                qApp->restoreOverrideCursor();
//                emit moduleLoadFinish( filename );
//                return ;
//            }
////            mutex.unlock();

////       mutex.lock();
//       FSD_BinaryFile *fileDriver = new FSD_BinaryFile();
//        if ( fileDriver->Open( aName, Storage_VSRead ) != Storage_VSOk ) {
//            mutex.unlock();
////            qApp->restoreOverrideCursor();
//            emit moduleLoadFinish( filename );
//            return ;
//		}
////    mutex.lock();
//        Handle(ShapeSchema) schema = new ShapeSchema();
//        Handle(Storage_Data) data  = schema->Read( *fileDriver );
//   mutex.unlock();
//		if ( data->ErrorStatus() != Storage_VSOk ) {
////            mutex.unlock();
////            qApp->restoreOverrideCursor();
//            emit moduleLoadFinish( filename );
//            return ;
//		}
//        fileDriver->Close();
////        mutex.unlock();

//TopoDS_Shape shape;

// //       aSequence = new TopTools_HSequenceOfShape();
//        Handle(Storage_HSeqOfRoot) roots = data->Roots();
// //       for ( int i = 1; i <= roots->Length() ; i++ )
//  //      {
//            Handle(Storage_Root) r = roots->Value( 1 );
//            Handle(Standard_Persistent) p = r->Object();
//            Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
//            if ( !aPShape.IsNull() )
//            {
//                PTColStd_PersistentTransientMap aMap;
//               // TopoDS_Shape aTShape;
//                MgtBRep::Translate( aPShape, aMap, shape, MgtBRep_WithTriangle );
//             //   aSequence->Append( aTShape );
//            }
//  //      }


////TopoDS_Shape shape = aSequence->Value( 1 );
//	shape.Location(TopLoc_Location(this->transform));
//	this->aisShape->Set( shape );
////	this->aisShape->Redisplay();

//    if (!this->isHidden) {
//        try {
////            if (this->aisShape->GetContext()->IsDisplayed(this->aisShape)) {
////                this->aisShape->GetContext()->Redisplay(this->aisShape, Standard_False);
////            } else {
//                this->aisShape->GetContext()->Display(this->aisShape, Standard_False);
////            }
//        } catch (...) { }

//        //this->aisShape->GetContext()->Select();
//        //this->aisShape->GetContext()->HilightWithColor(this->aisShape, Quantity_NOC_GRAY30, Standard_False);	//Quantity_NOC_GRAY25
//    }
///*
//	PTColStd_TransientPersistentMap *aMap = new PTColStd_TransientPersistentMap();
//	Handle(PTopoDS_HShape) pshape = MgtBRep::Translate( shape, *aMap, MgtBRep_WithTriangle );
//	Handle(Storage_Data) data = new Storage_Data();
//	Handle(ShapeSchema) schema = new ShapeSchema();
//	//data->AddRoot( filename.toAscii().data(), pshape );

//        //for ( int i = 1; i <= shapes->Length(); i++ )
//        //{
//        //    TopoDS_Shape shape = shapes->Value( i );
//        //    if ( shape.IsNull() )
//        //    {
//        //        lastError = ( "INF_TRANSLATE_ERROR_INVALIDSHAPE" );
//        //        return false;
//        //    }

//        //    Handle(PTopoDS_HShape) pshape = MgtBRep::Translate( shape, aMap, MgtBRep_WithTriangle );
//        //    TCollection_AsciiString objName = TCollection_AsciiString( "Object_" ) + TCollection_AsciiString( i );
//        //    data->AddRoot( objName, pshape );
//        //}

//	FSD_BinaryFile fileDriver;
//	fileDriver.Open( (filename + ".dzm").toAscii().data(), Storage_VSWrite );
//	schema->Write( fileDriver, data );
//	fileDriver.Close();
//*/

//    emit moduleLoadFinish( filename );
////    qApp->restoreOverrideCursor();

//}

#include <QDebug>
void ModuleLoadThread::run()
{
    TCollection_AsciiString aName( ("D:\\DzicsRobot\\" + filename + ".dzm").toAscii().data() );

    mutex.lock();
    if ( FSD_BinaryFile::IsGoodFileType( aName ) != Storage_VSOk ) {
        mutex.unlock();
        emit moduleLoadFinish( filename );
        return ;
    }

    FSD_BinaryFile *fileDriver = new FSD_BinaryFile();
    if ( fileDriver->Open( aName, Storage_VSRead ) != Storage_VSOk ) {
        mutex.unlock();
        emit moduleLoadFinish( filename );
        return ;
    }
    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data  = schema->Read( *fileDriver );
    mutex.unlock();
    if ( data->ErrorStatus() != Storage_VSOk ) {
        emit moduleLoadFinish( filename );
        return ;
    }
    fileDriver->Close();


    TopoDS_Shape shape;

    Handle(Storage_HSeqOfRoot) roots = data->Roots();

    Handle(Storage_Root) r = roots->Value( 1 );
    Handle(Standard_Persistent) p = r->Object();
    Handle(PTopoDS_HShape) aPShape = Handle(PTopoDS_HShape)::DownCast(p);
    if ( !aPShape.IsNull() )
    {
        PTColStd_PersistentTransientMap aMap;
        MgtBRep::Translate( aPShape, aMap, shape, MgtBRep_WithTriangle );
    }

    foreach (ModuleObject *module, moduleList) {

        TopLoc_Location originalLoc = shape.Location();
        module->originalTransform = originalLoc.Transformation();
        //by ronglk 设置上次保存的位置信息
        module->updateModouleMatrix();
        //module->quaternion.SetEulerAngles(gp_YawPitchRoll, module->yaw, module->pitch, module->roll);

//        shape.Location(TopLoc_Location(module->transform));
//        module->aisShape->Set( shape );

        shape.Location(TopLoc_Location(module->transform));
        module->aisShape->Set( shape );

        //module->aisShape->SetContext(myContext);
        //module->aisShape->Set(shape);
        //
        //module->aisShape->GetContext()->Update(module->aisShape, true);
        //module->aisShape = new AIS_Shape(shape);
        //update by zhugz
        //if (!module->isHidden) {
            try {
                module->aisShape->GetContext()->Display(module->aisShape, Standard_False);
//                module->aisShape->Redisplay();
//                module->aisShape->GetContext()->Update(module->aisShape, true);
        if (module->isHidden)
            module->aisShape->GetContext()->Erase(module->aisShape, false);
        } catch (...) { }
       // }

        //end
    }




//    shape.Location(TopLoc_Location(this->transform));
//    this->aisShape->Set( shape );
//    if (!this->isHidden) {
//        try {
//            this->aisShape->GetContext()->Display(this->aisShape, Standard_False);
//        } catch (...) { }
//    }

    emit moduleLoadFinish( filename );

}
