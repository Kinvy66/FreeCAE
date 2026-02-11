/**
 * @file FCMeshFeatureVTK.h
 * @brief 网格特征 VTK 管线（移植自 FITKInterfaceMesh FITKMeshFeatureVTK）
 *        使用标准 VTK 过滤器：vtkDataSetSurfaceFilter、vtkFeatureEdges、vtkPolyDataNormals
 */
#ifndef FCMESHFEATUREVTK_H
#define FCMESHFEATUREVTK_H

#include "FCMeshInterfaceAPI.h"
#include <QObject>

class vtkDataSet;
class vtkPolyData;
class vtkDataArray;
class vtkAlgorithmOutput;
class vtkIdFilter;
class vtkPassThrough;
class vtkDataSetSurfaceFilter;
class vtkFeatureEdges;
class vtkPolyDataNormals;

namespace FC {

/**
 * @brief 网格特征 VTK 管线（表面、法向、特征边提取）
 */
class FCMESHINTERFACE_API FCMeshFeatureVTK
{
public:
    static const char* GetPointIdArrayName();
    static const char* GetCellIdArrayName();
    static const char* GetFeatEdgeOriginalIdArrayName();

    explicit FCMeshFeatureVTK(vtkDataSet* dataSet);
    explicit FCMeshFeatureVTK(vtkAlgorithmOutput* connection);
    virtual ~FCMeshFeatureVTK();

    virtual void update();
    virtual void reset(vtkDataSet* dataSet);
    virtual void reset(vtkAlgorithmOutput* connection);

    void setCellAndPointIdsEnable(bool pointIdsOn, bool cellIdsOn);
    void setSurfEdgesEnable(bool isOn);

    vtkPassThrough* getMeshPassThrough();
    vtkIdFilter* getIdFilter();
    vtkDataSetSurfaceFilter* getSurfaceFilter();
    vtkFeatureEdges* getFeatureEdgeFilter();
    vtkFeatureEdges* getShellEdgeFilter();
    vtkPolyDataNormals* getNormalsFilter();

    /** type: 1-表面 2-法向 3-特征边 4-表面网格边 */
    vtkPolyData* getFeatureMesh(int type);
    vtkDataArray* getCellNormals();

private:
    void initialize();

    vtkDataSet* _dataSet{};
    vtkAlgorithmOutput* _connection{};
    vtkPassThrough* _passThrough{};
    vtkIdFilter* _idFilter{};
    vtkDataSetSurfaceFilter* _surfaceFilter{};
    vtkPolyDataNormals* _normalFilter{};
    vtkFeatureEdges* _featureEdgeFilter{};
    vtkFeatureEdges* _edgeFilter{};
};

} // namespace FC

#endif // FCMESHFEATUREVTK_H
