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
#include "Console.h"
#include "Files\ParamLoader.h"
#include "Game\Character.h"
#include "Game\Generic.h"
#include <iterator>
#include <stdexcept>
#include <iostream>
//#include <vld.h> //Visual Leak Detector library for checking memory leaks

U16 Console::Initialise(const I16 cols, const I16 rows)
{
	_eStatus = EStatus::Start;

	_hCScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);
	_hWndConsoleWindow = GetConsoleWindow();
	if (_hWndConsoleWindow == INVALID_HANDLE_VALUE)
	{
		return BADHANDLE;
	}
	if (_hCScreenBuffer == INVALID_HANDLE_VALUE)
	{
		if (AttachConsole(ATTACH_PARENT_PROCESS) == false)
		{
			if (AllocConsole() == false)
			{
				return BADHANDLE;
			}
			else
			{
				_hCScreenBuffer = CreateConsoleScreenBuffer(
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					CONSOLE_TEXTMODE_BUFFER,
					NULL);
			}
		}
	}
	if (!SetConsoleActiveScreenBuffer(_hCScreenBuffer))
	{
		return CANTSETNEWSCREENBUFFER;
	}
	else
	{
		_screenBufferSize = { cols, rows };
		_windowSize.Top = 0; _windowSize.Left = 0;
		_windowSize.Bottom = (rows - 1); _windowSize.Right = (cols - 1);
		if (!SetConsoleScreenBufferSize(_hCScreenBuffer, _screenBufferSize))
		{
			return CANTSETSCREENBUFFERSIZE;
		}
		if (!SetConsoleWindowInfo(_hCScreenBuffer, TRUE, &_windowSize))
		{
			_screenBufferSize = GetLargestConsoleWindowSize(_hCScreenBuffer);
			_windowSize.Top = 0; _windowSize.Left = 0;
			_windowSize.Bottom = (rows - 1); _windowSize.Right = (cols - 1);
			if (!SetConsoleWindowInfo(_hCScreenBuffer, TRUE, &_windowSize))
			{
				return CANTRESIZEWINDOW;
			}
		}
		if (!GetConsoleScreenBufferInfo(_hCScreenBuffer, &_cScreenBufferInfo))
		{
			return CANTGETSCREENBUFFERINFO;
		}
		if ((_hCInput = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
		{
			return BADHANDLE;
		}
		_inputMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(_hCInput, _inputMode))
		{
			return SETCONSOLEMODEFAILED;
		}
		_CellCount = _cScreenBufferInfo.dwSize.X * _cScreenBufferInfo.dwSize.Y; //Total number of characters
		_screenBufferSize.X = _cScreenBufferInfo.dwSize.X;
		_screenBufferSize.Y = _cScreenBufferInfo.dwSize.Y;
		_renderBuffer.bufferData.resize((_screenBufferSize.X * _screenBufferSize.Y)); //Set Render Buffer size
		_writeRect = _windowSize;
		_dwBufferCoord = { 0, 0 };
	}
	_eStatus = EStatus::Running;
	return SUCCESS;
}

U8 Console::Load()
{
	auto Base_01 = std::make_shared<Generic>();
	auto Base_02 = std::make_shared<Generic>();
	auto Base_03 = std::make_shared<Generic>();
	auto Player = std::make_shared<Character>();
	Base_01->Load("Base"); Base_02->Load("Base"); Base_03->Load("Base");
	Player->Load("Player");

	Base_01->SetPosition(Vector2D((_screenBufferSize.Y - (Base_01.get()->GetSize().x + Player.get()->GetSize().x + 3)), 20));
	Base_02->SetPosition(Vector2D((_screenBufferSize.Y - (Base_02.get()->GetSize().x + Player.get()->GetSize().x + 3)), 32));
	Base_03->SetPosition(Vector2D((_screenBufferSize.Y - (Base_03.get()->GetSize().x + Player.get()->GetSize().x + 3)), 48));
	Player->SetPosition(Vector2D(_screenBufferSize.Y - (Player.get()->GetSize().x), 5));
	sceneManager->AddEntity(Player);
	sceneManager->AddEntity(Base_01);
	sceneManager->AddEntity(Base_02);
	sceneManager->AddEntity(Base_03);
	return 0;
}

void Console::CleanUp()
{
	_eStatus = EStatus::Quit;
	_renderBuffer.bufferData.clear();
}

void Console::Render()
{
	WriteConsoleOutput(
		_hCScreenBuffer,
		_renderBuffer.bufferData.data(),
		_screenBufferSize,
		_dwBufferCoord,
		&_writeRect);
}

void Console::GameLogic()
{
	/*
	try
	{
	//sceneManager->GetGObjectByID("Player");
	}
	catch (const std::out_of_range& e)
	{

	}
	*/
}

bool Console::IsKeyDown(const UChar chr)
{
	for (U32 i = 0; i < _numOfEvents; i++)
	{
		if (_inputBuffer[i].EventType == KEY_EVENT)
		{
			if (_inputBuffer[i].Event.KeyEvent.uChar.AsciiChar == chr)
			{
				return true;
			}
			else if (_inputBuffer[i].Event.KeyEvent.wVirtualKeyCode == chr)
			{
				return true;
			}
		}
	}
	return false;
}

void Console::HandleInput()
{
	//DWORD previousMode = 0;
	//FlushConsoleInputBuffer(_hCInput);
	GetNumberOfConsoleInputEvents(_hCInput, &_numOfEvents);
	if (_numOfEvents != 0)
	{
		if (!SetConsoleMode(_hCInput, _inputMode))
		{
			throw printf("Error setting input mode. (%d)\n", GetLastError());
		}
		else if (!ReadConsoleInput(_hCInput, _inputBuffer, _numOfEvents, &_eventCount))
		{
			throw printf("Problem whilst reading events. (%d)\n", GetLastError());
		}
	}
	//	SetConsoleMode(_hCInput, previousMode);
}

void Console::Update()
{
	/*
	GetNumberOfConsoleInputEvents(_hCInput, &_numOfEvents);
	if (_numOfEvents != 0) {
	if (!SetConsoleMode(_hCInput, _inputMode))
	{
	throw GetLastError();
	}
	else if (!ReadConsoleInput(_hCInput, _inputBuffer, _numOfEvents, &_eventCount))
	{
	throw GetLastError();
	}
	for (U32 i = 0; i < _numOfEvents; i++)
	{
	if (_inputBuffer[i].EventType == KEY_EVENT)
	{

	}
	}
	}
	*/
	HandleInput();
	/*	if (IsKeyDown('A'))
	{
	Vector2D currentPos = sceneManager->GetGObjectByID("Player")->GetPosition();
	currentPos.y++;
	sceneManager->GetGObjectByID("Player")->SetPosition(currentPos);
	}
	*/
	if (GetAsyncKeyState(VK_LEFT))
	{
		Vector2D currentPos = sceneManager->GetGObjectByID("Player")->GetPosition();
		if (currentPos.y >= 1)
		{
			currentPos.y--;
		}
		sceneManager->GetGObjectByID("Player")->SetPosition(currentPos);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		Vector2D currentPos = sceneManager->GetGObjectByID("Player")->GetPosition();
		if (currentPos.y <= (_screenBufferSize.X - sceneManager->GetGObjectByID("Player")->GetSize().y) - 1)
		{
			currentPos.y++;
		}
		sceneManager->GetGObjectByID("Player")->SetPosition(currentPos);
	}
	SetBuffer();
}

void Console::SetBuffer()
{
	int globalCounter = 0;
	//Clear Buffer
	for (auto &itr : _renderBuffer.bufferData)
	{
		itr.Char.AsciiChar = ' ';
		itr.Char.UnicodeChar = ' ';
		itr.Attributes = 0;
	}

	//Render Objects stored in sceneManager
	if (sceneManager.get() == nullptr)
	{
		printf("Problem\n");
	}
	if (!sceneManager.get()->GetGObects().empty())
	{
		for (auto &objItr : sceneManager->GetGObects())
		{
			//Switch through different object types
			try
			{
				int positionX = (((objItr.get()->GetPosition().x - 1) * _screenBufferSize.X));
				int finalXPosition = positionX;
				globalCounter = 0;
				switch (objItr.get()->GetType())
				{
				case Etype::Player:
					for (render_X = 0; render_X < objItr.get()->GetSize().x; render_X++)
					{
						finalXPosition = positionX + (render_X * _screenBufferSize.X);
						for (render_Y = 0; render_Y < objItr.get()->GetSize().y; render_Y++)
						{
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Char.AsciiChar = objItr.get()->GetRenderData().at(globalCounter).Char.AsciiChar;
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Char.UnicodeChar = objItr.get()->GetRenderData().at(globalCounter).Char.UnicodeChar;
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Attributes = objItr.get()->GetRenderData().at(globalCounter).Attributes;
							globalCounter++;
						}
					}
					break;
				case Etype::Standard:
					for (render_X = 0; render_X < objItr.get()->GetSize().x; render_X++)
					{
						finalXPosition = positionX + (render_X * _screenBufferSize.X);
						for (render_Y = 0; render_Y < objItr.get()->GetSize().y; render_Y++)
						{
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Char.AsciiChar = objItr.get()->GetRenderData().at(globalCounter).Char.AsciiChar;
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Char.UnicodeChar = objItr.get()->GetRenderData().at(globalCounter).Char.UnicodeChar;
							_renderBuffer.bufferData.at((finalXPosition + objItr.get()->GetPosition().y) + render_Y)
								.Attributes = objItr.get()->GetRenderData().at(globalCounter).Attributes;
							globalCounter++;
						}
					}
					break;
				default:
					break;
				}
			}
			catch (const std::out_of_range& e)
			{
				std::cerr << "Out of range: " << e.what() << '\n';
				printf("Out of range!\n");
				//Log if DEBUG
			}
		}
	}
}
