#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include "EventConstants.h"
#include "GraphicsSceneWheelEvent.h"

namespace rm
{
GraphicsSceneWheelEvent::GraphicsSceneWheelEvent():GraphicsSceneEvent(EventType::WheelEvent)
{
}



GraphicsSceneWheelEvent::~GraphicsSceneWheelEvent()
{
}



MouseButtons GraphicsSceneWheelEvent::getButtons() const
{
    return _buttons;
}



int GraphicsSceneWheelEvent::getDelta() const
{
    return _delta;
}



KeyboardModifiers GraphicsSceneWheelEvent::getKeyboardModifiers() const
{
    return _keyboardModifiers;
}



const Orientation& GraphicsSceneWheelEvent::getOrientation() const
{
    return _orientation;
}



const Point2Df& GraphicsSceneWheelEvent::getPosition() const
{
    return _pos;
}



const Point2Df& GraphicsSceneWheelEvent::getGlobalPosition() const
{
    return _globalPos;
}



bool GraphicsSceneWheelEvent::hasButtonPressed() const
{
    return _buttons != static_cast<MouseButtons>(rm::MouseButton::NoButton);
}



bool GraphicsSceneWheelEvent::isLeftButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::LeftButton);
}



bool GraphicsSceneWheelEvent::isOnlyLeftButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::LeftButton);
}



bool GraphicsSceneWheelEvent::isRightButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::RightButton);
}



bool GraphicsSceneWheelEvent::isOnlyRightButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::RightButton);
}



bool GraphicsSceneWheelEvent::isMiddleButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::MiddleButton);
}



bool GraphicsSceneWheelEvent::isOnlyMiddleButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::MiddleButton);
}



bool GraphicsSceneWheelEvent::hasModifierPressed() const
{
    return _keyboardModifiers != static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);
}



bool GraphicsSceneWheelEvent::isControlKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneWheelEvent::isOnlyControlKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneWheelEvent::isShiftKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneWheelEvent::isOnlyShiftKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneWheelEvent::isAltKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



bool GraphicsSceneWheelEvent::isOnlyAltKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



void GraphicsSceneWheelEvent::setKeyboardModifiers(KeyboardModifiers modifiers)
{
    _keyboardModifiers = modifiers;
}



void GraphicsSceneWheelEvent::setButtons(MouseButtons buttons)
{
    _buttons = buttons;
}



void GraphicsSceneWheelEvent::setDelta (int delta)
{
    _delta = delta;
}



void GraphicsSceneWheelEvent::setPos(Point2Df pos)
{
    _pos = pos;
}



void GraphicsSceneWheelEvent::setGlobalPos(Point2Df globalPos)
{
    _globalPos = globalPos;
}



void GraphicsSceneWheelEvent::setOrientation(Orientation orientation)
{
    _orientation = orientation;
}

}
