#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
#include "bigint.h"

int main() {
	setlocale(LC_ALL, "Rus");


	std::cout << "Конструкторы:\n";

	BigInt x;
	std::cout << "x = " << x << "\n";
	x = BigInt(10312031);
	std::cout << "x = " << x << "\n";
	x = BigInt(-123124);
	std::cout << "x = " << x << "\n";
	x = BigInt("786275621793482731486231984782391649017827470237");
	std::cout << "x = " << x << "\n";
	x = BigInt("-314123");
	std::cout << "x = " << x << "\n";
	BigInt y(x);
	std::cout << "y = " << y << "\n";


	std::cout << "\nЛогика:\n";

	x = BigInt("21898412940814");
	y = BigInt("12312133123");

	std::cout << "x > y = " << (x > y) << "\n";
	std::cout << "x < y = " << (x < y) << "\n";
	std::cout << "x >= y = " << (x >= y) << "\n";
	std::cout << "x <= y = " << (x <= y) << "\n";
	std::cout << "x == y = " << (x == y) << "\n";
	std::cout << "x != y = " << (x != y) << "\n";

	std::cout << "~x = " << ~x << "\n";
	std::cout << "~y = " << ~y << "\n";

	std::cout << "x | y = " << (x | y) << "\n";
	std::cout << "x & y = " << (x & y) << "\n";
	std::cout << "x ^ y = " << (x ^ y) << "\n";

	std::cout << "-x | y = " << (-x | y) << "\n";
	std::cout << "-x & y = " << (-x & y) << "\n";
	std::cout << "-x ^ y = " << (-x ^ y) << "\n";

	std::cout << "x | -y = " << (x | -y) << "\n";
	std::cout << "x & -y = " << (x & -y) << "\n";
	std::cout << "x ^ -y = " << (x ^ -y) << "\n";

	std::cout << "-x | -y = " << (-x | -y) << "\n";
	std::cout << "-x & -y = " << (-x & -y) << "\n";
	std::cout << "-x ^ -y = " << (-x ^ -y) << "\n";


	std::cout << "\nАрифметика:\n";

	x = BigInt("375892487243508234098234293582");
	std::cout << "+x = " << +x << "\n";
	std::cout << "-x = " << -x << "\n";
	x = -x;
	std::cout << "+(-x) = " << +x << "\n";
	std::cout << "-(-x) = " << -x << "\n";
	x = -x;

	y = BigInt("326587236578726");

	x += y;
	std::cout << "x += y = " << x << "\n";
	x -= y;
	std::cout << "x -= y = " << x << "\n";

	std::cout << "x + y = " << (x + y) << "\n";
	std::cout << "x - y = " << (x - y) << "\n";
	std::cout << "x / y = " << (x / y) << "\n";
	std::cout << "x % y = " << (x % y) << "\n";

	x = -x;
	std::cout << "-x + y = " << (x + y) << "\n";
	std::cout << "-x - y = " << (x - y) << "\n";
	std::cout << "-x / y = " << (x / y) << "\n";
	std::cout << "-x % y = " << (x % y) << "\n";

	x = -x;
	y = -y;
	std::cout << "x + -y = " << (x + y) << "\n";
	std::cout << "x - -y = " << (x - y) << "\n";
	std::cout << "x / -y = " << (x / y) << "\n";
	std::cout << "x % -y = " << (x % y) << "\n";

	x = -x;
	std::cout << "-x + -y = " << (x + y) << "\n";
	std::cout << "-x - -y = " << (x - y) << "\n";
	std::cout << "-x / -y = " << (x / y) << "\n";
	std::cout << "-x % -y = " << (x % y) << "\n";

	x = -x;
	y = -y;
	std::cout << "x * y = " << (x * y) << "\n";
	std::cout << "x * y = " << (-x * y) << "\n";
	std::cout << "x * y = " << (x * -y) << "\n";
	std::cout << "x * y = " << (-x * -y) << "\n";

	x *= y;
	std::cout << "x *= y = " << x << "\n";
	x /= y;
	std::cout << "x /= y = " << x << "\n";
	x %= y;
	std::cout << "x %= y = " << x << "\n";
 }