/*
* TradeRecord.h
*
* An instance of TradeRecord manages a collection of trades for a particular stock,
* and provides methods for querying those trades and adding new trades.
*/
#pragma once
#ifndef SUPERSIMPLESTOCKS_TRADE_RECORD
#define SUPERSIMPLESTOCKS_TRADE_RECORD
#include"Trade.h"
#include<map>

class TradeRecord
{
	// Multimap 'trades' stores trades ordered by time, but with the possibility for several
	// trades to exist at the same point in time without issue.
	//
	std::multimap<TimeStamp, Trade> trades;

public:

	// Adds a Trade to the TradeRecord, using the current time as its timeStamp
	// This operation may improve insertion performance by assuming the trade is the newest trade.
	//   As with Trade::Trade:
	//			quantity must be 1 or greater or an invalid_argument will be thrown.
	//			price must be 0.0 or greater or an invalid_argument will be thrown.
	//
	void addTrade(int quantity, BuyOrSellType buyOrSellType, double price);

	// Adds a Trade to the TradeRecord, using the given time as its timeStamp
	//   As with Trade::Trade:
	//			quantity must be 1 or greater or an invalid_argument will be thrown.
	//			price must be 0.0 or greater or an invalid_argument will be thrown.
	//
	void addTrade(int quantity, BuyOrSellType buyOrSellType, double price, TimeStamp timeStamp);

	// Adds an existing Trade to the TradeRecord.
	//
	void addTrade(const Trade trade);

	// Returns the Volume Weighted Stock Price based on the last five minutes of trades
	// Out parameter foundTrades will be true if there were trades within that time.
	//		If not, foundTrades will be false, and the return value 0.0
	//
	double calculateVolumeWeightedStockPriceWithinFiveMinutes(bool&foundTrades)const;

	// Returns the Volume Weighted Stock Price based on the last "min" minutes.
	// Out parameter foundTrades will be true if there were trades within that time.
	//		If not, foundTrades will be false, and the return value 0.0
	//
	double calculateVolumeWeightedStockPriceWithin(bool&foundTrades, const std::chrono::minutes min)const;

	// Returns the Volume Weighted Stock Price from the given time startTimeStamp until the present.
	// Out parameter foundTrades will be true if there were trades within that time.
	//		If not, foundTrades will be false, and the return value 0.0
	//
	double calculateVolumeWeightedStockPriceSince(bool&foundTrades, const TimeStamp startTimeStamp)const;

	// Outputs the Trade Record to the given output stream as a text formatted table.
	// "title" will be used at the start of the table.
	// Note that this table could presumably be quite large, hence a distinct method
	// for printing rather than providing a stream operator.
	//
	void printTo(std::ostream&out, const std::string title);
};

#endif