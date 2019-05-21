#pragma once
namespace rm
{
using MouseButtons = unsigned char;
using KeyboardModifiers = unsigned char;

using SceneEvents = unsigned int;

enum class EventType : unsigned int
{
    NoEvent               = 0x00000000,
    MoveEvent             = 0x00000001,
    PressEvent            = 0x00000002,
    keyEvent              = 0x00000004,
    ContextMenuEvent      = 0x00000008,
    DragEnterEvent        = 0x00000010,
    DragLeaveEvent        = 0x00000020,
    DragMoveEvent         = 0x00000040,
    DropEvent             = 0x00000080,
    FocusEvent            = 0x00000100,
    InputMethodEvent      = 0x00000200,
    PaintEvent            = 0x00000400,
    ResizeEvent           = 0x00000800,
    ShowEvent             = 0x00001000,
    WheelEvent            = 0x00002000,
    DrawBackground        = 0x00004000,
    DrawForeground        = 0x00008000,
    HelpEvent             = 0x00010000,
    MouseDoubleClickEvent = 0x00020000,
    HoverEvent            = 0x00040000,
    LeaveEvent            = 0x00080000,
    EnterEvent            = 0x00100000,
    AllEvents             = 0xFFFFFFFF
};

enum class MouseButton : unsigned char
{
    NoButton     = 0x00,
    LeftButton   = 0x01,
    RightButton  = 0x02,
    MiddleButton = 0x04
};

enum class KeyboardModifier : unsigned char
{
    NoModifier           = 0x00,
    ShiftModifier        = 0x01,
    ControlModifier      = 0x02,
    AltModifier          = 0x04
};

enum class KeyAction
{
    pressEvent,
    releaseEvent
};

enum class Orientation
{
    Foward,
    Backward,
};

enum class MouseButtonAction
{
    Press,
    Release
};
}
