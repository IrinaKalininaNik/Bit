// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw ERROR_BITFIELD_SIZE; //выкидываем ошибку, размер не может быть меньше 0
	this->MemLen = ((len-1)/ BitInField())+1;//Размер массива
	this->BitLen = len;
	this->pMem = new TELEM[MemLen];//выделяем массив для битов
	for (size_t i = 0; i < MemLen; i++)//устанавливаем все биты в 0
		this->pMem[i] = 0;
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)//копируем массив с битами
		this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] this->pMem;//очищаем память из под массива
}

unsigned int TBitField::GetMemIndex(const int n) const // Ищем в каком элементе массива находится бит n
{
	if (n >= this->BitLen)
		throw ERROR_BITFIELD_INDEX;//n не может превышать максимальный размер
	return (n/ BitInField());
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= this->BitLen)
		throw ERROR_BITFIELD_INDEX;
	TELEM mask = TELEM();//переменная для маски равна 0
	mask |= 1 << n % BitInField();//устанавливаем нужны бит в n
	return mask;
}

int TBitField::BitInField()//размер битов у 1 элемента массива
{
	return sizeof(TELEM) * 8;
}

// доступ к битам битового поля
int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= this->BitLen || n<0)
		throw ERROR_BITFIELD_INDEX;
	TELEM mask = GetMemMask(n);//получаем маску для нужного элемента массива. 
	int index = GetMemIndex(n);//получаем порядковый номер элемента
	this->pMem[index] |= mask;//складываем биты
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= this->BitLen || n<0)
		throw ERROR_BITFIELD_INDEX;
	TELEM mask = GetMemMask(n);
	int index = GetMemIndex(n);
	this->pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= this->BitLen || n<0)
		throw ERROR_BITFIELD_INDEX;
	int index = GetMemIndex(n);
	TELEM mask = this->pMem[index] >> (n%BitInField());//получаем биты в нужном элементе и сдвигаем нужный бит в что бы он был первый

    return mask&1;//возвращаем операцию побитовго И либо 0 либо 1(в элементе N)
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete[] this->pMem;
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
		this->pMem[i] = bf.pMem[i];
	
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->BitLen != bf.BitLen)
		throw ERROR_BITFIELD_SIZE;
	bool check = true;
	for (int i = 0; i < MemLen; i++)
		if (this->pMem[i] != bf.pMem[i])//Если есть неравные элементы массива, битовые поля неравны
			check = false;
  return check;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !operator==(bf);//Отрицание оператора сравнения
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField temp(this->BitLen);
	for (size_t i = 0; i <MemLen && i <bf.MemLen; i++)
	{
		temp.pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return temp;
}
TBitField& TBitField::operator|=(const TBitField &bf) // операция "или="
{
	if (this->BitLen != bf.BitLen)
		throw ERROR_BITFIELD_SIZE;
	for (size_t i = 0; i < MemLen; i++)
	{
		this->pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField temp(this->BitLen);
	for (size_t i = 0; i < MemLen && i <bf.MemLen; i++)
	{
		temp.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return temp;
}

TBitField& TBitField::operator&=(const TBitField &bf) // операция "и="
{
	if (this->BitLen != bf.BitLen)
		throw ERROR_BITFIELD_SIZE;
	for (size_t i = 0; i < MemLen; i++)
	{
		this->pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(this->BitLen);
	TELEM temp_tel = temp.pMem[MemLen];
	for (size_t i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = ~this->pMem[i];
	}
	
	return temp;
}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	if (bf.BitLen < istr.gcount())
		throw ERROR_BITFIELD_SIZE;
	char temp;
	int index = 0;
	while(istr.get(temp))
	{
		if (temp == '\n')
			break;
		if (temp == ' ')
			continue;
		if (temp != '1' && temp != '0')
			throw ERROR_BITFIELD_VALUE;
		if (temp == '1')
			bf.SetBit(index++);
		else
			bf.ClrBit(index++);
	} 
	
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.MemLen; i++)
	{
		for (int z = TBitField::BitInField()-1; z >=0 ; z--)
		{
			if (z % 8 == 0 && z != 0)
				ostr << " ";
			ostr << ((bf.pMem[i] & 1 << z) ? 1 : 0);			
		}
		ostr<< endl;
	}
	return ostr;
}
