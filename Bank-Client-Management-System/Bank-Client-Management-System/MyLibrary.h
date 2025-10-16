#pragma once
#include <iostream>
#include <string>

using namespace std;

namespace input
{
	int ReadNumberInRange(string Message, int From, int To)
	{
		int Num = 0;
		do
		{
			cout << Message;
			cin >> Num;
		} while (Num < From || Num > To);

		return Num;
	}

	string ReadString(string Message)
	{
		string str = "";
		cout << Message;
		cin.ignore(1, '\n');
		getline(cin, str);
		return str;
	}
}

namespace Print_String_Format
{
	string PrintLine(string sign, int LengthOfLine)
	{
		string L = u8"";
		for (int i = 0; i < LengthOfLine; i++)
		{
			L += sign;
		}
		return L;
	}
}