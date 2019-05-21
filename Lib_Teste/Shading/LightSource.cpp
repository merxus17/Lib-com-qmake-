#include <vector>
#include "LightSource.h"
#include "LightSourceRepresentation.h"

namespace rm
{
LightSource::LightSource(LightType t)
{
    _light = new LightSourceRepresentation(t);
}



void LightSource::destroy()
{
    delete _light;
}



void LightSource::enable(bool enableLight)
{
    if (_light != nullptr)
    {
        _light->enable(enableLight);
    }
}



bool LightSource::isEnable() const
{
    if (_light != nullptr)
    {
        return _light->isEnable();
    }
    return false;
}



void LightSource::setLightPosition(QVector4D& p)
{
    if (_light != nullptr)
    {
        _light->setLightPosition(p);
    }
}



void LightSource::setLightPosition(QVector3D& p)
{
    if (_light != nullptr)
    {
        _light->setLightPosition(p);
    }
}



const QVector4D& LightSource::getLightPosition() const
{
    return _light->getLightPosition();
}



void LightSource::setDiffuseComponet(QColor& diffuse)
{
    if (_light != nullptr)
    {
        _light->setDiffuseComponet(diffuse);
    }
}



const QColor& LightSource::getDiffuseComponent() const
{
    return _light->getDiffuseComponent();
}



void LightSource::setAmbientComponent(QColor& ambient)
{
    _light->setAmbientComponent(ambient);
}



const QColor& LightSource::getAmbientComponent() const
{
    return _light->getAmbientComponent();
}



void LightSource::setSpecularComponent(QColor& specular)
{
    _light->setSpecularComponent(specular);
}



const QColor& LightSource::getSpecularComponent() const
{
    return _light->getSpecularComponent();
}



void LightSource::setShininess(float s)
{
    if (_light != nullptr)
    {
        _light->setShininess(s);
    }
}



float LightSource::getShininess() const
{
    if (_light != nullptr)
    {
        return _light->getShininess();
    }
    return 0.0f;
}



void LightSource::setSpotDirection(QVector3D& d)
{
    if (_light != nullptr)
    {
        _light->setSpotDirection(d);
    }
}



const QVector3D& LightSource::getSpotDirection() const
{
    return _light->getSpotDirection();
}



void LightSource::setCutoffAngle(float a)
{
    _light->setCutoffAngle(a);
}



float LightSource::getCuttofAngle() const
{
    return _light->getCuttofAngle();
}



void LightSource::setConstantAttenuation(float c)
{
    if (_light != nullptr)
    {
        _light->setConstantAttenuation(c);
    }
}



float LightSource::getConstantAttenuation() const
{
    return _light->getConstantAttenuation();
}



void LightSource::setLinearAttenuation(float l)
{
    if (_light != nullptr)
    {
        _light->setLinearAttenuation(l);
    }
}



float LightSource::getLinearAttenuation() const
{
    return _light->getLinearAttenuation();
}



void LightSource::setQuadraticAttenuation(float q)
{
    if (_light != nullptr)
    {
        _light->setQuadraticAttenuation(q);
    }
}



float LightSource::getQuadraticAttenuation() const
{
    return _light->getQuadraticAttenuation();
}



void LightSource::setLightType(LightType t)
{
    _light->setLightType(t);
}



LightType LightSource::getLightType()
{
    return _light->getLightType();
}



void LightSource::setAttenuation(float q, float l, float c)
{
    _light->setAttenuation(q, l, c);
}



LightSource& LightSource::operator=(const LightSource& m)
{
    _light = m._light;
    return *this;
}
}
