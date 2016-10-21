/*
* Stock.h
*
*	Main compilation unit for the class Stock, with a couple of minor types
*	StockSymbol and StockType.
*/
#include"stdafx.h"
#include"Stock.h"
#include"Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
// StockType 
////////////////////////////////////////////////////////////////////////////////

/* Returns a string representing the value of the given StockType
* Will throw an invalid_argument if the stockType is out of bounds
*/
std::string toString(StockType stockType)
{
	switch (stockType)
	{
	case COMMON_STOCK: return "Common";
	case PREFERRED_STOCK: return "Preferred";
	default:
		throw std::invalid_argument("toString:\tInvalid Stock Type.");
	}
}


////////////////////////////////////////////////////////////////////////////////
// Stock 
////////////////////////////////////////////////////////////////////////////////

const double Stock::NO_FIXED_DIVIDEND = 0.0;


// Build a Stock with the given fields
// Throws an invalid argument if either of the following are negative:
//		lastDividendIn, parValueIn or fixedDividendIn
Stock::Stock(StockSymbol symbolIn,
	StockType typeIn,
	double lastDividendIn,
	double parValueIn,
	double fixedDividendIn) :
	symbol(symbolIn),
	type(typeIn),
	lastDividend(lastDividendIn),
	parValue(parValueIn),
	fixedDividend(fixedDividendIn)
{
	if (lastDividendIn < 0.0)
	{
		throw std::invalid_argument("Stock::Stock:\tlastDividendIn cannot be negative.");
	}
	if (parValueIn < 0.0)
	{
		throw std::invalid_argument("Stock::Stock:\tparValueIn cannot be negative.");
	}
	if (fixedDividendIn < 0.0)
	{
		throw std::invalid_argument("Stock::Stock:\tfixedDividendIn cannot be negative.");
	}

}


// Returns this stocks Fixed Dividend, if it has one.
// If it does not, a InvalidOperation is thrown
double Stock::getFixedDividend()const
{
	if (!hasFixedDividend())
	{
		throw InvalidOperation("Stock::getFixedDividend:\tThis Stock has no Fixed Dividend"); //todo - add name of stock into exception
	}
	return fixedDividend;
}

// Returns a DividendYield on this Stock for the given price.
// Throws an invalid_argument for zero or negative prices
double Stock::calculateDividendYield(double price)
{
	if (price <= 0.0)
	{
		throw std::invalid_argument("Stock::calculateDividendYield:\tprice must be positive.");
	}

	if (NO_FIXED_DIVIDEND != fixedDividend)
	{
		return fixedDividend * parValue / price;
	}
	else
	{
		return lastDividend / price;
	}
}

// Returns the P/E ratio on this stock for the given price.
// Will return 0.0 If the dividend for this stock is zero
double Stock::calculatePERatio(double price)
{
	if (0.0 == lastDividend)
	{
		return 0.0;
	}

	return price / lastDividend; // todo - is the lastDividend the dividend to use for this formula?
}

////////////////////////////////////////////////////////////////////////////////


