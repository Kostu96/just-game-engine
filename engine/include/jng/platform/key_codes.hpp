/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {
    namespace Key {
        
        using Code = u16;

        // From glfw3.h
        constexpr Code Space = 32;
        constexpr Code Apostrophe = 39;
        constexpr Code Comma = 44;
        constexpr Code Minus = 45;
        constexpr Code Period = 46;
        constexpr Code Slash = 47;
        
        constexpr Code Number0 = 48;
        constexpr Code Number1 = 49;
        constexpr Code Number2 = 50;
        constexpr Code Number3 = 51;
        constexpr Code Number4 = 52;
        constexpr Code Number5 = 53;
        constexpr Code Number6 = 54;
        constexpr Code Number7 = 55;
        constexpr Code Number8 = 56;
        constexpr Code Number9 = 57;
         
        constexpr Code Semicolon = 59;
        constexpr Code Equal = 61;
        
        constexpr Code A = 65;
        constexpr Code B = 66;
        constexpr Code C = 67;
        constexpr Code D = 68;
        constexpr Code E = 69;
        constexpr Code F = 70;
        constexpr Code G = 71;
        constexpr Code H = 72;
        constexpr Code I = 73;
        constexpr Code J = 74;
        constexpr Code K = 75;
        constexpr Code L = 76;
        constexpr Code M = 77;
        constexpr Code N = 78;
        constexpr Code O = 79;
        constexpr Code P = 80;
        constexpr Code Q = 81;
        constexpr Code R = 82;
        constexpr Code S = 83;
        constexpr Code T = 84;
        constexpr Code U = 85;
        constexpr Code V = 86;
        constexpr Code W = 87;
        constexpr Code X = 88;
        constexpr Code Y = 89;
        constexpr Code Z = 90;

        constexpr Code LeftBracket = 91;
        constexpr Code Backslash = 92;
        constexpr Code RightBracket = 93;
        constexpr Code GraveAccent = 96;

        constexpr Code World1 = 161;
        constexpr Code World2 = 162;

        constexpr Code Escape = 256;
        constexpr Code Enter = 257;
        constexpr Code Tab = 258;
        constexpr Code Backspace = 259;
        constexpr Code Insert = 260;
        constexpr Code Delete = 261;
        constexpr Code Right = 262;
        constexpr Code Left = 263;
        constexpr Code Down = 264;
        constexpr Code Up = 265;
        constexpr Code PageUp = 266;
        constexpr Code PageDown = 267;
        constexpr Code Home = 268;
        constexpr Code End = 269;
        constexpr Code CapsLock = 280;
        constexpr Code ScrollLock = 281;
        constexpr Code NumLock = 282;
        constexpr Code PrintScreen = 283;
        constexpr Code Pause = 284;
        constexpr Code F1 = 290;
        constexpr Code F2 = 291;
        constexpr Code F3 = 292;
        constexpr Code F4 = 293;
        constexpr Code F5 = 294;
        constexpr Code F6 = 295;
        constexpr Code F7 = 296;
        constexpr Code F8 = 297;
        constexpr Code F9 = 298;
        constexpr Code F10 = 299;
        constexpr Code F11 = 300;
        constexpr Code F12 = 301;
        constexpr Code F13 = 302;
        constexpr Code F14 = 303;
        constexpr Code F15 = 304;
        constexpr Code F16 = 305;
        constexpr Code F17 = 306;
        constexpr Code F18 = 307;
        constexpr Code F19 = 308;
        constexpr Code F20 = 309;
        constexpr Code F21 = 310;
        constexpr Code F22 = 311;
        constexpr Code F23 = 312;
        constexpr Code F24 = 313;
        constexpr Code F25 = 314;

        constexpr Code KeyPad0 = 320;
        constexpr Code KeyPad1 = 321;
        constexpr Code KeyPad2 = 322;
        constexpr Code KeyPad3 = 323;
        constexpr Code KeyPad4 = 324;
        constexpr Code KeyPad5 = 325;
        constexpr Code KeyPad6 = 326;
        constexpr Code KeyPad7 = 327;
        constexpr Code KeyPad8 = 328;
        constexpr Code KeyPad9 = 329;
        constexpr Code KeyPadDecimal = 330;
        constexpr Code KeyPadDivide = 331;
        constexpr Code KeyPadMultiply = 332;
        constexpr Code KeyPadSubtract = 333;
        constexpr Code KeyPadAdd = 334;
        constexpr Code KeyPadEnter = 335;
        constexpr Code KeyPadEqual = 336;

        constexpr Code LeftShift = 340;
        constexpr Code LeftControl = 341;
        constexpr Code LeftAlt = 342;
        constexpr Code LeftSuper = 343;
        constexpr Code RightShift = 344;
        constexpr Code RightControl = 345;
        constexpr Code RightAlt = 346;
        constexpr Code RightSuper = 347;
        constexpr Code Menu = 348;

    } // namespace Key
} // namespace jng
