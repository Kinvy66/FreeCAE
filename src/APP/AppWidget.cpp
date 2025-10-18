#include "AppWidget.h"
#include "ui_AppWidget.h"

#include "DockAreaWidget.h"
#include "DockAreaTitleBar.h"
#include "DockAreaTabBar.h"
#include "FloatingDockContainer.h"
#include "DockComponentsFactory.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkNew.h>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>
#include <vtkConeSource.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

#include <BRepPrimAPI_MakeBox.hxx>
#include <STEPControl_Reader.hxx>
#include <IVtkOCC_Shape.hxx>

using namespace ads;

AppWidget::AppWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppWidget)
{
    ui->setupUi(this);
    
    STEPControl_Reader reader;
    // reader.ReadFile("");
    // reader.TransferRoot();
    // IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(reader.Shape());
    
    // CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    // CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    // CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    // DockManager = new CDockManager(this);
    
    
    // CDockWidget* TableDockWidget = DockManager->createDockWidget("Table 1");
    // DockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, TableDockWidget);
    
    // TableDockWidget = DockManager->createDockWidget("Table 2");
    // DockManager->addDockWidget(DockWidgetArea::BottomDockWidgetArea, TableDockWidget, TableArea);
    
    
    vtkNew<vtkRenderer> renderer;
    
    ui->openGLWidget->renderWindow()->AddRenderer(renderer);
    ui->openGLWidget->renderWindow()->Render();
    
    
    std::array<std::array<double, 3>, 8> pts = {
        { { {0, 0, 0} }
         ,{ {1, 0, 0} }
         ,{ {1, 1, 0} }
         ,{ {0, 1, 0} }
         ,{ {0, 0, 1} }
         ,{ {1, 0, 1} }
         ,{ {1, 1, 1} }
         ,{ {0, 1, 1} }}
    };
    
    std::array<std::array<vtkIdType, 4>, 6> ordering = {
        { { {0, 1, 2, 3} }
         ,{ {4, 5, 6, 7} }
         ,{ {0, 1, 5, 4} }
         ,{ {1, 2, 6, 5} }
         ,{ {2, 3, 7, 6} }
         ,{ {3, 0, 4, 7} }}
    };
    
    vtkNew<vtkPolyData> cube;
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> polys;
    vtkNew<vtkFloatArray> scalars;
    
    
    
    for (int i = 0; i < pts.size(); ++i) {
        points->InsertPoint(i, pts[i].data());
        scalars->InsertTuple1(i, i);
    }
    
    for (auto&& i : ordering) {
        polys->InsertNextCell(vtkIdType(i.size()), i.data());
    }
    
    cube->SetPoints(points);
    cube->SetPolys(polys);
    cube->GetPointData()->SetScalars(scalars);
    
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(cube);
    mapper->SetScalarRange(cube->GetScalarRange());
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    renderer->AddActor(actor);
}

AppWidget::~AppWidget()
{
    delete ui;
}
