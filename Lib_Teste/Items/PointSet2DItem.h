#pragma once

#include <vector>
#include <list>

#include "../Core/Graphics2DItem.h"

namespace rm
{

class PointSet2DItem : public Graphics2DItem
{
public:

    enum class LayoutType : unsigned char
    {
       Circle,
       Square,
       Triangle
    };

public:
    /**
     * @brief PointSet2DItem - Default constructor.
     */
    explicit PointSet2DItem();

    /**
     * @brief PointSet2DItem - Constructor that receives a vector with a set of points.
     * @param pointSet - Vector with a set of points.
     */
    explicit PointSet2DItem(const std::vector<Point2Df>& pointSet);

    /**
     * Destructor.
     */
    ~PointSet2DItem() override;

    /**
     * @brief add - Adds a new point p to point set.
     * @param p - New point to be added.
     * @return - Returns the number of points on point set.
     */
    unsigned int add(const Point2Df& p);

    /**
     * @brief getAABBRender - Gets the current AABB from the object. This AABB take in account the objects render
     * attributes like point radius, line thickeness...
     * @param pixelSize - The pixel size used to compute the aabb.
     * @return - Returns the current AABB from the object.
     */
    virtual AABB2D getAABBRender(const Point2Df& pixelSize = {0,0}) const override;

    /**
     * @brief getLayout - Returns the layout value.
     * @return - Returns the layout value.
     */
    LayoutType getLayout();

    /**
     * @brief getPointSet - Returns the 2D points in form of std vector.
     * @return - Returns the vector of 2D points.
     */
    const std::vector<Point2Df>& getPointSet() const;

    /**
     * @brief getPointSize - Gets the point size in pixels.
     * @return - Returns the point size in pixels.
     */
    float getPointSize() const;

    /**
     * @brief getVerticesVBOId - Gets the index of VBO vertices.
     * @return - Returns the index of VBO vertices.
     */
    unsigned int getVerticesVBOId() const;

    /**
     * @brief getWorldRadius - Gets the point radius in world metrics. It uses the item corrected pixelSize by the scale.
     * @return - Returns the point radius in world metrics.
     */
    float getWorldRadius() const;

    /**
     * @brief hardTranslate - Translates all points by a vector (dx, dy).
     * @param dx - x displacement.
     * @param dy - y displacement.
     */
    void hardTranslate(float dx, float dy);

    /**
     * @brief initialize - Initializes opengl objects to render.
     */
    virtual void initialize() override;

    /**
     * @brief insert - Inserts a new point p in a given position pos.
     * @param pos - The position where p should be inserted.
     * @param p - The point to be inserted.
     * @return - Returns the number of points on point set.
     */
    int insert(unsigned int pos, const Point2Df& p);

    /**
     * @brief intersects - Finds the index of the point that was clicked, if this point doesn't exist, returns -1. 
     * Allows the user to define a range of indexes [begin,end) to perform the search.
     * @param p - The point to test the intersection.
     * @param begin - Point index to start the search.
     * @param end - Point index to end searching, not used to test.
     * @return - Returns the index of the intersected point, or PointSet2DItem::NO_INTERSECTS if no point is intersected.
     */
    unsigned int intersects(const Point2Df& p, unsigned int begin = 0, unsigned int end = static_cast<unsigned int>(-1) ) const;

    /**
     * @brief isInitialized - Verifies if the item has been already initiazed.
     * @return - Returns true if the item is initialized and false otherwise.
     */
    bool isInitialized() const;

    /**
     * @brief isIntersecting - Checks intersection of screen point with item.
     * @param clickPoint - The point that was clicked.
     * @return - Returns the minimum distance of the point clicked to some point of the pointSet.
     */
    bool isIntersecting(const Point2Df& clickPoint) const override;

    /**
     * @brief singlePointMove - Moves a single point by a vector (dx, dy).
     * @param pointID - Point index to be moved.
     * @param delta - x and y displacement.
     */
    void pointMove(unsigned int pointID, const Point2Df& delta);

    /**
     * @brief render - Virtual function of GraphicsItem. It should contain whatever is necessary to draw
     * on the correct context, and should be implemented by the children.
     * @param viewId - View's identifier. Where the item will be rendered.
     */
    void render(int viewId) override;

    /**
     * @brief remove - Removes a point with id equal to pointID.
     * @param pointID - Index to point to be removed.
     */
    void remove(unsigned int pointID);


    /**
     * @brief setLayout - Sets the layout value.
     * @param type - The layout type.
     */
    void setLayout(LayoutType type);

    /**
     * @brief setNewPointSet - Defines a new set of points.
     * @param pointSet - New set of points.
     */
    void setNewPointSet(const std::vector<Point2Df>& pointSet);

    /**
     * @brief singlePointMove - Updates a single point to p.
     * @param pointId - Point index to be update.
     * @param p - New point to be inserted on pointId positon.
     */
    void setPoint(unsigned int pointId, const Point2Df& p);

    /**
     * @brief setPointSize - Sets the point size in pixels.
     * @param size - New point size value in pixels.
     */
    void setPointSize(float size);

    /**
     * @brief size - Gets the number of point on point set item.
     * @return - Returns the number of points on the pointSet.
     */
    unsigned int size() const;

    /**
     * @brief setBorderSize - Sets a floating value to the border size of the item.
     * @param s - The floating positive value in pixels of the new border size.
     */
    void setBorderSize(float size);

    /**
     * @brief getBorderSize - Gets the border size in pixels.
     * @return - Returns the border size in pixels.
     */
    float getBorderSize() const;

private:

    struct LocationVariables
    {
        /**
         * @brief vp - OpenGL identifier for view projection matrix.
         */
        int vp {-1};

        /**
         * @brief m - OpenGL identifier for model.
         */
        int m {-1};

        /**
         * @brief radius - OpengGL identifier for radius variable.
         */
        int radius {-1};

        /**
         * @brief brushColor - OpenGL identifier for brush color variable.
         */
        int brushColor {-1};

        /**
         * @brief penColor - OpenGL identifier for pen color variable.
         */
        int penColor {-1};

        /**
         * @brief borderSize - OpenGL identifier for border size variable.
         */
        int borderSize;

        /**
         * @brief penRatio - OpenGL identifier for pen ratio variable.
         */
        int brushRatio {-1}; //TODO: Remover
    };

private:
    /**
     * @brief _pointSet - A set of points.
     */
    std::vector<Point2Df> _pointSet;

    /**
     * @brief _vertexbuffer - Buffers identifiers.
     */
    unsigned int _vertexBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _layoutType - Stores the layoutType
     */
    LayoutType _layoutType {LayoutType::Circle};

    /**
     * @brief _borderSize - Size of the border in pixels.
     */
    float _borderSize {getDefaultBorderSize()};

    /**
     * @brief _size - Diameter of the point in pixels.
     */
    float _size {getDefaultSize()};

    /**
     * @brief _quadLocations - Stores all necessary locations to quad shader.
     */
    LocationVariables _squareLocations;

    /**
     * @brief _circleLocations - Stores all necessary locations to circle shader.
     */
    LocationVariables _circleLocations;

    /**
     * @brief _triangleLocations - Stores all necessary locations to triangle shader.
     */
    LocationVariables _triangleLocations;

    /**
     * @brief _squareProgram - Quad shader program.
     */
    QOpenGLShaderProgram* _squareProgram {nullptr};

    /**
     * @brief _circleProgram - Circle shader program.
     */
    QOpenGLShaderProgram* _circleProgram {nullptr};

    /**
     * @brief _triangleProgram - Triangle shader program.
     */
    QOpenGLShaderProgram* _triangleProgram {nullptr};


private:
    /**
     * @brief createBuffers - Creates OpenGL buffers.
     */
    void createBuffers();

    /**
     * @brief checkVao - Checks if vao was already created to this Item.
     * @param id - View ID.
     */
    void checkVao(int id);

    /**
     * @brief computeAABB - Computes object's AABB.
     */
    void computeAABB();

    /**
     * @brief createProgram - Creates a new program and saves its locations.
     * @param vertexSourceFile - Vertex shader source code file.
     * @param geometrySourceFile - Geometry shader source code file.
     * @param fragmentSourceFile - Fragment shader source code file.
     * @param locations - Structure used to save locations.
     * @return - Returns new opengl shader program.
     */
    QOpenGLShaderProgram* createProgram(const char* vertexSourceFile, const char* geometrySourceFile,
                                        const char* fragmentSourceFile, LocationVariables& locations);

    /**
     * @brief createProgram - Creates all necessary OpenGL programs.
     */
    void createPrograms();

    /**
     * @brief createVao - Creates and configures new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id - View ID
     */
    void createVao(int id);

    /**
     * @brief getDefaultSize - Returns de default point size in pixels.
     * @return - Returns default point size.
     */
    constexpr static float getDefaultSize() { return 15.0f; }

    /**
     * @brief getDefaultBorderSize - Returns de default border size in pixels.
     * @return - Returns default border size.
     */
    constexpr static float getDefaultBorderSize() { return 2.0f; }

    /**
     * @brief updateVertexBuffer - Updates vertex buffer.
     */
    void updateVertexBuffer();

    /**
     * @brief updateAABB - Updates the AABB with a new point. Be careful, this function cannot be used with a moving point.
     * @param p - New point.
     */
    void updateAABB(const Point2Df& p);
};
}
