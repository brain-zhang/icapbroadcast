#pragma once

#include <iostream>
#include <exception>

using namespace std;

class CAudioswExcept :
	public exception
{
public:
	CAudioswExcept(void){}
	CAudioswExcept(string value):exception(value.c_str()){}
	~CAudioswExcept(void){}
};


class CLedExcept :
	public exception
{
public:
	CLedExcept(void){}
	CLedExcept(string value):exception(value.c_str()){}
	~CLedExcept(void){}
};

class CMediaExcept :
	public exception
{
public:
	CMediaExcept(void){}
	CMediaExcept(string value):exception(value.c_str()){}
	~CMediaExcept(void){}
};