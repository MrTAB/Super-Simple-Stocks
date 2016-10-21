#include"stdafx.h"
#include"StockGroup.h"
#include"Exceptions.h"

// 
//
StockGroup::~StockGroup()
{
	for (auto itr : stocks)
	{
		delete itr.second;
	}
}

// Returns true if a stock with the given symbol is in the StockGroup
//
bool StockGroup::hasStock(StockSymbol symbol)const
{
	auto itr = stocks.find(symbol);
	return itr != stocks.end();
}

// Returns non-modifiable access to a stock with the given symbol.
// Throws an invalid_argument if the stock does not exist.
//
const Stock& StockGroup::accessStock(StockSymbol symbol) const
{
	auto itr = stocks.find(symbol);
	if (itr == stocks.end())
	{
		throw std::invalid_argument("StockGroup::accessStock:\tstock does not exist");
	}
	else
	{
		return *itr->second;
	}
}

// Returns modifiable direct access to a stock with the given symbol.
// Throws an invalid_argument if the stock does not exist.
//
Stock& StockGroup::accessStock(StockSymbol symbol)
{
	auto itr = stocks.find(symbol);
	if (itr == stocks.end())
	{
		throw std::invalid_argument("StockGroup::accessStock:\tstock does not exist");
	}
	else
	{
		return *itr->second;
	}
}

// Method deprecated: use version below
// Adds the stock to the StockGroup, recieving ownership of the given object.
// If the stock already exists in the group, an InvalidOperation is thrown
// If the stock pointer is null, an invalid_argument is thrown
//
/*void StockGroup::addStock(Stock*stock)
{
	if (nullptr == stock)
	{
		throw std::invalid_argument("StockSet::addStock:\tnull pointer stock supplied.");
	}

	auto itr = stocks.find(stock.getStockSymbol());
	if (itr != stocks.end())
	{
		// potential memory leak if stock was allocated inside try block, or memory access violation if we choose to delete it to prevent that leak..
		// method deprecated in favour of below version
		throw InvalidOperation("StockSet::addStock:\tStock already exists.");
	}
	stocks.insert(std::make_pair(stock.getStockSymbol(), stock));
}
*/

// Add a stock to the StockGroup.
// If a stock of that symbol already exists in the group, an InvalidOperation is thrown.
// This method allocates a Stock object internally using the given fields.
// See Stock's constructor for potential exceptions when supplying these fields.
//
void StockGroup::addStock(StockSymbol symbolIn,
	StockType typeIn,
	double lastDividendIn,
	double parValueIn,
	double fixedDividendIn)
{
	auto itr = stocks.find(symbolIn);
	if (itr != stocks.end())
	{
		throw InvalidOperation("StockSet::addStock:\tStock already exists.");
	}
	stocks.insert(std::make_pair(symbolIn,
		new Stock(symbolIn, typeIn, lastDividendIn, parValueIn, fixedDividendIn)));
}

// Returns the All Share Index for the map, using a Volume Weighted Stock Price
//	based on trades over the last 'min' minutes.
//
double StockGroup::calculateAllShareIndexWithin(std::chrono::minutes min)
{
	std::vector<double> vwsPrices;

	for (auto itr : stocks)
	{
		bool foundTrades;
		vwsPrices.push_back(itr.second->accessTradeRecord().calculateVolumeWeightedStockPriceWithin(foundTrades, min));
	}

	if (vwsPrices.empty())
	{
		return 0.0;
	}
	return calculateGeometricMean(vwsPrices);
}

