#pragma once

// Import our standard headers. These all come with STD.
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Forward class declarations:
namespace mnkgame { class GameBoard; }


namespace mnkgame
{
	class Program
	{

		// Create our variables.
	private:
		static bool bWinCondition;
		static bool bTieCondition;
		static bool bValidCoord;
	public:
		static int iTurnCount;
	private:
		static int iWinner;
		static GameBoard* cGameBoard;

		// Our main function.
	public:
		static void Main();

		// static void Main( vector<string>& args );

		// Complete a turn for a player.
		// Pass: p for player #
		static void DoTurn( int p );

		// Query our player to add or remove a marker.
		// Pass: p for player #, b for add or removal.
		static void DoMove( int p, bool b );

		// A function to end the game. Also, a good movie.
		static void EndGame();

		// A fun little function to ask players if they want to output a .txt file of the final board.
		static void PrintQuery();

		// A helper function to sanitize our input string to an array of numbers.
		// Pass: s for input string.
		// Return: A sanitized integer array.
		static vector<int> SanitizeInput( const string& s );

	};

	// Our game board class. 
	class GameBoard
	{
		// Create our variables.
	public:
		int iWinner = 0;
	private:
		int m = 0, n = 0, k = 0;
	public:
		vector<vector<string>> iBoardArray;

		// Variables for our win-search algorithm.
		vector<int> iLatestMove = { 0, 0, 0, 1 }; // Stores our latest moves. Struc: x, y, player, add: 1 | remove: 0
		int iDepth = 1;
		int iSearchDepth = 0;
		string sID; // Stores X or O, what we want to search for.

		// A constructor called upon class instance generation.
		GameBoard( int v1, int v2, int v3 );

		// Print out our board.
		void BoardPrint();

		// Check if we have any blank spaces left.
		// Return: If we do, return true.
		bool bMovesLeft();

		// Return a win, draw, or continue game status.
		// Return: Game status ID.
		string GetStatus();

		// Does a search in a certain direction.
		// Pass: x coord, y coord, x dir modifier, y dir modifier
		// Return: int representing search depth.
		int DoSearch( int x, int y, int a, int b );

		// Add or remove a play into the board.
		// Pass: player #, x coord, y coord, add = true; remove = false;
		// Return: true if move succeeded, false if move failed.
		bool ModifyBoard( int p, int x, int y, bool ar );

	};
}

// Notes:
// I probably could have created a class for players, 
// To store player turn #,
// Player latest move,
// Player ID #,
// And player marker string.
// This would allow for infinite players.
