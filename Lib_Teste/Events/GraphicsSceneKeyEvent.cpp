#include "GraphicsSceneKeyEvent.h"
namespace rm
{

GraphicsSceneKeyEvent::GraphicsSceneKeyEvent():GraphicsSceneEvent(EventType::keyEvent)
{
}



GraphicsSceneKeyEvent::GraphicsSceneKeyEvent(int , int , const KeyboardModifiers& , std::string& ,
                                             bool , int ):GraphicsSceneEvent(EventType::keyEvent)
{
}



KeyAction GraphicsSceneKeyEvent::getKeyEventType() const
{
    return _keyEventType;
}



KeyboardModifiers GraphicsSceneKeyEvent::getKeyboardModifiers() const
{
    return _keyboardModifiers;
}



int GraphicsSceneKeyEvent::getCount() const
{
    return _count;
}



int GraphicsSceneKeyEvent::getKey() const
{
    return _key;
}



//TODO
//bool GraphicsSceneKeyEvent::matches(const KeySequence& key) const
//{
//    //implement
//    return false;
//}




bool GraphicsSceneKeyEvent::isAutoRepeat() const
{
    return _autorep;
}



std::string GraphicsSceneKeyEvent::text() const
{
    return _text;
}



bool  GraphicsSceneKeyEvent::hasModifierPressed() const
{
    return _keyboardModifiers != static_cast<KeyboardModifiers>(rm::KeyboardModifier::NoModifier);
}



bool  GraphicsSceneKeyEvent::isControlKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool  GraphicsSceneKeyEvent::isOnlyControlKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ControlModifier);
}



bool  GraphicsSceneKeyEvent::isShiftKeyPressed() const
{
     return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool  GraphicsSceneKeyEvent::isOnlyShiftKeyPressed() const
{
     return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::ShiftModifier);
}



bool  GraphicsSceneKeyEvent::isAltKeyPressed() const
{
    return _keyboardModifiers & static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



bool  GraphicsSceneKeyEvent::isOnlyAltKeyPressed() const
{
    return _keyboardModifiers == static_cast<KeyboardModifiers>(rm::KeyboardModifier::AltModifier);
}



void  GraphicsSceneKeyEvent::setKeyboardModifiers(KeyboardModifiers modifiers)
{
    _keyboardModifiers = modifiers;
}



void GraphicsSceneKeyEvent::setKeyEventType(KeyAction type)
{
    _keyEventType = type;
}



void GraphicsSceneKeyEvent::setKey(int key)
{
    _key = key;
}



void GraphicsSceneKeyEvent::setText(std::string text)
{
    _text = text;
}



void GraphicsSceneKeyEvent::setAutoRep(bool autorep)
{
    _autorep = autorep;
}



void GraphicsSceneKeyEvent::setCount(int count)
{
    _count = count;
}
}
