#include "ShadingModelRepresentation.h"
#include "ShadingModel.h"
namespace rm
{
ShadingModel::ShadingModel(bool enable)
{
    _model = new ShadingModelRepresentation();
    enable == true ? _model->enableShadingModel() : _model->disableShadingModel();
}



void ShadingModel::destroy()
{
    delete _model;
}



void ShadingModel::disableShadingModel()
{
    assert(_model);
    _model->disableShadingModel();
}



void ShadingModel::enableShadingModel()
{
    assert(_model);
    _model->enableShadingModel();
}



LightSource ShadingModel::createLightSource(LightType t)
{
    assert(_model);
    return _model->createLightSource(t);
}



std::vector<LightSource>& ShadingModel::getLightSources()
{
    assert(_model);
    return _model->getShadingModel();
}



int ShadingModel::getNumLightSource() const
{
    assert(_model);
    return _model->getNumLightSources();
}



LightSource ShadingModel::getLightSource(unsigned int s) const
{
    assert(_model);
    return _model->getLightSource(s);
}



ShadingModel& ShadingModel::operator=(const ShadingModel& m) noexcept
{
    _model = m._model;
    return  *this;
}
}
