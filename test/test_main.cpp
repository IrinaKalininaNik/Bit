#include <gtest.h>
#include "tbitfield.h"
#include "tset.h"
#include <iostream>
#include <bitset>

#define TEST_USE 0 //0 тестирование tBitFIeld, остальное Tset


#if TEST_USE == 0
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	try
	{

		std::cout << RUN_ALL_TESTS();

	}
	catch (BITFIELD_ERRORS err)
	{
		std::cout << "Throw error number:" << err << endl;

	}
	system("PAUSE");

	return 0;
}
#else
int main(int argc, char **argv) {

	try
	{
		TSet temp(60);
		TSet temp1(temp);
		TBitField t0(60);
		TSet temp2(t0);
		(TBitField)temp2;

		std::cin >> temp;
		std::cout << temp;
		temp1.InsElem(25);
		std::cout << std::endl;
		temp2 = temp2 + 50;
		temp2 = temp2 + 25;
		std::cout << temp2<< std::endl;
		temp2 = temp2 - 50;
		std::cout << temp2<<std::endl;
		std::cout << (temp1 == temp2) << std::endl << std::endl;
		std::cout << ~temp2;
	}
	catch (SET_ERRORS err)
	{
		std::cout << "Throw error number:" << err << endl;

	}
	system("PAUSE");

	return 0;
}
#endif