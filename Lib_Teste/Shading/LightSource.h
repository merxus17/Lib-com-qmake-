#pragma once

#include <vector>
#include<QVector3D>
#include<QVector4D>
#include <QColor>
#include "LightType.h"

namespace rm
{
using Color = QColor;
class LightSourceRepresentation;
/**
 * @brief The LightSource class - represent a light source properties.
 */
class LightSource
{
public:
    /**
     * Set ShadingModelRepresentation as friend of LightSource to allow ShadingModelRepresentation access the private
     * region of LightSource.
     */
    friend class ShadingModelRepresentation;
public:
    /**
     * @brief LightSource - Default constructor. This constructor do not allocate a light source representation.
     */
    LightSource() = default;

    /**
     * @brief enable - Define if a light source is enable or not.
     * @param enableLight - Receives true if it is enable and false if it isn't
     */
    void enable(bool enableLight);

    /**
     * @brief isEnable - Get the flag that defines if a light source is enable or not.
     * @return - Returns true if the light is enabled and false if it isn't
     */
    bool isEnable() const;

    /**
     * @brief setLightPosition - Set the light source position in homogeneous coordinates. If w = 0, the light source is
     * positioned  in infinity.
     * @param p - Position of the light
     */
    void setLightPosition(QVector4D& p);

    /**
     * @brief setLightPosition - Set the light source position. As the position is defined in homogeneous coordinates, w
     * is set as w = 1 in this case.
     * @param p - Position of the light
     */
    void setLightPosition(QVector3D& p);

    /**
     * @brief getLightPosition - Get the current light position.
     * @return - Returns the position of the light
     */
    const QVector4D& getLightPosition() const;

    /**
     * @brief setDiffuseComponet - Set the light source diffuse component.
     * @param diffuse - Color of the diffuse light
     */
    void setDiffuseComponet(Color& diffuse);

    /**
     * @brief getDiffuseComponent - Get the light source diffuse component.
     */
    const Color& getDiffuseComponent() const;

    /**
     * @brief setAmbientComponent - Set the light source ambient component.
     * @param ambient - Color of the ambient component
     */
    void setAmbientComponent(Color& ambient);

    /**
     * @brief getAmbientComponent - Get the light source ambient component.
     * @return - Returns the color of the ambient component
     */
    const Color& getAmbientComponent() const;

    /**
     * @brief setSpecularComponent - Set the light source specular component.
     * @param specular - Color of the specular component
     */
    void setSpecularComponent(Color& specular);

    /**
     * @brief getSpecularComponent - Get the light source specular component.
     * @return - Returns the color of the specular component
     */
    const Color& getSpecularComponent() const;

    /**
     * @brief setShininess - Set the shininess parameter. This parameter defines the factor used as expoent for the
     * cossine of the reflected angle. In practice as the shininess value increase the highlight decreases.
     * @param s - Shininess parameter
     */
    void setShininess(float s);

    /**
     * @brief getShininess - Get the shininess parameter.This parameter is used as expoent for the cossine of the
     * reflected angle. In practice as the shininess value increase the highlight decreases.
     * @return - Returns the shininess
     */
    float getShininess() const;

    /**
     * @brief setSpotDirection - Set the spot light direction.
     * @param d - The direction of the spot light
     */
    void setSpotDirection(QVector3D& d);

    /**
     * @brief getSpotDirection - Get the spot light direction.
     * @return - Returns the direction of the spot light
     */
    const QVector3D& getSpotDirection() const;

    /**
     * @brief setCutoffAngle - Set the spot light cut-off angle.
     * @param a - The cut-off angle of the spot light
     */
    void setCutoffAngle(float a);

    /**
     * @brief getCuttofAngle - Get the spot light cut-off angle.
     * @return - Returns the cut-off angle of the spot light
     */
    float getCuttofAngle() const;

    /**
     * @brief setConstantAttenuation - Set constant (c) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     * @param c - The constant term
     */
    void setConstantAttenuation(float c);

    /**
     * @brief getConstantAttenuation - Get constant (c) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     *  Phisically the light intensity decreases quadratically.
     * @return - Returns the constant term
     */
    float getConstantAttenuation() const;

    /**
     * @brief setLinearAttenuation - Set linear (l) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     * @param l - The linear term
     */
    void setLinearAttenuation(float l);

    /**
     * @brief getLinearAttenuation - Get linear (l) attenuation value used in attenuation equation 1 / (qd^2 + ld + c).
     * Phisically the light intensity decreases quadratically.
     * @return - Returns the linear term
     */
    float getLinearAttenuation() const;

    /**
     * @brief setQuadraticAttenuation - Set quadratic (q) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @param q - The quadratic term
     */
    void setQuadraticAttenuation(float q);

    /**
     * @brief getQuadraticAttenuation - Get quadratic (q) attenuation value used in attenuation equation
     * 1 / (qd^2 + ld + c). Phisically the light intensity decreases quadratically.
     * @return - Returns the quadratic term
     */
    float getQuadraticAttenuation() const;

    /**
     * @brief setLightType - Define the light type. The available options is: directional, punctual and spot.
     * @param t - The type of the light, if it is directional, punctual or spot
     */
    void setLightType(LightType t);

    /**
     * @brief getLightType - Get the light type.
     * @return - Returns the light type
     */
    LightType getLightType();

    /**
     * @brief setAttenuation - Define all attenuation parameters in one single function call.
     * @param q - Quadratic term
     * @param l - Linear term
     * @param c - Constant term
     */
    void setAttenuation(float q, float l, float c);

    /**
     * @brief The overload of the operator = to copy a light source to the
     * current light source.
     * @param m - The light source that must be copied to the current light source.
     * @return - Returns the resultant light source.
     */
    LightSource& operator=(const LightSource& m);

private:
    /**
     * @brief LightSource - Private light source construtor. This constructor allocate a light source representation.
     */
    LightSource(LightType t);

    /**
     * @brief destroy - Function to destroy de light source representation. This function is private because just a friend
     * class that allocate the light source can destroy it.
     */
    void destroy();

private:
    /**
     * @brief _light - Pointer to real LightSource. Each LightSource object stores just a reference for the light source information.
     * This is a facilite to alow share light sources without copy. With one single representation, each change is
     * automaticaly updated in all source lights.
     */
    LightSourceRepresentation* _light = nullptr;
};
}
