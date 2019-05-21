#pragma once

#include <vector>
#include "LightSource.h"

namespace rm
{
class ShadingModelRepresentation
{
public:
    /**
     * Allow the ShadingModel objects to acess the ShadingModelRepresentation private region.
     */
    friend class ShadingModel;
protected:

private:
    /**
     * @brief ShadingModelRepresentation - Default constructor of class.
     */
    ShadingModelRepresentation() = default;

    /**
     * @brief ~ShadingModelRepresentation - Class destructor.
     */
    ~ShadingModelRepresentation();

    /**
     * @brief enableShadingModel - Enable shading model. This is equivalent to enable all light sources.
     */
    void enableShadingModel();

    /**
     * @brief disableShadingModel - Disable shading model. This is equivalent to disable all light sources.
     */
    void disableShadingModel();

    /**
     * @brief createLightSource - Add a new light source to the model. This light source is allocate by the GraphicsScene and returned to
     * the user.
     * @param type - LightType that will be added.
     * @return - Return the LightSource created by the user.
     */
    LightSource createLightSource(LightType type);

    /**
     * @brief getShadingModel - Return the shading model.
     * @return - Return the shading model.
     */
    std::vector<LightSource>& getShadingModel();

    /**
     * @brief getNumLightSources - Return the number of lights in the current shading model.
     * @return - Return the number of lights in the current shading model.
     */
    int getNumLightSources();

    /**
     * @brief getLightSource - Return the light source of index id.
     * @return - Return the light source of index id.
     */
    LightSource getLightSource(unsigned int id);

private:

    /**
     * @brief _shadingModel - A set of source lights defining the model.
     */
    std::vector<LightSource> _shadingModel;

    /**
     * @brief _isEnabled - True if shading Model is enabled, False if not.
     */
    bool _isEnabled;
};
}
