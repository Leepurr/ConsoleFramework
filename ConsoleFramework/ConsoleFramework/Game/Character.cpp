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
#include "Character.h"

const std::vector<CHAR_INFO>& Character::GetRenderData() const
{
	return chRenderData;
}

void Character::Update()
{
}

U8 Character::Load(const char* fileName)
{
	ParamLoader* Params = new ParamLoader(fileName);
	_Name = Params->GetName();
	_ID = Params->GetID();
	mType = Params->GetType();
	m_width = Params->GetWidth();
	m_height = Params->GetHeight();
	chRenderData = Params->GetBody();
	m_position.x = 60; m_position.y = 20;
	int Status = Params->GetStatus();
	delete Params;
	return Status;
}

Etype Character::GetType() const
{
	return mType;
}

std::string Character::GetID()
{
	return _ID;
}

const Vector2D Character::GetPosition() const
{
	return m_position;
}

const void Character::SetPosition(Vector2D && newPosition)
{
	m_position = newPosition;
}

const void Character::SetPosition(Vector2D& newPosition)
{
	m_position = newPosition;
}

const Vector2D Character::GetSize() const
{
	return Vector2D(m_height, m_width);
}