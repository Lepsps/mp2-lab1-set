// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h";

TBitField::TBitField(int len): BitLen(len)
{   
    if (len < 0) throw "Error";
    MemLen = (len + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM [MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
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
    if (pMem != nullptr)
        delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw "Error";
    return n/(sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) throw "Error";
    return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw "Error";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw "Error";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw "Error";
    return ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
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
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return false;
        return true;
    }
    return false;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return true;
        return false;
    }
    return true;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    bool fl = 0;
    int maxLen = BitLen, minLen = BitLen;
    if (bf.BitLen < BitLen) { // fl если больший изначальный 
        fl = 1;
        maxLen = BitLen;
        minLen = bf.BitLen;
    }
    else {
        maxLen = bf.BitLen;
        minLen = BitLen;
    }
    TBitField res(maxLen);
    for (int i = 0; i < minLen; i++)
        if (this->GetBit(i) || bf.GetBit(i)) res.SetBit(i);
    for (int i = minLen; i < maxLen; i++) {
        if (fl && GetBit(i))
            res.SetBit(i);
        else if (!fl && bf.GetBit(i))
            res.SetBit(i);
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField res(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    for (int i = min(BitLen, bf.BitLen); i < max(BitLen, bf.BitLen); i++)
        res.ClrBit(i);
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            res.SetBit(i);
        }
        else
            res.ClrBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        bool e = 0;
        if(istr >> e) {
            if (e) bf.SetBit(i);
            else bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    cout << endl;
    return ostr;
}
