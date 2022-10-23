// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    TBitField tmp(BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem >= MaxPower)
        throw out_of_range("Elem is out of universe");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= MaxPower)
        throw out_of_range("Elem is out of universe");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s)
        return *this;

    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
    return *this;
}

bool TSet::operator== (const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

bool TSet::operator!= (const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    if (this->MaxPower >= s.MaxPower)
    {
        TSet tmp(this->MaxPower);
        tmp.BitField = this->BitField | s.BitField;
        return tmp;
    }
    else
    {
        TSet tmp(s.MaxPower);
        tmp.BitField = this->BitField | s.BitField;
        return tmp;
    }
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(BitField);
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(BitField);
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    if (this->MaxPower >= s.MaxPower)
    {
        TSet tmp(this->MaxPower);
        tmp.BitField = this->BitField & s.BitField;
        return tmp;
    }
    else
    {
        TSet tmp(s.MaxPower);
        tmp.BitField = this->BitField & s.BitField;
        return tmp;
    }
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(~BitField);
    return tmp;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    string ent;
    int n = s.GetMaxPower();
    for (int i = 0; i < n; i++)
    {
        cout << i << " yavlyaetsya elemntom mnozhestva?" << endl;
        istr >> ent;
        if ((ent == "Da") || (ent == "Yes") || (ent == "+") || (ent == "da") || (ent == "yes"))
            s.InsElem(i);
        else if ((ent == "Net") || (ent == "No") || (ent == "-") || (ent == "net") || (ent == "no"))
            s.DelElem(i);
        else
            throw invalid_argument("Otvet mozhet bit' tol'ko da ili net");
        cout << endl;
    }
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    char ch = '{';
    int n = s.GetMaxPower();
    for (int i = 0; i < n; i++)
    {
        if (s.IsMember(i))
        {
            ostr << ch << ' ' << i;
            ch = ',';
        }
    }
    ostr << " }";
    return ostr;
}
