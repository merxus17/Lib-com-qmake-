#pragma once

#include "LightType.h"
#include "LightSource.h"

namespace rm
{
/**
 * Forward declaration to ShadingModelRepresentation.
 */
//class LightSource;
class ShadingModelRepresentation;

/**
 * @brief The ShadingModel class - Define a shading model in a view 3D.
 */
class ShadingModel
{
public:
    /**
     * Allow GraphicsScene object to access the private members and attributtes of ShadingModel objects.
     */
    friend class GraphicsScene;
public:
    /**
     * @brief ShadingModel - Default constructor. Do not construct a valid object. A valid object must be constructed
     * by GraphicsScene object.
     */
    ShadingModel() = default;

    /**
     * @brief disableShadingModel - Disable shading model. This is equivalent to disable all light sources.
     */
    void disableShadingModel();

    /**
     * @brief enableShadingModel - Enable shading model. This is equivalent to enable all light sources.
     */
    void enableShadingModel();

    /**
     * @brief addLightSource - Add a new light  source to the model. This light source is allocate by the
     * ShadingModel and returned to the user.
     * @param t - Light type.
     * @return - Returns a new light source.
     */
    LightSource createLightSource(LightType t);

    /**
     * @brief getLightSources - Returns all created light sources.
     * @return - Returns vector with all created light sources.
     */
	std::vector<LightSource>& getLightSources();

    /**
     * @brief getNumLightSource - Return the number of lights in the current shading model.
     * @return - Returns number of lights.
     */
	int getNumLightSource() const;

    /**
     * @brief getLightSource - Return the light source of index s.
     * @param s - Index of the light.
     * @return - Returns the light source.
     */
    LightSource getLightSource(unsigned int s) const;

    /**
     * @brief operator= - Create a new reference to the shading model.
     * @param m - Shading model to be copied.
     * @return - Returns a reference to new resulting object.
     */
    ShadingModel& operator=(const ShadingModel& m) noexcept;

private:
    /**
     * @brief ShadingModel - Private (and real) constructor. This constructor is used to a GraphicsScene object to
     * create a new ShadingModel Model.
     * @param enable - Flag to determine if the model is created enabled or disabled.
     */
    ShadingModel(bool enable);

    /**
     * @brief destroy - Real destructor. This functions is used by a GraphicsScene object to delete the shading model
     * representation.
     */
    void destroy();
private:
    /**
     * @brief _model - A set of source lights defining the shading model.
     */
    ShadingModelRepresentation* _model = nullptr;
};
}
