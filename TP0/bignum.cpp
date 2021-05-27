#include "Bignum.h"
#include "utils.h"
#include <iostream>
#include <cmath>

using namespace std;

Bignum::Bignum(){
	sign = false;
	size = 0;
	digits = nullptr;
}

Bignum::Bignum(const bool &_sign,const size_t &_size,const unsigned short *_digits) {

	sign = _sign;
	size_t z = zerocount(_digits, _size);

	if ( z == _size ){
		size = 1;
		sign = false;
		digits = new unsigned short[size];
		*digits = 0 ;
	}
	else{
		size = _size-z;
		digits = new unsigned short[size];
		for(size_t i = 0; i < size; i++) {
			digits[i] = _digits[i+z];
		}
	}
}

Bignum::Bignum(const string& n){

	size_t i,begin = 0;
	sign = false ;

	if ((sign = is_negative(n))){
		begin = 1;
	}

	size = n.size() - begin;

	digits = new unsigned short[size];

	for (i = 0; i < size; i++){
		if( isdigit(n[i+begin]) == false ){
			exit(1);
		}
		digits[i] = n[i+begin] - '0';
	}

}

Bignum::Bignum(const Bignum &b) {
	sign = b.sign;
	size = b.size;
	digits = new unsigned short[size];

	for (size_t i = 0; i < size; i++){
		digits[i] = b.digits[i];
	}
}

Bignum::~Bignum(){
	if (digits==nullptr){
		return;
	}
	delete[] digits;
}

Bignum const& Bignum::operator=(const Bignum &b){
	sign = b.sign;
	size = b.size;

	if(this==&b){
		return *this;
	}
	if (digits){
		delete [] digits;
	}
	digits = new unsigned short[size];

	for (size_t i = 0; i < size; i++){
		digits[i] = b.digits[i];
	}
	return *this;
}

Bignum operator+(const Bignum &b1, const Bignum &b2){
	Bignum nuevo;
	if(b1.sign != b2.sign) {
		if(b1.sign == true) {
			Bignum _b1(b1);
			_b1.sign = !b1.sign;
			nuevo = b2-_b1;
		}
		else {
			Bignum _b2(b2);
			_b2.sign = !b2.sign;
			nuevo = b1-_b2;
		}
	}
	else {
		bool sign = b1.sign;
		size_t size = (b1.size >= b2.size)?b1.size+1:b2.size+1;
		unsigned short *digits = new unsigned short[size];
		unsigned short carry = 0;

		for (size_t i = 1; i <= size; i++) {
			if (b1.size-i > size) {
				if(b2.size-i > size) {
					digits[size-i] = carry;
					carry = 0;
				}
				else {
					digits[size-i] = (b2.digits[b2.size-i] + carry)%10;
					carry = (b2.digits[b2.size-i] + carry)/10;
				}
			}
			else if(b2.size-i > size){
				digits[size-i] = (b1.digits[b1.size-i] + carry)%10;
				carry = (b1.digits[b1.size-i] + carry)/10;
			}
			else{
				digits[size-i] = (b1.digits[b1.size-i] + b2.digits[b2.size-i] + carry)%10;
				carry = (b1.digits[b1.size-i] + b2.digits[b2.size-i] + carry)/10;
			}
		}
		nuevo = Bignum(sign,size,digits);
		delete[] digits;
	}
	return nuevo;
}

Bignum operator-(const Bignum &b1, const Bignum &b2){
	Bignum nuevo;
	if(b2>b1){
		nuevo = b2-b1;
		nuevo.sign = !nuevo.sign;
	}
	else if(b2.sign == true){
		Bignum _b2(b2);
		_b2.sign = !b2.sign;
		nuevo = b1 + _b2;
	}
	else if(b1.sign == true){
		Bignum _b1(b1);
		_b1.sign = !b1.sign;
		nuevo = _b1+b2;
		nuevo.sign = !nuevo.sign;
	}
	else{
		bool sign = b1.sign;
		size_t size = (b1.size >= b2.size)?b1.size:b2.size;
		unsigned short *digits = new unsigned short[size];
		short carry = 0;

		for (size_t i = 1; i <= size; i++) {
			if(b1.size-i > size) {
				digits[size-i] = 10 - (b2.digits[b2.size-i] + carry);
				carry = 1;
			}
			else if(b2.size-i > size) {
				if(b1.digits[b1.size-i] < carry) {
					digits[size-i] = (b1.digits[b1.size-i]+10) - carry;
					carry = 1;
				}
				else {
					digits[size-i] = b1.digits[b1.size-i] - carry;
					carry = 0;
				}
			}
			else if(b1.digits[b1.size-i] < (b2.digits[b2.size-i] + carry)) {
				digits[size-i] = (b1.digits[b1.size-i]+10) - (b2.digits[b2.size-i] + carry);
				carry = 1;
			}
			else {
				digits[size-i] = (b1.digits[b1.size-i] - (b2.digits[b2.size-i] + carry));
				carry = 0;
			}
		}
		if(carry == 1) {
			sign = !sign;
		}
		nuevo = Bignum(sign,size,digits);
		delete[] digits;
	}
	return nuevo;
}

Bignum operator*(const Bignum&b1, const Bignum&b2){

	if(b1.size == 0 || b2.size == 0){
		Bignum zero("0");
		return zero;
	}

	bool signo;
	size_t tam (b1.size + b2.size);
	unsigned short carry = 0, *auxdig = new unsigned short [tam]{0};

	for(int i = b2.size-1; i >= 0; i--){
		carry = 0;
		for(int h = b1.size-1; h >= 0; h--){
			auxdig[i+h+1] += (carry + b2.digits[i] * b1.digits[h]);
			carry = auxdig[i+h+1] / 10;
			auxdig[i+h+1] = auxdig[i+h+1] % 10;
		}
		auxdig[i] = carry;
	}
	signo = !(b1.sign == b2.sign);

	Bignum nuevo(signo, tam, auxdig);
	delete [] auxdig;
	return nuevo;
}

bool operator>(const Bignum &b1, const Bignum &b2){
	if(b1.sign == true && b2.sign == false) {
		return false;
	}
	else if(b1.sign == false && b2.sign == true) {
		return true;
	}
	else if (b1.sign == false) {
		if(b1.size > b2.size){
			return true;
		}
		else if(b2.size > b1.size){
			return false;
		}
	}
	else if(b1.sign == true) {
		if(b1.size < b2.size){
			return true;
		}
		else if(b2.size < b1.size){
			return false;
		}
	}
	for(size_t i = 0; i < b1.size; i++) {
		if (b1.digits[i] == b2.digits[i])
			continue;

		return (b1.digits[i] < b2.digits[i]) ? b1.sign: !b1.sign;
	}
	return false;
}

bool operator<(const Bignum& b1, const Bignum& b2) {
	if (b1 == b2)
		return false;
	return !(b1 > b2);
}

ostream& operator<<(ostream &out, const Bignum &b) {
	if(b.size==0){
		out << "0";
	}
	if(b.sign) out << '-';
	for (size_t i = 0; i < b.size; i++){
		out << b.digits[i];
	}
	return out;
}

istream& operator>>(istream &in, Bignum &b){
	bool signo = false;
	unsigned char c;
	Bignum n;

	c = in.get();
	while(!in.eof()){
		if (isdigit(c))
			n = n * Bignum("10") + Bignum(string(1, c));
		else if (n.isEmpty()) {
			if (c == '-')
				signo = true;
		}
		else{
			in.putback(c);
			break;
		}
		c = in.get();
	}
	n.sign = signo;
	b = n;
	return in;
}

bool operator==(const Bignum &a, const Bignum &b){
	if (a.isEmpty() && b.isEmpty()) {
		return true;
	}
	if (a.isEmpty() || b.isEmpty()) {
		return false;
	}
	return (a.size == b.size) && is_digits_equal(a.digits, b.digits, a.size) && (a.sign == b.sign);
}

bool Bignum::isEmpty() const {
	return this->digits == nullptr;
}
