#pragma once
#include <stack>
#include <QVector4D>
#include <QMatrix4x4>

namespace rm
{


class OpenGLMatrix
{
public:
    /**
     * @brief OpenGLMatrix - Default constructor.
     */
    OpenGLMatrix( );

    /**
     * @brief ~OpenGLMatrix - Destructor.
     */
    ~OpenGLMatrix( ) = default;

    /**
     * @brief sX - Gets the x direction scale factor.
     * @return - the x direction scale factor.
     */
    float sX() const;

    /**
     * @brief sY - Gets the y direction scale factor.
     * @return - the y direction scale factor.
     */
    float sY() const;

    /**
     * @brief sZ - Gets the z direction scale factor.
     * @return - the z direction scale factor.
     */
    float sZ() const;

    /**
     * @brief getScaleFactors - Gets the sx, sy, sz scale factors.
     * @return - The sx, sy, sz scale factors.
     */
    QVector3D getScaleFactors() const;

    /**
     * @brief push - Push a copy of the current matrix to the top of stack.
     */
    void push( );

    /**
     * @brief pop - Remove a matrix from the top of stack and turn it as current.
     */
    void pop( );

    /**
     * @brief loadIdentity - Define the current matrix as identity.
     */
    void loadIdentity( );

    /**
     * @brief loadMatrix - Set a new matrix as current matrix.
     * @param m - Matrix to be the new current matrix.
     */
    void loadMatrix( const QMatrix4x4& m );

    /**
     * @brief multMatrix - Multiply the current matrix to the matrix rhs on righ. This is equivalent to do a *= m;
     * @param rhs - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    const OpenGLMatrix& multMatrix(const OpenGLMatrix& rhs);

    /**
     * @brief multLeftMatrix - Multiply the current matrix to the matrix rhs on left. This is equivalent to do a = m *a;
     * @param rhs - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    const OpenGLMatrix& multLeftMatrix(const OpenGLMatrix& rhs);

    /**
     * @brief translate - Apply the translate operation to the current matrix.
     * @param (dx, dy, dz) - Translation vector.
     */
    void translate(float dx, float dy, float dz );

    /**
     * @brief scale - Apply the scale operation to the current matrix.
     * @param (sx, sy, sz) - Scale vector.
     */
    void scale( float sx, float sy, float sz );

    /**
     * @brief rotate - Apply the rotation operation of an angle a around the vector (x, y, z).
     * @param a - Rotation angle.
     * @param (x, y, z) - Rotation vector.
     */
    void rotate( float a, float x, float y, float z );

    /**
     * @brief rotate - Apply the rotation operation using quaternion q.
     * @param q - Quaternion that defines the rotation.
     */
    void rotate(const QQuaternion& q);

    /**
     * @brief lookAt - Define the camera parameteres. This is equivalent to gluLookAt.
     * @param (eyeX, eyeY, eyeZ) - Camera/Observer position.
     * @param (centerX, centerY, centerY) - Camera target.
     * @param (upX, upY, upZ) - Up camera direction.
     */
    void lookAt( float eyeX, float eyeY, float eyeZ,
                 float centerX, float centerY, float centerZ,
                 float upX, float upY, float upZ );

    /**
     * @brief frustum - Define a perspective matriz using frustum. Equivalent to glFrustum.
     * @param (xMin, yMin) - Minimum corner.
     * @param (xMax, yMin) - Maximum corner.
     * @param zNear - Minimal distance to observer.
     * @param zFar - Maxima distance to observer.
     */
    void frustum( float xMin, float xMax, float yMin, float yMax, float zNear, float zFar );

    /**
     * @brief ortho - Define an orthographic matrix.
     * @param (left, right) - Minimum corner.
     * @param (bottom, top) - Maximum corner.
     * @param nearVal - Minimal distance to observer.
     * @param farVal - Maxima distance to observer.
     */
    void ortho( float left, float right, float bottom, float top, float nearVal, float farVal );

    /**
     * @brief perspective - Define the standard perspective matrix.
     * @param fovY - Field of view in y-direction.
     * @param aspect - Screen aspect ratio.
     * @param zNear - Minimal distance to observer.
     * @param zFar - Maxima distance to observer.
     */
    void perspective( float fovY, float aspect, float zNear, float zFar );

    /**
     * @brief getMatrix - Get the current matrix.
     * @return - the current matrix.
     */
    const QMatrix4x4& topMatrix() const;


private:
    /**
     * @brief _matrixStack - Matrices' stack.
     */
    std::stack<QMatrix4x4> _matrixStack;

    /**
     * @brief _currentMatrix - Current matrix.
     */
    QMatrix4x4 _currentMatrix;

};

}
