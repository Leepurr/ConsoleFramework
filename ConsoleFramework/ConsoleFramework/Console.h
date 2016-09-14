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
#include <vector>
#include "Core\Datatypes.h"
#include "Core\GObject.h"
#include "Core\Vector.h"
#include "SceneManager.h"
#include "interfaces.h"

#define SUCCESS					0x00
#define CANTSETNEWSCREENBUFFER	0x01
#define CANTGETSCREENBUFFERINFO 0x02
#define CANTSETSCREENBUFFERSIZE 0x03
#define CANTRESIZEWINDOW		0x04
#define BADHANDLE				0x05
#define SETCONSOLEMODEFAILED	0x06

#define SAFE_DELETE(p) { if(p) {delete (p); (p)=NULL; }}
#define SAFE_DELETE_ARRAY(p) { if(p) {delete[] (p); (p)=NULL; }}

enum class EStatus
{
	Start = 0,
	Running,
	Quit
};

struct RenderBuffer
{
	std::vector<CHAR_INFO>	bufferData;
	I32 GetBufferSize() const { return bufferData.size(); }
};

class Console
{
private:
	//ConsoleMode
	HANDLE _hCInput;
	DWORD _inputMode;
	DWORD _numOfEvents;
	DWORD _eventCount;
	INPUT_RECORD _inputBuffer[128];
	//~ConsoleMode
	HWND _hWndConsoleWindow;
	HANDLE _hCScreenBuffer;
	CONSOLE_SCREEN_BUFFER_INFO _cScreenBufferInfo;
	COORD _screenBufferSize;
	SMALL_RECT _windowSize;
	SMALL_RECT _writeRect;
	COORD _dwBufferCoord;				//The Coordinates of the upper-left cell
	RenderBuffer _renderBuffer;
	U32 render_X, render_Y;						//Render Iterators
	U16 _CellCount;
	std::unique_ptr<SceneManager> sceneManager;
private:
	void Console::HandleInput();
public:
	EStatus _eStatus;
public:
	Console() : sceneManager(std::make_unique<SceneManager>()) {}
	~Console() {}
	Console(Console const &) = delete;
	Console(Console const &&) = delete;
	Console& operator=(Console const &) = delete;
	Console& operator=(Console const &&) = delete;
	U16 Initialise(const I16 cols, const I16 rows);
	U8 Load();
	void CleanUp();
	void Update();
	void Render();
	void GameLogic();
	void SetBuffer();
	bool Console::IsKeyDown(const UChar chr);
};