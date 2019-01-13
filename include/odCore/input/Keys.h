/*
 * Keys.h
 *
 *  Created on: 6 Jan 2019
 *      Author: zal
 */

#ifndef INCLUDE_ODCORE_INPUT_KEYS_H_
#define INCLUDE_ODCORE_INPUT_KEYS_H_

namespace odInput
{

    /**
     * Key code enum. The order and values of these items were directly adapted from OSG's GUIEventAdapter.
     * See http://www.openscenegraph.org/images/LICENSE.txt
     */
    enum class Key
    {
        Unknown         = 0,
        Space           = 0x20,

        _0              = '0',
        _1              = '1',
        _2              = '2',
        _3              = '3',
        _4              = '4',
        _5              = '5',
        _6              = '6',
        _7              = '7',
        _8              = '8',
        _9              = '9',
        A               = 'a',
        B               = 'b',
        C               = 'c',
        D               = 'd',
        E               = 'e',
        F               = 'f',
        G               = 'g',
        H               = 'h',
        I               = 'i',
        J               = 'j',
        K               = 'k',
        L               = 'l',
        M               = 'm',
        N               = 'n',
        O               = 'o',
        P               = 'p',
        Q               = 'q',
        R               = 'r',
        S               = 's',
        T               = 't',
        U               = 'u',
        V               = 'v',
        W               = 'w',
        X               = 'x',
        Y               = 'y',
        Z               = 'z',

        Exclaim         = 0x21,
        Quotedbl        = 0x22,
        Hash            = 0x23,
        Dollar          = 0x24,
        Ampersand       = 0x26,
        Quote           = 0x27,
        Leftparen       = 0x28,
        Rightparen      = 0x29,
        Asterisk        = 0x2A,
        Plus            = 0x2B,
        Comma           = 0x2C,
        Minus           = 0x2D,
        Period          = 0x2E,
        Slash           = 0x2F,
        Colon           = 0x3A,
        Semicolon       = 0x3B,
        Less            = 0x3C,
        Equals          = 0x3D,
        Greater         = 0x3E,
        Question        = 0x3F,
        At              = 0x40,
        Leftbracket     = 0x5B,
        Backslash       = 0x5C,
        Rightbracket    = 0x5D,
        Caret           = 0x5E,
        Underscore      = 0x5F,
        Backquote       = 0x60,

        BackSpace       = 0xFF08,
        Tab             = 0xFF09,
        Linefeed        = 0xFF0A,
        Clear           = 0xFF0B,
        Return          = 0xFF0D,
        Pause           = 0xFF13,
        Scroll_Lock     = 0xFF14,
        Sys_Req         = 0xFF15,
        Escape          = 0xFF1B,
        Delete          = 0xFFFF,

        Home            = 0xFF50,
        Left            = 0xFF51,
        Up              = 0xFF52,
        Right           = 0xFF53,
        Down            = 0xFF54,
        Prior           = 0xFF55,
        Page_Up         = 0xFF55,
        Next            = 0xFF56,
        Page_Down       = 0xFF56,
        End             = 0xFF57,
        Begin           = 0xFF58,

        Select          = 0xFF60,
        Print           = 0xFF61,
        Execute         = 0xFF62,
        Insert          = 0xFF63,
        Undo            = 0xFF65,
        Redo            = 0xFF66,
        Menu            = 0xFF67,
        Find            = 0xFF68,
        Cancel          = 0xFF69,
        Help            = 0xFF6A,
        Break           = 0xFF6B,
        Mode_switch     = 0xFF7E,
        Script_switch   = 0xFF7E,
        Num_Lock        = 0xFF7F,

        KP_Space        = 0xFF80,
        KP_Tab          = 0xFF89,
        KP_Enter        = 0xFF8D,
        KP_F1           = 0xFF91,
        KP_F2           = 0xFF92,
        KP_F3           = 0xFF93,
        KP_F4           = 0xFF94,
        KP_Home         = 0xFF95,
        KP_Left         = 0xFF96,
        KP_Up           = 0xFF97,
        KP_Right        = 0xFF98,
        KP_Down         = 0xFF99,
        KP_Prior        = 0xFF9A,
        KP_Page_Up      = 0xFF9A,
        KP_Next         = 0xFF9B,
        KP_Page_Down    = 0xFF9B,
        KP_End          = 0xFF9C,
        KP_Begin        = 0xFF9D,
        KP_Insert       = 0xFF9E,
        KP_Delete       = 0xFF9F,
        KP_Equal        = 0xFFBD,
        KP_Multiply     = 0xFFAA,
        KP_Add          = 0xFFAB,
        KP_Separator    = 0xFFAC,
        KP_Subtract     = 0xFFAD,
        KP_Decimal      = 0xFFAE,
        KP_Divide       = 0xFFAF,

        KP_0            = 0xFFB0,
        KP_1            = 0xFFB1,
        KP_2            = 0xFFB2,
        KP_3            = 0xFFB3,
        KP_4            = 0xFFB4,
        KP_5            = 0xFFB5,
        KP_6            = 0xFFB6,
        KP_7            = 0xFFB7,
        KP_8            = 0xFFB8,
        KP_9            = 0xFFB9,

        F1              = 0xFFBE,
        F2              = 0xFFBF,
        F3              = 0xFFC0,
        F4              = 0xFFC1,
        F5              = 0xFFC2,
        F6              = 0xFFC3,
        F7              = 0xFFC4,
        F8              = 0xFFC5,
        F9              = 0xFFC6,
        F10             = 0xFFC7,
        F11             = 0xFFC8,
        F12             = 0xFFC9,
        F13             = 0xFFCA,
        F14             = 0xFFCB,
        F15             = 0xFFCC,
        F16             = 0xFFCD,
        F17             = 0xFFCE,
        F18             = 0xFFCF,
        F19             = 0xFFD0,
        F20             = 0xFFD1,
        F21             = 0xFFD2,
        F22             = 0xFFD3,
        F23             = 0xFFD4,
        F24             = 0xFFD5,
        F25             = 0xFFD6,
        F26             = 0xFFD7,
        F27             = 0xFFD8,
        F28             = 0xFFD9,
        F29             = 0xFFDA,
        F30             = 0xFFDB,
        F31             = 0xFFDC,
        F32             = 0xFFDD,
        F33             = 0xFFDE,
        F34             = 0xFFDF,
        F35             = 0xFFE0,

        Shift_L         = 0xFFE1,
        Shift_R         = 0xFFE2,
        Control_L       = 0xFFE3,
        Control_R       = 0xFFE4,
        Caps_Lock       = 0xFFE5,
        Shift_Lock      = 0xFFE6,

        Meta_L          = 0xFFE7,
        Meta_R          = 0xFFE8,
        Alt_L           = 0xFFE9,
        Alt_R           = 0xFFEA,
        Super_L         = 0xFFEB,
        Super_R         = 0xFFEC,
        Hyper_L         = 0xFFED,
        Hyper_R         = 0xFFEE
    };

}

#endif /* INCLUDE_ODCORE_INPUT_KEYS_H_ */
