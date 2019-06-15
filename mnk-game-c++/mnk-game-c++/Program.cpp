#include "pch.h"
#include "Program.h"

using namespace std;

namespace mnkgame
{

	// Create our variables.
	bool Program::bWinCondition = false;
	bool Program::bTieCondition = false;
	bool Program::bValidCoord = false;
	int Program::iTurnCount = 0;
	int Program::iWinner = 0;
	GameBoard* Program::cGameBoard;

	// Our main function.
	void Program::Main()
	{
		cout << "Hello, welcome to a C++ implementation of the m, n, k game! Please input your m, n, and k!\n\ni.e. For an m of 3, a n of 4, and k of 5, input: 3, 4, 5" << endl;
		string sInput;
		getline( cin, sInput );

		// Sanitize and parse input.
		vector<int> iV = SanitizeInput( sInput );

		// Check if our input length is valid.
		while ( !( 2 < iV.size() ) )
		{
			cout << "\nYour input was invalid! Please try again!" << endl;
			getline( cin, sInput );
			iV = SanitizeInput( sInput );
		}

		// Generate our game board.
		cGameBoard = new GameBoard( iV[0], iV[1], iV[2] );

		// Play the game until win.
		while ( !bWinCondition | !bTieCondition )
		{
			for ( int i = 1; i <= 2; i++ )
			{
				// This is a check I had to add because of how the loop works.
				if ( bWinCondition | bTieCondition )
				{
					break;
				}

				// Print the board and run game queries.
				cGameBoard->BoardPrint();
				DoTurn( i );

				// Get the status of our game, and end the game if needed.
				if ( cGameBoard->GetStatus() == "continue" )
				{
				}
				else if ( cGameBoard->GetStatus() == "draw" )
				{
					bTieCondition = true;
					EndGame();
				}
				else if ( cGameBoard->GetStatus() == "win" )
				{
					bWinCondition = true;
					iWinner = cGameBoard->iWinner;
					EndGame();
				}
			}
		}
	}

	// Complete a turn for a player.
	// Pass: p for player #
	void Program::DoTurn( int p )
	{
		// Grab our input.
		cout << "\nPlayer " << p << ", would you like to add or remove a marker? ( type 'add' or 'remove' )" << endl;
		string sInput;
		getline( cin, sInput );

		// Add or remove a marker, based on input.
		if ( sInput == "add" )
		{
			DoMove( p, true );
		}
		else if ( sInput == "remove" )
		{
			DoMove( p, false );
		}
		else
		{
			cout << "\nInvalid query. Assuming 'add' as your query." << endl;
			DoMove( p, true );
		}

		iTurnCount++;
	}

	// Query our player to add or remove a marker.
	// Pass: p for player #, b for add or removal.
	void Program::DoMove( int p, bool b )
	{
		cout << "\nPlayer " << p << ", please input the coordinates of your move. ( i.e., type '1, 1' )" << endl;

		// Sanitize and parse input.
		string sInput;
		getline( cin, sInput );
		vector<int> iM = SanitizeInput( sInput );

		// This is a small snippet to check if our coordinates are out of bounds or not.
		// If our coordinates are valid, bValidCoord will be set to true.
		try
		{
			// Throw an exception without being fatal.
			string tmp = cGameBoard->iBoardArray.at(iM.at(0) - 1).at(iM.at(0) - 1);
			bValidCoord = true;
		}
		catch (const std::out_of_range& e)
		{
			bValidCoord = false;
		}

		if ( bValidCoord )
		{
			// Modify our board and if our move was unsuccessful, ask them again.
			if ( b && ( cGameBoard->ModifyBoard( p, iM[0] - 1, iM[1] - 1, true ) ) )
			{
				cout << "\nPlayer " << p << " claimed x: " << iM[0] << ", y: " << iM[1] << endl;
			}
			else if ( !b && ( cGameBoard->ModifyBoard( p, iM[0] - 1, iM[1] - 1, false ) ) )
			{
				cout << "\nPlayer " << p << " unclaimed x: " << iM[0] << ", y: " << iM[1] << endl;
			}
			else
			{
				cout << "Move failed!" << endl;
				DoMove( p, b );
			}
		}
		else
		{
			cout << "\nInvalid coordinates!" << endl;
			DoMove( p, b );
		}

		// Reset our variable after the move ends.
		bValidCoord = false;
	}

	// A function to end the game. Also, a good movie.
	void Program::EndGame()
	{
		cGameBoard->BoardPrint();
		if ( bWinCondition )
		{
			cout << "\nCongratulations! Player " << iWinner << ", you win! There was a total of " << iTurnCount << " turns." << endl;
			// PrintQuery();
		}
		else
		{
			cout << "\nLooks like you both won. It's a tie! Well played. There was a total of " << iTurnCount << " turns." << endl;
			// PrintQuery();
		}
	}

	// A fun little function to ask players if they want to output a .txt file of the final board.
	void Program::PrintQuery()
	{
		cout << "\n\nWould you like the output the final board to a .txt file? (  Y / N  )" << endl;
	}

	// A helper function to sanitize our input string to an array of numbers.
	// Pass: s for input string.
	// Return: A sanitized integer array.
	vector<int> Program::SanitizeInput( const string& si )
	{

		try
		{
			// Define some variables for our stream.
			string s = si;
			std::vector<int> iTbl;
			stringstream ss( s );
			int i;

			// Seperate our string into an array, based on commas & ignoring spaces.
			while ( ss >> i )
			{
				iTbl.push_back( i );
				if ( ss.peek() == ',' || ss.peek() == ' ' )
				{
					ss.ignore();
				}
			}

			return iTbl;
		}
		catch ( ... )
		{
			cout << "\nYour input was invalid! Please try again!" << endl;
			string sInput;
			getline( cin, sInput );
			return SanitizeInput( sInput );
		}
	}

	// A constructor called upon class instance generation.
	GameBoard::GameBoard( int v1, int v2, int v3 )
	{
		this->m = v1;
		this->n = v2;
		this->k = v3;

		// Initialize our board array.
		vector<vector<string>> iBoardArrayTmp( v1 );
		for ( int i = 0; i < v1; i++ )
		{
			iBoardArrayTmp[i] = vector<string>( v2 );
			for (  int j = 0; j < v2; j++  )
			{
				iBoardArrayTmp[i][j] = " ";
			}
		}
		iBoardArray = iBoardArrayTmp;

		cout << "\nYou have created a game board of M: " << v1 << ", N: " << v2 << ", and K: " << v3 << endl;
	}

	// Print out our board.
	void GameBoard::BoardPrint()
	{
		cout << "Here is the board:" << endl;

		// Print [ ], [X], and [O] depending on size of board.
		for ( int i = 0; i < this->m; i++ )
		{
			cout << "\n" << endl;
			for ( int j = 0; j < this->n; j++ )
			{
				cout << " [" << iBoardArray[i][j] << "] ";
			}
		}

		cout << "\n" << endl;
	}

	// Check if we have any blank spaces left.
	// Return: If we do, return true.
	bool GameBoard::bMovesLeft()
	{
		for ( int i = 0; i < this->m; i++ )
		{
			for ( int j = 0; j < this->n; j++ )
			{
				if ( iBoardArray[i][j] == " " )
				{
					return true;
				}
			}
		}
		return false;
	}

	// Return a win, draw, or continue game status.
	// Return: Game status ID.
	string GameBoard::GetStatus()
	{
		if ( !bMovesLeft() ) { return "draw"; }
		if ( iLatestMove[3] == 0 ) { return "continue"; }

		// We check for our win with search depth. We know for a fact our last
		// game board wasn't a win, so we take our latest move, cycle through
		// all of the direction pairs ( you need to check both, TRUST me ), 
		// and find out if our depth matches our required k value.

		// Coordinate modifiers, uses addition; N-S, W-E, NE-SW, SE-NW
		vector<vector<vector<int>>> mod =
		{
			{
				{ 0, 1 },
				{ 0, -1 }
			},
			{
				{ 1, 0 },
				{ -1, 0 }
			},
			{
				{ 1, 1 },
				{ -1, -1 }
			},
			{
				{ 1, -1 },
				{ -1, 1 }
			}
		};

		// First loop, loops through direction pairs.
		for ( int i = 0; i < 4; i++ )
		{
			// Second loop, loops through directions.
			for ( int k = 0; k < 2; k++ )
			{
				DoSearch( iLatestMove[0], iLatestMove[1], mod[i][k][0], mod[i][k][1] );
			}

			// After we're done checking our pair, reset our temporary counter.
			iSearchDepth = iDepth;
			iDepth = 1;

			// If our search depth is large enough, return a win.
			if ( iSearchDepth >= this->k )
			{
				iWinner = iLatestMove[2];
				return "win";
			}
		}
		return "continue";
	}

	// Does a search in a certain direction.
	// Pass: x coord, y coord, x dir modifier, y dir modifier
	// Return: int representing search depth.
	int GameBoard::DoSearch( int x, int y, int a, int b )
	{
		try
		{
			// This throws our exception without being fatal.
			string tmp = iBoardArray.at( x + a ).at(y + b);

			if ( iBoardArray[x + a][y + b] == sID )
			{
				iDepth++;
				DoSearch( x + a, y + b, a, b );
			}
		}
		catch ( ... )
		{
			// Edge piece, invalid direction to search.
		}
		return iDepth;
	}

	// Add or remove a play into the board.
	// Pass: player #, x coord, y coord, add = true; remove = false;
	// Return: true if move succeeded, false if move failed.
	bool GameBoard::ModifyBoard( int p, int x, int y, bool ar )
	{
		// If our spot is empty, and we want to add, add.
		if ( ar && ( iBoardArray[x][y] == " " ) )
		{
			switch ( p )
			{
			case 1:
				iBoardArray[x][y] = "X";
				sID = iBoardArray[x][y];
				break;
			case 2:
				iBoardArray[x][y] = "O";
				sID = iBoardArray[x][y];
				break;
			}

			// Setup our latest move variables for our win-condition search.
			iLatestMove[3] = ar ? 1 : 0;
			iLatestMove[0] = x;
			iLatestMove[1] = y;
			iLatestMove[2] = p;

			return true;
		}
		// If our spot is already empty, and we want to remove, return false!
		else if ( !ar && ( iBoardArray[x][y] == " " ) )
		{
			return false;
		}
		// If our spot is not empty, and we want to remove, remove!
		else if ( !ar && !( iBoardArray[x][y] == " " ) )
		{
			if ( ( p == 1 ) && ( iBoardArray[x][y] == "X" ) )
			{
				iBoardArray[x][y] = " ";
				return true;
			}
			else if ( ( p == 2 ) && ( iBoardArray[x][y] == "O" ) )
			{
				iBoardArray[x][y] = " ";
				return true;
			}
			return false;
		}

		// Return false for anything else. i.e. A spot is already taken.
		return false;

	}
}
