#pragma once
#include <string>
using namespace std;

class Date
{
private:
	static bool is31(int month)
	{
		return month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12;
	}

	static bool isRun(int year)
	{
		if (year % 4 == 0 && year % 100 != 0)
			return true;
		else if (year % 400 == 0)
			return true;
		else
			return false;
	}

	static int getDay(int year, int month)
	{
		if (Date::is31(month))
			return 31;
		else
		{
			if (month == 2)
			{
				if (isRun(year))
					return 29;
				else
					return 28;
			}
			else
				return 30;
		}
	}
public:
	Date()
	{
		year = 2018;
		month = 1;
		day = 1;
	}
	Date(int year, int month, int day) : year(year), month(month), day(day) {}
	Date(const Date &d) : year(d.year), month(d.month), day(d.day) {}
	void reSet(int year, int month, int day)
	{
		this->year = year;
		this->month = month;
		this->day = day;
	}
	int year;
	int month;
	int day;

	string toString()
	{
		return to_string(year) + "年" + to_string(month) + "月" + to_string(day) + "日";
	}

	void nextMonth()
	{
		this->month += 1;
		if (this->month == 13)
		{
			this->year += 1;
			this->month = 1;
		}
	}

	void nextDay()
	{
		this->day += 1;
		int monDay = Date::getDay(this->year, this->month);
		if (day == monDay + 1)
		{
			this->day = 1;
			this->nextMonth();
		}
	}

	bool isBigger(Date &d) //本体比d大返回true
	{
		if (this->year > d.year)
			return true;
		else if (this->year < d.year)
			return false;
		else
		{
			if (this->month > d.month)
				return true;
			else if (this->month < d.month)
				return false;
			else
			{
				return this->day > d.day;
			}
		}
	}

	int dvalue(Date &d) //参数是大的
	{
		int result = 0;
		Date ct = *this;

		if (ct.month != d.month) //需要追
		{
			result += Date::getDay(ct.year, ct.month) - ct.day + 1;
			ct.day = 1;
			ct.nextMonth();
		}

		for (; !(ct.year == d.year && ct.month == d.month); ct.nextMonth()) //让ct追d
			result += Date::getDay(ct.year, ct.month);

		result += d.day - ct.day; //同月补全

		return result;
	}
};