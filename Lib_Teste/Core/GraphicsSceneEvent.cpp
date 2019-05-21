#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include "../Events/EventConstants.h"
#include "GraphicsSceneEvent.h"
#include <QEvent>
namespace rm
{

GraphicsSceneEvent::GraphicsSceneEvent(EventType eventType) : _eventType(eventType)
{
}


MouseButtons GraphicsSceneEvent::convertMouseButtons(unsigned int qtButtons) const
{
   MouseButtons buttons = 0;
   if (qtButtons & Qt::NoButton)
       buttons |= static_cast<MouseButtons>(rm::MouseButton::NoButton);

   if (qtButtons & Qt::LeftButton)
       buttons |= static_cast<MouseButtons>(rm::MouseButton::LeftButton);

   if (qtButtons & Qt::RightButton)
       buttons |= static_cast<MouseButtons>(rm::MouseButton::RightButton);

   if (qtButtons & Qt::MiddleButton)
       buttons |= static_cast<MouseButtons>(rm::MouseButton::MiddleButton);

   return buttons;
}




KeyboardModifiers GraphicsSceneEvent::convertKeyboardModifiers(unsigned int qtModifiers) const
{
    rm::KeyboardModifiers modifiers = 0;
    if (qtModifiers & Qt::NoModifier)
        modifiers |= static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);

    if (qtModifiers & Qt::ShiftModifier)
        modifiers |= static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);

    if (qtModifiers & Qt::ControlModifier)
        modifiers |= static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);

    if (qtModifiers & Qt::AltModifier)
        modifiers |= static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);

    return modifiers;
}

EventType GraphicsSceneEvent::getEventType()
{
    return _eventType;
}


}
