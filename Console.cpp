﻿#include "Console.h"

HANDLE hConsoleOutput;
HANDLE hConsoleInput;

// Function to change console screen size.
void resizeConsole(SHORT width, SHORT height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);

	COORD crd = { width, height };
	SMALL_RECT rec = { 0, 0, width - 1, height - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(hConsoleOutput, TRUE, &rec);
	SetConsoleScreenBufferSize(hConsoleOutput, crd);
}

// The function to clear the screen does not cause lag.
void clrscr(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	COORD Home = { 0, 0 };
	DWORD dummy;

	hConsoleOutput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	FillConsoleOutputCharacter(hConsoleOutput, ' ', screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y, Home, &dummy);
	screen_buffer_info.dwCursorPosition.X = 0;
	screen_buffer_info.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOutput, screen_buffer_info.dwCursorPosition);
}

// The function moves the cursor to the x coordinate, idea.
void gotoXY(SHORT x, SHORT y)
{
	COORD Cursor_an_Pos = { x, y };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

// Returns the current x position.
SHORT wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOutput, &coninfo);
	return coninfo.dwCursorPosition.X;
}

// Returns the current y position.
SHORT wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOutput, &coninfo);
	return coninfo.dwCursorPosition.Y;
}

// Color function
void setColor(WORD color)
{
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

// Function to change display background color.
void setBackgroundColor(WORD color)
{
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	color <<= 4; // Dich trai 3 bit de phu hop voi mau nen
	wAttributes &= 0xff0f; // Cai 0 cho 1 bit chu nhay va 3 bit mau nen
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

WORD textattr()
{
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
	GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleInfo);
	return ConsoleInfo.wAttributes;
}

void resettextattr()
{
	DWORD Mau_Mac_Dinh = textattr();
	SetConsoleTextAttribute(hConsoleOutput, Mau_Mac_Dinh);
}

// The function prints the letter form and changes forever so it is displayed according to the x position, meaning.
void setColorBGTextXY(SHORT x, SHORT y, WORD color, WORD background, LPCSTR str, ...)
{
	gotoXY(x, y);
	setBackgroundColor(background);
	setColor(color);

	/*Print more words*/
	va_list args;
	va_start(args, str);
	vprintf(str, args);
	va_end(args);
	
	resettextattr();
	//setColor(7);
}

// The function hides and shows the pointer.
void Cursor(BOOL bVisible, DWORD dwSize)
{
	CONSOLE_CURSOR_INFO ConsoleCursorInfo{};
	ConsoleCursorInfo.bVisible = bVisible;
	ConsoleCursorInfo.dwSize = dwSize; // Phan tram bao trum o cua con tro chuot
	SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);
}

// Delete rows, SStartPos - Site starts deleting, SNumberRow number of rows need to delete
void deleteRow(SHORT SStartPos, SHORT SNumberRow)
{
	CONSOLE_SCREEN_BUFFER_INFO  ConsoleInfo;
	COORD Pos = { 0, SStartPos };
	DWORD Tmp;
	GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleInfo);
	FillConsoleOutputCharacter(hConsoleOutput, ' ', ConsoleInfo.dwSize.X * SNumberRow, Pos, &Tmp);
	FillConsoleOutputAttribute(hConsoleOutput, 15, ConsoleInfo.dwSize.X * SNumberRow, Pos, &Tmp);
	SetConsoleCursorPosition(hConsoleOutput, Pos);
}

// Functions print the color of the cells according to X and Y.
void setColorCell(SHORT x, SHORT y, SHORT t_color, SHORT b_color, std::string name)
{
	setBackgroundColor(b_color);
	int len = name.length();
	setColor(t_color);
	for (int i = 0; i < len; ++i)
	{
		gotoXY(x + i, y);
		std::cout << " ";
	}
	gotoXY(x, y); std::cout << name;
	setBackgroundColor(0);
}
