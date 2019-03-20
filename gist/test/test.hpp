#pragma once
#include <iostream>

#define TEST(CODE) if (CODE()) success++; else fail++
#define JUDGE(RIGHT) if (result == RIGHT)\
					{\
						cout<<name + "---->PASS"<<endl;\
						return true;\
					}\
					else\
					{\
						cout<<name + "---->FAIL"<<endl;\
						return false;\
					}
#define JUDGE2(CON) if (CON)\
					{\
						cout<<name + "---->PASS"<<endl;\
						return true;\
					}\
					else\
					{\
						cout<<name + "---->FAIL"<<endl;\
						return false;\
					}
#define NAME(N) string name = N

//下面是例子
class testborrowStru
{
private:
	book *b;
	reader *r;
public:
	testborrowStru()
	{
		b = new book("语文书", "语文组", "语文出版社", Date(), 50, "教科书", 50);
		r = new reader(1, "宇文述", "一年一班", readerType("学生", 1, 10));
	}

	bool constr()
	{
		NAME("constr");

		borrowStru bs(b, r, Date(2018, 1, 2));
		Date d2 = bs.d2;

		JUDGE2(d2.year == 2018 && d2.month == 2 && d2.day == 2)
	}

	bool con()
	{
		NAME("con");

		borrowStru bs(b, r, Date(2018, 1, 2));
		bs.con();
		Date d2 = bs.d2;

		JUDGE2(d2.year == 2018 && d2.month == 3 && d2.day == 2)
	}

	bool con2()
	{
		NAME("con2");

		borrowStru bs(b, r, Date(2018, 1, 2));
		bs.con();
		bs.con();
		bool result= bs.con();

		JUDGE(false)
	}

	bool con3()
	{
		NAME("con3");

		borrowStru bs(b, r, Date(2018, 1, 2));
		bool result = bs.con();

		JUDGE(true)
	}

	bool calu()
	{
		NAME("calu");

		borrowStru bs(b, r, Date(2018, 1, 1)); //应该二月一日还书
		int result = bs.calu(Date(2018, 3, 1)); //但实际三月一号还，应罚款28元

		JUDGE(28)
	}

	bool isPublish1()
	{
		NAME("isPublish1");

		borrowStru bs(b, r, Date(2018, 1, 1)); //应该二月一日还书
		bool result = bs.isPublish(Date(2018, 3, 1));

		JUDGE(true)
	}

	bool isPublish2()
	{
		NAME("isPublish2");

		borrowStru bs(b, r, Date(2018, 1, 1)); //应该二月一日还书
		bool result = bs.isPublish(Date(2018, 1, 10));

		JUDGE(false)
	}

	void runAll()
	{
		string name = "testborrowStru";
		cout<<"----" + name + "测试组启动----"<<endl;

		int success = 0;
		int fail = 0;

		TEST(constr);
		TEST(con);
		TEST(con2);
		TEST(con3);
		TEST(calu);
		TEST(isPublish1);
		TEST(isPublish2);

		cout<<name + "测试组：成功" + to_string(success) + "个 失败" + to_string(fail) + "个"<<endl;
	}
};
