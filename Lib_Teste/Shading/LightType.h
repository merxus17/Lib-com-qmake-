#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>


/**
 * @brief The LightType enum - Define all possibles light types.
 *
 * Directional light. This kind of lights has no fixed position. It is assumed to be in the infinity.
 * Punctual lights has a position in 3D space well defined. It is like  a lamp in a room.
 * Spot light is a special case of punctual light. It has a cut-off angle to iluminate a region and a specific direction.
 */
enum class LightType : unsigned char
{
    // Directional light. This kind of lights has no fixed position. It is assumed to be in the infinity.
    Directional     = 0x01,

    // Punctual lights has a position in 3D space well defined. It is like a lamp in a room.
    Punctual        = 0x02,

    // Spot light is a special case of punctual light. It has a cut-off angle to iluminate a region and a specific
    //direction.
    Spot            = 0x04
};
