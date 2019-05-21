#pragma once
#include <vector>
#include <map>
#include "../../Geometry/Vector2D.h"
namespace rm
{
/**
 * @brief Tri3ToTri6Conversor - convert a linear triangle mesh to a quadratic triangle mesh.
 * @param mesh - linear triangle mesh indexes.
 * @param points - points
 * @param randomMove - if true the new vertices is randomly moved. If false, new points are computed over the edge
 * midpoint.
 */
void Tri3ToTri6Conversor(std::vector<unsigned int>& mesh, std::vector<Point2Df>& points, bool randomMove = false)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    std::map<std::pair<std::size_t, std::size_t>, std::size_t> edges;
    std::size_t numberOfTriangles = mesh.size() / 3;
    std::vector<unsigned int> newMesh;
    newMesh.reserve(6 * numberOfTriangles);

    for (std::size_t t = 0; t < numberOfTriangles; t++)
    {
        //Copy linear triangle.
        for (std::size_t c = 3 * t; c <= 3 * t + 2; c++)
        {
            newMesh.push_back(mesh[c]);
        }

        //Add new vertices.
        for (std::size_t c = 3 * t; c <= 3 * t + 2; c++)
        {
            unsigned int i = std::min(mesh[c], mesh[3 * t + (c + 1) % 3]);
            unsigned int j = std::max(mesh[c], mesh[3 * t + (c + 1) % 3]);

            std::pair<std::size_t, std::size_t> newEdge(i, j);

            auto it =  edges.find(newEdge);
            if (it != edges.end())
            {
                newMesh.push_back(static_cast<unsigned int>(it->second));
            }
            else
            {
                //Compute midle point.
                Point2Df newPoint = 0.5 * (points[newEdge.first] + points[newEdge.second]);
                if  (randomMove)
                {
                    Point2Df dir = points[newEdge.first] - points[newEdge.second];
                    float l = dir.norm();
                    Point2Df dirMove(2 * (rand() % 100) / 99.0f - 1.0f, 2 * (rand() % 100) / 99.0f - 1.0f);
                    dirMove.normalize();

                    float t = ((rand() % 100) / 99.0f) / 10.0f;
                    newPoint += (l * dirMove) * t;
                }

                //Add new point.
                edges[newEdge] = points.size();
                newMesh.push_back(static_cast<unsigned int>(points.size()));
                points.push_back(newPoint);
            }
        }
    }

    mesh = newMesh;
    points.shrink_to_fit();
}
}
