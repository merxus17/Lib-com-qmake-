#pragma once

#include "../Core/GraphicsSceneEvent.h"
#include "../Geometry/Vector2D.h"
namespace rm
{
class GraphicsSceneHoverEvent : public GraphicsSceneEvent
{
public:
	/**
	 * constructor
	 */
    GraphicsSceneHoverEvent();

	/**
	 * virtual destructor
	 */
    ~GraphicsSceneHoverEvent() = default;

    /**
     * @brief getKeyboardModifiers - Get integer that stores which modifiers are pressed. Each bit represents one of
     * modifiers. These bits are consistent with the KeyboardModifier enum definition.
     * @return - integer that stores which modifiers are pressed.
     */
    KeyboardModifiers getKeyboardModifiers() const;

    /**
     * @brief getGlobalPositon - Get the current mouse position on global coordinate system, i. e., the
     * coordinate system that takes account all screens as one single screen.
     * @return - point on global system coordinate.
     */
    const Point2Df& getGlobalPositon() const;

    /**
     * @brief getLocalPosition - Get the current mouse position on local coordinate system, i. e., the
     * coordinate system that takes account just the canvas.
     * @return - point on local system coordinate.
     */
    const Point2Df& getLocalPosition() const;

    /**
     * @brief getScreenPosition - Get the current mouse position on sceen coordinate system, i. e., the
     * coordinate system that takes account just the current.
     * @return - point on screen system coordinate.
     */
    const Point2Df& getScreenPosition() const;

    /**
     * @brief getWindowPosition - Get the current mouse position on window coordinate system, i. e.,
     * the coordinate system that takes account the dialog space.
     * @return - point on window system coordinate.
     */
    const Point2Df& getWindowPosition() const;

    /**
     * @brief getFirstWorldPosition - Get the first mouse position on world coordinates system, i. e., the coordinate
     * system where the primitives is rendered.
     * @return - first mouse position on world coordinates system.
     */
    const Point2Df& getFirstWorldPosition() const;

    /**
     * @brief getWorldPosition - Get the curret mouse position on world coordinate system, i. e., the coordinate
     * system where the primitives is rendered.
     * @return - world position.
     */
    const Point2Df& getWorldPosition() const;

    /**
     * @brief getOldWorldPosition - Get the old mouse position on world coordinate system, i. e., the coordinate
     * system where the primitives is rendered.
     * @return - old world mouse position.
     */
    const Point2Df& getOldWorldPosition() const;

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
     * @brief setGlobalPosition - define the current mouse coordinate on global coordinate system, i. e., the
     * coordinate system that takes account all screens as one single screen.
     * @param global - point on global system coordinate.
     */
    void setGlobalPosition(const Point2Df& global);

    /**
     * @brief setLocalPosition - define the current mouse coordinate on local coordinate system, i. e., the
     * coordinate system that takes account just the canvas.
     * @param local - point on local system coordinate.
     */
    void setLocalPosition(const Point2Df& local);

    /**
     * @brief setScreenPosition - define the current mouse coordinate on screen coordinate system, i. e., the
     * coordinate system that takes account just the current.
     * @param screen - point on screen system coordinate.
     */
    void setScreenPosition(const Point2Df& screen);

    /**
     * @brief setWindowPosition - define the current mouse coordinate on window, or dialog, coordinate system, i. e.,
     * the coordinate system that takes account the dialog space.
     * @param window - point on window system coordinate.
     */
    void setWindowPosition(const Point2Df& window);

    /**
     * @brief setFirstWorldPosition - Set the first mouse position on world coordinates system, i. e., the coordinate
     * system where the primitives is rendered.
     * @param world - first mouse position on world coordinates.
     */
    void setFirstWorldPosition(const Point2Df& world);

    /**
     * @brief setWorldPosition - define the current mouse coordinate on world coordinate system, i. e., the coordinate
     * system where the primitives is rendered.
     * @param world - world position.
     */
    void setWorldPosition(const Point2Df& world);

    /**
     * @brief setOldWorldPosition - define the old mouse coordinate on world coordinate system, i. e., the coordinate
     * system where the primitives is rendered.
     * @param old - world position.
     */
    void setOldWorldPosition(const Point2Df& old);
private:
    /**
     * Takes account all screens on a single coordinate system.
     */
    Point2Df _globalPosition;

    /**
     * Takes account the widget coordinate system.
     */
    Point2Df _localPosition;

    /**
     * Takes account the current screen coordinate system.
     */
    Point2Df _screenPosition;

    /**
     * Takes account the dialog coordinate system.
     */
    Point2Df _windowPosition;

    /**
     * Position on world coordinantes system.
     */
    Point2Df _worldPosition;

    /**
     * Old position on world coordinantes system.
     */
    Point2Df _oldWorldPosition;

    /**
     * First world position on mouve move event.
     */
    Point2Df _firstWorldPosition;

    /**
     * Store the pressed modifiers during the event. Each bit represents one modifier. These bits are consistent with
     * the KeyboardModifier enum definition.
     */
    KeyboardModifiers _keyboardModifiers;
};
}
