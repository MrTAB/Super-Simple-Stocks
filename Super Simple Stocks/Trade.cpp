#include"stdafx.h"
#include"Trade.h"
#include"Exceptions.h"

// Construct a Trade object with a timestamp of the present moment.
//  quantity must be 1 or greater or an invalid_argument will be thrown.
//  price must be 0.0 or greater or an invalid_argument will be thrown.
//
Trade::Trade(unsigned int quantityIn,
	BuyOrSellType buyOrSellTypeIn,
	double priceIn) :
	quantity(quantityIn),
	buyOrSellType(buyOrSellTypeIn),
	price(priceIn),
	timeStamp(std::chrono::system_clock::now())
{
	if (price < 0.0)
	{
		throw std::invalid_argument("Trade::Trade:\tPrice given to trade cannot be negative.");
	}
	if (quantity < 1)
	{
		throw std::invalid_argument("Trade::Trade:\tQuantity must be 1 or more.");
	}
}

// Construct a Trade object with the given timeStamp.
//  quantity must be 1 or greater or an invalid_argument will be thrown.
//  price must be 0.0 or greater or an invalid_argument will be thrown.
//
Trade::Trade(unsigned int quantityIn,
	BuyOrSellType buyOrSellTypeIn,
	double priceIn,
	TimeStamp timeStampIn) :
	quantity(quantityIn),
	buyOrSellType(buyOrSellTypeIn),
	price(priceIn),
	timeStamp(timeStampIn)
{
	if (price < 0.0)
	{
		throw std::invalid_argument("Trade::Trade:\tPrice given to trade cannot be negative.");
	}
	if (quantity < 1)
	{
		throw std::invalid_argument("Trade::Trade:\tQuantity must be 1 or more.");
	}
}

///////