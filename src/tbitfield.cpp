// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len): BitLen(len)
{   
    if (len < 0) throw "Error";
    MemLen = sizeof(TELEM)*8 * (BitLen + sizeof(TELEM) * 8 - 1) >> 5;
    pMem = new TELEM [MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf): BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw "Error";
    for (int i = 0; i < MemLen; i++) {
        if (i == n)
            return i;
    }
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (n % (sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw "Error";
    pMem[n] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw "Error";
    pMem[n] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw "Error";
    return pMem[n];
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    if (pMem != nullptr) delete[] pMem;
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM [MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < BitLen; i++)
            if (pMem[i] != bf.pMem[i])
                return false;
        return true;
    }
    return false;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < BitLen; i++)
            if (pMem[i] != bf.pMem[i])
                return true;
        return false;
    }
    return true;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int m = min(BitLen, bf.BitLen);
    TBitField res(max(BitLen, bf.BitLen));
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < m; i++)
            res.pMem[i] = pMem[i] | bf.pMem[i];
        return res;
    }
    /*
    * int m = max(BitLen, bf.BitLen);
    * int rz = m - min(BitLen, bf.BitLen);
    if (BitLen > bf.BitLen)
        for (int i = 0; i < BitLen - bf.BitLen; i++)
            res.pMem[i] = pMem[i];
    else if (BitLen < bf.BitLen)
        for (int i = 0; i < BitLen - bf.BitLen; i++)
            res.pMem[i] = bf.pMem[i];
    */
    for (int i = 0; i < m; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (BitLen > bf.BitLen)
        for (int i = bf.BitLen; i < BitLen; i++)
            res.pMem[i] = pMem[i];
    else if (BitLen < bf.BitLen)
        for (int i = BitLen; i < bf.BitLen; i++)
            res.pMem[i] = bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int m = min(BitLen, bf.BitLen);
    TBitField res(max(BitLen, bf.BitLen));
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < m; i++)
            res.pMem[i] = pMem[i] & bf.pMem[i];
        return res;
    }

    for (int i = 0; i < m; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            res.SetBit(i);
        }
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++)
        istr >> bf.pMem[i];
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.pMem[i] << endl;
    return ostr;
}
