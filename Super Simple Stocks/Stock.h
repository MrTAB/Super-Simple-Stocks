/*
* Stock.h
*
*	Main compilation unit for the class Stock, with a couple of minor types
*	StockSymbol and StockType.
*/
#pragma once
#ifndef SUPERSIMPLESTOCKS_STOCK
#define SUPERSIMPLESTOCKS_STOCK
#include"TradeRecord.h"
#include<iostream>
#include<string>

////////////////////////////////////////////////////////////////////////////////
// StockSymbol
////////////////////////////////////////////////////////////////////////////////

/*	StockSymbols are defined as strings allowing a large variety of potential stock symbols
*   while providing map sortability.
*	If a definite restriction is to be placed on the size of stock symbols (e.g. max 4 or 8
*	characters), then this could be optimized to storing in an int or long int, or a struct
*	containing an array with defined sorting operands reinterpreting the contents as int or
*	long int
*/

typedef std::string StockSymbol;

////////////////////////////////////////////////////////////////////////////////
// StockType
////////////////////////////////////////////////////////////////////////////////

/* StockType must be one of these values
*/
enum StockType
{
	COMMON_STOCK,
	PREFERRED_STOCK
};

/* Returns a string representing the value of the given StockType.
* Will throw an invalid_argument if the stockType is out of bounds
*/
std::string toString(StockType stockType);


////////////////////////////////////////////////////////////////////////////////
//	Stock
////////////////////////////////////////////////////////////////////////////////

/* Stock represents an individual stock, with a record of trades and some
*	queries that can be made on that stock.
*	Stocks follow R.A.I.I principles, so an existing stock instance is a
*	valid stock.
*	Stocks are non-copyable.
*/
class Stock
{
	StockSymbol symbol;
	StockType type;
	double lastDividend;
	double parValue;
	double fixedDividend;
	TradeRecord trades;

	Stock(const Stock&) = delete;
	Stock& operator=(const Stock&) = delete;

public:

	static const double NO_FIXED_DIVIDEND;

	// Build a Stock with the given fields
	// Throws an invalid argument if either of the following are negative:
	//		lastDividendIn, parValueIn or fixedDividendIn
	Stock(StockSymbol symbolIn,
		StockType typeIn,
		double lastDividendIn,
		double parValueIn,
		double fixedDividendIn = NO_FIXED_DIVIDEND);

	// Returns this stocks symbol
	StockSymbol getStockSymbol()const
	{
		return symbol;
	}

	// Returns this stocks type
	StockType getStockType()const
	{
		return type;
	}

	// Returns this stocks last dividend
	double getLastDividend()const
	{
		return lastDividend;
	}

	// Returns this stocks Par Value
	double getParValue()const
	{
		return parValue;
	}

	// Returns true IFF this stock has a fixedDivided
	bool hasFixedDividend()const
	{
		return fixedDividend != NO_FIXED_DIVIDEND;
	}

	// Returns this stocks Fixed Dividend, if it has one.
	// If it does not, a InvalidOperation is thrown
	double getFixedDividend()const;

	// Non modifiable direct access to Trade record
	const TradeRecord& accessTradeRecord()const
	{
		return trades;
	}

	// Modifiable direct access to Trade record
	TradeRecord& accessTradeRecord()
	{
		return trades;
	}

	// Returns a DividendYield on this Stock for the given price.
	// Throws an invalid_argument for zero or negative prices
	double calculateDividendYield(double price);

	// Returns the P/E ratio on this stock for the given price.
	// Will return 0.0 If the dividend for this stock is zero
	double calculatePERatio(double price);

};

// Output a stock to a standard stream
// Will appear as follows: "Stock: symbol TEA, type Common, last dividend 100 (fixed 2%), par value 100."
inline std::ostream& operator<< (std::ostream& out, const Stock& stock)
{
	out << "Stock: symbol " << stock.getStockSymbol()
		<< ", type " << toString(stock.getStockType())
		<< ", last dividend " << stock.getLastDividend();

	if (stock.hasFixedDividend())
	{
		out << " (fixed " << stock.getFixedDividend() << "%)";
	}

	out << ", par value " << stock.getParValue();

	return out << ".";
}


#endif