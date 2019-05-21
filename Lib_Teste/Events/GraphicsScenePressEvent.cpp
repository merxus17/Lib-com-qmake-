#include <vector>

#include "GraphicsScenePressEvent.h"
namespace rm
{
GraphicsScenePressEvent::GraphicsScenePressEvent():GraphicsSceneEvent(EventType::PressEvent)
{
}


MouseButton GraphicsScenePressEvent::getButton() const
{
    return _button;
}



KeyboardModifiers GraphicsScenePressEvent::getKeyboardModifiers() const
{
    return _keyboardModifiers;
}



const Point2Df& GraphicsScenePressEvent::getGlobalPosition() const
{
    return _globalPosition;
}



const Point2Df& GraphicsScenePressEvent::getLocalPosition() const
{
    return _localPosition;
}



const Point2Df& GraphicsScenePressEvent::getScreenPosition() const
{
    return _screenPosition;
}



const Point2Df& GraphicsScenePressEvent::getWindowPosition() const
{
    return _windowPosition;
}



const Point2Df& GraphicsScenePressEvent::getWorldPosition() const
{
    return _worldPosition;
}



MouseButtonAction GraphicsScenePressEvent::getMouseButtonAction() const
{
    return _action;
}



bool GraphicsScenePressEvent::isLeftButtonPressed() const
{
    return _button == static_cast<MouseButton>(rm::MouseButton::LeftButton);
}



bool GraphicsScenePressEvent::isRightButtonPressed() const
{
    return _button == static_cast<MouseButton>(rm::MouseButton::RightButton);
}



bool GraphicsScenePressEvent::isMiddleButtonPressed() const
{
    return _button == static_cast<MouseButton>(rm::MouseButton::MiddleButton);
}



bool GraphicsScenePressEvent::hasModifierPressed() const
{
    return _keyboardModifiers != static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);
}



bool GraphicsScenePressEvent::isControlKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsScenePressEvent::isOnlyControlKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool GraphicsScenePressEvent::isShiftKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsScenePressEvent::isOnlyShiftKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool GraphicsScenePressEvent::isAltKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



bool GraphicsScenePressEvent::isOnlyAltKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



void GraphicsScenePressEvent::setButton(MouseButton button)
{
    _button = button;
}



void GraphicsScenePressEvent::setKeyboardModifiers(KeyboardModifiers modifiers)
{
    _keyboardModifiers = modifiers;
}



void GraphicsScenePressEvent::setGlobalPosition(const Point2Df& global)
{
    _globalPosition = global;
}



void GraphicsScenePressEvent::setLocalPosition(const Point2Df& local)
{
    _localPosition = local;
}



void GraphicsScenePressEvent::setScreenPosition(const Point2Df& screen)
{
    _screenPosition = screen;
}



void GraphicsScenePressEvent::setWindowPosition(const Point2Df& window)
{
    _windowPosition = window;
}



void GraphicsScenePressEvent::setWorldPosition(const Point2Df& world)
{
    _worldPosition = world;
}



void GraphicsScenePressEvent::setMouseButtonAction(MouseButtonAction action)
{
    _action = action;
}
}
