
#include "GraphicsSceneHoverEvent.h"
#include "EventConstants.h"
namespace rm
{

GraphicsSceneHoverEvent::GraphicsSceneHoverEvent():GraphicsSceneEvent(EventType::HoverEvent)
{
}



KeyboardModifiers GraphicsSceneHoverEvent::getKeyboardModifiers() const
{
    return _keyboardModifiers;
}



const Point2Df& GraphicsSceneHoverEvent::getGlobalPositon() const
{
    return _globalPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getLocalPosition() const
{
    return _localPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getScreenPosition() const
{
    return _screenPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getWindowPosition() const
{
    return _windowPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getFirstWorldPosition() const
{
    return _firstWorldPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getWorldPosition() const
{
    return _worldPosition;
}



const Point2Df& GraphicsSceneHoverEvent::getOldWorldPosition() const
{
    return _oldWorldPosition;
}



bool GraphicsSceneHoverEvent::hasModifierPressed() const
{
    return _keyboardModifiers != static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);
}



bool GraphicsSceneHoverEvent::isControlKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneHoverEvent::isOnlyControlKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneHoverEvent::isShiftKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneHoverEvent::isOnlyShiftKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneHoverEvent::isAltKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



bool GraphicsSceneHoverEvent::isOnlyAltKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}




void GraphicsSceneHoverEvent::setKeyboardModifiers(KeyboardModifiers modifiers)
{
    _keyboardModifiers = modifiers;
}



void GraphicsSceneHoverEvent::setGlobalPosition(const Point2Df& global)
{
    _globalPosition = global;
}



void GraphicsSceneHoverEvent::setLocalPosition(const Point2Df& local)
{
    _localPosition = local;
}



void GraphicsSceneHoverEvent::setScreenPosition(const Point2Df& screen)
{
    _screenPosition = screen;
}



void GraphicsSceneHoverEvent::setWindowPosition(const Point2Df& window)
{
    _windowPosition = window;
}



void GraphicsSceneHoverEvent::setFirstWorldPosition(const Point2Df& world)
{
    _firstWorldPosition = world;
}



void GraphicsSceneHoverEvent::setWorldPosition(const Point2Df& world)
{
    _worldPosition = world;
}


void GraphicsSceneHoverEvent::setOldWorldPosition(const Point2Df& old)
{
    _oldWorldPosition = old;
}
}

