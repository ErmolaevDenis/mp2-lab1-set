// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

using namespace std;

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		MemLen = (len) / (sizeof(TELEM) * 8);
		if (MemLen * sizeof(TELEM) * 8 < len)
			++MemLen;
		BitLen = len;
		pMem = new TELEM[MemLen];
		memset(pMem, 0, MemLen * sizeof(TELEM));
	}
	else
		throw exception("length must be greater than 0");
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n < 0)
		throw out_of_range("bit pos is out of range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0)
		throw out_of_range("bit pos is out of range");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0)
		throw out_of_range("bit pos is out of range");
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this == &bf)
		return *this;

	TELEM* tmp = new TELEM[bf.MemLen];
	copy(bf.pMem, bf.pMem + bf.MemLen, tmp);
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = tmp;
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int bl_max = max(BitLen, bf.BitLen);
	int ml_min = min(MemLen, bf.MemLen);
	TBitField tmp(bl_max);
	for (int i = 0; i < ml_min; i++)
		tmp.pMem[i] = pMem[i] | bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int bl_max = max(BitLen, bf.BitLen);
	int ml_min = min(MemLen, bf.MemLen);
	TBitField tmp(bl_max);
	for (int i = 0; i < ml_min; i++)
		tmp.pMem[i] = pMem[i] & bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (this->GetBit(i) == 0)
			tmp.SetBit(i);
		else
			tmp.ClrBit(i);
	}
	return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int len = bf.GetLength();
	unsigned int ent;
	for (int i = 0; i < len; i++)
	{
		istr >> ent;
		if (ent == 1)
			bf.SetBit(i);
		else if (ent == 0)
			bf.ClrBit(i);
		else
			throw invalid_argument("Bit can only be equal to 0 or 1");
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	return ostr;
}