#pragma once

#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <QVector3D>
#include "../Geometry/Vector2D.h"
using Point3Df = QVector3D;

namespace loader
{
/**
 * @brief readerFileOFF - Reads .off file of a 2D/3D mesh.
 * @param filename - Name/Path of the file that will be read.
 * @param points - Vector of the points (QVector3D) of the mesh.
 * @param polygons - Vector with the index of the points(vector<unsigned int>) that for the polygon.
 */
void readFileOFF(const std::string& filename, std::vector<QVector3D>& points, std::vector<std::vector<unsigned int>>& polygons);

/**
 * @brief readTriangle3DFileOFF -  Reads .off file of a 3D triangle mesh.
 * @param filename - Name/Path of the file that will be read.
 * @param points - Vector of the points (QVector3D) of the mesh.
 * @param triangles - Vector with the index of the points that for the polygon.
 */
void readTriangle3DFileOFF(const std::string& filename, std::vector<Point3Df>& points, std::vector<unsigned int>& triangles);

/**
 * @brief readerFileOFF - Reads .off file of a 2D triangle mesh.
 * @param filename - Name/Path of the file that will be read.
 * @param points - Vector of the points (Point2Df) of the mesh.
 * @param triangles - Vector with the index of the points that for the polygon.
 */
void readTriangle2DFileOFF(const std::string& filename, std::vector<Point2Df>& points, std::vector<unsigned int>& triangles);

/**
 * @brief readerFileOFF - Reads .off file of a 3D quadrilateral mesh.
 * @param filename - Name/Path of the file that will be read.
 * @param points - Vector of the points (QVector3D) of the mesh.
 * @param quads - Vector with the index of the points that for the polygon.
 */
void readQuad3DFileOFF(const std::string& filename, std::vector<Point3Df>& points, std::vector<unsigned int>& quads);

/**
 * @brief readerFileOFF - Reads .off file of a 2D quadrilateral mesh.
 * @param filename - Name/Path of the file that will be read.
 * @param points - Vector of the points (QVector3D) of the mesh.
 * @param quads - Vector with the index of the points that for the polygon.
 */
void readQuad2DFileOFF(const std::string& filename, std::vector<Point2Df>& points, std::vector<unsigned int>& quads);
}
