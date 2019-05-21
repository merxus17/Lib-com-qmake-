#pragma once

#include "../Core/GraphicsSceneEvent.h"
#include "../Geometry/Vector2D.h"

namespace rm
{

class GraphicsSceneWheelEvent : public GraphicsSceneEvent
{
public:
	/**
     * Constructor
     */
	GraphicsSceneWheelEvent();

	/**
     * Virtual destructor
     */
	virtual ~GraphicsSceneWheelEvent();

	/**
     * @brief getButtons - Returns the mouse buttons that were pressed when the wheel event occurred.
     * @return - the mouse buttons that were pressed when the wheel event occurred.
	 */
    MouseButtons getButtons() const;

	/**
     * @brief getDelta - Returns the distance that the wheel is rotated, in eighths (1/8s) of a degree. A positive value indicates that
     * the wheel was rotated forwards away from the user; a negative value indicates that the wheel was rotated
     * backwards toward the user. Most mouse types work in steps of 15 degrees, in which case the delta value is a
     * multiple of 120 (== 15 * 8).
     * @return - the distance that the wheel is rotated.
	 */
    int getDelta() const;

	/**
     * @brief getKeyboardModifiers - Returns the keyboard modifiers that were active when the wheel event occurred.
     * @return - the keyboard modifiers that were active when the wheel event occurred.
	 */
    KeyboardModifiers getKeyboardModifiers() const;

	/**
     * @brief getOrientation - Returns the wheel orientation. The return says if it's direction is foward or backward.
     * @return - the wheel orientation.
	 */
    const Orientation& getOrientation() const;

	/**
     * @brief getPosition - Returns the position of the cursor in item coordinates when the wheel event occurred.
     * @return - the position of the cursor in item coordinates when the wheel event occurred.
	 */
    const Point2Df& getPosition() const;

	/**
     * @brief getGlobalPosition - Returns the global position of the mouse pointer at the time of the event.
     * @return - the global position of the mouse pointer at the time of the event.
	 */
    const Point2Df& getGlobalPosition() const;

    /**
     * @brief hasButtonPressed - verify if there is any pressed button.
     * @return - true if at least one button is pressed, false otherwise.
     */
    bool hasButtonPressed() const;

    /**
     * @brief isLeftButtonPressed - verify if the left button is one of the pressed buttons.
     * @return - true if the left button is one of the pressed buttons and false otherwise.
     */
    bool isLeftButtonPressed() const;

    /**
     * @brief isOnlyLeftButtonPressed - verify if the left button is the only pressed button.
     * @return - true if the left button is the only pressed button and false otherwise.
     */
    bool isOnlyLeftButtonPressed() const;

    /**
     * @brief isRightButtonPressed - verify if the left button is one of the pressed buttons.
     * @return - true if the right button is one of the pressed buttons and false otherwise.
     */
    bool isRightButtonPressed() const;

    /**
     * @brief isOnlyRightButtonPressed - verify if the right button is the only pressed button.
     * @return - true if the right button is the only pressed button and false otherwise.
     */
    bool isOnlyRightButtonPressed() const;

    /**
     * @brief isMiddleButtonPressed - verify if the middle button is one of the pressed buttons.
     * @return - true if if the middle button is one of the pressed buttons and false otherwise.
     */
    bool isMiddleButtonPressed() const;

    /**
     * @brief isOnlyMiddleButtonPressed - verify if the middle button is the only pressed button.
     * @return - true if the midlle button is the only pressed button and false otherwise.
     */
    bool isOnlyMiddleButtonPressed() const;

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
     * @brief setKeyboardModifiers - Sets the keyboard modifiers that were pressed when the wheel event occurred.
     */
    void setKeyboardModifiers(KeyboardModifiers getKeyboardModifiers);

    /**
     * @brief setButtons - Sets the mouse buttons that were pressed when the wheel event occurred.
     */
    void setButtons(MouseButtons getButtons);

    /**
     * @brief setDelta - Sets the distance that the wheel is rotated, in eighths (1/8s) of a degree. A positive value indicates that
     * the wheel was rotated forwards away from the user; a negative value indicates that the wheel was rotated
     * backwards toward the user. Most mouse types work in steps of 15 degrees, in which case the delta value is a
     * multiple of 120 (== 15 * 8).
     */
    void setDelta (int getDelta);

    /**
     * @brief setPos - Sets the position of the cursor in item coordinates when the wheel event occurred.
     */
    void setPos(Point2Df getPosition);

    /**
     * @brief setGlobalPos - Sets the global position of the mouse pointer at the time of the event.
     */
    void setGlobalPos(Point2Df getGlobalPosition);

    /**
     * @brief setOrientation - Sets the orientation.
     */
    void setOrientation(Orientation getOrientation);

private:
    int _delta;
    Point2Df _pos;
    Point2Df _globalPos;
    MouseButtons _buttons;
    KeyboardModifiers _keyboardModifiers;
    Orientation _orientation;
};
}
