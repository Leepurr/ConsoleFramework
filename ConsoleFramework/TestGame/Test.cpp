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
#include "Test.h"

int main(void)
{
	LARGE_INTEGER Frequency, Tick, Tock;	int result = _Console.Initialise((short)160, (short)50);
	if (result == 0)
	{
		isRunning = true;
		QueryPerformanceFrequency(&Frequency);
		double SecondsPerTick = (1.0 / (double)Frequency.QuadPart);
		QueryPerformanceCounter(&Tick);
		//		MSG msg;
		if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
		{
			_Console.Load();
			while (isRunning)
			{
				//Deal with OS Messages
				//	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				//	{
				//		TranslateMessage(&msg);
				//		DispatchMessage(&msg);
				//	}

				_Console.Update();
				_Console.Render();
				Sleep(10);
				/*
				Do Game Update stuff here
				*/

				QueryPerformanceCounter(&Tock);
				I64 interval = (Tock.QuadPart - Tick.QuadPart);
				double secondsPassed = ((double)interval * SecondsPerTick);
				QueryPerformanceCounter(&Tick);
			}
		}
	}
	return 0;
}

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		printf("Ctrl-C event\n\n");
		Beep(750, 300);
		return(TRUE);

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
		isRunning = false;
		_Console.CleanUp();
		Beep(50, 200);
		return(TRUE);
	}
	return(FALSE);
}