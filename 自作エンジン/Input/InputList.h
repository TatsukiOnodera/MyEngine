#pragma once

class InputList
{
public:
	/// <summary>
	/// ゲームパッド（XBOX）
	/// </summary>
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LS,
		Button_RS,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	/// <summary>
	/// マウス
	/// </summary>
	enum MouseKind
	{
		M_Left, M_Right, M_Middle
	};

	/// <summary>
	/// スティック
	/// </summary>
	enum StickKind
	{
		S_Up, S_Down, S_Right, S_Left
	};
};