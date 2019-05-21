#pragma once
#include "Vector2D.h"
#include <vector>
#include <algorithm>

template<class T>
using Polygon2D = std::vector<Point2D<T>>;

template<class T>
class Geometry2DAlgorithms
{
public:
    /**
     * @brief inPolygon - Check if point is inside polygon
     * @param p - Point to check
     * @param polygon - Polygon
     * @return - Outside 0, inside 1, under a segment -1
     */
    static int inPolygon( const Point2D<T>& p, const Polygon2D<T>& polygon );

    /**
     * @brief polygonArea - Calculate polygon area
     * @param polygon - Polygon
     * @return - Area value with signal
     */
    static T polygonArea( const Polygon2D<T>& polygon );

    /**
     * @brief radialSort - Definition of comparison funcion for sorting
     * @param p - First point of comparison
     * @param q - Second point of comparison
     * @return - Returns ​true if the first argument is less than (i.e. is ordered before) the second
     */
    static bool radialSort( const Point2D<T>& p,  const Point2D<T>& q );

    /**
     * @brief convexHull - Gives the polygon defined by a set of points
     * @param points - Vector of points
     * @return - Points in order
     */
    static Polygon2D<T> convexHull( std::vector<Point2D<T>>& points );

public:
    static Point2D<T> _pivot;
};

template<class T>
Point2D<T> Geometry2DAlgorithms<T>::_pivot;

template<class T>
int Geometry2DAlgorithms<T>::inPolygon( const Point2D<T>& p, const Polygon2D<T>& polygon )
{
    T angle = 0.0;
    size_t n = polygon.size( );
    for ( size_t i = 0; i < n; i++ )
    {
        if ( Point2D<T>::pointOverSegment( polygon[i], p, polygon[( i + 1 ) % n] ) )
        {
            return -1;
        }
        angle += Point2D<T>::angle( polygon[i], p, polygon[( i + 1 ) % n] );
    }
    return Point2D<T>::cmp( angle ) != 0;
}


template<class T>
T Geometry2DAlgorithms<T>::polygonArea( const Polygon2D<T>& polygon )
{
    T area = 0.0;
    size_t n = polygon.size( );
    for ( size_t i = 0; i < n; i++ )
    {
       area += polygon[i] ^ polygon[( i + 1 ) % n];
    }
    return area / 2.0;
}

template<class T>
bool Geometry2DAlgorithms<T>::radialSort( const Point2D<T>& p,  const Point2D<T>& q  )
{
    Vector2D<T> P = p - Geometry2DAlgorithms<T>::_pivot, Q = q - Geometry2DAlgorithms<T>::_pivot;
    T R = P ^ Q;
    if ( Point2D<T>::cmp( R ) ) return R > 0;
    return Point2D<T>::cmp( P * P, Q * Q ) < 0;
}


template<class T>
Polygon2D<T> Geometry2DAlgorithms<T>::convexHull( std::vector<Point2D<T>>& points )
{
    size_t j = 0, k;
    size_t n = points.size( );
    Polygon2D<T> U( n );
    Geometry2DAlgorithms<T>::_pivot = *std::min_element( points.begin( ), points.end( ) );
    std::sort( points.begin( ), points.end( ), radialSort );

    for ( k = n - 2; k >= 0 && Vector2D<T>::ccw( points[0], points[n - 1], points[k] ) == 0; k-- );
    std::reverse( ( k + 1 ) + points.begin( ), points.end( ) );

    for ( size_t i = 0; i < n; i++ )
    {
        // troque o >= por > para manter pontos colineares
        while (j > 1 && Vector2D<T>::ccw( U[j - 1], U[j - 2], points[i] ) >= 0) j--;
        U[j++] = points[i];
    }
    U.resize( j );
    return U;
}
