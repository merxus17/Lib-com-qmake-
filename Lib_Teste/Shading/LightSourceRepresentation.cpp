#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "LightSourceRepresentation.h"

namespace rm
{
LightSourceRepresentation::LightSourceRepresentation(LightType t) :
    _type(t)
{

}



void LightSourceRepresentation::enable(bool enableLight)
{
    _enabled = enableLight;
}



bool LightSourceRepresentation::isEnable() const
{
    return _enabled;
}



void LightSourceRepresentation::setLightPosition(Point4D& p)
{
    _position = p;
}



void LightSourceRepresentation::setLightPosition(Point3D& p)
{
    _position = Point4D(p.x(), p.y(), p.z(), 1);
}



const Point4D& LightSourceRepresentation::getLightPosition() const
{
    return _position;
}



void LightSourceRepresentation::setDiffuseComponet(Color& diffuse)
{
    _diffuseComponent = diffuse;
}



const Color& LightSourceRepresentation::getDiffuseComponent() const
{
    return _diffuseComponent;
}



void LightSourceRepresentation::setAmbientComponent(Color& ambient)
{
    _ambientComponent = ambient;
}



const Color& LightSourceRepresentation::getAmbientComponent() const
{
    return _ambientComponent;
}



void LightSourceRepresentation::setSpecularComponent(Color& specular)
{
    _specularComponent = specular;
}



const Color& LightSourceRepresentation::getSpecularComponent() const
{
    return _specularComponent;
}



void LightSourceRepresentation::setShininess(float s)
{
    _shininess = s;
}



float LightSourceRepresentation::getShininess() const
{
    return _shininess;
}



void LightSourceRepresentation::setSpotDirection(Point3D& d)
{
    _spotDirection = d;
}



const Point3D& LightSourceRepresentation::getSpotDirection() const
{
    return _spotDirection;
}



void LightSourceRepresentation::setCutoffAngle(float a)
{
    _cutoffAngle = a;
}



float LightSourceRepresentation::getCuttofAngle() const
{
    return _cutoffAngle;
}



void LightSourceRepresentation::setConstantAttenuation(float c)
{
    _cutoffAngle = c;
}



float LightSourceRepresentation::getConstantAttenuation() const
{
    return _constantAttenuation;
}



void LightSourceRepresentation::setLinearAttenuation(float l)
{
    _linearAttenuation = l;
}



float LightSourceRepresentation::getLinearAttenuation() const
{
    return _linearAttenuation;
}



void LightSourceRepresentation::setQuadraticAttenuation(float q)
{
    _quadraticAttenuation = q;
}



float LightSourceRepresentation::getQuadraticAttenuation() const
{
    return _quadraticAttenuation;
}



void LightSourceRepresentation::setLightType(LightType t)
{
    _type = t;
}



LightType LightSourceRepresentation::getLightType()
{
    return _type;
}



void LightSourceRepresentation::setAttenuation(float q, float l, float c)
{
    _quadraticAttenuation = q;
    _linearAttenuation = l;
    _constantAttenuation = c;
}
}
