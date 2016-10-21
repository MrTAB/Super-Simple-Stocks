/*
*	Trade.h
*
*	A Trade object represents an individual trade, with a defined timestamp.
*/
#pragma once
#ifndef SUPERSIMPLESTOCKS_TRADE
#define SUPERSIMPLESTOCKS_TRADE
#define __STDC_WANT_LIB_EXT1__ 1
#include<ctime>
#include<chrono>

enum BuyOrSellType
{
	BUY_TYPE = 0,
	SELL_TYPE
};

typedef std::chrono::time_point<std::chrono::system_clock> TimeStamp;

class Trade
{
	unsigned int quantity;
	BuyOrSellType buyOrSellType;
	double price;
	TimeStamp timeStamp;

public:

	// Construct a Trade object with a timestamp of the present moment.
	//  quantity must be 1 or greater or an invalid_argument will be thrown.
	//  price must be 0.0 or greater or an invalid_argument will be thrown.
	//
	Trade(unsigned int quantity,
		BuyOrSellType buyOrSellType,
		double price);

	// Construct a Trade object with the given timeStamp.
	//  quantity must be 1 or greater or an invalid_argument will be thrown.
	//  price must be 0.0 or greater or an invalid_argument will be thrown.
	//
	Trade(unsigned int quantity,
		BuyOrSellType buyOrSellType,
		double price,
		TimeStamp timeStamp);

	unsigned int getQuantity()const
	{
		return quantity;
	}

	// Returns the Trades by or sell type
	//
	BuyOrSellType getBuyOrSellType()const
	{
		return buyOrSellType;
	}

	// Returns the price of the trade
	//
	double getPrice()const
	{
		return price;
	}

	// Returns the timeStamp representing when this trade took place
	//
	TimeStamp getTimeStamp()const
	{
		return timeStamp;
	}

};


#endif