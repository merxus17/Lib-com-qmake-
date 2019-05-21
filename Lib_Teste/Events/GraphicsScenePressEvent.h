#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "../Core/GraphicsSceneEvent.h"
#include "../Geometry/Vector2D.h"
namespace rm
{
    class GraphicsScenePressEvent : public GraphicsSceneEvent
    {
    public:
        /**
         * Constructor
         */
        GraphicsScenePressEvent();

        /**
        * Virtual destructor
        */
        ~GraphicsScenePressEvent() = default;

        /**
         * @brief getButton - Returns the button that caused the event.
         * It is consistent with the MouseButton enum definition.
         * @return - the button that caused the event.
         */
        MouseButton getButton() const;

        /**
         * @brief getKeyboardModifiers - Get integer that stores which modifiers are pressed. Each bit represents one of
         * modifiers. These bits are consistent with the KeyboardModifier enum definition.
         * @return - Integer that stores which modifiers are pressed.
         */
        KeyboardModifiers getKeyboardModifiers() const;

        /**
         * @brief getGlobalPositon - Get the current mouse position on global coordinate system, i. e., the
         * coordinate system that takes account all screens as one single screen.
         * @return - Point on global system coordinate.
         */
        const Point2Df& getGlobalPosition() const;

        /**
         * @brief getLocalPosition - Get the current mouse position on local coordinate system, i. e., the
         * coordinate system that takes account just the canvas.
         * @return - Point on local system coordinate.
         */
        const Point2Df& getLocalPosition() const;

        /**
         * @brief getScreenPosition - Get the current mouse position on sceen coordinate system, i. e., the
         * coordinate system that takes account just the current.
         * @return - Point on screen system coordinate.
         */
        const Point2Df& getScreenPosition() const;

        /**
         * @brief getWindowPosition - Get the current mouse position on window coordinate system, i. e.,
         * the coordinate system that takes account the dialog space.
         * @return - Point on window system coordinate.
         */
        const Point2Df& getWindowPosition() const;

        /**
         * @brief getWorldPosition - Get the curret mouse position on world coordinate system, i. e., the coordinate
         * system where the primitives is rendered.
         * @return - World position.
         */
        const Point2Df& getWorldPosition() const;

        /**
         * @brief getMouseButtonAction - Get the current mouse button action.
         * @return - Current button action: Press or release.
         */
        MouseButtonAction getMouseButtonAction() const;

        /**
         * @brief isLeftButtonPressed - Verify if the left button is the pressed button.
         * @return - True if the left button is the pressed button and false otherwise.
         */
        bool isLeftButtonPressed() const;

        /**
         * @brief isRightButtonPressed - Verify if the left button is the pressed button.
         * @return - True if the right button is the pressed button and false otherwise.
         */
        bool isRightButtonPressed() const;

        /**
         * @brief isMiddleButtonPressed - Verify if the middle button is the pressed button.
         * @return - True if if the middle button is the pressed button and false otherwise.
         */
        bool isMiddleButtonPressed() const;

        /**
         * @brief hasModifierPressed - Verify if there is any pressed modifier.
         * @return - True if at least one modifier is pressed, false otherwise.
         */
        bool hasModifierPressed() const;

        /**
         * @brief isControlKeyPressed - Verify if the control key is one of the pressed modifiers.
         * @return - True if the control key is one of the pressed modifiers and false otherwise.
         */
        bool isControlKeyPressed() const;

        /**
         * @brief isOnlyControlKeyPressed - Verify if the control key is the only pressed modifier.
         * @return - True if the control key is the only pressed modifier and false otherwise.
         */
        bool isOnlyControlKeyPressed() const;

        /**
         * @brief isShiftKeyPressed - Verify if the shift key is one of the pressed modifiers.
         * @return - True if the shift key is one of the pressed modifiers and false otherwise.
         */
        bool isShiftKeyPressed() const;

        /**
         * @brief isOnlyShiftKeyPressed - Verify if the shift key is the only pressed modifier.
         * @return - True if the shift key is the only pressed modifier and false otherwise.
         */
        bool isOnlyShiftKeyPressed() const;

        /**
         * @brief isAltKeyPressed - Verify if the alt key is one of the pressed modifiers.
         * @return - true if the alt key is one of the pressed modifiers and false otherwise.
         */
        bool isAltKeyPressed() const;

        /**
         * @brief isOnlyAltKeyPressed - Verify if the alt key is the only pressed modifier.
         * @return - True if the alt key is the only pressed modifier and false otherwise.
         */
        bool isOnlyAltKeyPressed() const;

        /**
         * @brief setButton - Set which button is pressed. It is consistent with the
         * MouseButton enum definition.
         * @param button - The button pressed.
         */
        void setButton(MouseButton button);

        /**
         * @brief setKeyboardModifiers - Set the integer that stores which modifiers are pressed. Each bit represents
         * one modifiers. These bits are consistent with the KeyboardModifier enum definition.
         * @param modifiers - Integer that stores which modifiers are pressed.
         */
        void setKeyboardModifiers(KeyboardModifiers getKeyboardModifiers);

        /**
         * @brief setGlobalPosition - Define the current mouse coordinate on global coordinate system, i. e., the
         * coordinate system that takes account all screens as one single screen.
         * @param global - Point on global system coordinate.
         */
        void setGlobalPosition(const Point2Df& global);

        /**
         * @brief setLocalPosition - Define the current mouse coordinate on local coordinate system, i. e., the
         * coordinate system that takes account just the canvas.
         * @param local - Point on local system coordinate.
         */
        void setLocalPosition(const Point2Df& local);

        /**
         * @brief setScreenPosition - Define the current mouse coordinate on screen coordinate system, i. e., the
         * coordinate system that takes account just the current.
         * @param screen - Point on screen system coordinate.
         */
        void setScreenPosition(const Point2Df& screen);

        /**
         * @brief setWindowPosition - Define the current mouse coordinate on window, or dialog, coordinate system, i. e.,
         * the coordinate system that takes account the dialog space.
         * @param window - Point on window system coordinate.
         */
        void setWindowPosition(const Point2Df& window);

        /**
         * @brief setWorldPosition - Define the current mouse coordinate on world coordinate system, i. e., the coordinate
         * system where the primitives is rendered.
         * @param world - World position.
         */
        void setWorldPosition(const Point2Df& world);

        /**
         * @brief setMouseButtonAction - Define the mouse button action during the event: Press or Release.
         * @param action - Define if the current action is Press or Release.
         */
        void setMouseButtonAction(MouseButtonAction action);

    private:
        /**
         * @brief _globalPosition - Takes account all screens on a single coordinate system.
         */
        Point2Df _globalPosition;

        /**
         * @brief _localPosition - Takes account the widget coordinate system.
         */
        Point2Df _localPosition;

        /**
         * @brief _screenPosition - Takes account the current screen coordinate system.
         */
        Point2Df _screenPosition;

        /**
         * @brief _windowPosition - Takes account the dialog coordinate system.
         */
        Point2Df _windowPosition;

        /**
         * @brief _worldPosition - Position on world coordinantes system.
         */
        Point2Df _worldPosition;

        /**
         * @brief _button - Stores the button that caused the event.
         */
        MouseButton _button;

        /**
         * @brief _keyboardModifiers - Store the pressed modifiers during the event. Each bit represents one modifier.
         * These bits are consistent with the KeyboardModifier enum definition.
         */
        KeyboardModifiers _keyboardModifiers;

        /**
         * @brief _action - Store the mouse button action during the event: Press or Release.
         */
        MouseButtonAction _action;
    };

}
