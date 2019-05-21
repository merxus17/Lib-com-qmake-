#pragma once
#include "Vector2D.h"
#include <QVector3D>
#include <QMatrix4x4>

namespace rm
{
template <class T, int D>
class AxisAligmentBoundingBox
{
public:
    /**
     * @brief AxisAligmentBoundingBox - default constructor.
     */
    AxisAligmentBoundingBox() = default;

    /**
     * @brief AxisAligmentBoundingBox - construtor that receives the two extreme points.
     * @param minCornerPoint - minimal corner point.
     * @param maxCornerPoint - maximal corner point.
     */
    AxisAligmentBoundingBox(const T& minCornerPoint, const T& maxCornerPoint)
    {
        _minCornerPoint = minCornerPoint;
        _maxCornerPoint = maxCornerPoint;
    }

    /**
     * @brief ~AxisAligmentBoundingBox - default destructor.
     */
    ~AxisAligmentBoundingBox() = default;

    /**
     * @brief getMinCornerPoint - get the minimal corner point.
     * @return - the minimal corner point.
     */
    const T& getMinCornerPoint() const
    {
        return _minCornerPoint;
    }

    /**
     * @brief getMinCornerPoint - get the minimal corner point.
     * @return - the minimal corner point.
     */
    T& getMinCornerPoint()
    {
        return _minCornerPoint;
    }

    /**
     * @brief getMaxCornerPoint - get the maximal corner point.
     * @return - the maximal corner point.
     */
    const T& getMaxCornerPoint() const
    {
        return _maxCornerPoint;
    }

    /**
     * @brief getMaxCornerPoint - get the maximal corner point.
     * @return - the maximal corner point.
     */
    T& getMaxCornerPoint()
    {
        return _maxCornerPoint;
    }

    /**
     * @brief setMinCornerPoint - set a new minimal corner point.
     * @param minCornerPoint - new minimal corner point.
     */
    void setMinCornerPoint(const T& minCornerPoint)
    {
        _minCornerPoint = minCornerPoint;
    }

    /**
     * @brief setMaxCornerPoint - set a new maximal corner point.
     * @param maxCornerPoint - new maximal corner point.
     */
    void setMaxCornerPoint(const T& maxCornerPoint)
    {
        _maxCornerPoint = maxCornerPoint;
    }

    /**
     * @brief isAABBInside - check if a point p is inside of the current AABB.
     * @param p - ponint to be tested.
     * @return - true if the point is inside of the current AABB and false otherwise.
     */
    bool isAABBInside(const T& p) const
    {
        for (int i = 0; i < D; i++)
        {
            if (p[i] < _minCornerPoint[i] || p[i] > _maxCornerPoint[i])
                return false;
        }
        return true;
    }

    /**
     * @brief operator + - get the AABB from two diferent AABB's.
     * @param rhs - AABB to operates with the current AABB to form a new AABB.
     * @return - new AABB that contains the old two AABB's.
     */
    AxisAligmentBoundingBox operator+(const AxisAligmentBoundingBox& rhs)
    {
        //Get extreme points.
        AxisAligmentBoundingBox newAABB;
        T& minCorner = newAABB.getMinCornerPoint();
        T& maxCorner = newAABB.getMaxCornerPoint();

        //Compute new AABB.
        for (int i = 0; i < D; i++)
        {
            minCorner[i] = std::min(_minCornerPoint[i], rhs.getMinCornerPoint()[i]);
            maxCorner[i] = std::max(_maxCornerPoint[i], rhs.getMaxCornerPoint()[i]);
        }

        return std::move(newAABB);
    }


    /**
     * @brief operator += - Get the AABB from two diferent AABB's.
     * @param rhs - AABB to operates with the current AABB to form a new AABB.
     * @return - Reference to resulting left hand side AABB.
     */
    AxisAligmentBoundingBox& operator+=(const AxisAligmentBoundingBox& rhs)
    {
        //Get extreme points.
        AxisAligmentBoundingBox newAABB;
        T& minCorner = getMinCornerPoint();
        T& maxCorner = getMaxCornerPoint();

        //Compute new AABB.
        for (int i = 0; i < D; i++)
        {
            minCorner[i] = std::min(_minCornerPoint[i], rhs.getMinCornerPoint()[i]);
            maxCorner[i] = std::max(_maxCornerPoint[i], rhs.getMaxCornerPoint()[i]);
        }

        return *this;
    }

    /**
     * @brief operator * - Operator to transform an AABB by a matrix.
     * @param m - Matrix that must be used to tranform tha AABB corrdinates space.
     * @param aabb - AABB that must be transformed by matrix m.
     * @return - New transformed AABB.
     */
    friend AxisAligmentBoundingBox operator * (const QMatrix4x4& m, const AxisAligmentBoundingBox& aabb)
    {
        T minPoint = aabb.getMinCornerPoint();
        T maxPoint = aabb.getMaxCornerPoint();

        QVector4D p1(0, 0, 0, 1);
        QVector4D p2(0, 0, 0, 1);

        for (int i = 0; i < D; i++)
        {
            p1[i] = minPoint[i];
            p2[i] = maxPoint[i];
        }

        QVector4D newMinCorner = m * p1;
        QVector4D newMaxCorner = m * p2;

        for (int i = 0; i < D; i++)
        {
            minPoint[i] = std::min(newMinCorner[i], newMaxCorner[i]);
            maxPoint[i] = std::max(newMinCorner[i], newMaxCorner[i]);
        }

        return AxisAligmentBoundingBox(minPoint,maxPoint);
    }


private:
    T _minCornerPoint;
    T _maxCornerPoint;
};

using AABB2D = AxisAligmentBoundingBox<Point2Df, 2>;
using AABB3D = AxisAligmentBoundingBox<QVector3D, 3>;
}
