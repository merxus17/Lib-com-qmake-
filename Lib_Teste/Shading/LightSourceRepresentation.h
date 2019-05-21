#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <QVector4D>
#include <QVector3D>
#include <QColor>
#include <assert.h>

#include "LightType.h"
#include "ShadingModel.h"
#include "ShadingModelRepresentation.h"

namespace rm
{

/**
 * Some alias to geometry and color types.
 */
using Point4D = QVector4D;
using Point3D = QVector3D;
using Color = QColor;

/**
 * Forward declaration to ShadingModelRepresentation.
 */
class ShadingModelRepresentation;

/**
 * @brief The LightSourceRepresentation class - Define the set of a light source parameters. This class can be shared
 * by many LoghtSource objects. For default values consult the following link in team drive:
 *  https://docs.google.com/document/d/1MsTiyex7RGM-BzgeG4f2-tH1quKSW9i4Nq7d2cCbPsk/edit
 */
class LightSourceRepresentation
{
public:
    /**
     * Allow LightSource to access the private members of LightSourceRepresentation.
     */
    friend class LightSource;

private:
    /**
     * @brief LightSourceRepresentation - Light source representation constructor.
     * @param t - Type of light source.
     */
	LightSourceRepresentation(LightType t);

    /**
     * Default destructor.
     */
    ~LightSourceRepresentation() = default;

    /**
     * @brief enable - Define if a light source is enable or not.
     * @param enableLight - Flag to enable/disable light.
     */
	void enable(bool enableLight);

    /**
     * @brief isEnable - verify if the light source ir enable or not.
     * @return - True if the light is enable and false otherwise.
     */
	bool isEnable() const;

    /**
     * @brief setLightPosition - Set the light source position in homogeneous coordinates. If w = 0, the light source is
     * positioned in infinity and this light is set as directional type.
     * @param p - Light position in homogeneous coordinates.
     */
    void setLightPosition(Point4D& p);

    /**
     * @brief setLightPosition - Set the light source position. As the position is defined in homogeneous coordinates,
     * w is set as w = 1 in this case.
     * @param p - Light position.
     */
    void setLightPosition(Point3D& p);

    /**
     * @brief getLightPosition - Get the current light position.
     * @return - Current light position.
     */
    const Point4D& getLightPosition() const;

    /**
     * @brief setDiffuseComponet - Set the light source diffuse component.
     * @param diffuse - Light source diffuse component.
     */
    void setDiffuseComponet(Color& diffuse);

    /**
     * @brief getDiffuseComponent - Get the light source diffuse component.
     * @return - Light source diffuse component
     */
    const Color& getDiffuseComponent() const;

    /**
     * @brief setAmbientComponent - Set the light source ambient component.
     * @param ambient - Light source ambient component
     */
    void setAmbientComponent(Color& ambient);

    /**
     * @brief getAmbientComponent - Get the light source ambient component.
     * @return - Light source ambient component.
     */
    const Color& getAmbientComponent() const;

    /**
     * @brief setSpecularComponent - Set the light source specular component.
     * @param specular - Light source specular component.
     */
    void setSpecularComponent(Color& specular);

    /**
     * @brief getSpecularComponent - Get the light source specular component.
     * @return - Light source specular component.
     */
    const Color& getSpecularComponent() const;

    /**
     * @brief setShininess - Set the shininess parameter. This parameter defines the factor used as expoent for the
     * cossine of the reflected angle. In practice as the shininess value increase the highlight decreases.
     * @param s - Shininess value.
     */
	void setShininess(float s);

    /**
     * @brief getShininess - Get the shininess parameter.This parameter is used as expoent for the cossine of the
     * reflected angle. In practice as the shininess value increase the highlight decreases.
     * @return - Shininess value.
     */
	float getShininess() const;

    /**
     * @brief setSpotDirection - Set the spot light direction.
     * @param d - Spot light direction
     */
    void setSpotDirection(Point3D& d);

    /**
     * @brief getSpotDirection - Get the spot light direction.
     * @return - Spot light direction
     */
    const Point3D& getSpotDirection() const;

    /**
     * @brief setCutoffAngle - Set the spot light cut-off angle.
     * @param a - Spot light cut-off angle.
     */
	void setCutoffAngle(float a);

    /**
     * @brief getCuttofAngle - Get the spot light cut-off angle.
     * @return - Spot light cut-off angle.
     */
	float getCuttofAngle() const;

    /**
     * @brief setConstantAttenuation - Set constant (c) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @param c - Constante attenuation value.
     */
	void setConstantAttenuation(float c);

    /**
     * @brief getConstantAttenuation - Get constant (c) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @return - Constante attenuation value.
     */
	float getConstantAttenuation() const;

    /**
     * @brief setLinearAttenuation - Set linear (l) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     * @param l - Linear attenuation value
     */
	void setLinearAttenuation(float l);

    /**
     * @brief getLinearAttenuation - Get linear (l) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     * @return - Linear attenuation value
     */
	float getLinearAttenuation() const;

    /**
     * @brief setQuadraticAttenuation - Set quadratic (q) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @param q - Quadratic attenuation.
     */
	void setQuadraticAttenuation(float q);

    /**
     * @brief getQuadraticAttenuation - Get quadratic (q) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @return - Quadratic attenuation value.
     */
	float getQuadraticAttenuation() const;

    /**
     * @brief setLightType - Define the light type. The available options is: directional, punctual and spot.
     * @param t - Light type.
     */
	void setLightType(LightType t);

    /**
     * @brief getLightType - Get the light type.
     * @return - Light type
     */
	LightType getLightType();

    /**
     * @brief setAttenuation - Define all attenuation parameters in one single function call.
     * @param q - Quadratic attenuation value.
     * @param l - Linear attenuation value.
     * @param c - Constant attenuation value.
     */
	void setAttenuation(float q, float l, float c);

private:
    /**
     * @brief _position - Define the light position in homogeneus coordinates. If w = 0 it means this light source is
     * positioned at the infinity.
     */
    Point4D _position = Point4D(0.0f, 0.0f, 1.0f, 0.0f);

    /**
     * @brief _diffuseComponent - Define the light source diffuse color component.
     */
    Color _diffuseComponent = Color(204, 204, 204, 255);

    /**
     * @brief _ambientComponent - Define the light source ambient color component.
     */
    Color _ambientComponent = Color(32, 32, 32, 255);

    /**
     * @brief _specularComponent - Define the light source specular color component.
     */
    Color _specularComponent = Color(255, 255, 255, 255);

    /**
     * @brief _shininess - Define the factor used as expoent for the cossine of the reflected angle. In practice as the
     * shininess value increase the highlight decreases.
     */
    float _shininess = 128.0f;

    /**
     * @brief _spotDirection - Define a direction for spot light sources.
     */
    Point3D _spotDirection = Point3D(0.0f, 0.0f, -1.0f);

    /**
     * @brief _cutoffAngle - Define the cutoff angle for spot light sources. This angle is defined in degrees and
     * represent the angle from
     * the center to the border.
     */
    float _cutoffAngle = 180.0f;

    /**
     * @brief _constantAttenuation - Constant (c) attenuation used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     */
    float _constantAttenuation = 1.0f;

    /**
     * @brief _linearAttenuation - Linear (l) attenuation used in attenuation equation 1 / (qd^2 + ld + c). Phisically
     * the light intensity decreases quadratically.
     */
    float _linearAttenuation = 0.0f;

    /**
     * @brief _quadraticAttenuation - Quadratic (q) attenuation used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     */
    float _quadraticAttenuation;

    /**
     * @brief _type - Define the light type.
     */
    LightType _type = LightType::Directional;
    
	/**
     * @brief _enabled flag controling if the light is enabled or not.
     */
    bool _enabled = true;
};
};
