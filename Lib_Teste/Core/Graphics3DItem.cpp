#include "Graphics3DItem.h"

namespace rm
{
Graphics3DItem::~Graphics3DItem()
{

}

void Graphics3DItem::setViewMatrix(const OpenGLMatrix& viewMatrix)
{
    _viewMatrix = viewMatrix;
}



void Graphics3DItem::setShadingModel(ShadingModel m)
{
    _shadingModel = m;
}



const AABB3D& Graphics3DItem::getAABB() const
{
    return _aabb;
}



AABB3D& Graphics3DItem::getAABB()
{
    return _aabb;
}

};
