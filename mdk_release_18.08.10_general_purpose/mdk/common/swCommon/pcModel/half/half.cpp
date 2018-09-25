// -----------------------------------------------------------------------------
// Copyright (C) 2012 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : half implementation
// -----------------------------------------------------------------------------

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "half.h"


using namespace std;

unsigned int excepHalf;

// ############
// Constructors
// ############
#ifdef __cplusplus

half::half()
{
  _h = 0;
}

half::half(float v)
{
  unsigned int f_val = *(unsigned int *)&v;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

half::half(double v)
{
  float f = (float)v;
  unsigned int f_val = *(unsigned int *)&f;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

half::half(unsigned char v)
{
  float f = (float)v;
  unsigned int f_val = *(unsigned int *)&f;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

half::half(char v)
{
  float f = (float)v;
  unsigned int f_val = *(unsigned int *)&f;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

half::half(int v)
{
  float f = (float)v;
  unsigned int f_val = *(unsigned int *)&f;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

// ############
// Destructor
// ############

half::~half()
{
}

// #####################################
// General purpose set and get functions
// #####################################

void
half::setUnpackedValue(float v)
{
  unsigned int f_val = *(unsigned int *)&v;
  _h = (unsigned short)f32_to_f16_conv(f_val, F32_RND_NEAREST_EVEN, &excepHalf);
}

void
half::setPackedValue(unsigned short v)
{
  _h = v;
}

float
half::getUnpackedValue()
{
  float f;
  unsigned int f_val = f16_to_f32_conv(_h, &excepHalf);
  f = *(float *)&f_val;
  return f;
}

unsigned short
half::getPackedValue()
{
  return _h;
}

// #######################################
// Auxiliary swap function for assignments
// #######################################

void
swap(half& first, half& second)
{
  using std::swap;

  swap(first._h, second._h);
}

// ####################
// Assignment operators
// ####################

half &
half::operator=(half rhs)
{
  HANDLE_NEGZERO(rhs);
  swap(*this, rhs);
  return *this;
}

half &
half::operator=(float rhs)
{
  half temp(rhs);
  HANDLE_NEGZERO(temp);
  swap(*this, temp);
  return *this;
}

//half &
//half::operator=(double rhs)
//{
//  half temp(rhs);
//  swap(*this, temp);
//  return *this;
//}

// ###########################
// Binary comparison operators
// ###########################

bool
half::operator == (const half rhs)
{
  unsigned int op1, op2;
  float f1, f2;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  f1 = *(float *)&op1;
  f2 = *(float *)&op2;
  return f1 == f2;
}

bool
half::operator != (const half rhs)
{
  return !(*this == rhs);
}

bool
half::operator >  (const half rhs)
{
  unsigned int op1, op2;
  float f1, f2;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  f1 = *(float *)&op1;
  f2 = *(float *)&op2;
  return f1 > f2;
}

bool
half::operator <  (const half rhs)
{
  unsigned int op1, op2;
  float f1, f2;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  f1 = *(float *)&op1;
  f2 = *(float *)&op2;
  return f1 < f2;
}

bool
half::operator >= (const half rhs)
{
  return !(*this < rhs);
}

bool
half::operator <= (const half rhs)
{
  return !(*this > rhs);
}

bool
half::operator == (const float rhs)
{
  unsigned int op1;
  float f1;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  f1 = *(float *)&op1;
  return f1 == rhs;
}

bool
half::operator != (const float rhs)
{
  return !(*this == rhs);
}

bool
half::operator >  (const float rhs)
{
  unsigned int op1;
  float f1;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  f1 = *(float *)&op1;
  return f1 > rhs;
}

bool
half::operator <  (const float rhs)
{
  unsigned int op1;
  float f1;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  f1 = *(float *)&op1;
  return f1 < rhs;
}

bool
half::operator >= (const float rhs)
{
  return !(*this < rhs);
}

bool
half::operator <= (const float rhs)
{
  return !(*this > rhs);
}

// #############################
// Compound assignment operators
// #############################

half &
half::operator += (const half rhs)
{
  unsigned int op1, op2, res;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  res = f32_add(op1, op2, F32_RND_NEAREST_EVEN, &excepHalf);
  this->_h = (unsigned short)f32_to_f16_conv(res, F32_RND_NEAREST_EVEN, &excepHalf);
  return *this;
}

half &
half::operator -= (const half rhs)
{
  unsigned int op1, op2, res;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  res = f32_sub(op1, op2, F32_RND_NEAREST_EVEN, &excepHalf);
  this->_h = (unsigned short)f32_to_f16_conv(res, F32_RND_NEAREST_EVEN, &excepHalf);
  return *this;
}

half &
half::operator *= (const half rhs)
{
  unsigned int op1, op2, res;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  res = f32_mul(op1, op2, F32_RND_NEAREST_EVEN, &excepHalf);
  this->_h = (unsigned short)f32_to_f16_conv(res, F32_RND_NEAREST_EVEN, &excepHalf);
  return *this;
}

half &
half::operator /= (const half rhs)
{
  unsigned int op1, op2, res;
  op1 = f16_to_f32_conv(this->_h, &excepHalf);
  op2 = f16_to_f32_conv(rhs._h, &excepHalf);
  res = f32_div(op1, op2, F32_RND_NEAREST_EVEN, &excepHalf);
  this->_h = (unsigned short)f32_to_f16_conv(res, F32_RND_NEAREST_EVEN, &excepHalf);
  return *this;
}

half &
half::operator += (const float rhs)
{
  half op2(rhs);
  *this += op2;
  return *this;
}

half &
half::operator -= (const float rhs)
{
  half op2(rhs);
  *this -= op2;
  return *this;
}

half &
half::operator *= (const float rhs)
{
  half op2(rhs);
  *this *= op2;
  return *this;
}

half &
half::operator /= (const float rhs)
{
  half op2(rhs);
  *this /= op2;
  return *this;
}

// ###########################
// Binary Arithmetic Operators
// ###########################

/*half half::operator+ (const half rhs)
{
  half res = *this;
  res += rhs;
  return res;
}

half half::operator- (const half rhs)
{
  half res = *this;
  res -= rhs;
  return res;
}

half half::operator* (const half rhs)
{
  half res = *this;
  res *= rhs;
  return res;
}

half half::operator/ (const half rhs)
{
  half res = *this;
  res /= rhs;
  return res;
}

half half::operator+ (const float rhs)
{
  half res = *this;
  res += rhs;
  return res;
}

half half::operator- (const float rhs)
{
  half res = *this;
  res -= rhs;
  return res;
}

half half::operator* (const float rhs)
{
  half res = *this;
  res *= rhs;
  return res;
}

half half::operator/ (const float rhs)
{
  half res = *this;
  res /= rhs;
  return res;
}*/

// #############
// I/O operators
// #############

ostream &
operator << (ostream &os, half h)
{
    os << float(h);
    return os;
}

istream &
operator >> (istream &is, half &h)
{
    float f;
    is >> f;
    h = half(f);
    return is;
}

// #############
// Cast operator
// #############

half::operator float () const
{
  unsigned int f_val = f16_to_f32_conv(_h, &excepHalf);
  float f = *(float *)&f_val;
  return f;
}

// ##############
// Unary operator
// ##############

half half::operator - () const
{
  half tmp;
  tmp._h = _h ^ 0x8000;
  return tmp;
}

// #######################
// Absolute value operator
// #######################

float halfabs(half no)
{
  unsigned int f_val = f16_to_f32_conv(no._h, &excepHalf);
  float f = *(float *)&f_val;
  return fabs(f);
}
#endif
