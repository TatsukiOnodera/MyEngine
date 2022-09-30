#pragma once

/// <summary>
/// ゲームパッド（XBOX）
/// </summary>
enum BUTTON
{
	U_CROSS = 0x0001,
	D_CROSS = 0x0002,
	L_CROSS = 0x0004,
	R_CROSS = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	L_STICK = 0x0040,
	R_STICK = 0x0080,
	LB = 0x0100,
	RB = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};

/// <summary>
/// マウス
/// </summary>
enum MOUSE
{
	LEFT, RIGHT, MIDDLE
};
