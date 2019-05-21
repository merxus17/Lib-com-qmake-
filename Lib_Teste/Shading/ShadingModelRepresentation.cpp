#include "ShadingModelRepresentation.h"
#include "LightType.h"
namespace rm
{
ShadingModelRepresentation::~ShadingModelRepresentation()
{
    for (size_t l = 0; l < _shadingModel.size(); l++)
    {
        _shadingModel[l].destroy();
    }
}



void ShadingModelRepresentation::enableShadingModel()
{
    _isEnabled = true;
}



void ShadingModelRepresentation::disableShadingModel()
{
    _isEnabled = false;
}



LightSource ShadingModelRepresentation::createLightSource(LightType type)
{
    //Create a new light source.
    LightSource l(type);

    //Add the new light source to the vector.
    _shadingModel.push_back(l);

    //Return new light source.
    return  l;
}



std::vector<LightSource>& ShadingModelRepresentation::getShadingModel()
{
    return _shadingModel;
}



int ShadingModelRepresentation::getNumLightSources()
{
    return static_cast<int>(_shadingModel.size());
}



LightSource ShadingModelRepresentation::getLightSource(unsigned int id)
{
    return _shadingModel[id];
}
}

