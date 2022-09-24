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
		Button_View,
		Button_Menu,
		Button_LS,
		Button_RS,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left
	};

	/// <summary>
	/// マウス
	/// </summary>
	enum MouseKind
	{
		M_Left, M_Right, M_Middle
	};
};