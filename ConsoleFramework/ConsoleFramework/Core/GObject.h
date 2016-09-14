/*
Original code by Martyn Leeper (www.martynleeper.com)
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source
distribution.
*/
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Datatypes.h"
#include <vector>

class Vector2D;

enum Etype
{
	Standard = 0,
	Player,
	Enemy,
	Base
};

class GObject
{
public:
	virtual const std::vector<CHAR_INFO>&GetRenderData() const = 0;
	virtual void Update() = 0;
	virtual U8 Load(const char* fileName) = 0;
	virtual Etype GetType() const = 0;
	virtual std::string GetID() = 0;
	virtual const Vector2D GetPosition() const = 0;
	virtual const void SetPosition(Vector2D && newPosition) = 0;
	virtual const void SetPosition(Vector2D & newPosition) = 0;
	virtual const Vector2D GetSize() const = 0;
};