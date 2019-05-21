#pragma once

#include <QMainWindow>
#include <stack>
#include <QMenu>
#include <QTreeWidgetItem>
#include "../../Core/GraphicsScene.h"
#include "../../Core/Graphics2DView.h"
#include "../../Core/GraphicsView.h"
#include "../../Core/Graphics3DView.h"
#include "../../Tools/ViewControllerTool.h"
#include "../../Shading/ShadingModel.h"
#include "../../Shading/LightSource.h"
#include "../../Tools/Select2DItemTool.h"
#include "../../Tools/CreatePolyline2DItemTool.h"
#include "../../Tools/EditPolyline2DItemTool.h"
#include "../../Tools/EditPointSet2DItemTool.h"
#include "../../Tools/CreatePointSet2DItemTool.h"
#include "../../Tools/CreateRectangle2DItemTool.h"
#include "../../Utility/ReaderOFF.h"
#include "../../Items/TriangleMesh3DItem.h"
#include "../../Items/TriangleMesh2DItem.h"
#include "../../Items/QuadMesh3DItem.h"
#include "../../Items/QuadMesh2DItem.h"
#include "../../Items/PointSet2DItem.h"
#include "../../Items/Polyline2DItem.h"
#include "../../Items/Rectangle2DItem.h"
#include <QTreeWidget>
#include <QStandardItem>

namespace Ui
{
    class MainWindow;
}

namespace rm
{
    class GraphicsScene;
    class GraphicsTool;    
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
     * @brief initializeItemsTree - Initialize the ItemTree.
     */
    void initializeItemsTree();

    /**
     * @brief addTreeItem - Adds a treeItem to the ItemTree.
     * @param name - Name of the item.
     * @param treeItem - The item to be added.
     * @param count - Boolean to verify if the item will have a counter on the end of its name.
     * @return
     */
    QStandardItem* addTreeItem(QString name, QStandardItem* treeItem);

    /**
     * @brief expandTreeItem - Expand the item and all its parents and children in the ItemTree.
     * @param itemModel - The item that must be expanded in the ItemTree.
     * @return
     */
    void expandTreeItem(QStandardItem* itemModel);

    /**
     * @brief expandAllTreeItems - Expand all the item stored as a row in the ItemTree.
     * @return
     */
    void expandAllTreeItems();

    /**
     * @brief setTreeItemVisible - Set the item and all its children as visible or not.
     * Also updates the interface checkbox state. Recursive function.
     * @param item - The item thay should become visiboe or invisible
     * @param state - The new item state
     */
    void setTreeItemVisible(QStandardItem* item, Qt::CheckState state);

    /**
     * @brief eventFilter - Filter all events incomming to objects in main window
     * @param obj - Object that received the event
     * @param event - The events received
     * @return - True if the event was handled or false if not.
     */
    bool eventFilter(QObject *obj, QEvent *event );

    /**
     * @brief getTreeItemDepth  - Given a tree item, returns its depth
     * @param item - A tree item
     * @return - The depth of the given item
     */
    unsigned int  getTreeItemDepth(QStandardItem *item) const;

    /**
     * @brief addTreeItemAndModel - Adds a item model and its graphic item into de tree model
     * @param name - Name of new item
     * @param itemModel - Item tree model
     * @param gItem - Graphics item to be added on scene
     */
    void addTreeItemAndModel(QString name, QStandardItem* itemModel, rm::GraphicsItem* gItem, bool count = true);

private slots:

    /**
     * @brief on_actionZoomIn_triggered - Activate zoomInOut function from the view in focus.
     */
    void on_actionZoomIn_triggered();

    /**
     * @brief on_actionZoomOut_triggered - Activate zoomInOut function from the view in focus.
     */
    void on_actionZoomOut_triggered();

    /**
     * @brief on_actionDefaut_triggered - Emplace the ViewController tool on top of the stack.
     * @param checked - Boolean that says if the button was actived or disactivated.
     */
    void on_actionDefault_triggered(bool checked);

    /**
     * @brief on_actionSelectionTool_triggered - Emplace the SelectionTool on top of the stack.
     * @param checked - Boolean that says if the button was actived or disactivated.
     */
    void on_actionSelectionTool_triggered(bool checked);

    /**
     * @brief on_actionView3D_triggered - Make the actionView3D pressed or dispressed.
     * @param checked - Boolean that says if the button was actived or disactivated.
     */
    void on_actionView3D_triggered(bool checked);

    /**
     * @brief pushTool - Emplace the tool on top of the stack
     * @param tool - The tool that is going to be emplaced
     * @param action - The action linked to that tool
     */
    void pushTool(rm::GraphicsTool* tool, QAction* action);

    /**
     * @brief popTool - This function pop the tool stack.
     */
    void popTool();

    /**
     * @brief popAndDeleteTool - This function pop the tool stack and delete the popped object.
     */
    void popAndDeleteTool();

    /**
     * @brief on_actionFit_triggered - Activate fit function from the view in focus.
     */
    void on_actionFit_triggered();

    /**
     * @brief on_actionTriangular2DMesh_triggered - Activate the function from the view in focus which reads and creates a Tringular2DMesh.
     */
    void on_actionTriangular2DMesh_triggered();

    /**
     * @brief on_actionTriangular3DMesh_triggered - Activate the function from the view in focus which reads and creates a Tringular3DMesh.
     */
    void on_actionTriangular3DMesh_triggered();

    /**
     * @brief on_actionQuad2DMesh_triggered - Activate the function from the view in focus which reads and creates a Quad2DMesh.
     */
    void on_actionQuad2DMesh_triggered();

    /**
     * @brief on_actionQuad3DMesh_triggered - Activate the function from the view in focus which reads and creates a Quad3DMesh.
     */
    void on_actionQuad3DMesh_triggered();

    /**
     * @brief on_actionPlaneNegativeX_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlaneNegativeX_triggered();

    /**
     * @brief on_actionPlanePositiveX_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlanePositiveX_triggered();

    /**
     * @brief on_actionPlanePositiveY_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlanePositiveY_triggered();

    /**
     * @brief on_actionPlaneNegativeY_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlaneNegativeY_triggered();

    /**
     * @brief on_actionPlanePositiveZ_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlanePositiveZ_triggered();

    /**
     * @brief on_actionPlaneNegativeZ_triggered - Activate viewFrom function from the view in focus.
     */
    void on_actionPlaneNegativeZ_triggered();

    /**
     * @brief createPointSet - Emplace the tool to create PointSet on stack and updates the TreeWidget and the itemList.
     */
    void createPointSet();

    /**
     * @brief editPointSet - Emplace the tool to edit PointSet on stack and updates the itemList.
     */
    void editPointSet();

    /**
     * @brief createPolyline - Emplace the tool to create Polyline on stack and updates the TreeWidget and the itemList.
     */
    void createPolyline();

    /**
     * @brief editPolyline - Emplace the tool to edit Polyline on stack and updates the itemList.
     */
    void editPolyline();

    /**
     * @brief createRectangle - Emplace the tool to create Rectangle on stack and updates the TreeWidget and the itemList.
     */
    void createRectangle();

    /**
     * @brief createEllipse - Emplace the tool to create Ellipse on stack and updates the TreeWidget and the itemList.
     */
    void createEllipse();

    /**
     * @brief editEllipse - Emplace the tool to edit Ellipse on stack and updates the itemList.
     */
    void editEllipse();

    /**
     * @brief editItem - Edits the corresponding item to the selected tree item.
     */
    void editItem();

    /**
     * @brief removeItem - Remove item from the scene and updates de TreeWidget.
     */
    void removeItem();

    /**
     * @brief showContextMenu - Show a menu of screen with different option depending on the selected item.
     * @param pos - Position of the mouse on the screen.
     */
    void showContextMenu(const QPoint &pos);

    /**
     * @brief renameTree - edit the name of a branch of the TreeWidget.
     */
    void renameTree();

    /**
     * @brief renameItem - Edit the name of an item.
     */
    void renameItem();

    /**
     * @brief modelItemChanged - Called when a tree item changes
     * @param item - The item that changed.
     */
    void modelItemChanged(QStandardItem* item);

private:
    Ui::MainWindow* _ui;

    rm::GraphicsScene* _scene;

    std::stack<QAction*>* _toolType;

    QStandardItem* _tree2D;

    QStandardItem* _tree3D;

    QStandardItem* _pointSet2DTree;

    QStandardItem* _polyline2DTree;

    QStandardItem* _quadMesh2DTree;

    QStandardItem* _triangleMesh2DTree;

    QStandardItem* _quadMesh3DTree;

    QStandardItem* _triangleMesh3DTree;

    QStandardItem* _rectangle2DTree;

    QStandardItem* _ellipse2DTree;

    rm::Graphics2DView* _view2D;

    unsigned int _itemsCounter {0};

    QAction* _createPolylineAction;
    QAction* _editPolylineAction;
    QAction* _createPointSetAction;
    QAction* _editPointSetAction;
    QAction* _removeAction;
    QAction* _renameAction;
    QAction* _createRectangleAction;
    QAction* _createEllipseAction;
    QAction* _editEllipseAction;

    QMenu _contextMenuCreatePolyline;

    QMenu _contextMenuCreatePointSet;

    QMenu _contextMenuEditPolyline;

    QMenu _contextMenuEditPointSet;

    QMenu _contextMenuRemoveMesh;

    QMenu _contextMenuCreateEllipse;

    QMenu _contextMenuEditEllipse;

    QMenu _contextMenuCreateRectangle;

    QMenu _contextMenuEditRectangle;

    QStandardItemModel* _model;
};
