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
#include "..\Core\GObject.h"
#include "..\Core\Vector.h"
#include "..\Files\ParamLoader.h"
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Character : public GObject
{
protected:
	//tinyxml2::XMLDocument doc;
	std::string _Name;
	std::string _ID;
	Vector2D m_position;
	Etype mType = Etype::Player;
	U8 m_width = 0;
	U8 m_height = 0;
	std::vector<CHAR_INFO> chRenderData;
public:
	virtual const std::vector<CHAR_INFO>&GetRenderData() const;
	virtual void Update();
	virtual U8 Load(const char* fileName);
	virtual Etype GetType() const;
	virtual std::string GetID();
	virtual const Vector2D GetPosition() const;
	virtual const void SetPosition(Vector2D && newPosition);
	virtual const void SetPosition(Vector2D & newPosition);
	virtual const Vector2D GetSize() const;
};