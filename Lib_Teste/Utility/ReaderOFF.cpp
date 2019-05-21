#include "ReaderOFF.h"

namespace loader
{
void readFileOFF(const std::string& filename, std::vector<Point3Df>& points,
                 std::vector<std::vector<unsigned int>>& polygons)
{
    std::ifstream ifile;
    ifile.open(filename);

    if (!ifile.is_open())
    {
        std::cout << "Could not open " << std::endl;
        perror(filename.c_str());
        return;
    }

    char OFF[100];
    unsigned int numPoints;
    unsigned int numPolygons;
    unsigned int numFaces;

    ifile >> OFF; //discarding the first line of the file .off
    ifile >> numPoints;
    ifile >> numPolygons;
    ifile >> numFaces;

    points.resize(numPoints);
    polygons.resize(numPolygons);

    //reading the points
    for (unsigned int i = 0; i < numPoints; i++)
    {
        float x, y, z;

        ifile >> x;
        ifile >> y;
        ifile >> z;

        QVector3D point;
        point.setX(x);
        point.setY(y);
        point.setZ(z);

        points[i] = point;
    }

    //reading the polygons
    for (unsigned int i = 0; i < numPolygons; i++)
    {
        unsigned int numOfPoints; //how many points the polygon have

        ifile >> numOfPoints;

        std::vector<unsigned int> pointID;
        pointID.resize(numOfPoints);

        //reading the point IDs
        for (unsigned int j = 0; j < numOfPoints; j++)
        {
            ifile >> pointID[j];
        }

        polygons[i] = pointID;
    }

    ifile.close();
}



void readTriangle3DFileOFF(const std::string& filename, std::vector<Point3Df>& points,
                           std::vector<unsigned int>& triangles)
{
    std::ifstream ifile;
    ifile.open(filename);

    if (!ifile.is_open())
    {
        std::cout << "Could not open " << std::endl;
        perror(filename.c_str());
        return;
    }

    char OFF[100];
    unsigned int numPoints;
    unsigned int numTriangles;
    unsigned int numFaces;

    ifile >> OFF; //discarding the first line of the file .off
    ifile >> numPoints;
    ifile >> numTriangles;
    ifile >> numFaces;

    points.resize(numPoints);
    triangles.resize(3 * numTriangles);

    //reading the points
    for (unsigned int i = 0; i < numPoints; i++)
    {
        float x, y, z;

        ifile >> x;
        ifile >> y;
        ifile >> z;

        QVector3D point;
        point.setX(x);
        point.setY(y);
        point.setZ(z);

        points[i] = point;
    }

    //reading the polygons
    for (unsigned int i = 0; i < numTriangles; i++)
    {
        unsigned int numOfPoints; //how many points the polygon have

        ifile >> numOfPoints;

        ifile >> triangles[3 * i + 0];
        ifile >> triangles[3 * i + 1];
        ifile >> triangles[3 * i + 2];
    }

    ifile.close();
}



void readTriangle2DFileOFF(const std::string& filename, std::vector<Point2Df>& points,
                           std::vector<unsigned int>& triangles)
{
    std::ifstream ifile;
    ifile.open(filename);

    if (!ifile.is_open())
    {
        std::cout << "Could not open " << std::endl;
        perror(filename.c_str());
        return;
    }

    char OFF[100];
    unsigned int numPoints;
    unsigned int numTriangles;
    unsigned int numFaces;

    ifile >> OFF; //discarding the first line of the file .off
    ifile >> numPoints;
    ifile >> numTriangles;
    ifile >> numFaces;

    points.resize(numPoints);
    triangles.resize(3 * numTriangles);

    //reading the points
    for (unsigned int i = 0; i < numPoints; i++)
    {
        float x, y, z;

        ifile >> x;
        ifile >> y;
        ifile >> z;

        Point2Df point;
        point[0] = x;
        point[1] = y;

        points[i] = point;
    }

    //reading the polygons
    for (unsigned int i = 0; i < numTriangles; i++)
    {
        unsigned int numOfPoints; //how many points the polygon have

        ifile >> numOfPoints;

        ifile >> triangles[3 * i + 0];
        ifile >> triangles[3 * i + 1];
        ifile >> triangles[3 * i + 2];
    }

    ifile.close();
}



void readQuad3DFileOFF(const std::string& filename, std::vector<Point3Df>& points,
                              std::vector<unsigned int>& quads)
{
    std::ifstream ifile;
    ifile.open(filename);

    if (!ifile.is_open())
    {
        std::cout << "Could not open " << std::endl;
        perror(filename.c_str());
        return;
    }

    char OFF[100];
    unsigned int numPoints;
    unsigned int numQuads;
    unsigned int numFaces;

    ifile >> OFF; //discarding the first line of the file .off
    ifile >> numPoints;
    ifile >> numQuads;
    ifile >> numFaces;

    points.resize(numPoints);
    quads.resize(4 * numQuads);

    //reading the points
    for (unsigned int i = 0; i < numPoints; i++)
    {
        float x, y, z;

        ifile >> x;
        ifile >> y;
        ifile >> z;

        Point3Df point;
        point.setX(x);
        point.setY(y);
        point.setZ(z);

        points[i] = point;
    }

    //reading the polygons
    for (unsigned int i = 0; i < numQuads; i++)
    {
        unsigned int numOfPoints; //how many points the polygon have

        ifile >> numOfPoints;

        ifile >> quads[4 * i + 0];
        ifile >> quads[4 * i + 1];
        ifile >> quads[4 * i + 2];
        ifile >> quads[4 * i + 3];
    }

    ifile.close();
}



void readQuad2DFileOFF(const std::string& filename, std::vector<Point2Df>& points,
                              std::vector<unsigned int>& quads)
{
    std::ifstream ifile;
    ifile.open(filename);

    if (!ifile.is_open())
    {
        std::cout << "Could not open " << std::endl;
        perror(filename.c_str());
        return;
    }

    char OFF[100];
    unsigned int numPoints;
    unsigned int numQuads;
    unsigned int numFaces;

    ifile >> OFF; //discarding the first line of the file .off
    ifile >> numPoints;
    ifile >> numQuads;
    ifile >> numFaces;

    points.resize(numPoints);
    quads.resize(4 * numQuads);

    //reading the points
    for (unsigned int i = 0; i < numPoints; i++)
    {
        float x, y, z;

        ifile >> x;
        ifile >> y;
        ifile >> z;

        Point2Df point;
        point[0] = x;
        point[1] = y;

        points[i] = point;
    }

    //reading the polygons
    for (unsigned int i = 0; i < numQuads; i++)
    {
        unsigned int numOfPoints; //how many points the polygon have

        ifile >> numOfPoints;

        ifile >> quads[4 * i + 0];
        ifile >> quads[4 * i + 1];
        ifile >> quads[4 * i + 2];
        ifile >> quads[4 * i + 3];
    }

    ifile.close();
}
}
