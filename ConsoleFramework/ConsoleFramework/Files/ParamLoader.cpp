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
#include "ParamLoader.h"
#include "..\3rdParty\tinyxml2.h"
#include <string>
#include <stdio.h>

#define INVALID_FILE -1

ParamLoader::ParamLoader(const char* fileName) :
	_Path(RESOURCEPATH), _Status(0), _Width(0),
	_Height(0), charCount(0), attribCount(0),
	charLoop(1), attribLoop(1)
{
	_Path.append(fileName); _Path.append(".xml");
	using namespace tinyxml2;
	XMLDocument Document;
	XMLError	eResult;
	XMLNode*	pRoot;
	XMLElement* pElement;
	eResult = Document.LoadFile(_Path.c_str());
	if (eResult == XML_SUCCESS)
	{
		_Status = PARAMSUCCESS;
		if ((pRoot = Document.FirstChild()) != nullptr)
		{
			_Status = PARAMSUCCESS;
			pElement = pRoot->FirstChildElement("Name");
			if (pElement != nullptr)
			{
				_Status = PARAMSUCCESS;
				if (pElement->GetText() != nullptr)
					_Name = pElement->GetText();
			}
			else
			{
				_Status = PARAMFAILURE;
			}
			if ((pElement = pRoot->FirstChildElement("ID")) != nullptr)
			{
				_Status = PARAMSUCCESS;
				if (pElement->GetText() != nullptr)
					_ID = pElement->GetText();
			}
			else
			{
				_Status = PARAMFAILURE;
			}
			if ((pElement = pRoot->FirstChildElement("Size")) != nullptr)
			{
				_Status = PARAMSUCCESS;
				pElement->QueryAttribute("Width", &_Width);
				pElement->QueryAttribute("Height", &_Height);
			}
			else
			{
				_Status = PARAMFAILURE;
			}
			if ((pElement = pRoot->FirstChildElement("Type")) != nullptr)
			{
				_Status = PARAMSUCCESS;
				std::string checker;
				if (pElement->GetText() != nullptr)
					checker = pElement->GetText();
				if (checker.compare("Base") == 0)
				{
					_Type = Etype::Base;
				}
				if (checker.compare("Enemy") == 0)
				{
					_Type = Etype::Enemy;
				}
				if (checker.compare("Player") == 0)
				{
					_Type = Etype::Player;
				}
				if (checker.compare("Standard") == 0)
				{
					_Type = Etype::Standard;
				}
			}
			else
			{
				_Status = PARAMFAILURE;
			}
			if ((pElement = pRoot->FirstChildElement("Body")) != nullptr)
			{
				_Status = PARAMSUCCESS;
				pElement->QueryAttribute("charCount", &charCount);
			}
			else
			{
				_Status = PARAMFAILURE;
			}
		}
		else
		{
			_Status = PARAMFAILURE;
		}
		charLoopString = "Character" + std::to_string(charLoop);
		for ((pElement = pRoot->FirstChildElement("Body")
			 ->FirstChildElement("Character1"));
			 pElement != nullptr;
			 pElement = pRoot->FirstChildElement("Body")
			 ->FirstChildElement(charLoopString.c_str()))
		{
			_Status = PARAMSUCCESS;
			ResetCharacter();
			pElement->QueryAttribute("attribCount", &attribCount);
			if ((pElement = pRoot->FirstChildElement("Body")
				->FirstChildElement(charLoopString.c_str())
				->FirstChildElement("Text")) != nullptr)
			{
				_Status = PARAMSUCCESS;
				if (pElement->GetText() == (const char*)' ')
				{
					Character.Char.AsciiChar = ' ';
					Character.Char.UnicodeChar = ' ';
				}
				else if (pElement->GetText() == nullptr)
				{
					Character.Char.AsciiChar = ' ';
					Character.Char.UnicodeChar = ' ';
				}
				else
				{
					Character.Char.AsciiChar = (CHAR)pElement->GetText();
					Character.Char.UnicodeChar = (CHAR)pElement->GetText();
				}
			}
			else
			{
				_Status = PARAMFAILURE;
			}
			attribLoopString = "Attrib" + std::to_string(attribLoop);
			for (pElement = pRoot->FirstChildElement("Body")
				 ->FirstChildElement(charLoopString.c_str())
				 ->FirstChildElement("Attrib1");
				 pElement != nullptr;
				 pElement = pRoot->FirstChildElement("Body")
				 ->FirstChildElement(charLoopString.c_str())
				 ->FirstChildElement(attribLoopString.c_str()))
			{
				_Status = PARAMSUCCESS;
				std::string checker;
				if (pElement->GetText() != nullptr)
				{
					_Status = PARAMSUCCESS;
					checker = pElement->GetText();
					if (checker.compare("BACKGROUND_RED") == 0)
					{
						Character.Attributes += BACKGROUND_RED;
					}
					if (checker.compare("BACKGROUND_GREEN") == 0)
					{
						Character.Attributes += BACKGROUND_GREEN;
					}
					if (checker.compare("BACKGROUND_BLUE") == 0)
					{
						Character.Attributes += BACKGROUND_BLUE;
					}
					if (checker.compare("BACKGROUND_INTENSITY") == 0)
					{
						Character.Attributes += BACKGROUND_INTENSITY;
					}
					if (checker.compare("FOREGROUND_RED") == 0)
					{
						Character.Attributes += FOREGROUND_RED;
					}
					if (checker.compare("FOREGROUND_GREEN") == 0)
					{
						Character.Attributes += FOREGROUND_GREEN;
					}
					if (checker.compare("FOREGROUND_BLUE") == 0)
					{
						Character.Attributes += FOREGROUND_BLUE;
					}
					if (checker.compare("FOREGROUND_INTENSITY") == 0)
					{
						Character.Attributes += FOREGROUND_INTENSITY;
					}
					if (checker.compare("BACKGROUND_BLACK") == 0)
					{
						Character.Attributes = 0x00;
					}
					checker.clear();
					if (attribLoop <= attribCount)
					{
						attribLoop++;
						if (attribLoop >= 0 && attribLoop < 11)
						{
							attribLoopString.erase(attribLoopString.size() - 1);
						}
						if (attribLoop >= 11)
						{
							attribLoopString.erase(attribLoopString.size() - 2);
						}
						attribLoopString.append(std::to_string(attribLoop));
					}
				}
				else
				{
					_Status = PARAMFAILURE;
				}
			}
			_Body.push_back(Character);
			if (charLoop <= charCount)
			{
				charLoop++;
				if (charLoop >= 0 && charLoop < 11)
				{
					charLoopString.erase(charLoopString.size() - 1);
				}
				if (charLoop >= 11)
				{
					charLoopString.erase(charLoopString.size() - 2);
				}
				charLoopString.append(std::to_string(charLoop));
				attribLoop = 1;
			}
		}
	}
	else
	{
		_Status = PARAMFAILURE;
	}
}

void ParamLoader::ResetCharacter()
{
	Character.Char.AsciiChar = ' ';
	Character.Char.UnicodeChar = ' ';
	Character.Attributes = 0x00;
}