#include"stdafx.h"
#include"TradeRecord.h"
#include<stdexcept>
#include<string>

// Adds a Trade to the TradeRecord, using the current time as its timeStamp
// This operation may improve insertion performance by assuming the trade is the newest trade.
//   As with Trade::Trade:
//			quantity must be 1 or greater or an invalid_argument will be thrown.
//			price must be 0.0 or greater or an invalid_argument will be thrown.
//
void TradeRecord::addTrade(int quantity, BuyOrSellType buyOrSellType, double price)
{
	TimeStamp now = std::chrono::system_clock::now();
	trades.emplace_hint(
		trades.end(),
		std::piecewise_construct,
		std::forward_as_tuple(now),
		std::forward_as_tuple(quantity, buyOrSellType, price, now));
}

// Adds a Trade to the TradeRecord, using the given time as its timeStamp
//   As with Trade::Trade:
//			quantity must be 1 or greater or an invalid_argument will be thrown.
//			price must be 0.0 or greater or an invalid_argument will be thrown.
//
void TradeRecord::addTrade(int quantity, BuyOrSellType buyOrSellType, double price, TimeStamp timeStamp)
{
	trades.emplace(std::piecewise_construct,
		std::forward_as_tuple(timeStamp),
		std::forward_as_tuple(quantity, buyOrSellType, price, timeStamp));
}

// Adds an existing Trade to the TradeRecord.
//
void TradeRecord::addTrade(const Trade trade)
{
	trades.insert(std::make_pair(trade.getTimeStamp(), trade));
}

// Returns the Volume Weighted Stock Price based on the last five minutes of trades
// Out parameter foundTrades will be true if there were trades within that time.
//		If not, foundTrades will be false, and the return value 0.0
//
double TradeRecord::calculateVolumeWeightedStockPriceWithinFiveMinutes(bool&foundTrades)const
{
	return calculateVolumeWeightedStockPriceWithin(foundTrades, std::chrono::minutes(5));
}

// Returns the Volume Weighted Stock Price based on the last "min" minutes.
// Out parameter foundTrades will be true if there were trades within that time.
//		If not, foundTrades will be false, and the return value 0.0
//
double TradeRecord::calculateVolumeWeightedStockPriceWithin(bool&foundTrades, const std::chrono::minutes min)const
{
	TimeStamp startTimeStamp = std::chrono::system_clock::now() - std::chrono::duration_cast<std::chrono::system_clock::duration>(min);
	return calculateVolumeWeightedStockPriceSince(foundTrades, startTimeStamp);
}

// Returns the Volume Weighted Stock Price from the given time startTimeStamp until the present.
// Out parameter foundTrades will be true if there were trades within that time.
//		If not, foundTrades will be false, and the return value 0.0
//
double TradeRecord::calculateVolumeWeightedStockPriceSince(bool&foundTrades, const TimeStamp startTimeStamp)const
{
	if (trades.empty())
	{
		foundTrades = false;
		return 0.0;
	}

	double quantitySum = 0;
	double sumOfPriceAndQuantity = 0;

	for (auto tradeItr = trades.crbegin(); tradeItr != trades.crend(); ++tradeItr)
	{
		if (tradeItr->first < startTimeStamp)
		{
			break;
		}
		quantitySum += tradeItr->second.getQuantity();
		sumOfPriceAndQuantity += tradeItr->second.getPrice()*tradeItr->second.getQuantity();
	}

	foundTrades = true;
	if (0.0 == quantitySum)
	{
		return 0.0;
	}
	return sumOfPriceAndQuantity / quantitySum;
}

// Outputs the Trade Record to the given output stream as a text formatted table.
// "title" will be used at the start of the table.
// Note that this table could presumably be quite large, hence a distinct method
// for printing rather than providing a stream operator.
//
void TradeRecord::printTo(std::ostream&out, const std::string title)
{
	using std::endl;

	out << "\tTrade Record for " << title << endl
		<< "-------------------------------------------------------------------------------\n"
		<< "Quantity\tBuy Or Sell\tPrice\t\tTime stamp\n";
	for (auto itr = trades.cbegin(); itr != trades.end(); ++itr)
	{
		std::time_t time = std::chrono::system_clock::to_time_t(itr->second.getTimeStamp());
		std::tm date;
		localtime_s(&date, &time);
		out << itr->second.getQuantity()
			<< "\t\t" << (itr->second.getBuyOrSellType() == BUY_TYPE ? "Buy" : "Sell")
			<< "\t\t" << itr->second.getPrice()
			<< "\t\t" << date.tm_hour << ':' << date.tm_min << ':' << date.tm_sec
			<< endl;
	}
	out << "-------------------------------------------------------------------------------\n\n";
}