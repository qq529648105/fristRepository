/****************************************************************************
**
**  Created By:  Daiyl  at  2013-12-30
**  Copyright (c) 2013 Dzics.com, All rights reserved.
**
****************************************************************************/
#include "ModuleImportThread.h"

#include "SysUtils.h"

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


#include "TopTools_SequenceOfShape.hxx"
#include "StlAPI_Reader.hxx"
#include "BRepTools.hxx"
#include "BRep_Builder.hxx"
#include "BRepBuilderAPI.hxx"
#include "STEPControl_Reader.hxx"
#include "STEPControl_Writer.hxx"
#include "IGESControl_Reader.hxx"
#include "IGESControl_Writer.hxx"
#include "IGESControl_Controller.hxx"
#include "Interface_Static.hxx"
#include "StlAPI_Writer.hxx"
#include "TopTools_HSequenceOfShape.hxx"
#include "AIS_Shape.hxx"
#include "AIS_InteractiveContext.hxx"
#include "FSD_BinaryFile.hxx"
#include "ShapeSchema.hxx"
#include "Storage_Data.hxx"
#include "Storage_HSeqOfRoot.hxx"
#include "Storage_Root.hxx"
#include "PTopoDS_HShape.hxx"
#include "PTColStd_PersistentTransientMap.hxx"
#include "MgtBRep.hxx"
#include "TCollection_ExtendedString.hxx"
#include "PTColStd_TransientPersistentMap.hxx"
#include "TopoDS_Compound.hxx"
#include "VrmlAPI_Writer.hxx"
#include "TopExp_Explorer.hxx"
#include "BRep_Tool.hxx"
#include "TopoDS.hxx"
#include "Geom_Line.hxx"
#include "Geom_Surface.hxx"

#include <STEPConstruct_Styles.hxx>
#include <TransferBRep.hxx>
#include <StepBasic_ProductDefinition.hxx>
#include <XSControl_TransferReader.hxx>
#include <StepShape_ShapeRepresentation.hxx>
#include <STEPConstruct.hxx>
#include <StepVisual_StyledItem.hxx>

//#pragma comment(lib,"TKSTEP.lib")

#include "FSD_BinaryFile.hxx"
#include "mesh/MeshTranslate.h"

#include <QApplication.h>
#include <QFile>
#include <QFileInfo>

ModuleImportThread::ModuleImportThread(QString filename, ModuleObject *module)
{
    this->filename = filename;
    this->module = module;
//    this->id = id;
//    this->aisShape = aisShape;
//    this->transform = transform;
//    this->moduleFileName = moduleFileName;
}


ModuleImportThread::~ModuleImportThread(void)
{

}

QMutex mutexImport;
Handle_TopTools_HSequenceOfShape importSTEPWithColor( const QString& file, Handle_AIS_Shape aisShape )
{
    Handle_TopTools_HSequenceOfShape aSequence;
    STEPControl_Reader aReader;
    IFSelect_ReturnStatus status = aReader.ReadFile( file.toAscii().data() );
    if ( status == IFSelect_RetDone )
    {
        int nbr = aReader.NbRootsForTransfer();
        for ( Standard_Integer n = 1; n <= nbr; n++ )
        {
            aReader.TransferRoot( n );
            int nbs = aReader.NbShapes();
            if ( nbs > 0 )
            {
                aSequence = new TopTools_HSequenceOfShape();
                for ( int i = 1; i <= nbs; i++ )
                {
                    TopoDS_Shape shape = aReader.Shape( i );
                    aSequence->Append( shape );

//                    TopExp_Explorer ex;
//                    for (ex.Init(shape, TopAbs_SOLID); ex.More(); ex.Next())
//                    {
//                        const TopoDS_Solid& aSolid = TopoDS::Solid(ex.Current());
//                        aSequence->Append( aSolid );

//                        Handle_AIS_Shape newAisShape = new AIS_Shape(aSolid);
//                        newAisShape->SetContext(aisShape->GetContext());
//                        newAisShape->GetContext()->Display(newAisShape, Standard_True);
//                    }
                }
            }
        }

        if (aSequence->Length() > 0) {
            aisShape->Set( aSequence->Value( 1 ) );
        }

        STEPConstruct_Styles Styles ( aReader.WS() );
        if ( ! Styles.LoadStyles() ) {
            if (!aisShape->HasMaterial())
                aisShape->SetMaterial(Graphic3d_NOM_CHROME);
            return aSequence;
        }
        Standard_Integer nb = Styles.NbStyles();
        if (nb > 0) {
            Handle(StepVisual_StyledItem) style = Styles.Style ( 1 );
            if ( style.IsNull() ) {
                if (!aisShape->HasMaterial())
                    aisShape->SetMaterial(Graphic3d_NOM_CHROME);
                return aSequence;
            }
            Handle(StepVisual_Colour) SurfCol, BoundCol, CurveCol;
            Standard_Boolean IsComponent;
            if ( ! Styles.GetColors ( style, SurfCol, BoundCol, CurveCol, IsComponent ) ) {
                if (!aisShape->HasMaterial())
                    aisShape->SetMaterial(Graphic3d_NOM_CHROME);
                return aSequence;
            }
            if (IsComponent && !SurfCol.IsNull()) {
                Quantity_Color color;
                Styles.DecodeColor ( SurfCol, color );
                if (!aisShape->HasMaterial())
                    aisShape->SetMaterial(Graphic3d_NOM_DEFAULT);
                aisShape->SetColor(color);
            }
        }
    }
    return aSequence;
}

void ModuleImportThread::run()
{
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::BusyCursor);
    QFileInfo fileInfo(filename);
    QString suffix = fileInfo.suffix().toLower();
    if (fileInfo.exists()) {
        if (suffix == "stp" || suffix == "step") {
            qDebug(filename.toLocal8Bit().constData());
            Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
            try {
                mutexImport.lock();
                qApp->restoreOverrideCursor();
                qApp->setOverrideCursor(Qt::BusyCursor);
//                TopoDS_Shape shape;
//                shape = MeshTranslate::loadSTEP(filename);
                sps = MeshTranslate::importModel(MeshTranslate::FormatSTEP, filename);
//                sps = importSTEPWithColor(filename);
//                sps = importSTEPWithColor(filename, this->aisShape);
                mutexImport.unlock();
                if (sps->Length() > 0) {
                    TopoDS_Shape shape = sps->Value( 1 );
                    shape.Location(TopLoc_Location(module->transform));
                    module->aisShape->Set( shape );
                }
//                if (sps->Length() == 1) {
//                    this->aisShape->Set( sps->Value( sps->Length() ) );
//                } else {
//                    TopoDS_Compound aRes;
//                    BRep_Builder aBuilder;
//                    aBuilder.MakeCompound (aRes);
//                    for (int i=1;i<=sps->Length();i++) {
//                        aBuilder.Add (aRes , sps->Value( i ));
//                    }
//                    this->aisShape->Set( aRes );
//                }

                module->aisShape->GetContext()->Display(module->aisShape, Standard_True);
            } catch (...) { }
            mutexImport.lock();
//            MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, "D:\\DzicsRobot\\Module\\" + id + ".dzm", sps);
            MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, "D:\\DzicsRobot\\" + module->filename + ".dzm", sps);
            mutexImport.unlock();
            emit importFinish( module->id );
        } else if (suffix == "dzm") {
            mutexImport.lock();
            Handle_TopTools_HSequenceOfShape sps = MeshTranslate::importModel(MeshTranslate::FormatCSFDB, filename);
//             MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, filename, sps);
            mutexImport.unlock();
            if (sps->Length() > 0) {
//                module->aisShape->Set( sps->Value( 1 ) );
                TopoDS_Shape shape = sps->Value( 1 );
                shape.Location(TopLoc_Location(module->transform));
                module->aisShape->Set( shape );
            }
//            if (!aisShape->HasMaterial())
//                aisShape->SetMaterial(Graphic3d_NOM_CHROME);
            module->aisShape->GetContext()->Display(module->aisShape, Standard_True);
            qDebug()<<"module thread del dzm:"<<QFile::remove("D:\\DzicsRobot\\" + module->filename + ".dzm");

            QFile::copy(filename, "D:\\DzicsRobot\\" + module->filename + ".dzm");
            emit importFinish( module->id );
        } else if (suffix == "igs" || suffix == "iges") {
            Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
            try {
                mutexImport.lock();
                qApp->restoreOverrideCursor();
                qApp->setOverrideCursor(Qt::BusyCursor);
                sps = MeshTranslate::importModel(MeshTranslate::FormatIGES, filename);
                mutexImport.unlock();
                if (sps->Length() > 0) {
//                    module->aisShape->Set( sps->Value( 1 ) );
                    TopoDS_Shape shape = sps->Value( 1 );
                    shape.Location(TopLoc_Location(module->transform));
                    module->aisShape->Set( shape );
                }
//                if (!aisShape->HasMaterial())
//                    aisShape->SetMaterial(Graphic3d_NOM_CHROME);
                module->aisShape->GetContext()->Display(module->aisShape, Standard_True);
            } catch (...) { }
            mutexImport.lock();
            MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, "D:\\DzicsRobot\\" + module->filename + ".dzm", sps);
            mutexImport.unlock();
            emit importFinish( module->id );
        } else if (suffix == "stl") {
            Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
            try {
                mutexImport.lock();
                qApp->restoreOverrideCursor();
                qApp->setOverrideCursor(Qt::BusyCursor);
                sps = MeshTranslate::importModel(MeshTranslate::FormatSTL, filename);
                mutexImport.unlock();
                if (sps->Length() > 0) {
//                    module->aisShape->Set( sps->Value( 1 ) );
                    TopoDS_Shape shape = sps->Value( 1 );
                    shape.Location(TopLoc_Location(module->transform));
                    module->aisShape->Set( shape );
                }
//                if (!aisShape->HasMaterial())
//                    aisShape->SetMaterial(Graphic3d_NOM_CHROME);
                module->aisShape->GetContext()->Display(module->aisShape, Standard_True);
            } catch (...) { }
            mutexImport.lock();
            MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, "D:\\DzicsRobot\\" + module->filename + ".dzm", sps);
            mutexImport.unlock();
            emit importFinish( module->id );
        }
    } else {
        emit importFinish( module->id );
    }

    qApp->restoreOverrideCursor();
}
