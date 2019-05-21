#include "MainWindow.h"
#include "Vector2D.h"
#include "GraphicsItemModel.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardItemModel>
#include <QPushButton>
#include <QMouseEvent>

Q_DECLARE_METATYPE(rm::GraphicsItemModel)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _toolType(new std::stack<QAction*>)
{
    _ui->setupUi(this);

    using namespace rm;

    //Creates a scene.
    _scene = new GraphicsScene();
    _scene->enableMouseEvents();
    _scene->enableEvent(EventType::keyEvent);

    //Creates two GraphicsView2D
    _view2D = _scene->create2DView(this);
    _view2D->preview(false); //define it as preview
    _view2D->installEventFilter(this);


    Graphics3DView* view3D = _scene->create3DView(this);
    view3D->preview(false);
    view3D->lookAt(QVector3D(0.05f, 0.1f, 0.15f), QVector3D(0, 0.1f, 0), QVector3D(0, 1, 0));

    //Create shading model
    ShadingModel shadeModel = _scene->createShadingModel();
    LightSource light = shadeModel.getLightSource(0);
    QColor redColor(255, 0, 0);
    QColor greenColor(0, 255, 0);
    light.setAmbientComponent(redColor);
    light.setDiffuseComponet(greenColor);
    view3D->setShadingModel(shadeModel);
    view3D->installEventFilter(this);

    //Initialize the scene tool stack with default view controler
    pushTool(new ViewControllerTool(_scene), _ui->actionDefault);

    //add views into layout defined in QtDesign
    _ui->main2DHorizontalLayout->addWidget(_view2D);
    _ui->main3DHorizontalLayout->addWidget(view3D);

    initializeItemsTree();

    //create menu callbacks
    _removeAction = new  QAction("Remove", this);
    connect(_removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));

    _renameAction = new QAction ("Rename",this);
    connect(_renameAction, SIGNAL(triggered()), this, SLOT(renameTree()));

    _createPolylineAction = new  QAction("Create", this);
    connect(_createPolylineAction, SIGNAL(triggered()), this, SLOT(createPolyline()));
    _contextMenuCreatePolyline.addAction(_createPolylineAction);

    _createPointSetAction = new  QAction("Create", this);
    connect(_createPointSetAction, SIGNAL(triggered()), this, SLOT(createPointSet()));
    _contextMenuCreatePointSet.addAction(_createPointSetAction);

    _createRectangleAction = new  QAction("Create", this);
    connect(_createRectangleAction, SIGNAL(triggered()), this, SLOT(createRectangle()));
    _contextMenuCreateRectangle.addAction(_createRectangleAction);

    _createEllipseAction = new QAction("Create", this);
    connect(_createEllipseAction, SIGNAL(triggered()), this, SLOT(createEllipse()));
    _contextMenuCreateEllipse.addAction(_createEllipseAction);

    _editPolylineAction = new  QAction("Edit", this);
    connect(_editPolylineAction, SIGNAL(triggered()), this, SLOT(editPolyline()));
    _contextMenuEditPolyline.addAction(_editPolylineAction);
    _contextMenuEditPolyline.addAction(_removeAction);
    _contextMenuEditPolyline.addAction(_renameAction);

    _editPointSetAction = new  QAction("Edit", this);
    connect(_editPointSetAction, SIGNAL(triggered()), this, SLOT(editPointSet()));
    _contextMenuEditPointSet.addAction(_editPointSetAction);
    _contextMenuEditPointSet.addAction(_removeAction);
    _contextMenuEditPointSet.addAction(_renameAction);
    _contextMenuRemoveMesh.addAction(_removeAction);

    _editEllipseAction = new  QAction("Edit", this);
    connect(_editEllipseAction, SIGNAL(triggered()), this, SLOT(editPolyline()));
    _contextMenuEditEllipse.addAction(_editEllipseAction);
    _contextMenuEditEllipse.addAction(_removeAction);
    _contextMenuEditEllipse.addAction(_renameAction);

    _contextMenuEditRectangle.addAction(_removeAction);
    _contextMenuEditRectangle.addAction(_renameAction);

    //connect(_ui->sceneTreeView,SIGNAL(itemChanged(QStandardItem*)), this, SLOT(renameItem()));
}



MainWindow::~MainWindow()
{
    delete _toolType;
    delete _scene;
    delete _ui;
    delete _model;
}



void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = _ui->sceneTreeView->currentIndex();
    QStandardItem* item = _model->itemFromIndex(index);
    if(item->text() == "Polyline")
    {
        _contextMenuCreatePolyline.exec(_ui->sceneTreeView->mapToGlobal(pos));
    }
    else if (item->text() == "Point Set")
    {
        _contextMenuCreatePointSet.exec(_ui->sceneTreeView->mapToGlobal(pos));
    }
    else if(item->text() == "Rectangle")
    {
        _contextMenuCreateRectangle.exec(_ui->sceneTreeView->mapToGlobal(pos));
    }
    else
    {
        if(index.isValid() && getTreeItemDepth(item) > 2)
        {
            QVariant var = item->data();
            if (!var.isValid())
            {
                std::cout << "Invalid QVariant! " << std::endl;
                return;
            }
            rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
            if(dynamic_cast<rm::PointSet2DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuEditPointSet.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::Polyline2DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuEditPolyline.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::TriangleMesh2DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuRemoveMesh.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::TriangleMesh3DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuRemoveMesh.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::QuadMesh2DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuRemoveMesh.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::QuadMesh3DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuRemoveMesh.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
            else if (dynamic_cast<rm::Rectangle2DItem*>(gItemModel.getItem()) != nullptr)
            {
                _contextMenuEditRectangle.exec(_ui->sceneTreeView->mapToGlobal(pos));
            }
        }
   }
}



void MainWindow::renameTree()
{
    _ui->sceneTreeView->edit(_ui->sceneTreeView->currentIndex());
}



void MainWindow::renameItem()
{
    QModelIndex index = _ui->sceneTreeView->currentIndex();
    QStandardItem* treeItem = _model->itemFromIndex(index);
    QVariant var = treeItem->data();
    if (var.isValid())
    {
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        std::string newName = treeItem->text().toStdString();
        gItemModel.getItem()->setName(newName);
    }

}



void MainWindow::createPointSet()
{

    if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
    {
        // To Do usar apenas pop
        popAndDeleteTool();
    }
    rm::CreatePointSet2DItemTool* tool = new rm::CreatePointSet2DItemTool(_view2D);
    tool->initialize();
    pushTool(tool, _createPointSetAction);
    statusBar()->showMessage("Press left mouse button to create a point set.");
    this->setCursor(Qt::ArrowCursor);
    addTreeItemAndModel("PointSet", _pointSet2DTree, tool->getPointSet2DItem(), true);

}



void MainWindow::editPointSet()
{
    QModelIndex index = _ui->sceneTreeView->currentIndex();
    QVariant var = _model->itemFromIndex(index)->data();
    if (var.isValid())
    {
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        rm::GraphicsItem *item = gItemModel.getItem();
        rm::PointSet2DItem* pointset = dynamic_cast<rm::PointSet2DItem*>(item);
        if (pointset != nullptr)
        {
            this->setCursor(Qt::PointingHandCursor);
            rm::EditPointSet2DItemTool * tool = new rm::EditPointSet2DItemTool(_view2D, pointset);
            pushTool(tool, _editPointSetAction);
            statusBar()->showMessage("Press left mouse button to edit pointset points or right mouse button to remove then.");

        }
   }
}



void MainWindow::createPolyline()
{
    if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
    {
        // To Do usar apenas pop
        popAndDeleteTool();
    }

    rm::CreatePolyline2DItemTool* tool = new rm::CreatePolyline2DItemTool(_view2D);
    tool->initialize();
    pushTool(tool,_createPolylineAction);
    statusBar()->showMessage("Press left mouse button to create polyline points.WARNING :Points cannot be added in edition");
    this->setCursor(Qt::ArrowCursor);
    addTreeItemAndModel("Polyline", _polyline2DTree, tool->getPolylineItem(), true);
}



void MainWindow::editPolyline()
{
    QModelIndex index = _ui->sceneTreeView->currentIndex();
    QVariant var = _model->itemFromIndex(index)->data();
    if (var.isValid())
    {
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        rm::GraphicsItem *item = gItemModel.getItem();
        rm::Polyline2DItem* polyline2D = dynamic_cast<rm::Polyline2DItem*>(item);
        if (polyline2D != nullptr)
        {
            this->setCursor(Qt::PointingHandCursor);
            rm::EditPolyline2DItemTool* tool = new rm::EditPolyline2DItemTool(_view2D, polyline2D);
            pushTool(tool, _editPolylineAction);
            statusBar()->showMessage("Press left mouse button to edit polyline points or right mouse button to remove then.");
       }
    }
}



void MainWindow::createRectangle()
{

    if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
    {
        // To Do just use pop
        popAndDeleteTool();
    }

    // it´s only necessary to change the creationtool and update the edit item function.
    rm::CreateRectangle2DItemTool* tool = new rm::CreateRectangle2DItemTool(_view2D);
    tool->initialize();
    pushTool(tool, _createRectangleAction);
    statusBar()->showMessage("Press left mouse button to create a Rectangle.");
    this->setCursor(Qt::ArrowCursor);
    addTreeItemAndModel("Rectangle", _rectangle2DTree, tool->getRectangle2DItem(),  true);
}



void MainWindow::createEllipse()
{
    if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
    {
        // TODO: use only pop
        popAndDeleteTool();
    }

    rm::CreatePolyline2DItemTool* tool = new rm::CreatePolyline2DItemTool(_view2D);
    tool->initialize();
    pushTool(tool,_createEllipseAction);
    statusBar()->showMessage("Press left mouse button to create an ellipse");
    this->setCursor(Qt::ArrowCursor);
    addTreeItemAndModel("Ellipse", _ellipse2DTree , tool->getPolylineItem(),true );
}



void MainWindow::editEllipse()
{
    QModelIndex index = _ui->sceneTreeView->currentIndex();
    QVariant var = _model->itemFromIndex(index)->data();
    if (var.isValid())
    {
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        rm::GraphicsItem *item = gItemModel.getItem();
        rm::Polyline2DItem* polyline2D = dynamic_cast<rm::Polyline2DItem*>(item);
        if (polyline2D != nullptr)
        {
            this->setCursor(Qt::PointingHandCursor);
            rm::EditPolyline2DItemTool* tool = new rm::EditPolyline2DItemTool(_view2D, polyline2D);
            pushTool(tool, _editPolylineAction);
            statusBar()->showMessage("Press left mouse button to edit the ellipse.");
       }
    }
}



void MainWindow::editItem()
{

    QModelIndex indexButton = _ui->sceneTreeView->currentIndex();
    QModelIndex index = indexButton.siblingAtColumn(0);
    QVariant var = _model->itemFromIndex(index)->data();

    if (var.isValid())
     {
         if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
         {
             // TODO usar apenas pop
             popAndDeleteTool();
         }

         //turns QVariant into a GraphicsItemModel
         rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
         //recover the item pointed by our model
         rm::GraphicsItem *item = gItemModel.getItem();

         if(dynamic_cast<rm::Polyline2DItem*>(item)!= nullptr)
         {

            rm::Polyline2DItem* polyline2D = dynamic_cast<rm::Polyline2DItem*>(item);

             //TO DO : when the function edit polyline revisit to evaluate possible replicated code.
             this->setCursor(Qt::PointingHandCursor);
             rm::EditPolyline2DItemTool* tool = new rm::EditPolyline2DItemTool(_view2D, polyline2D);
             pushTool(tool, _editPolylineAction);
             statusBar()->showMessage("Press left mouse button to edit polyline points or right mouse button to remove then. WARNING :Points cannot be added in edition");
         }
         else if( dynamic_cast<rm::PointSet2DItem*>(item)!= nullptr)
         {
            rm::PointSet2DItem* pointSet2D = dynamic_cast<rm::PointSet2DItem*>(item);
            //TO DO : when the function edit point set revisit to evaluate possible replicated code.
            this->setCursor(Qt::PointingHandCursor);
            rm::EditPointSet2DItemTool * tool = new rm::EditPointSet2DItemTool(_view2D, pointSet2D);
            pushTool(tool, _editPointSetAction);
            statusBar()->showMessage("Press left mouse button to edit pointset points or right mouse button to remove then.");
         }
         else
         {
           statusBar()->showMessage("This item is not editable");
         }
     }
}



void  MainWindow::removeItem()
{
    //The tree item is set as the current item when the button is clicked.
    //Use it to retreive the button's model index
    QModelIndex indexButton = _ui->sceneTreeView->currentIndex();
    //The tree model item is always stored at the first position,
    //so we retreive the index of delete button sibling at 0,0
    QModelIndex index = indexButton.siblingAtColumn(0);

    //Now that we have the item index we retreive its data from the model.
    //index.data does not work. Don't know why...
    QVariant var = _model->itemFromIndex(index)->data();
    if (var.isValid())
    {
        //turns QVariant into a GraphicsItemModel
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        //recover the item pointed by our model
        rm::GraphicsItem *item = gItemModel.getItem();

        if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
        {

            if(dynamic_cast<rm::Polyline2DItem*>(item) != nullptr)
            {
                rm::Polyline2DItem* polyline = dynamic_cast<rm::Polyline2DItem*>(item);
                 if( dynamic_cast<rm::CreatePolyline2DItemTool*>(_scene->topTool()) != nullptr)
                 {
                   rm::CreatePolyline2DItemTool* polylineTool = dynamic_cast<rm::CreatePolyline2DItemTool*>(_scene->topTool());

                   if(polyline == polylineTool->getPolylineItem())
                   {
                       popAndDeleteTool();
                   }
                 }
                 if( dynamic_cast<rm::EditPolyline2DItemTool*>(_scene->topTool())!=nullptr)
                 {
                    rm::EditPolyline2DItemTool* editPolyline = dynamic_cast<rm::EditPolyline2DItemTool*>(_scene->topTool());
                    if(polyline == editPolyline->getPolyline2DItem())
                    {
                        popAndDeleteTool();
                    }

                }
            }
            if(dynamic_cast<rm::PointSet2DItem*>(item) != nullptr)
            {
                 rm::PointSet2DItem* pointSet = dynamic_cast<rm::PointSet2DItem*>(item);
                if( dynamic_cast<rm::CreatePointSet2DItemTool*>(_scene->topTool()) != nullptr )
                {
                   rm::CreatePointSet2DItemTool* pointSetTool = dynamic_cast<rm::CreatePointSet2DItemTool*>(_scene->topTool());

                   if(pointSet == pointSetTool->getPointSet2DItem())
                   {

                       popAndDeleteTool();

                   }
                }
                if(dynamic_cast<rm::EditPointSet2DItemTool*>(_scene->topTool()) != nullptr)
                {
                    rm::EditPointSet2DItemTool* editPointSet =dynamic_cast<rm::EditPointSet2DItemTool*>(_scene->topTool());
                    if(pointSet == editPointSet->getPointSet2DItem())
                    {

                        popAndDeleteTool();

                    }
                }
            }
            if(dynamic_cast<rm::Rectangle2DItem*>(item) != nullptr)
            {
                rm::Rectangle2DItem* rectangle = dynamic_cast<rm::Rectangle2DItem*>(item);
                if( dynamic_cast<rm::CreateRectangle2DItemTool*>(_scene->topTool()) != nullptr )
                {
                   rm::CreateRectangle2DItemTool* rectangleTool = dynamic_cast<rm::CreateRectangle2DItemTool*>(_scene->topTool());

                   if(rectangle == rectangleTool->getRectangle2DItem())
                   {

                       popAndDeleteTool();

                   }
                }
            }
        }

            //remove it from the scene
            _scene->removeItem(item);
            //remove it from the model.
            _model->removeRow(index.row(), index.parent());

        }

}



void MainWindow::initializeItemsTree()
{
    using namespace rm;

    _model = new QStandardItemModel( 2, 3 );

    _tree2D = new QStandardItem(QString("2D Objects") );
    _tree2D->setCheckable(true);
    _tree2D->setCheckState(Qt::Checked);
    _tree2D->setFlags(_tree2D->flags() & ~Qt::ItemIsEditable);

    _pointSet2DTree = new QStandardItem( QString("Point Set ") );
    _pointSet2DTree->setCheckable(true);
    _pointSet2DTree->setCheckState(Qt::Checked);
    _pointSet2DTree->setFlags(_pointSet2DTree->flags() & ~Qt::ItemIsEditable);
    QStandardItem* AddPointSetButton = new QStandardItem();

    _polyline2DTree = new QStandardItem( QString("Polyline") );
    _polyline2DTree->setCheckable(true);
    _polyline2DTree->setCheckState(Qt::Checked);
    _polyline2DTree->setFlags(_polyline2DTree->flags() & ~Qt::ItemIsEditable);
    QStandardItem* AddPolylineButton = new QStandardItem();

    _rectangle2DTree = new QStandardItem( QString("Rectangle") );
    _rectangle2DTree->setCheckable(true);
    _rectangle2DTree->setCheckState(Qt::Checked);
    _rectangle2DTree->setFlags(_rectangle2DTree->flags() & ~Qt::ItemIsEditable);
    QStandardItem* AddRectangleButton = new QStandardItem();

    _ellipse2DTree = new QStandardItem( QString("Ellipse") );
    _ellipse2DTree->setCheckable(true);
    _ellipse2DTree->setCheckState(Qt::Checked);
    _ellipse2DTree->setFlags(_ellipse2DTree->flags() & ~Qt::ItemIsEditable);
    QStandardItem* AddEllipseButton = new QStandardItem();

    _quadMesh2DTree = new QStandardItem( QString("Quad Mesh") );
    _quadMesh2DTree->setCheckable(true);
    _quadMesh2DTree->setCheckState(Qt::Checked);
    _quadMesh2DTree->setFlags(_quadMesh2DTree->flags() & ~Qt::ItemIsEditable);

    _triangleMesh2DTree = new QStandardItem( QString("Triangle Mesh") );
    _triangleMesh2DTree->setCheckable(true);
    _triangleMesh2DTree->setCheckState(Qt::Checked);
    _triangleMesh2DTree->setFlags(_triangleMesh2DTree->flags() & ~Qt::ItemIsEditable);

    QList<QStandardItem *> pointSet({_pointSet2DTree, AddPointSetButton});
    _tree2D->appendRow(pointSet);

    QList<QStandardItem *> polyline ({_polyline2DTree, AddPolylineButton});
    _tree2D->appendRow(polyline);

    QList<QStandardItem *> rectangle({_rectangle2DTree,AddRectangleButton});
    _tree2D->appendRow(rectangle);

    QList<QStandardItem *> ellipse ({_ellipse2DTree, AddEllipseButton});
    _tree2D->appendRow(ellipse);

    _tree2D->appendRow(_quadMesh2DTree);
    _tree2D->appendRow(_triangleMesh2DTree);

    _tree3D = new QStandardItem(QString("3D Objects") );
    _tree3D->setCheckable(true);
    _tree3D->setCheckState(Qt::Checked);
    _tree3D->setFlags(_tree3D->flags() & ~Qt::ItemIsEditable);

    _quadMesh3DTree = new QStandardItem( QString("Quad Mesh") );
    _quadMesh3DTree->setCheckable(true);
    _quadMesh3DTree->setCheckState(Qt::Checked);
    _quadMesh3DTree->setFlags(_quadMesh3DTree->flags() & ~Qt::ItemIsEditable);

    _triangleMesh3DTree = new QStandardItem( QString("Triangle Mesh") );
    _triangleMesh3DTree->setCheckable(true);
    _triangleMesh3DTree->setCheckState(Qt::Checked);
    _triangleMesh3DTree->setFlags(_triangleMesh3DTree->flags() & ~Qt::ItemIsEditable);

    _tree3D->appendRow(_quadMesh3DTree);
    _tree3D->appendRow(_triangleMesh3DTree);

    _model->setItem(0, 0, _tree2D);
    _model->setItem(1, 0, _tree3D);

    _model->setHorizontalHeaderItem( 0, new QStandardItem( "Scene" ) );
    _model->setHorizontalHeaderItem( 1, new QStandardItem( "" ) );
    _model->setHorizontalHeaderItem( 2, new QStandardItem( "" ) );
    _ui->sceneTreeView->setModel( _model );

    //Tree View properties ajustments
    _ui->sceneTreeView->header()->setStretchLastSection(false); //prevents last column to take too much space
    _ui->sceneTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch); //tells first column to take all space available.
    //fits other columns to content
    _ui->sceneTreeView->resizeColumnToContents(1);
    _ui->sceneTreeView->resizeColumnToContents(2);
    _ui->sceneTreeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    _ui->sceneTreeView->show();

    //Changes in model shall trigger a slot
    connect(_model, SIGNAL(itemChanged(QStandardItem*)),this, SLOT(modelItemChanged(QStandardItem*)));

    connect(_ui->sceneTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
    
    //add Polyline button
    QPushButton* buttonAddPolyline = new QPushButton(QIcon(":/img/AddIcon.png"), "");
    buttonAddPolyline->setMaximumSize(buttonAddPolyline->sizeHint());
    buttonAddPolyline->setObjectName("buttonAddPolyline");
    _ui->sceneTreeView->setIndexWidget(AddPolylineButton->index(), buttonAddPolyline);
    connect(buttonAddPolyline, SIGNAL(clicked()), this, SLOT(createPolyline()));

    //add pointset button
    QPushButton* buttonAddPointSet = new QPushButton(QIcon(":/img/AddIcon.png"), "");
    buttonAddPointSet->setMaximumSize(buttonAddPointSet->sizeHint());
    buttonAddPointSet->setObjectName("buttonAddPointSet");
    _ui->sceneTreeView->setIndexWidget(AddPointSetButton->index(), buttonAddPointSet);
    connect(buttonAddPointSet, SIGNAL(clicked()), this, SLOT(createPointSet()));

    //add Rectangle button
    QPushButton* buttonAddRectangle = new QPushButton(QIcon(":/img/AddIcon.png"), "");
    buttonAddRectangle->setMaximumSize(buttonAddRectangle->sizeHint());
    buttonAddRectangle->setObjectName("buttonAddRectangle");
    _ui->sceneTreeView->setIndexWidget(AddRectangleButton->index(), buttonAddRectangle);
    connect(buttonAddRectangle, SIGNAL(clicked()), this, SLOT(createRectangle()));

    //add Ellipse button
    QPushButton* buttonAddEllipse = new QPushButton(QIcon(":/img/AddIcon.png"), "");
    buttonAddEllipse->setMaximumSize(buttonAddEllipse->sizeHint());
    buttonAddEllipse->setObjectName("buttonAddEllipse");
    _ui->sceneTreeView->setIndexWidget(AddEllipseButton->index(), buttonAddEllipse);
    connect(buttonAddEllipse, SIGNAL(clicked()), this, SLOT(createEllipse()));

    // Expands all the items in the initial item tree.
    expandAllTreeItems();

}



void MainWindow::expandAllTreeItems()
{
    for(int i = 0; i < _model->rowCount(); i++) {
        QStandardItem* rowItem = _model->item(i);
        if(!_ui->sceneTreeView->isExpanded(rowItem->index()))
        {
            _ui->sceneTreeView->expand(rowItem->index());
        }
    }
}






void MainWindow::expandTreeItem(QStandardItem* itemModel)
{
    // Expands the tree levels in order to show the added item in the expanded tree view.
    QStandardItem* item = itemModel;

    do
    {
        if(!_ui->sceneTreeView->isExpanded(item->index()))
        {
            _ui->sceneTreeView->expand(item->index());
        }

        item = item->parent();
    } while(item != nullptr);
}



QStandardItem* MainWindow::addTreeItem(QString name, QStandardItem* treeItem)
{
    QStandardItem* item;
    QStandardItem* itemButtonDelete;
    QStandardItem* itemButtonEdit;
    item = new QStandardItem(name);
    item->setCheckable(true);
    item->setCheckState(Qt::CheckState::Checked);

    itemButtonDelete = new QStandardItem();
    itemButtonEdit = new QStandardItem();

    QList<QStandardItem *> list({item, itemButtonDelete,itemButtonEdit});
    treeItem->appendRow(list);

    QPushButton* buttonTrash = new QPushButton(QIcon(":/img/trash.png"), "");
    buttonTrash->setMaximumSize(buttonTrash->sizeHint());
    buttonTrash->setObjectName(name);
    _ui->sceneTreeView->setIndexWidget(itemButtonDelete->index(), buttonTrash);

    connect(buttonTrash, SIGNAL(clicked()), this, SLOT(removeItem()));

    if(QString("Rectangle").append(" " + QString::number(_itemsCounter)).compare(name) != 0)
    {

        QPushButton* buttonEdit = new QPushButton(QIcon(":/img/EditIcon.png"), "");
        buttonEdit->setMaximumSize(buttonEdit->sizeHint());
        buttonEdit->setObjectName(name);
        _ui->sceneTreeView->setIndexWidget(itemButtonEdit->index(), buttonEdit);

        connect(buttonEdit, SIGNAL(clicked()), this, SLOT(editItem()));

    }
    _itemsCounter++;

    return item;
}



void MainWindow::setTreeItemVisible(QStandardItem *item, Qt::CheckState state)
{
    if (item != nullptr)
    {
        item->setCheckState(state);
        QVariant var = item->data();
        if (var.isValid())
        {
            rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
            gItemModel.getItem()->visible(state);
        }
        //do the same for all item childrens
        for (int i = 0; i < item->rowCount(); i++)
        {
            QStandardItem* child = item->child(i);
            setTreeItemVisible(child, state);
        }
    }
}



void MainWindow::on_actionZoomIn_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    Graphics2DView* view2d = dynamic_cast<Graphics2DView*>(widget);
    if (view2d != nullptr)
    {
        view2d->zoomInOut(0.95f);
        view2d->update();
        return;
    }
    else
    {
        Graphics3DView* view3d = dynamic_cast<Graphics3DView*>(widget);
        if (view3d != nullptr)
        {
            view3d->zoomInOut(0.05f, _scene->computeAABB3D());
            view3d->update();
            return;
        }
    }
}



void MainWindow::on_actionZoomOut_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    Graphics2DView* view2d = dynamic_cast<Graphics2DView*>(widget);
    if (view2d != nullptr)
    {
        view2d->zoomInOut(1.05f);
        view2d->update();
        return;
    }
    else
    {
        Graphics3DView* view3d = dynamic_cast<Graphics3DView*>(widget);
        if (view3d != nullptr)
        {
            view3d->zoomInOut(-0.05f, _scene->computeAABB3D());
            view3d->update();
            return;
        }
    }
}



void MainWindow::on_actionDefault_triggered(bool checked)
{
    using namespace rm;
    if (checked)
    {
        popAndDeleteTool();
        if( dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr )
        {
            pushTool(new ViewControllerTool(_scene), _ui->actionDefault);
        }
    }
    else
    {
        popAndDeleteTool();
    }
    this->setCursor(Qt::ArrowCursor);
}



void MainWindow::on_actionSelectionTool_triggered(bool checked)
{
    using namespace rm;

    if(dynamic_cast<rm::ViewControllerTool*>(_scene->topTool()) == nullptr)
    {
        // TODO usar apenas pop
        popAndDeleteTool();
    }

    if (checked)
    {
        pushTool(new Select2DItemTool(_scene), _ui->actionSelectionTool);
    }
    else
    {
        popAndDeleteTool();
    }
    this->setCursor(Qt::CrossCursor);
}



void MainWindow::pushTool(rm::GraphicsTool* tool, QAction* action)
{
    if (_toolType->size() != 0)
    {
        _toolType->top()->setChecked(false);
    }
    _scene->pushTool(tool);
    _toolType->push(action);
    _toolType->top()->setChecked(true);
    QString name(tool->name().c_str());
    _ui->listWidget->insertItem(_ui->listWidget->count(),name);

    //Giving background color to tool's feedback on listwidget
    _ui->listWidget->item(_ui->listWidget->count() - 1)->setBackgroundColor(QColor("LIGHT GREEN"));
    if(_ui->listWidget->count() > 1)
    {
       _ui->listWidget->item(_ui->listWidget->count() - 2)->setBackgroundColor(QColor("WHITE"));
    }

}



void MainWindow::popTool()
{
    using namespace rm;
    if(_toolType->size() > 1)
    {
        QListWidgetItem * item = _ui->listWidget->takeItem(_ui->listWidget->row(_ui->listWidget->currentItem()));
        _ui->listWidget->removeItemWidget(item);
        _toolType->top()->setChecked(false);
        _scene->popTool();
        _toolType->pop();
        _toolType->top()->setChecked(true);
        _ui->listWidget->takeItem(_ui->listWidget->count());
    }
    else if (_toolType->size() == 1)
    {
        _toolType->top()->setChecked(false);
        _scene->popTool();
        _toolType->pop();
        pushTool(new ViewControllerTool(_scene), _ui->actionDefault);
        this->setCursor(Qt::ArrowCursor);
    }
    else
    {
        pushTool(new ViewControllerTool(_scene), _ui->actionDefault);
        this->setCursor(Qt::ArrowCursor);
    }
}



void MainWindow::popAndDeleteTool()
{
    using namespace rm;
    if(_toolType->size() > 1)
    {
        GraphicsTool* sceneTool = _scene->topTool();
        _toolType->top()->setChecked(false);
        _scene->popTool();
        _toolType->pop();
        _toolType->top()->setChecked(true);

        //Refresh color when a tool is popped
        _ui->listWidget->takeItem(_ui->listWidget->count() - 1);
        _ui->listWidget->item(_ui->listWidget->count() - 1)->setBackgroundColor(QColor("LIGHT GREEN"));

        delete sceneTool;
    }
    else if (_toolType->size() == 1)
    {
        GraphicsTool* sceneTool = _scene->topTool();
        _toolType->top()->setChecked(false);
        _scene->popTool();
        _toolType->pop();
        pushTool(new ViewControllerTool(_scene), _ui->actionDefault);
        this->setCursor(Qt::ArrowCursor);
        _ui->listWidget->takeItem(_ui->listWidget->count() - 1);
        _ui->listWidget->item(_ui->listWidget->count() - 1)->setBackgroundColor(QColor("LIGHT GREEN"));
        delete sceneTool;
    }
    else
    {
        pushTool(new ViewControllerTool(_scene), _ui->actionDefault);
        this->setCursor(Qt::ArrowCursor);
    }
}



void MainWindow::on_actionFit_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    if (view != nullptr)
    {
        view->fit();
        view->update();
    }
}



void MainWindow::on_actionTriangular2DMesh_triggered()
{
    using namespace rm;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Triangle Mesh 2D"), "", tr("Mesh Files(*.off)"));
    if(fileName.size())
    {

        QStringList list;
        list = fileName.split("/");
        QString name = list[list.size() - 1];
        std::string file = fileName.toStdString();
        std::vector<Point2Df> points;
        std::vector<unsigned int> triangles;
        loader::readTriangle2DFileOFF(file, points, triangles);
        rm::TriangleMesh2DItem* triangleMesh = new rm::TriangleMesh2DItem(triangles, points);

        //TODO REMOVE: just for research tests.
        //rm::Tri3ToTri6Conversor(triangles, points, true);
        //rm::TriangleMesh2DItem* triangleMesh = new rm::TriangleMesh2DItem(triangles, points,
        //                                                                 rm::TriangleMesh2DItem::TriangleType::TRI6);

        _scene->addItem(triangleMesh);
        _scene->update();

        //create a new treeItem with delete button using our method addTreeItem
        addTreeItemAndModel(name, _triangleMesh2DTree, triangleMesh);
    }

}



void MainWindow::on_actionTriangular3DMesh_triggered()
{
    using namespace rm;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Triangle Mesh 3D"), "", tr("Mesh Files(*.off)"));
    if(fileName.size())
    {
        QStringList list;
        list = fileName.split("/");
        QString name = list[list.size() - 1];
        std::string file = fileName.toStdString();
        std::vector<Point3Df> points;
        std::vector<unsigned int> triangles;
        loader::readTriangle3DFileOFF(file, points, triangles);
        rm::TriangleMesh3DItem* triangleMesh = new rm::TriangleMesh3DItem(triangles, points);
        Point3Df color(0.1f, 0.5f, 1.0f );
        triangleMesh->setBrushColor(color);
        _scene->addItem(triangleMesh);
        _scene->update();

        addTreeItemAndModel(name, _triangleMesh3DTree, triangleMesh);
    }
}



void MainWindow::on_actionQuad2DMesh_triggered()
{
    using namespace rm;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Quad Mesh 2D"), "", tr("Mesh Files(*.off)"));
    if(fileName.size())
    {
        QStringList list;
        list = fileName.split("/");
        QString name = list[list.size() - 1];
        std::string file = fileName.toStdString();
        std::vector<Point2Df> points;
        std::vector<unsigned int> triangles;
        loader::readQuad2DFileOFF(file, points, triangles);
        rm::QuadMesh2DItem* quadMesh = new rm::QuadMesh2DItem(triangles, points);
        _scene->addItem(quadMesh);
        _scene->update();

        addTreeItemAndModel(name, _quadMesh2DTree, quadMesh);
    }
}



void MainWindow::on_actionQuad3DMesh_triggered()
{
    using namespace rm;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Quad Mesh 3D"), "", tr("Mesh Files(*.off)"));
    if(fileName.size())
    {
        QStringList list;
        list = fileName.split("/");
        QString name = list[list.size() - 1];
        std::string file = fileName.toStdString();
        std::vector<Point3Df> points;
        std::vector<unsigned int> triangles;
        loader::readQuad3DFileOFF(file, points, triangles);
        rm::QuadMesh3DItem* quadMesh = new rm::QuadMesh3DItem(triangles, points);
        Point3Df color(1.0f, 0.5f, 0.2f );
        quadMesh->setBrushColor(color);
        _scene->addItem(quadMesh);

        _scene->update();

        addTreeItemAndModel(name, _quadMesh3DTree, quadMesh);
    }

}



void MainWindow::on_actionPlanePositiveX_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setX(point.x() + 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionPlaneNegativeX_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setX(point.x() - 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionPlanePositiveY_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setY(point.y() + 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionPlaneNegativeY_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setY(point.y() - 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionPlanePositiveZ_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setZ(point.z() + 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionPlaneNegativeZ_triggered()
{
    using namespace rm;
    QWidget* widget = QApplication::focusWidget();
    GraphicsView* view = dynamic_cast<GraphicsView*>(widget);
    QVector3D point = (_scene->computeAABB3D().getMaxCornerPoint() + _scene->computeAABB3D().getMinCornerPoint()) / 2;

    point.setZ(point.z() - 1);
    if(Graphics3DView* view3D = dynamic_cast<Graphics3DView*>(view))
    {
        view3D->viewFrom(point);
        view3D->setSceneModelToIdentity();
        view3D->update();
    }
}



void MainWindow::on_actionView3D_triggered(bool checked)
{
    if(checked)
    {
        _ui->actionView3D->setChecked(true);
    }
    else
    {
        _ui->actionView3D->setChecked(false);
    }
}



bool MainWindow::eventFilter(QObject *obj, QEvent *event )
{
    using namespace rm;
    GraphicsView* view = dynamic_cast<GraphicsView*>(obj);
    // if a graphics view is receiving a mouse move the ui should display event local position in
    // status bar.
    if ((view != nullptr) &&
        (event->type() == QEvent::MouseMove))
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        double localX = mouseEvent->localPos().x();
        double localY = mouseEvent->localPos().y();
        std::string mouseCoords = "(" + std::to_string(localX) + ", " + std::to_string(localY) + ")";
        statusBar()->showMessage(QString::fromStdString(mouseCoords));        
    }
    else if((view != nullptr) &&
            (event->type() == QEvent::KeyPress))
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Escape)
        {
            popAndDeleteTool();
        }
    }
    return false;
}



void MainWindow::modelItemChanged(QStandardItem* item)
{
    //TODO: always been called. Check if its possible to identify if
    //the item's checkbox was modified
    setTreeItemVisible(item, item->checkState());
    QVariant var = item->data();
    if (var.isValid())
    {
        rm::GraphicsItemModel gItemModel = var.value<rm::GraphicsItemModel>();
        rm::GraphicsItem* gItem = gItemModel.getItem();
        gItem->setName(item->text().toStdString());
    }

    _scene->update();
}



unsigned int MainWindow::getTreeItemDepth(QStandardItem *item) const
{
    unsigned int depth = 0;
    while (item != nullptr)
    {
        item = item->parent();
        depth++;
    }
    return depth;
}



void MainWindow::addTreeItemAndModel(QString name, QStandardItem* itemModel, rm::GraphicsItem* gItem, bool count)
{
    if(count == true)
    {
        name = name.append(" " + QString::number(_itemsCounter));
    }

    gItem->setName(name.toStdString());
    //create a new treeItem with delete button using our method addTreeItem
    QStandardItem* itemTree = addTreeItem(name, itemModel);
    //create a QVariant, a GraphicsItemModel and store it as data into new treeItem.
    QVariant var;
    rm::GraphicsItemModel graphicsItemModel(gItem);
    var.setValue(graphicsItemModel);
    itemTree->setData(var);

    expandTreeItem(itemModel);
}
