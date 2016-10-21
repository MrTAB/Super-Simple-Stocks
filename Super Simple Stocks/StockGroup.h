/*
*	StockGroup.h
*	
*	A StockGroup class managed a collection of stocks ordered for fast retrieval via
*	their StockSymbol, while also providing methods to query the Stocks stored.
*	Note that StockGroup maintains memory ownership of the stocks added to it.
*/
#pragma once
#ifndef SUPERSIMPLESTOCKS_STOCKGROUP
#define SUPERSIMPLESTOCKS_STOCKGROUP
#include"Stock.h"
#include<map>
#include<vector>
#include<numeric>
#include<cassert>

/* Maintains a group of Stocks for efficient lookup.
*   
*/
class StockGroup
{
protected:
	std::map<StockSymbol, Stock*> stocks;

	// Internal utility for calculating the nth root of the given value
	//
	static inline double nthRoot(double value, double n)
	{
		assert(n != 0);
		return std::pow(value, 1.0 / n);
	}

	// Internal utility; calculates the Geometric Mean of the given array of values
	//
	static double inline calculateGeometricMean(const std::vector<double> &values)
	{
		assert(!values.empty());
		const double n = values.size();
		const double product = std::accumulate(std::cbegin(values), std::end(values), 1.0, std::multiplies<double>());
		return nthRoot(product, n);
	}

public:
	
	// Deconstructor: Note that StockGroup maintains memory ownership of the stocks added to it.
	//
	virtual ~StockGroup();

	// Returns true if a stock with the given symbol is in the StockGroup
	//
	bool hasStock(StockSymbol symbol)const;

	// Returns non-modifiable access to a stock with the given symbol.
	// Throws an invalid_argument if the stock does not exist.
	//
	const Stock& accessStock(StockSymbol symbol) const;

	// Returns modifiable direct access to a stock with the given symbol.
	// Throws an invalid_argument if the stock does not exist.
	//
	Stock& accessStock(StockSymbol symbol);


	// Method deprecated: use version below
	// Adds the stock to the StockGroup, recieving ownership of the given object.
	// If the stock already exists in the group, an InvalidOperation is thrown
	// If the stock pointer is null, an invalid_argument is thrown
	//
	//void addStock(Stock*stock);

	// Add a stock to the StockGroup.
	// If a stock of that symbol already exists in the group, an InvalidOperation is thrown.
	// This method allocates a Stock object internally using the given fields.
	// See Stock's constructor for potential exceptions when supplying these fields.
	//
	void addStock(StockSymbol symbolIn,
		StockType typeIn,
		double lastDividendIn,
		double parValueIn,
		double fixedDividendIn = Stock::NO_FIXED_DIVIDEND);

	// Returns the All Share Index for the map, using a Volume Weighted Stock Price
	//	based on trades over the last 'min' minutes.
	//
	double calculateAllShareIndexWithin(std::chrono::minutes min);

	//  Returns the All Share Index for the map, using a Volume Weighted Stock Price
	//  based on trades over the last 5 minutes.
	//
	inline double calculateAllShareIndexWithinFiveMinutes()
	{
		return calculateAllShareIndexWithin(std::chrono::minutes(5));
	}
};



#endif