#-------------------------------------------------
#
# Project created by QtCreator 2019-05-21T16:16:36
#
#-------------------------------------------------

QT       += widgets opengl svg

TARGET = Lib_Teste
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Core/CoreItems/AABB2DItem.cpp \
        Core/Graphics2DItem.cpp \
        Core/Graphics2DView.cpp \
        Core/Graphics3DItem.cpp \
        Core/Graphics3DView.cpp \
        Core/GraphicsItem.cpp \
        Core/GraphicsScene.cpp \
        Core/GraphicsSceneEvent.cpp \
        Core/GraphicsTool.cpp \
        Core/GraphicsView.cpp \
        Events/GraphicsSceneHoverEvent.cpp \
        Events/GraphicsSceneKeyEvent.cpp \
        Events/GraphicsSceneMoveEvent.cpp \
        Events/GraphicsScenePressEvent.cpp \
        Events/GraphicsSceneWheelEvent.cpp \
        Geometry/OpenGLMatrix.cpp \
        Items/Group2DItem.cpp \
        Items/Group3DItem.cpp \
        Items/PointSet2DItem.cpp \
        Items/Polyline2DItem.cpp \
        Items/QuadMesh2DItem.cpp \
        Items/QuadMesh3DItem.cpp \
        Items/Rectangle2DItem.cpp \
        Items/RectangleZoomItem.cpp \
        Items/SelectionGroup2DItem.cpp \
        Items/SelectionGroup3DItem.cpp \
        Items/TriangleMesh2DItem.cpp \
        Items/TriangleMesh3DItem.cpp \
        Shading/LightSource.cpp \
        Shading/LightSourceRepresentation.cpp \
        Shading/ShadingModel.cpp \
        Shading/ShadingModelRepresentation.cpp \
        Tools/CreatePointSet2DItemTool.cpp \
        Tools/CreatePolyline2DItemTool.cpp \
        Tools/CreateRectangle2DItemTool.cpp \
        Tools/EditPointSet2DItemTool.cpp \
        Tools/EditPolyline2DItemTool.cpp \
        Tools/Select2DItemTool.cpp \
        Tools/ViewControllerTool.cpp \
        Utility/ReaderOFF.cpp \
        Utility/WireframeTextureBuilder.cpp \
        lib_teste.cpp

HEADERS += \
        Core/CoreItems/AABB2DItem.h \
        Core/Graphics2DItem.h \
        Core/Graphics2DView.h \
        Core/Graphics3DItem.h \
        Core/Graphics3DView.h \
        Core/GraphicsItem.h \
        Core/GraphicsScene.h \
        Core/GraphicsSceneEvent.h \
        Core/GraphicsTool.h \
        Core/GraphicsView.h \
        Events/EventConstants.h \
        Events/GraphicsSceneHoverEvent.h \
        Events/GraphicsSceneKeyEvent.h \
        Events/GraphicsSceneMoveEvent.h \
        Events/GraphicsScenePressEvent.h \
        Events/GraphicsSceneWheelEvent.h \
        Geometry/AxisAligmentBoundingBox.h \
        Geometry/Geometry2DAlgorithms.h \
        Geometry/OpenGLMatrix.h \
        Geometry/Vector2D.h \
        Items/Group2DItem.h \
        Items/Group3DItem.h \
        Items/PointSet2DItem.h \
        Items/Polyline2DItem.h \
        Items/QuadMesh2DItem.h \
        Items/QuadMesh3DItem.h \
        Items/Rectangle2DItem.h \
        Items/RectangleZoomItem.h \
        Items/SelectionGroup2DItem.h \
        Items/SelectionGroup3DItem.h \
        Items/TriangleMesh2DItem.h \
        Items/TriangleMesh3DItem.h \
        Models/GraphicsItemModel.h \
        Shading/LightSource.h \
        Shading/LightSourceRepresentation.h \
        Shading/LightType.h \
        Shading/ShadingModel.h \
        Shading/ShadingModelRepresentation.h \
        Tools/CreatePointSet2DItemTool.h \
        Tools/CreatePolyline2DItemTool.h \
        Tools/CreateRectangle2DItemTool.h \
        Tools/EditPointSet2DItemTool.h \
        Tools/EditPolyline2DItemTool.h \
        Tools/Select2DItemTool.h \
        Tools/ViewControllerTool.h \
        Utility/ReaderOFF.h \
        Utility/Tri3ToTri6Conversor.h \
        Utility/WireframeTextureBuilder.h \
        lib_teste.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
