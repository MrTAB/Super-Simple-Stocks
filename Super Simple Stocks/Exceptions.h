#pragma once
#ifndef SUPERSIMPLESTOCKS_EXCEPTION
#define SUPERSIMPLESTOCKS_EXCEPTION
#include<stdexcept>

class InvalidOperation : public std::logic_error
{
public:
	explicit InvalidOperation(const std::string& msg) : logic_error(msg)
	{
		// done //
	}
};

class InvalidTimeError : public std::runtime_error
{
public:
	explicit InvalidTimeError(const std::string& msg) : runtime_error(msg)
	{
		// done //
	}
};


#endif