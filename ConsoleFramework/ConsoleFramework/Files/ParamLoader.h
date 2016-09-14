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
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "..\Core\GObject.h"

#define RESOURCEPATH "Resources/Entities/"
#define PARAMSUCCESS 1
#define PARAMFAILURE -1

class ParamLoader
{
private:
	//DATA
	std::string _Name;
	std::string _ID;
	int _Width;
	int _Height;
	Etype _Type;
	std::vector<CHAR_INFO> _Body;
	//CODE
	int _Status;
	std::string _Path;
	int charCount;
	int attribCount;
	CHAR_INFO Character;
	int charLoop;
	int attribLoop;
	std::string charLoopString;
	std::string attribLoopString;
	//METHODS
	void ResetCharacter();
public:
	explicit ParamLoader(const char* fileName);
	const int GetStatus() const { return _Status; }
	const std::string GetName() const { return _Name; }
	const std::string GetID() const { return _ID; }
	const int GetWidth() const { return _Width; }
	const int GetHeight() const { return _Height; }
	const Etype GetType() const { return _Type; }
	const std::vector<CHAR_INFO> GetBody()const { return _Body; }
};