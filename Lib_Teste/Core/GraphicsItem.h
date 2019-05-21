#pragma once
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QVector4D>
#include <algorithm>
#include "../Geometry/OpenGLMatrix.h"

#include "../Geometry/Vector2D.h"

class Texture;
class Font;
class QOpenGLShaderProgram;
enum class ItemStyle: char {Fill, Wireframe, FillAndIWireframe};

namespace rm
{
class GraphicsItem : protected QOpenGLFunctions
{
public:
    /**
     * @brief NO_INTERSECTS - Define a test variable to no intersection case.
     */
    static constexpr unsigned int NO_INTERSECTS = static_cast<unsigned int>(-1);
public:
   /**
    * @brief Empty Constructor
    */
   GraphicsItem() = default;

   /**
    * @brief Class Destructor
    */
   virtual ~GraphicsItem() = default;

   /**
    * @brief initialize - Virtual function of graphicsitem. Should group shaders and
    * initialize buffers. Needs to be implemented by the class' children.
    */
   virtual void initialize() = 0;

   /**
    * @brief render - Virtual function of GraphicsItem. It should contain whatever
    * is necessary to draw on the correct context, and should be implemented by the children.
    * @param viewId - View's identifier. Where the item will be rendered.
    */
   virtual void render(int viewId) = 0;

   /**
    * @brief hasVao - Verifies if there is a VAO alocated to an specific id.
    * @param id - id window to verifiy if the VAO is allocated.
    * @return - Returns true if already exists an allocated VAO and false otherwise.
    */
   bool hasVao(int id) const;

   /**
    * @brief getBrushColor - Gets the current brush color, i. e., the color used to fill the objects.
    * @return - Returns the current color used to fill objects.
    */
   const QVector4D& getBrushColor() const;

   /**
    * @brief setBrushColor - Sets color to fill the object.
    * @param color - New color to fill the object.
    */
   void setBrushColor(const QVector4D& color);

   /**
    * @brief setBrushColor - Sets color to fill the object.
    * @param color - New color to fill the object.
    */
   virtual void setBrushColor(const QVector3D& color);

   /**
    * @brief getPenColor - Gets the current pen color, i. e., the color used to paint lines on object contour.
    * @return - Returns the current color used to paint lines on object contour.
    */
   const QVector4D& getPenColor() const;

   /**
    * @brief setPenColor - Sets color used to paint lines on object contour.
    * @param color - New color used to paint lines on object contour.
    */
   void setPenColor(const QVector4D& color);

   /**
    * @brief setPenColor - Sets color used to paint lines on object contour.
    * @param color - New color used to paint lines on object contour.
    */
   virtual void setPenColor(const QVector3D& color);

   /**
    * @brief isVisible - Verifies if the current item is visible.
    * @return - Returns true if the item is visible and false otherwise.
    */
   bool isVisible() const;

   /**
    * @brief visible - Defines the object visibility.
    * @param visible - True to turn on the visibility and false otherwise.
    */
   virtual void visible(bool v);

   /**
    * @brief getTexture - Gets the item's texture.
    * @return - Returns the current texture.
    */
   Texture& getTexture();

   /**
    * @brief setTexture - Sets the item's texture.
    * @param texture - Texture that will be set for the item.
    */
   void setTexture(Texture* texture);

   /**
    * @brief getLineWidth - Gets the current line width used to render the item.
    * @return - Returns current line width.
    */
   float getLineWidth() const;

   /**
    * @brief setLineWidth - Defines a new line width value. This value is used to render the item lines.
    * @param width - New line width value.
    */
   virtual void setLineWidth(float width);

   /**
    * @brief getStyle - Gets the current style of the item.
    * @return - Returns current style of item.
    */
   ItemStyle getStyle() const;

   /**
    * @brief setStyle - Sets the item's style.
    * @param style - New item's style.
    */
   void setStyle(ItemStyle style);

   /**
    * @brief getFont - Gets the item's font.
    * @return - Returns current font of the item.
    */
   Font& getFont();

   /**
    * @brief setFont - Sets the item's font.
    * @param font - New item's font.
    */
   void setFont(Font* font);

   /**
    * @brief setProjectionMatrix - Sets projection matrix of the item.
    * @param proj - New item's projection matrix.
    */
   virtual void setProjectionMatrix(const OpenGLMatrix &proj);

   /**
    * @brief isIntersecting - Checks if a given point intersects the item. Should be implemented by all itens.
    * @param clickPoint - The point to be check for intersection.
    * @return - Returns true if a given point is intersecting the item. Return false otherwise.
    */
   virtual bool isIntersecting(const Point2Df& clickPoint) const = 0;

   /**
    * @brief getOnFocus - Returns if this item is on focus or not.
    * @return - Returns true if the item is on focus, false if its not.
    */
   bool isOnFocus() const;

   /**
    * @brief setOnFocus - Sets this item as the item on focus.
    * @param onFocus - True to set the item on focus, false to unset.
    */
   virtual void onFocus(bool onFocus);

   /**
    * @brief setName - Sets the object name.
    * @param name - New item's name.
    */
   void setName(std::string name);

   /**
    * @brief getName - Gets the item name.
    * @return - Returns current item's name.
    */
   const std::string& getName() const;

   /**
    * @brief setModelToIdentity - Defines the model matrix to identity.
    */
   virtual void setModelToIdentity();

   /**
    * @brief translate - Translates the object by the vector t.
    * @param t - Translation vector.
    */
   virtual void translate(const QVector3D& t);

   /**
    * @brief scale - Scales the object.
    * @param s - Scales factors.
    */
   virtual void scale(const QVector3D& s);

   /**
    * @brief rotate - Rotates the object.
    * @param r - Rotate factors.
    * @param angle - Angle of the rotation.
    */
   virtual void rotate(const QVector3D& r, const float angle);

   /**
    * @brief setModelMatrix - Defines a new model matrix.
    * @param m - New model matrix.
    */
   virtual void setModelMatrix(const OpenGLMatrix &m);

   /**
    * @brief getModelMatrix - Gets the current model matrix.
    * @return - Returns current model matrix.
    */
   const OpenGLMatrix& getModelMatrix() const;

   /**
    * @brief pushModelMatrix - Stacks a copy of the current model matrix.
    */
   virtual void pushModelMatrix();

   /**
    * @brief popModelMatrix - Remove the matrix, if it exists, of the top of the stack and turn it as current matrix.
    */
   virtual void popModelMatrix();

   /**
    * @brief multModelMatrix - Multiplies the current matrix by another matrix. This is equivalent to a *= m;
    * @param m - Matrix to be multiplied to the current matrix.
    */
   virtual void multModelMatrix(const OpenGLMatrix &m);

   /**
    * @brief multLefModelMatrix - Multiplies the current matrix by another matrix. This is equivalent to a = m * a;
    * @param m - Matrix to be multiplied to the current matrix.
    */
   virtual void multLefModelMatrix(const OpenGLMatrix &m);
protected:

    /**
    * @brief _name - Item's name.
    */
    std::string _name;

    /**
     * @brief _proj - Item's projection matrix.
     */
    OpenGLMatrix _proj;

    /**
     * @brief _modelMatrix - Item's model matriz.
     */
    OpenGLMatrix _modelMatrix;

    /**
     * @brief _brushColor - Color used to fill the object.
     */
    QVector4D _brushColor{0, 0, 1, 1};

    /**
     * @brief _penColor - Color used to paint the lines on object contour.
     */
    QVector4D _penColor{0, 0, 0, 1};

   /**
    * @brief _visible - Visibility atributte.
    */
   bool _visible {true};

   /**
    * @brief _texture - Item's attached texture.
    */
   Texture* _texture {nullptr};

   /**
    * @brief _lineWidth - Line thickness. By line it is referring to contour line.
    */
   float _lineWidth {3};

   /**
    * @brief _style - Enum atribbute for line pattern.
    */
   ItemStyle _style {ItemStyle::Fill};

   /**
    * @brief _font - Attached font to GraphicsItem.
    */
   Font* _font {nullptr};

   /**
    * @brief _vao - GraphicsItem's vertex array object.
    */
   std::map<int, QOpenGLVertexArrayObject*> _vao;

   /**
    * @brief _onFocus - Define if the item has focus or not.
    */
   bool _onFocus {false};
};
}
