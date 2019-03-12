#include <string>
#include <math.h>
using namespace std;

int evalChar(char c)
{
	if (c <= '9')
		return c - 48;
	if(c>='A')
		return c - 55;
}

int eval(string num, int system)
{
	int result = 0;
	for (int i = 0; i < num.size(); i++)
		result += evalChar(num[i])*pow(system, num.size() - 1 - i);
	return result;
}

string toString(int bit)
{
	char c;
	if (bit >= 10)
		c = bit + 55;
	else
		c = bit + 48;
	return string(1, c);
}

string toOthSystem(int num, int system)
{
	string result;
	do
	{
		int bit = num % system;
		result = toString(bit) + result;
		num /= system;
	} while (num != 0);
	return result;
}
