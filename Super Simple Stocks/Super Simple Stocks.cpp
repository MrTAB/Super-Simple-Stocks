/*
*  Super Simple Stocks.cpp : This file and VS project serves as a
*	demonstration and testing point.
*
*	Please note that for the formula for P/E Ratio I used the LastDividend as the denominator.
*
*/

#include"stdafx.h"
#include"Exceptions.h"
#include"StockGroup.h"
#include<cassert>
#include<random>
#include<thread>

using std::cout;
using std::endl;
using std::cin;

//	Random distributions are defined globally to be used throughout the tests below.
//
std::random_device rd;
std::mt19937 engine(rd());
std::uniform_int_distribution<unsigned int> randomSymbol(0, 4);
std::uniform_int_distribution<unsigned int> randomQuantity(1, 100);
std::uniform_int_distribution<unsigned int> randomBool(0, 1);
std::uniform_real_distribution<double> randomPrice(0, 200);

////////////////////////////////////////////////////////////////////////////////
//  Prototypes for methods used in demonstration/testing
////////////////////////////////////////////////////////////////////////////////


// Used inbetween tests to request an action from the user to continue.
// This gives the user time to read the test results
//
void pauseForInput();

// Builds the sample stocks. Note that later methods are dependant on the given
// test symbols "TEA", "POP" etc.
// An alternative testing environment could seek out and use any given test symbols
//
void buildTestStocks(StockGroup&stocks);

// Prints out stock access results including a tested attempt to access invalid stocks
//
void demonstrateStockAccess(StockGroup&stocks);

// For each of the given stocks, this outputs the dividedYields for static test prices
//
void outputDividendYields(StockGroup&stocks);

// For each of the given stocks, this outputs the P/E ratio for static test prices
//
void outputPERatios(StockGroup&stocks);

// Updates the Volume Weighted Stock Prices in stocks that have been stored in vwsPrices.
// vwsPricesChanged is set to true if the stocks have changed in being recalculated.
//
void updateVwsPrices(StockGroup&stocks,
	std::vector<std::string> symbols,
	std::vector<double>& vwsPrices,
	bool&vwsPricesChanged,
	std::chrono::minutes mins);

// Generates test trades in recent time, such that they would be included in
// the 5 minute check for Volume Weighted Stock Prices. "count" trades are
// generated randomly accross the stocks
//
void generateRecentTestTrades(StockGroup&stocks, std::vector<std::string> symbols, unsigned int count);

// Generates a trade for each symbol such that each trade occurred 6 minutes ago.
// Such new trades should thus not affect the 5 minute Volume Weighted Stock Prices.
//
void generateNonRecentTestTrades(StockGroup&stocks, std::vector<std::string> symbols);

// This runs through a series of related tests comparing additions to the Trade Records
// against expected changes and lack of changes to the calculated Volume Weighted Stock
// Prices based on the times of the Trades and the duration on which the Volume
// Weighted Stock Price is based.
//
void testVwsForChanges(StockGroup&stocks, std::vector<std::string> symbols, std::vector<double>&vwsPrices);

// Outputs all trades for stocks in the given symbols.
// An exception will be thrown if any of the symbol are not found in the stock group.
//
void outputAllTrades(StockGroup&stocks, std::vector<std::string> symbols);

// Outputs the Volume Weighted Stock Prices for eachStock in symbols.
// An exception will be thrown if any of the symbol are not found in the stock group.
//
void outputVolumeWeightedStockPrices(StockGroup&stocks, std::vector<std::string> symbols, std::vector<double>&vwsPrices);

// Outputs the All Share Index for the given stock group
//
void outputAllShareIndex(StockGroup&stocks);


////////////////////////////////////////////////////////////////////////////////
// program entry point
////////////////////////////////////////////////////////////////////////////////

int main()
{
	try
	{
		StockGroup stocks;
		TradeRecord tradeRecord;
		std::vector<std::string> symbols{ "TEA", "POP", "ALE", "GIN", "JOE" };
		std::vector<double> vwsPrices{ 0.0,0.0,0.0,0.0,0.0 };

		buildTestStocks(stocks);

		demonstrateStockAccess(stocks);
		outputDividendYields(stocks);
		pauseForInput();

		outputPERatios(stocks);
		pauseForInput();

		cout << "** working";
		generateRecentTestTrades(stocks, symbols, 10);
		outputAllTrades(stocks, symbols);
		pauseForInput();

		// Output results of this - less than five minutes has passed so this will go through all
		// trades, and it is likely that some stocks may not have trades yet.
		outputVolumeWeightedStockPrices(stocks, symbols, vwsPrices);

		testVwsForChanges(stocks, symbols, vwsPrices);
		pauseForInput();

		cout << "** working";
		generateRecentTestTrades(stocks, symbols, 25);
		cout << endl;

		outputVolumeWeightedStockPrices(stocks, symbols, vwsPrices);
		outputAllShareIndex(stocks);

		cout << "\n\ndemonstration ended.\n";

	}
	catch (InvalidOperation& io)
	{
		cout << "An invalid operation was thrown:\n   " << io.what() << endl;
		cin.get();
	}
	catch (InvalidTimeError& it)
	{
		cout << "An invalid time error was thrown:\n   " << it.what() << endl;
		cin.get();
	}
	catch (std::invalid_argument& ia)
	{
		cout << "An invalid argument was thrown:\n   " << ia.what() << endl;
		cin.get();
	}
	catch (std::exception&e)
	{
		cout << "An exception was thrown:\n   " << e.what() << endl;
	}


	return 0;
}



////////////////////////////////////////////////////////////////////////////////
// Methods used in demonstration/testing
////////////////////////////////////////////////////////////////////////////////

// Used inbetween tests to request an action from the user to continue.
// This gives the user time to read the test results
//
void pauseForInput()
{
	cout << "\n** please press return.\n\n";
	cin.get();
}

// Builds the sample stocks. Note that later methods are dependant on the given
// test symbols "TEA", "POP" etc.
// An alternative testing environment could seek out and use any given test symbols
//
void buildTestStocks(StockGroup&stocks)
{
	//	   addStock(symbol,	type, last dividend,  par value, [fixed dividend]
	stocks.addStock("TEA", COMMON_STOCK, 0, 100);
	stocks.addStock("POP", COMMON_STOCK, 8, 100);
	stocks.addStock("ALE", COMMON_STOCK, 23, 60);
	stocks.addStock("GIN", PREFERRED_STOCK, 8, 100, 2);
	stocks.addStock("JOE", COMMON_STOCK, 13, 250);
}

// Prints out stock access results including a tested attempt to access invalid stocks
//
void demonstrateStockAccess(StockGroup&stocks)
{
	cout << "Demonstrating stock access:\n"
		<< stocks.accessStock("TEA") << endl
		<< stocks.accessStock("GIN") << endl
		<< "\n\n";

	try
	{
		cout << " Attempt to calculate dividend yield for invalid stock TONIC with price 150:\t" << stocks.accessStock("TONIC").calculateDividendYield(150) << endl;
	}
	catch (std::invalid_argument&ia)
	{
		cout << "Caught attempt to evaluate invalid stock:\n   " << ia.what() << endl;
	}
}

// For each of the given stocks, this outputs the dividedYields for static test prices
//
void outputDividendYields(StockGroup&stocks)
{
	cout << "Given any price as input, calculate the dividend yield:\n"
		<< " Dividend yield for TEA with price 50:\t" << stocks.accessStock("TEA").calculateDividendYield(50) << endl
		<< " Dividend yield for POP with price 25:\t" << stocks.accessStock("POP").calculateDividendYield(25) << endl
		<< " Dividend yield for POP with price 30:\t" << stocks.accessStock("POP").calculateDividendYield(30) << endl
		<< " Dividend yield for GIN with price 30:\t" << stocks.accessStock("GIN").calculateDividendYield(30) << "\n\n";
}

// For each of the given stocks, this outputs the P/E ratio for static test prices
//
void outputPERatios(StockGroup&stocks)
{
	cout << "Given any price as input,  calculate the P/E Ratio:\n"
		<< " P/E Ratio for TEA with price 75:\t" << stocks.accessStock("TEA").calculatePERatio(75) << endl
		<< " P/E Ratio for ALE with price 60:\t" << stocks.accessStock("ALE").calculatePERatio(60) << endl
		<< " P/E Ratio for ALE with price 120:\t" << stocks.accessStock("ALE").calculatePERatio(120) << endl
		<< " P/E Ratio for JOE with price 33:\t" << stocks.accessStock("JOE").calculatePERatio(33) << "\n\n";
}

// Updates the Volume Weighted Stock Prices in stocks that have been stored in vwsPrices.
// vwsPricesChanged is set to true if the stocks have changed in being recalculated.
//
void updateVwsPrices(StockGroup&stocks,
	std::vector<std::string> symbols,
	std::vector<double>& vwsPrices,
	bool&vwsPricesChanged,
	std::chrono::minutes mins)
{
	vwsPricesChanged = false;
	assert(vwsPrices.size() == symbols.size());
	for (unsigned int t = 0; t < symbols.size(); ++t)
	{
		bool foundTrades;
		double newVWSPrice =
			stocks.accessStock(symbols[t]).
			accessTradeRecord().
			calculateVolumeWeightedStockPriceWithin(foundTrades, mins);
		if (vwsPrices[t] != newVWSPrice)
		{
			vwsPricesChanged = true;
		}
		vwsPrices[t] = newVWSPrice;
	}
}

// Generates test trades in recent time, such that they would be included in
// the 5 minute check for Volume Weighted Stock Prices. "count" trades are
// generated randomly accross the stocks
//
void generateRecentTestTrades(StockGroup&stocks, std::vector<std::string> symbols, unsigned int count)
{
	for (unsigned int t = 0; t < count; ++t)
	{
		stocks.accessStock(symbols[randomSymbol(engine)]).
			accessTradeRecord().
			addTrade(randomQuantity(engine),
			(randomBool(engine) ? BUY_TYPE : SELL_TYPE),
				randomPrice(engine));
		cout << ".";
		if (randomBool(engine))
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	//Insert one trade as a minute ago - will appear at start
	TimeStamp timeStamp = std::chrono::system_clock::now() - std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::minutes(1));
	stocks.accessStock("TEA").accessTradeRecord().addTrade(10, BUY_TYPE, 20, timeStamp);
}

// Generates a trade for each symbol such that each trade occurred 6 minutes ago.
// Such new trades should thus not affect the 5 minute Volume Weighted Stock Prices.
//
void generateNonRecentTestTrades(StockGroup&stocks, std::vector<std::string> symbols)
{
	TimeStamp timeStamp = std::chrono::system_clock::now() - std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::minutes(6));
	for (auto symbol : symbols)
	{
		stocks.accessStock(symbol).
			accessTradeRecord().
			addTrade(randomQuantity(engine),
			(randomBool(engine) ? BUY_TYPE : SELL_TYPE),
				randomPrice(engine),
				timeStamp);
		if (randomBool(engine))
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

// This runs through a series of related tests comparing additions to the Trade Records
// against expected changes and lack of changes to the calculated Volume Weighted Stock
// Prices based on the times of the Trades and the duration on which the Volume
// Weighted Stock Price is based.
//
void testVwsForChanges(StockGroup&stocks, std::vector<std::string> symbols, std::vector<double>&vwsPrices)
{
	//Insert one trade as 2 minutes ago - VWSprices should change
	//
	TimeStamp timeStamp = std::chrono::system_clock::now() - std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::minutes(2));
	stocks.accessStock("TEA").accessTradeRecord().addTrade(10, BUY_TYPE, 20, timeStamp);

	bool vwsPricesChanged;
	updateVwsPrices(stocks, symbols, vwsPrices, vwsPricesChanged, std::chrono::minutes(5));
	if (vwsPricesChanged)
	{
		cout << "Success: Volume Weighted Stock Prices changed on addition of recent trade at specific time\n.";
	}
	else
	{
		cout << "ERROR: Volume Weighted Stock Prices did not change on addition of recent trade at specific time\n.";
	}

	//Insert a trade for each entry over 5 minutes ago; VWSprices should not change
	generateNonRecentTestTrades(stocks, symbols);

	updateVwsPrices(stocks, symbols, vwsPrices, vwsPricesChanged, std::chrono::minutes(5));
	if (vwsPricesChanged)
	{
		cout << "ERROR Volume Weighted Stock Prices changed on addition of non recent trade at specific time\n.";
	}
	else
	{
		cout << "Success: Volume Weighted Stock Prices did not change on addition of recent trade at specific time\n.";
	}

	//Check over a period greater than 5 to include
	updateVwsPrices(stocks, symbols, vwsPrices, vwsPricesChanged, std::chrono::minutes(15));
	if (vwsPricesChanged)
	{
		cout << "Success: non recent Volume Weighted Stock Prices changed on addition of non recent trade at specific time\n.";
	}
	else
	{
		cout << "ERROR: non recent Volume Weighted Stock Prices did not change on addition of recent trade at specific time\n.";
	}
}

// Outputs all trades for stocks in the given symbols.
// An exception will be thrown if any of the symbol are not found in the stock group.
//
void outputAllTrades(StockGroup&stocks, std::vector<std::string> symbols)
{
	for (auto symbol : symbols)
	{
		stocks.accessStock(symbol).accessTradeRecord().printTo(cout, symbol);
	}
}

// Outputs the Volume Weighted Stock Prices for eachStock in symbols.
// An exception will be thrown if any of the symbol are not found in the stock group.
//
void outputVolumeWeightedStockPrices(StockGroup&stocks, std::vector<std::string> symbols,std::vector<double>&vwsPrices)
{
	assert(symbols.size() == vwsPrices.size());

	for (unsigned int t = 0; t < symbols.size(); ++t)
	{
		bool foundTrades;
		vwsPrices[t] =
			stocks.accessStock(symbols[t]).
			accessTradeRecord().
			calculateVolumeWeightedStockPriceWithinFiveMinutes(foundTrades);
		if (foundTrades)
		{
			cout << "Found Trades for " << symbols[t] << ". Volume Weighted Stock Price: " << vwsPrices[t] << endl;
		}
		else
		{
			cout << "Found no Trades for " << symbols[t] << endl;
		}
	}
}

// Outputs the All Share Index for the given stock group
//
void outputAllShareIndex(StockGroup&stocks)
{
	cout << "\n\n\nAll Share Index: " << stocks.calculateAllShareIndexWithin(std::chrono::minutes(5));
}

