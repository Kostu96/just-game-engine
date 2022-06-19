/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {
	namespace Key {
		
		using KeyCode = uint16;

		// From glfw3.h
		constexpr KeyCode Space = 32;
		constexpr KeyCode Apostrophe = 39;
		constexpr KeyCode Comma = 44;
		constexpr KeyCode Minus = 45;
		constexpr KeyCode Period = 46;
		constexpr KeyCode Slash = 47;
		
		constexpr KeyCode Number0 = 48;
		constexpr KeyCode Number1 = 49;
		constexpr KeyCode Number2 = 50;
		constexpr KeyCode Number3 = 51;
		constexpr KeyCode Number4 = 52;
		constexpr KeyCode Number5 = 53;
		constexpr KeyCode Number6 = 54;
		constexpr KeyCode Number7 = 55;
		constexpr KeyCode Number8 = 56;
		constexpr KeyCode Number9 = 57;
		 
		constexpr KeyCode Semicolon = 59;
		constexpr KeyCode Equal = 61;
		
		constexpr KeyCode A = 65;
		constexpr KeyCode B = 66;
		constexpr KeyCode C = 67;
		constexpr KeyCode D = 68;
		constexpr KeyCode E = 69;
		constexpr KeyCode F = 70;
		constexpr KeyCode G = 71;
		constexpr KeyCode H = 72;
		constexpr KeyCode I = 73;
		constexpr KeyCode J = 74;
		constexpr KeyCode K = 75;
		constexpr KeyCode L = 76;
		constexpr KeyCode M = 77;
		constexpr KeyCode N = 78;
		constexpr KeyCode O = 79;
		constexpr KeyCode P = 80;
		constexpr KeyCode Q = 81;
		constexpr KeyCode R = 82;
		constexpr KeyCode S = 83;
		constexpr KeyCode T = 84;
		constexpr KeyCode U = 85;
		constexpr KeyCode V = 86;
		constexpr KeyCode W = 87;
		constexpr KeyCode X = 88;
		constexpr KeyCode Y = 89;
		constexpr KeyCode Z = 90;

		constexpr KeyCode LeftBracket = 91;
		constexpr KeyCode Backslash = 92;
		constexpr KeyCode RightBracket = 93;
		constexpr KeyCode GraveAccent = 96;

		constexpr KeyCode World1 = 161;
		constexpr KeyCode World2 = 162;

		constexpr KeyCode Escape = 256;
		constexpr KeyCode Enter = 257;
		constexpr KeyCode Tab = 258;
		constexpr KeyCode Backspace = 259;
		constexpr KeyCode Insert = 260;
		constexpr KeyCode Delete = 261;
		constexpr KeyCode Right = 262;
		constexpr KeyCode Left = 263;
		constexpr KeyCode Down = 264;
		constexpr KeyCode Up = 265;
		constexpr KeyCode PageUp = 266;
		constexpr KeyCode PageDown = 267;
		constexpr KeyCode Home = 268;
		constexpr KeyCode End = 269;
		constexpr KeyCode CapsLock = 280;
		constexpr KeyCode ScrollLock = 281;
		constexpr KeyCode NumLock = 282;
		constexpr KeyCode PrintScreen = 283;
		constexpr KeyCode Pause = 284;
		constexpr KeyCode F1 = 290;
		constexpr KeyCode F2 = 291;
		constexpr KeyCode F3 = 292;
		constexpr KeyCode F4 = 293;
		constexpr KeyCode F5 = 294;
		constexpr KeyCode F6 = 295;
		constexpr KeyCode F7 = 296;
		constexpr KeyCode F8 = 297;
		constexpr KeyCode F9 = 298;
		constexpr KeyCode F10 = 299;
		constexpr KeyCode F11 = 300;
		constexpr KeyCode F12 = 301;
		constexpr KeyCode F13 = 302;
		constexpr KeyCode F14 = 303;
		constexpr KeyCode F15 = 304;
		constexpr KeyCode F16 = 305;
		constexpr KeyCode F17 = 306;
		constexpr KeyCode F18 = 307;
		constexpr KeyCode F19 = 308;
		constexpr KeyCode F20 = 309;
		constexpr KeyCode F21 = 310;
		constexpr KeyCode F22 = 311;
		constexpr KeyCode F23 = 312;
		constexpr KeyCode F24 = 313;
		constexpr KeyCode F25 = 314;

		constexpr KeyCode KeyPad0 = 320;
		constexpr KeyCode KeyPad1 = 321;
		constexpr KeyCode KeyPad2 = 322;
		constexpr KeyCode KeyPad3 = 323;
		constexpr KeyCode KeyPad4 = 324;
		constexpr KeyCode KeyPad5 = 325;
		constexpr KeyCode KeyPad6 = 326;
		constexpr KeyCode KeyPad7 = 327;
		constexpr KeyCode KeyPad8 = 328;
		constexpr KeyCode KeyPad9 = 329;
		constexpr KeyCode KeyPadDecimal = 330;
		constexpr KeyCode KeyPadDivide = 331;
		constexpr KeyCode KeyPadMultiply = 332;
		constexpr KeyCode KeyPadSubtract = 333;
		constexpr KeyCode KeyPadAdd = 334;
		constexpr KeyCode KeyPadEnter = 335;
		constexpr KeyCode KeyPadEqual = 336;

		constexpr KeyCode LeftShift = 340;
		constexpr KeyCode LeftControl = 341;
		constexpr KeyCode LeftAlt = 342;
		constexpr KeyCode LeftSuper = 343;
		constexpr KeyCode RightShift = 344;
		constexpr KeyCode RightControl = 345;
		constexpr KeyCode RightAlt = 346;
		constexpr KeyCode RightSuper = 347;
		constexpr KeyCode Menu = 348;

	} // namespace Key
} // namespace jng
