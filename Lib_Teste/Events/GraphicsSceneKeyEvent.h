#pragma once

#include <string>
#include "../Core/GraphicsSceneEvent.h"
namespace rm
{
class KeySequence;

class GraphicsSceneKeyEvent : public GraphicsSceneEvent
{

public:
    /**
     * Simple Constructor
     */
    GraphicsSceneKeyEvent();
	/**
	 * Event Constructor
	 */
    GraphicsSceneKeyEvent(int type, int key, const KeyboardModifiers& modifiers, std::string& text, bool autorep,
                          int count);

    /**
    * @brief Default destructor
    */
    ~GraphicsSceneKeyEvent() = default;

    /**
     * @brief getKeyEventType - Returns the key event type
     * @return
     */
    KeyAction getKeyEventType() const;

    /**
     * @brief getKeyboardModifiers - Get integer that stores which modifiers are pressed. Each bit represents one of
     * modifiers. These bits are consistent with the KeyboardModifier enum definition.
     * @return - integer that stores which modifiers are pressed.
     */
    KeyboardModifiers getKeyboardModifiers() const;

    /**
     * @brief getCount - Returns the number of keys involved in this event
     */
    int getCount() const;

    /**
     * @Returns the code of the key that was pressed or released.
     */
    int getKey() const;

    /**
     * TODO
     * Returns true if the key event matches the given standard  key binding ; otherwise returns false.
     */
   // bool matches(const KeySequence& key) const;

    /**
     * Returns true if this event comes from an auto-repeating key; returns false if it comes from an initial key press.
     */
    bool isAutoRepeat() const;

    /**
     * Returns the Unicode text that this key generated. The text returned can be an empty string in cases where modifier keys, such as Shift, Control, Alt, and Meta, are being pressed or released. In such cases key() will contain a valid value.
     */
    std::string text() const;

    /**
     * @brief hasModifierPressed - verify if there is any pressed modifier.
     * @return - true if at least one modifier is pressed, false otherwise.
     */
    bool hasModifierPressed() const;
    /**
     * @brief isControlKeyPressed - verify if the control key is one of the pressed modifiers.
     * @return - true if the control key is one of the pressed modifiers and false otherwise.
     */
    bool isControlKeyPressed() const;

    /**
     * @brief isOnlyControlKeyPressed - verify if the control key is the only pressed modifier.
     * @return - true if the control key is the only pressed modifier and false otherwise.
     */
    bool isOnlyControlKeyPressed() const;

    /**
     * @brief isShiftKeyPressed - verify if the shift key is one of the pressed modifiers.
     * @return - true if the shift key is one of the pressed modifiers and false otherwise.
     */
    bool isShiftKeyPressed() const;

    /**
     * @brief isOnlyShiftKeyPressed - verify if the shift key is the only pressed modifier.
     * @return - true if the shift key is the only pressed modifier and false otherwise.
     */
    bool isOnlyShiftKeyPressed() const;

    /**
     * @brief isAltKeyPressed - verify if the alt key is one of the pressed modifiers.
     * @return - true if the alt key is one of the pressed modifiers and false otherwise.
     */
    bool isAltKeyPressed() const;

    /**
     * @brief isOnlyAltKeyPressed - verify if the alt key is the only pressed modifier.
     * @return - true if the alt key is the only pressed modifier and false otherwise.
     */
    bool isOnlyAltKeyPressed() const;

    /**
     * @brief setKeyboardModifiers - Set the integer that stores which modifiers are pressed. Each bit represents
     * one modifiers. These bits are consistent with the KeyboardModifier enum definition.
     * @param modifiers -  integer that stores which modifiers are pressed.
     */
    void setKeyboardModifiers(KeyboardModifiers modifiers);
    /**
     * @brief setKeyEventType - Set the type of the event. if it is 0, the event is keyPress and if it is 1 the event is keyRelease
     * @param type -  integer that indicates the type of the key event.
     */
    void setKeyEventType(KeyAction type);
    /**
     * @brief setText - Set the text that was written with the keyboard.
     * @param text -  string that stores the text written.
     */
    void setText(std::string text);

    /**
     * @brief setAutoRep - Set autorep as true if this event comes from auto-repeating key.Set false if it comes from an initial key press
     * @param autorep -  boolean that stores if the key is repeated or not.
     */
    void setAutoRep(bool autorep);

    /**
     * @brief setCount - Set the number of keys that are involved in this event.
     * @param count -  integer that stores the number of keys involved in the event.
     */
    void setCount(int count);

    /**
     * @brief setKey - Returns the code of the key that was pressed or released.
     * @param key -  integer with the code of the key that was pressed previously.
     */
    void setKey(int key);

private:
    /**
     * Is 0 if the event is keyPress and 1 if it is keyRelease
     */
    KeyAction _keyEventType;

    /**
     * Text that was written
     */
    std::string _text;

    /**
     * Indicates if the key is repeated or not
     */
    bool _autorep {false};

    /**
     * Number of keys involved in this event
     */
    int _count {0};

    /**
     * Store the pressed modifiers during the event. Each bit represents one modifier. These bits are consistent with
     * the KeyboardModifier enum definition.
     */
    KeyboardModifiers _keyboardModifiers;

    /**
     * Code of the last pressed key
     */
    int _key {-1};

};
}
