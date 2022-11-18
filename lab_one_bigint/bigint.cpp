#include <iostream>
#include <vector>
#include <algorithm>
#include "bigint.h"

BigInt::BigInt() {
	this->sign = 1;
}
BigInt::BigInt(int num) {
	this->sign = num >= 0;
	num = abs(num);
	if (num) {
		while (num != 0) {
			this->num.push_back(num % 10);
			num /= 10;
		}
	}
}
BigInt::BigInt(std::string string) {
	this->sign = string[0] == '-' ? 0 : 1;
	int i = !this->sign;
	bool flag = 0;  // 0 - не было цифр, 1 - были
	while (string[i] != '\0') {
		if (string[i] < 48 || string[i] > 57) {
			*this = BigInt(0);
			throw std::invalid_argument("Wrong symbol");
		}
		if (!(string[i] == '0' && !flag)) {
			if (!flag) {
				flag = 1;
			}
			this->num.push_back(string[i] - '0');
		}
		++i;
	}
	std::reverse(this->num.begin(), this->num.end());
}
BigInt::BigInt(const BigInt& orig) {
	this->sign = orig.sign;
	for (int i = 0; i < orig.num.size(); ++i) {
		this->num.push_back(orig.num[i]);
	}
}
BigInt::~BigInt() = default;

BigInt biggest_power_of_two(const BigInt& orig) {
	BigInt bigint = orig;
	BigInt res = BigInt(1);
	BigInt two = BigInt(2);
	bigint.sign = true;
	while (res <= bigint) {
		res *= two;
	}
	return res / two;
}
std::string to_binary(BigInt orig) {
	std::string res;
	BigInt bigint = orig;
	BigInt two = BigInt(2);
	BigInt begin = biggest_power_of_two(bigint);

	if (bigint == BigInt()) {
		return res;
	}

	bigint.sign = true;
	res += 1;
	bigint -= begin;
	while (true) {
		begin /= two;

		if (begin.num.size() == 0) {
			break;
		}

		if (begin <= bigint) {
			res += 1;
			bigint -= begin;
		}
		else {
			res += char(0);
		}
	}

	bigint.sign = orig.sign;
	if (!bigint.sign) {
		for (int i = 0; i < res.size(); ++i) {
			if (res[i] == 1) res[i] = 0;
			else res[i] = 1;
		}
		int i = res.size() - 1;
		res[i] += 1;
		while (i > -1 && res[i] == 2) {
			res[i] = 0;
			--i;
		}
	}
	return res;

}
BigInt to_bigint(std::string string, bool sign) {
	BigInt two = BigInt(2);
	BigInt factor = BigInt(1);
	BigInt res;
	if (!sign) {
		for (int i = string.size() - 1; i > -1; --i) {
			if (string[i] == 1) {
				string[i] = 0;
				break;
			}
			else {
				string[i] = 1;
			}
		}
		for (int i = 0; i < string.size(); ++i) {
			if (string[i] == 1) string[i] = 0;
			else string[i] = 1;
		}
	}
	for (int i = string.size() - 1; i > -1; --i) {
		if (string[i] == 1) {
			res += factor;
		}
		factor *= two;
	}
	return res;
}
void to_one_len(std::string& first, std::string& second, bool f_sign, bool s_sign) {
	bool changeable_sign;
	bool not_changeable_sign;
	std::string* changeable;
	std::string* not_changeable;
	if (first.size() < second.size()) {
		changeable = &first;
		changeable_sign = f_sign;
		not_changeable = &second;
	}
	else {
		changeable = &second;
		changeable_sign = s_sign;
		not_changeable = &first;
	}
	if (changeable_sign) {
		while (changeable->size() != not_changeable->size()) {
			*changeable = char(0) + *changeable;
		}
	}
	else {
		while (changeable->size() != not_changeable->size()) {
			*changeable = char(1) + *changeable;
		}
	}
}
BigInt BigInt::operator~() const {
	return -*this - BigInt(1);
}
BigInt operator%(const BigInt& left, const BigInt& right) {
	BigInt div;
	div = left / right * right;
	if (!left.sign) {
		right.sign ? div -= right : div += right;
	}
	return left - div;
}
BigInt operator&(const BigInt& left, const BigInt& right) {
	std::string left_b = to_binary(left);
	std::string right_b = to_binary(right);
	std::string res_b;
	BigInt res;
	to_one_len(left_b, right_b, left.sign, right.sign);
	for (int i = 0; i < left_b.size(); ++i) {
		left_b[i] == 1 && right_b[i] == 1 ? res_b += 1 : res_b += char(0);
	}
	res = to_bigint(res_b, left.sign | right.sign);
	res.sign = left.sign | right.sign;
	return res;
}
BigInt operator|(const BigInt& left, const BigInt& right) {
	std::string left_b = to_binary(left);
	std::string right_b = to_binary(right);
	std::string res_b;
	BigInt res;
	to_one_len(left_b, right_b, left.sign, right.sign);
	for (int i = 0; i < left_b.size(); ++i) {
		left_b[i] == 1 || right_b[i] == 1 ? res_b += 1 : res_b += char(0);
	}
	res = to_bigint(res_b, left.sign & right.sign);
	res.sign = left.sign & right.sign;
	return res;
}
BigInt operator^(const BigInt& left, const BigInt& right) {
	std::string left_b = to_binary(left);
	std::string right_b = to_binary(right);
	std::string res_b;
	BigInt res;
	to_one_len(left_b, right_b, left.sign, right.sign);
	for (int i = 0; i < left_b.size(); ++i) {
		left_b[i] != right_b[i] ? res_b += 1 : res_b += char(0);
	}
	res = to_bigint(res_b, !(left.sign ^ right.sign));
	res.sign = !(left.sign ^ right.sign);
	return res;
}

std::ostream& operator<<(std::ostream& o, const BigInt& i) {
	if (!i.num.size()) {
		o << 0;
	}
	else {
		if (!i.sign) {
			o << "-";
		}
		for (int j = i.num.size() - 1; j > -1; --j) {
			o << int(i.num[j]);
		}
	}
	return o;
}

BigInt& BigInt::operator=(const BigInt& right) {
	BigInt temp = BigInt(right);
	this->sign = temp.sign;
	this->num.erase(this->num.cbegin(), this->num.cend());
	for (int i = 0; i < temp.num.size(); ++i) {
		this->num.push_back(temp.num[i]);
	}
	return *this;
}

BigInt BigInt::operator+() const {
	BigInt update = BigInt(*this);
	return update;
}
BigInt BigInt::operator-() const {
	BigInt update = BigInt(*this);
	update.sign = !update.sign;
	return update;
}

bool BigInt::operator==(const BigInt& right) const {
	if (this->sign != right.sign || this->num.size() != right.num.size()) {
		return false;
	}
	for (int i = 0; i < this->num.size(); i++) {
		if (this->num[i] != right.num[i]) {
			return false;
		}
	}
	return true;
}
bool BigInt::operator!=(const BigInt& right) const {
	return !(*this == right);
}
bool BigInt::operator<(const BigInt& right) const {
	if (!this->sign && right.sign) {
		return true;
	}
	else if (this->sign && !right.sign) {
		return false;
	}
	if (this->num.size() < right.num.size()) {
		return this->sign;
	}
	else if (this->num.size() > right.num.size()) {
		return !this->sign;
	}
	else {
		for (int i = this->num.size() - 1; i > -1; --i) {
			if (this->num[i] < right.num[i]) {
				return this->sign;
			}
			else if (this->num[i] > right.num[i]) {
				return !this->sign;
			}
		}
		return !this->sign;
	}
}
bool BigInt::operator>(const BigInt& right) const {
	if (!(*this < right) && *this != right) {
		return true;
	}
	else {
		return false;
	}
}
bool BigInt::operator<=(const BigInt& right) const {
	return !(*this > right);
}
bool BigInt::operator>=(const BigInt& right) const {
	return !(*this < right);
}

BigInt::operator int() const {
	int res = 0;
	for (int i = this->num.size() - 1; i > -1; --i) {
		res *= 10;
		res += this->num[i];
	}
	if (!this->sign) res = -res;
	return res;
}
BigInt::operator std::string() const {
	std::string res = "";
	if (!this->sign) res += "-";
	for (int i = this->num.size() - 1; i > -1; --i) {
		res += this->num[i] + 48;
	}
	return res;
}

BigInt& BigInt::operator+=(const BigInt& right) {
	*this = *this + right;
	return *this;
}
BigInt& BigInt::operator*=(const BigInt& right) {
	*this = *this * right;
	return *this;
}
BigInt& BigInt::operator-=(const BigInt& right) {
	*this = *this - right;
	return *this;
}
BigInt& BigInt::operator/=(const BigInt& right) {
	*this = *this / right;
	return *this;
}
BigInt& BigInt::operator^=(const BigInt& right) {
	*this = *this ^ right;
	return *this;
}
BigInt& BigInt::operator%=(const BigInt& right) {
	*this = *this % right;
	return *this;
}
BigInt& BigInt::operator&=(const BigInt& right) {
	*this = *this & right;
	return *this;
}
BigInt& BigInt::operator|=(const BigInt& right) {
	*this = *this | right;
	return *this;
}

BigInt operator+(const BigInt& left, const BigInt& right) {
	int val;
	int carry = 0;
	int i = 0;
	BigInt res;

	if (!left.sign && right.sign) {
		return right - (-left);
	}
	if (left.sign && !right.sign) {
		return left - (-right);
	}

	left.sign ? res.sign = true : res.sign = false;

	while (true) {
		val = 0;

		if (i >= left.num.size() && i >= right.num.size()) {
			break;
		}

		if (i < left.num.size()) {
			val += left.num[i];
		}
		if (i < right.num.size()) {
			val += right.num[i];
		}

		val += carry;
		carry = val / 10;
		val %= 10;
		res.num.push_back(val);

		++i;
	}
	if (carry) {
		res.num.push_back(carry);
	}
	return res;
}
BigInt operator-(const BigInt& left, const BigInt& right) {
	int val;
	int size;
	int carry = 0;
	int i = 0;
	BigInt res;

	if (!left.sign && right.sign || left.sign && !right.sign) {
		return left + (-right);
	}
	if (!left.sign && !right.sign) {
		return (-right) - (-left);
	}
	if (right > left) {
		return -(right - left);
	}

	while (true) {
		val = 0;

		if (i >= left.num.size() && i >= right.num.size()) {
			break;
		}

		if (i < left.num.size()) {
			val += left.num[i];
		}
		if (i < right.num.size()) {
			val -= right.num[i];
		}

		val -= carry;
		if (val < 0) {
			val += 10;
			carry = 1;
		}
		else {
			carry = 0;
		}
		res.num.push_back(val);

		++i;
	}
	size = res.num.size();
	for (int i = size - 1; i > -1; --i) {
		if (res.num[i] == 0) {
			res.num.erase(res.num.cbegin() + i);
		}
		else {
			break;
		}
	}
	return res;
}

BigInt naive_mult(const BigInt& left, const BigInt& right) {
	BigInt res;
	for (int i = 0; i < right.num.size(); i++) {
		for (int j = 0; j < right.num[i] * std::pow(10, i); j++) {
			res = res + left;
		}
	}
	return res;
}
void add_zeros(BigInt& orig, int count_of_zeros) {
	for (int i = 0; i < count_of_zeros; i++) {
		orig.num.push_back(0);
	}
}
void remove_zeros(BigInt& orig) {
	while (orig.num.size() > 0 && orig.num[orig.num.size() - 1] == 0) {
		orig.num.pop_back();
	}
}
BigInt operator*(const BigInt& left, const BigInt& right) {  // умножение Карацубы
	BigInt res;

	if (left == BigInt(1)) {
		res = BigInt(right);
		res.sign = left.sign == right.sign;
		return res;
	}
	if (right == BigInt(1)) {
		res = BigInt(left);
		res.sign = left.sign == right.sign;
		return res;
	}

	if (!(left.num.size() == 0 || right.num.size() == 0)) {
		if (left.num.size() < 4 && right.num.size() < 4) {
			res = naive_mult(left, right);
		}
		else {
			BigInt new_left = BigInt(left);
			BigInt new_right = BigInt(right);

			int dif = new_left.num.size() - new_right.num.size();
			if (dif != 0) {
				dif > 0 ? add_zeros(new_right, dif) : add_zeros(new_left, -dif);
			}

			if (new_left.num.size() % 2) {
				add_zeros(new_left, 1);
				add_zeros(new_right, 1);
			}

			int len = new_left.num.size() / 2;
			BigInt left_left = BigInt(new_left);
			BigInt left_right = BigInt(new_left);
			BigInt right_left = BigInt(new_right);
			BigInt right_right = BigInt(new_right);
			std::reverse(left_left.num.begin(), left_left.num.end());
			std::reverse(right_left.num.begin(), right_left.num.end());
			for (int i = 0; i < len; i++) {
				left_left.num.pop_back();
				left_right.num.pop_back();
				right_left.num.pop_back();
				right_right.num.pop_back();
			}
			std::reverse(left_left.num.begin(), left_left.num.end());
			std::reverse(right_left.num.begin(), right_left.num.end());
			remove_zeros(left_left);
			remove_zeros(left_right);
			remove_zeros(right_left);
			remove_zeros(right_right);

			BigInt a = left_left * right_left;
			BigInt b = (left_left * right_right) + (left_right * right_left);
			BigInt c = left_right * right_right;

			std::reverse(a.num.begin(), a.num.end());
			std::reverse(b.num.begin(), b.num.end());
			add_zeros(a, len * 2);
			add_zeros(b, len);
			std::reverse(a.num.begin(), a.num.end());
			std::reverse(b.num.begin(), b.num.end());

			res = a + b + c;
			remove_zeros(res);
		}
	}

	res.sign = left.sign == right.sign;
	return res;
}

int find_factor(const BigInt& dividend, const BigInt& divider) {
	int l = 1;
	int r = 9;
	BigInt check;
	BigInt check_res;

	check = BigInt(l);
	if (divider * check == dividend) {
		return l;
	}
	check = BigInt(r);
	if (divider * check == dividend) {
		return r;
	}

	while (r - l > 1) {
		check = BigInt((l + r) / 2);
		check_res = divider * check;

		if (check_res == dividend) {
			return (l + r) / 2;
		}

		else if (check_res > dividend) {
			r = (l + r) / 2;
		}
		else if (check_res < dividend) {
			l = (l + r) / 2;
		}
	}

	if (divider * BigInt(r) <= dividend) {
		return r;
	}
	else {
		return l;
	}
}
BigInt operator/(const BigInt& left, const BigInt& right) {
	bool flag = false;
	int factor;
	BigInt dividend;
	BigInt res;
	BigInt divider = BigInt(right);
	if (!divider.sign) {
		divider.sign = true;
	}

	if (!right.num.size()) {
		throw std::invalid_argument("Divider is zero :/");
	}

	int k = left.num.size() - 1;
	dividend.num.push_back(left.num[k]);

	while (true) {
		if (dividend >= divider) {
			if (!flag) {
				flag = 1;
			}
			factor = find_factor(dividend, divider);
			res.num.push_back(factor);
			dividend -= divider * BigInt(factor);
		}

		else {
			--k;
			if (k < 0) {
				break;
			}
			std::reverse(dividend.num.begin(), dividend.num.end());
			dividend.num.push_back(0);
			std::reverse(dividend.num.begin(), dividend.num.end());
			dividend += BigInt(left.num[k]);
			remove_zeros(dividend);

			if (flag && dividend < divider) {
				res.num.push_back(0);
			}
		}
	}
	std::reverse(res.num.begin(), res.num.end());
	res.sign = left.sign == right.sign;
	return res;
}