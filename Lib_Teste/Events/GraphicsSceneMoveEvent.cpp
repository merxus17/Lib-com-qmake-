
#include "GraphicsSceneMoveEvent.h"
#include "EventConstants.h"
namespace rm
{

GraphicsSceneMoveEvent::GraphicsSceneMoveEvent():GraphicsSceneEvent(EventType::MoveEvent)
{
}



MouseButtons GraphicsSceneMoveEvent::getButtons() const
{
    return _buttons;
}



KeyboardModifiers GraphicsSceneMoveEvent::getKeyboardModifiers() const
{
    return _keyboardModifiers;
}



const Point2Df& GraphicsSceneMoveEvent::getGlobalPosition() const
{
    return _globalPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getLocalPosition() const
{
    return _localPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getScreenPosition() const
{
    return _screenPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getWindowPosition() const
{
    return _windowPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getFirstWorldPosition() const
{
    return _firstWorldPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getWorldPosition() const
{
    return _worldPosition;
}



const Point2Df& GraphicsSceneMoveEvent::getOldWorldPosition() const
{
    return _oldWorldPosition;
}



bool GraphicsSceneMoveEvent::hasButtonPressed() const
{
    return _buttons != static_cast<MouseButtons>(rm::MouseButton::NoButton);
}



bool GraphicsSceneMoveEvent::isLeftButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::LeftButton);
}



bool GraphicsSceneMoveEvent::isOnlyLeftButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::LeftButton);
}



bool GraphicsSceneMoveEvent::isRightButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::RightButton);
}



bool GraphicsSceneMoveEvent::isOnlyRightButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::RightButton);
}



bool GraphicsSceneMoveEvent::isMiddleButtonPressed() const
{
    return _buttons & static_cast<MouseButtons>(rm::MouseButton::MiddleButton);
}



bool GraphicsSceneMoveEvent::isOnlyMiddleButtonPressed() const
{
    return _buttons == static_cast<MouseButtons>(rm::MouseButton::MiddleButton);
}



bool GraphicsSceneMoveEvent::hasModifierPressed() const
{
    return _keyboardModifiers != static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);
}



bool GraphicsSceneMoveEvent::isControlKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneMoveEvent::isOnlyControlKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsSceneMoveEvent::isShiftKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneMoveEvent::isOnlyShiftKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsSceneMoveEvent::isAltKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



bool GraphicsSceneMoveEvent::isOnlyAltKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



void GraphicsSceneMoveEvent::setButtons(MouseButtons buttons)
{
    _buttons = buttons;
}



void GraphicsSceneMoveEvent::setKeyboardModifiers(KeyboardModifiers modifiers)
{
    _keyboardModifiers = modifiers;
}



void GraphicsSceneMoveEvent::setGlobalPosition(const Point2Df& global)
{
    _globalPosition = global;
}



void GraphicsSceneMoveEvent::setLocalPosition(const Point2Df& local)
{
    _localPosition = local;
}



void GraphicsSceneMoveEvent::setScreenPosition(const Point2Df& screen)
{
    _screenPosition = screen;
}



void GraphicsSceneMoveEvent::setWindowPosition(const Point2Df& window)
{
    _windowPosition = window;
}



void GraphicsSceneMoveEvent::setFirstWorldPosition(const Point2Df& world)
{
    _firstWorldPosition = world;
}



void GraphicsSceneMoveEvent::setWorldPosition(const Point2Df& world)
{
    _worldPosition = world;
}


void GraphicsSceneMoveEvent::setOldWorldPosition(const Point2Df& old)
{
    _oldWorldPosition = old;
}
}

