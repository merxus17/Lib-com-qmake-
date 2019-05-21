#pragma once
#include <cmath>
#include <iostream>
/**
 * Forward declaration to vector class.
 */
template<class T>
class Vector2D;

/**
 * Define nicknames to vector class to be used as a point.
 */
template<class T>
using Point2D = Vector2D<T>;
using Point2Df = Point2D<float>;

/**
 * @brief The Vector2D class - 2D vector/point class.
 */
template<class T>
class Vector2D
{
public:
    /**
     * @brief Vector2D - Default constructor.
     */
    Vector2D( ) :
        _xp(0),
        _yp(0)
    {
    }

    /**
     * @brief Vector2D - Constructor that receives vector coordinates as parameters.
     * @param x - x coordinate.
     * @param y - y coordinate.
     */
    Vector2D( T x, T y ) :
        _xp(x),
        _yp(y)
    {
    }

    /**
     * @brief angle - Compute the angle between the current vector and q vector.
     * @param q - The Vector with which the current one shall be compared
     * @return - Angle between vectors in rad.
     */
    T angle( const Vector2D& q ) const
    {
        const Vector2D& u = *this;
        return std::atan2( u ^ q, u * q );
    }

    /**
     * @brief angle - compute the angle between vectors u = p - r e w = r - q.
     * @param p - first point.
     * @param q - base point of vectors.
     * @param r - second point.
     * @return
     */
    static T angle( const Point2D<T>& p, const Point2D<T> &q, const Point2D<T>& r )
    {
        Point2D<T> u = p - q, w = r - q;
        return atan2( u ^ w, u * w );
    }

    /**
     * @brief ccw - Check if direction is counter clock wise
     * @param p - first Point2D
     * @param q - second Point2D
     * @param r - third Point2D
     * @return - Returns 1 if it is counter clockwise , -1 if it is clockwise and 0 if it is a line
     */
    static int ccw( const Point2D<T>& p, const Point2D<T>& q, const Point2D<T>& r )
    {
		return cmp((p - r) ^ (q - r));
    }

    /**
     * @brief cmp - Compare two vectors.
     * @param q - vector to be compared with the current vector.
     * @return -1 if current < y, 0 if current == y and 1 if current > y considering the tolerance.
     * eps.
     */
    int cmp( const Vector2D& q ) const
    {
        if (int t = cmp( _xp, q._xp ))
            return t;
        return cmp( _yp, q._yp );
    }

    /**
     * @brief cmp - Verify if x <= y with a tolerance eps.
     * @param x - x value to be compared with y.
     * @param y - y value to be compared with x.
     * @param eps - tolerance to be used.
     * @return -1 if x < y, 0 if x == y and 1 if x > y considering the tolerance eps.
     */
    static int cmp( T x, T y = static_cast<T>(0), T eps = 1e-1 )
    {
        return (x <= y + eps) ? (x + eps < y) ? -1 : 0 : 1;
    }

    /**
     * @brief norm - Compute the norm of current Vector
     * @return - Template T , norm
     */
    T norm( ) const
    {
        return std::sqrt(_xp * _xp + _yp * _yp);
    }

    /**
     * @brief normalize - Normalize the current vector
     * @return - the previous norm of the Vector (Template T)
     */
    T normalize( )
    {
        T nor = norm();
        _xp = _xp / nor;
        _yp = _yp / nor;
		return nor;
    }

    /**
     * @brief operator + - Sum two vector and return the resulting vector: a + p.
     * @param p - Vector that will be added
     * @return - New vector which is the sum between the caller of the funcion and p
     */
    Vector2D operator+(const Vector2D& p) const
    {
         return Vector2D<T> (_xp + p._xp, _yp + p._yp);
    }

    /**
     * @brief operator += - Sum a vector q with the current vector and return the current vector: a += q.
     * @param q - Vector that will be added
     * @return - Modifies the caller of the function and returns it ;
     */
    Vector2D& operator+=(const Vector2D& q)
    {
        _xp += q._xp;
        _yp += q._yp;
        return *this;
	}

    /**
     * @brief operator - - Invert coordinate signals: -(a).
     * @return - Returns new vector which is the negative of the caller of the function
     */
    Vector2D operator-() const
    {
        return Vector2D<T> (-_xp, -_yp);
    }

    /**
     * @brief operator - - Subtract two vectors: a - p.
     * @param p - Vector that will be subtracted.
     * @return - New vector that is a minus p.
     */
    Vector2D operator-(const Vector2D& p) const
    {
        return Vector2D<T> (_xp - p._xp, _yp - p._yp);
    }

    /**
     * @brief operator -= - Subtract a vector q from the current vector: a -= q.
     * @param p - Vector that will be subtracted.
     * @return - the caller modifidied and returned.
     */
    Vector2D& operator-=(const Vector2D& q)
    {
        _xp -= q._xp;
        _yp -= q._yp;
		return *this;
    }

    /**
     * @brief operator * - Multiply a vector by a scalar t: a * t.
     * @param t - scalar multiplier.
     * @return - New vector which is a * t.
     */
    Vector2D operator*(T t) const
    {
        return Vector2D<T>( t * _xp , t * _yp );
    }

    /**
     * @brief operator [] - operator [] overload to access the coordinates.
     * @param i - dimension index: 0 to x and 1 to y.
     * @return - the coordinate.
     */
    T &operator[](int i)
    {
       return *(&_xp + i);
    }

    /**
     * @brief operator [] - operator [] overload to access the coordinates.
     * @param i - dimension index: 0 to x and 1 to y.
     * @return - the coordinate.
     */
    T operator[](int i) const
    {
        return *(&_xp + i);
    }

    /**
     * @brief x - get the x coordiante.
     * @return - x coordinate.
     */
    T x() const
    {
        return _xp;
    }

    /**
     * @brief y - get the y coordinate.
     * @return - y coordiante.
     */
    T y() const
    {
        return _yp;
    }

    /**
     * @brief operator * - Multiply a vector by a scalar t: t * a.
     * @param p - vector that is being multiplied.
     * @return - returns new vector with value t * a.
     */
    friend Vector2D<T> operator*(T t, const Vector2D<T>& p)
    {
        return Vector2D<T>(t * p._xp, t * p._yp);
    }

    /**
     * @brief operator * - Dot product: a * p.
     * @param p - Vector that appears after the operator.
     * @return - Template T with the result of operation.
     */
    T operator*(const Vector2D& p) const
    {
        return _xp * p._xp + _yp * p._yp;
    }

    /**
     * @brief operator *= - Multiply the current vector to a scalar t: a *= t.
     * @param t - double escalar.
     * @return - current vector with modified values.
     */
    Vector2D& operator*=(T t)
    {
        _xp *= t;
        _yp *= t;
		return *this;
    }

    /**
     * @brief operator / - Divide a vector to a scalar t: a / t.
     * @param t -double escalar.
     * @return - New Vector with expected values.
     */
    Vector2D operator/(T t) const
    {
        return Vector2D<T>(_xp / t, _yp / t);
    }

    /**
     * @brief operator /= - Divide the current vector to a escalar t a /= t.
     * @param t - scalar denominator.
     * @return  - modifies and return current vector.
     */
    Vector2D& operator/=(T t)
    {
        _xp /= t;
        _yp /= t;
        return *this;
    }

    /**
     * @brief operator ^ - Cross product: a x q
     * @param q - vector that apperas after operator.
     * @return - Result in template T.
     */
    T operator^(const Vector2D& q) const
    {
        return _xp * q._yp - q._xp * _yp;
    }

    /**
     * @brief operator == - Compare if two vectors are equals.
     * @param q vector that is being verified along with the caller.
     * @return true if equal , false if diffent.
     */
    bool operator==(const Vector2D& q) const
    {
        return cmp( q ) == 0;
    }

    /**
     * @brief operator != - Verify if two vectors are different.
     * @param q - vector that is being verified along with the caller.
     * @return - true if diffent , false if equal.
     */
    bool operator!=(const Vector2D& q) const
    {
        return cmp( q ) != 0;
    }

    /**
     * @brief operator < - Compare two point in order to determine the order of them.
     * @param q - vector that is being compared.
     * @return - true if < , false if >=.
     */
    bool operator<(const Vector2D& q) const
    {
        return cmp( q ) < 0;
    }

    /**
     * @brief operator << - Print the point coordinates.
     * @param o - stream that will be used to print.
     * @param p - Vector or Point you desire to print.
     * @return - modified stream with expected format.
     */
    friend std::ostream& operator<<( std::ostream& o, const Vector2D& p)
    {
        o << "(" << p._xp << "," << p._yp << ")";
		return o;
    }

    /**
     * @brief projectPointOnSegment - Compute the coordinates of projection point p over segment p1 - p2.
     * @param p - point to be projected over segment.
     * @param p1, p2 - points that define the segment.
     * @return - projected point.
     */
    static Point2D<T> projectPointOnSegment(const Point2D<T>& p, const Point2D<T>& p1, const Point2D<T>& p2)
    {
        Vector2D<T> v = p - p1;
        Vector2D<T> w = p2 - p1;

        //Projection Vector v on w
        Vector2D<T> u = w * ((v * w) / (w * w));

        return p1 + u;
    }

    /**
     * @brief pointOverLine - Tests if a Point2D is over the line formed by 2 other points.
     * @param p - point ot be checked.
     * @param p1 - first point of the line.
     * @param p2 - second point of the line.
     * @return - if point is over the line returns true otherwise returns false.
     */
    static bool pointOverLine( Point2D<T>& p, Point2D<T>& p1, Point2D<T>& p2)
	{
        return ccw(p1, p, p2) == 0;
	}
	
    /**
     * @brief pointOverSegment - Test if Point2D is over the segment.
     * @param p - point to be checked.
     * @param p1 - first extrimity of the the segment.
     * @param p2 - second extrimity of the segment.
     * @return - if point is over the segment returns true otherwise returns false.
     */
    static bool pointOverSegment(const Point2D<T>& p, const Point2D<T>& p1, const Point2D<T>& p2)
    {
        return ccw( p1, p2, p ) == 0 && cmp( ( p1 - p ) * ( p2 - p ) ) <= 0;
    }

    /**
     * @brief pointSegmentDist - Compute distance between Point2D and segment.
     * @param p - point to be checked.
     * @param p1 - first extrimity of the the segment.
     * @param p2 - second extrimity of the segment.
     * @return - distance in a static double.
     */
    static T pointSegmentDist(const Point2D<T>& p, const Point2D<T>& p1, const Point2D<T>& p2)
	{
        Vector2D<T> A = p1 - p;
        Vector2D<T> B = p2 - p1;
        Vector2D<T> C = p2 - p;
        auto a = A * A;
        auto b = B * B;
        auto c = C * C;
		if (c > a + b)
		{
			return sqrt(a);
		}
		if (a > b + c)
		{
			return sqrt(c);
		}
		else
		{
			return ((fabs(A ^ B)) / sqrt(b));
		}
	}

	/**      
     * @brief segmentIntersect - Tests if two segment intersect or not.
     * @param p1, p2 - first segment.
     * @param p3, p4 - second segment.
     * @return - true if the segments intersect and false otherwise.
     */
    static bool segmentIntersect( const Point2D<T>& p1, const Point2D<T>& p2, const Point2D<T>& p3, const Point2D<T>& p4 )
    {
        Point2D<T> A = p2 - p1, B = p4 - p3, C = p4 - p1, D = p3 - p1, E = p2 - p3, F = p1 - p3;
        int a = cmp((A % C) * (A % D)), b = cmp((E % B) * (F % B));
        return a <= 0 && b <= 0;
    }

    /**
     * @brief sqrNorm - compute the squared norm.
     * @return - the squared norm.
     */
    T sqrNorm( ) const
    {
        return _xp * _xp + _yp * _yp;
    }

    /**
     * @brief setPoint
     * @param p - new to coordinates to be set
     */
    void setPoint( const Vector2D<T>& p )
    {
        _xp = p._xp;
        _yp = p._yp;
    }

    /**
     * @brief setX - set a new x coordinate.
     * @param x - new x coordinate.
     */
    void setX(T x)
    {
        _xp = x;
    }

    /**
     * @brief setY - set a new y coordinate.
     * @param y - new y coordinate.
     */
    void setY(T y)
    {
        _yp = y;
    }
private:
    /**
     * Vector coordinates.
     */
    T _xp, _yp;
};
