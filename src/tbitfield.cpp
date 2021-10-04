// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <limits>
#include <iostream>
#include <sstream>
#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{

	if (len > 0) {
		BitLen = len;
		MemLen = ((BitLen)/BITS_IN_ONE_MEM)+1;
		pMem = new TELEM[MemLen];

		for (size_t i = 0; i < MemLen; i++) {
			pMem[i] = 0;
		}
	}
	else {
		throw "error";
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	/*for (size_t i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}*/

	for (size_t i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0 && n < BitLen) {
		return n / BITS_IN_ONE_MEM;
	}
	else {
		throw "error index";
	}
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= 0 && n < BitLen) {
		return (TELEM)1 << (n%BITS_IN_ONE_MEM);
	}
	else {
		throw "error number bit";
	}
	return 0;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen) {
		int indexInt = n / BITS_IN_ONE_MEM;
		pMem[indexInt] = pMem[indexInt] | GetMemMask(n);
	}
	else {
		throw "error set bit";
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen) {
		int indexInt = n / BITS_IN_ONE_MEM;
		pMem[indexInt] = pMem[indexInt] & (~GetMemMask(n));
	}
	else {
		throw "error clean bit";
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n <= BitLen) {
		return (pMem[n / BITS_IN_ONE_MEM] & ((TELEM)1 << (n%BITS_IN_ONE_MEM))) > 0;
	}
	else {
		throw "error get bit";
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	delete[] pMem;
	pMem = new TELEM[bf.MemLen];

	for (int i = 0; i < bf.MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}

	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
	}

	for (size_t i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	
	TBitField Result(1);

	if (bf.BitLen > BitLen)
	{
		Result = bf;
		for (int i = 0; i < Result.MemLen; i++)
			Result.pMem[i] |= pMem[i];
	}
	else
	{
		Result = *this;
		for (int i = 0; i < Result.MemLen; i++)
			Result.pMem[i] |= bf.pMem[i];
	}
	return Result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField Result(1);
	if (bf.BitLen > BitLen)
	{
		Result = bf;
		for (int i = 0; i < Result.MemLen; i++)
			Result.pMem[i] &= pMem[i];
	}
	else
	{
		Result = *this;
		for (int i = 0; i < Result.MemLen; i++)
			Result.pMem[i] &= bf.pMem[i];
	}
	return Result;

}

TBitField TBitField::operator~(void) // отрицание
	{
	TBitField res(*this);
	for (int i = 0; i < MemLen - 1; i++)
		res.pMem[i] = ~res.pMem[i];
	if (MemLen > 0)
		for (int i = (MemLen - 1) * BITS_IN_ONE_MEM; i < res.BitLen; i++)
			if (GetBit(i))
				res.ClrBit(i);
			else
				res.SetBit(i);
	return res;
	}


// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	size_t size;
	string in_string;
	istr >> in_string;
	
	size = in_string.length();
	bf = TBitField(size);
	for (int i = 0; i < size; ++i)
	{
		if (in_string[i] == '1')
			bf.SetBit(i);
			//bf.SetBit(size - i - 1);
		else if (in_string[i] != '1' && in_string[i] != '0')
		{
			throw "Bad input";
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	try
	{
		for (size_t i = bf.GetLength(); i>0; --i)
		{
			ostr << bf.GetBit(i-1);
		}
	}
	catch (std::exception& error)
	{
		throw error;
	}
	return ostr;
	/*string str;
	for (size_t i = 0; i < bf.BitLen; i++) {
		str.push_back(bf.GetBit(i) + '0');
	}
	return ostr << str;*/
}
