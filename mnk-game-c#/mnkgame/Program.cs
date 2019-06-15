using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace mnkgame
{
    class Program
    {
        
        // Create our variables.
        static bool bWinCondition = false;
        static bool bTieCondition = false;
        static bool bValidCoord = false;
        public static int iTurnCount = 0;
        static int iWinner;
        static GameBoard cGameBoard;

        // Our main function.
        static void Main( string[] args )
        {
            Console.WriteLine( "Hello, welcome to a C# implementation of the m, n, k game! Please input your m, n, and k!\n\ni.e. For an m of 3, a n of 4, and k of 5, input: 3, 4, 5" );
            var sInput = Console.ReadLine();

            // Sanitize and parse input.
            int[] iV = SanitizeInput( sInput );

            // Check if our input length is valid.
            while ( !( 2 < iV.Length ) )
            {
                Console.WriteLine( "\nYour input was invalid! Please try again!" );
                sInput = Console.ReadLine();
                iV = SanitizeInput( sInput );
            }

            // Generate our game board.
            cGameBoard = new GameBoard( iV[0], iV[1], iV[2] );

            // Play the game until win.
            while( !bWinCondition | !bTieCondition )
            {
                for ( int i = 1; i <= 2; i++ )
                {
                    // This is a check I had to add because of how the loop works.
                    if( bWinCondition | bTieCondition) { break; }

                    // Print the board and run game queries.
                    cGameBoard.BoardPrint();
                    DoTurn( i );

                    // Get the status of our game, and end the game if needed.
                    switch ( cGameBoard.GetStatus() )
                    {
                        case "continue":
                            break;
                        case "draw":
                            bTieCondition = true;
                            EndGame();
                            break;
                        case "win":
                            bWinCondition = true;
                            iWinner = cGameBoard.iWinner;
                            EndGame();
                            break;
                    }
                }
            }            
        }

        // Complete a turn for a player.
        // Pass: p for player #
        static void DoTurn( int p )
        {
            // Grab our input.
            Console.WriteLine( "\nPlayer {0}, would you like to add or remove a marker? (type 'add' or 'remove')", p );
            var sInput = Console.ReadLine();

            // Add or remove a marker, based on input.
            if( sInput == "add" ) { DoMove(p, true); }
            else if( sInput == "remove" ) { DoMove(p, false); }
            else { Console.WriteLine( "Invalid query. Assuming 'add' as your query.", p ); DoMove( p, true ); }

            iTurnCount++;
        }

        // Query our player to add or remove a marker.
        // Pass: p for player #, b for add or removal.
        static void DoMove( int p, bool b )
        {
            Console.WriteLine( "\nPlayer {0}, please input the coordinates of your move. (i.e., type '1, 1')", p );

            // Sanitize and parse input.
            var sInput = Console.ReadLine();
            int[] iM = SanitizeInput( sInput );

            // This is a small snippet to check if our coordinates are out of bounds or not.
            // If our coordinates are valid, bValidCoord will be set to true.
            try
            {
                string tmp = cGameBoard.iBoardArray[iM[0] - 1, iM[0] - 1];
                bValidCoord = true;
            }
            catch ( Exception ex ) { bValidCoord = false; }

            if ( bValidCoord )
            {
                // Modify our board and if our move was unsuccessful, ask them again.
                if ( b && ( cGameBoard.ModifyBoard(p, iM[0] - 1, iM[1] - 1, true ) ) )
                {
                    Console.WriteLine( "\nPlayer {0} claimed x: {1}, y: {2}!", p, iM[0], iM[1] );
                }
                else if ( !b && ( cGameBoard.ModifyBoard(p, iM[0] - 1, iM[1] - 1, false ) ) )
                {
                    Console.WriteLine( "\nPlayer {0} unclaimed x: {1}, y: {2}!", p, iM[0], iM[1] );
                }
                else
                {
                    Console.WriteLine( "Move failed!" );
                    DoMove( p, b );
                }
            }
            else
            {
                Console.WriteLine( "Invalid coordinates!" );
                DoMove( p, b );
            }

            // Reset our variable after the move ends.
            bValidCoord = false;
        }

        // A function to end the game. Also, a good movie.
        static void EndGame()
        {
            cGameBoard.BoardPrint();
            if ( bWinCondition )
            {
                Console.WriteLine( "\nCongratulations! Player {0}, you win! There was a total of {1} turns.", iWinner, iTurnCount );
                // PrintQuery();
            }
            else
            {
                Console.WriteLine( "\nLooks like you both won. It's a tie! Well played. There was a total of {0} turns.", iTurnCount );
                // PrintQuery();
            }
        }

        // A fun little function to ask players if they want to output a .txt file of the final board.
        static void PrintQuery()
        {
            Console.WriteLine("\n\nWould you like the output the final board to a .txt file? ( Y / N )");
        }

        // A helper function to sanitize our input string to an array of numbers.
        // Pass: s for input string.
        // Return: A sanitized integer array.
        static int[] SanitizeInput( string s )
        {
            try
            {
                return s.Replace(" ", "").Split(',').Select(n => Convert.ToInt32(n)).ToArray();
            }
            catch (Exception ex)
            {
                Console.WriteLine("\nYour input was invalid! Please try again!");
                string sInput = Console.ReadLine();
                return SanitizeInput(sInput);
            }
        }

    }

    // Our game board class. 
    class GameBoard
    {
        // Create our variables.
        public int iWinner;
        private int m, n, k;
        public string[,] iBoardArray;

        // Variables for our win-search algorithm.
        public int[] iLatestMove = { 0, 0, 0, 1 }; // Stores our latest moves. Struc: x, y, player, add: 1 | remove: 0
        public int iDepth = 1;
        public int iSearchDepth;
        public string sID; // Stores X or O, what we want to search for.

        // A constructor called upon class instance generation.
        public GameBoard( int v1, int v2, int v3 )
        {
            this.m = v1;
            this.n = v2;
            this.k = v3;

            // Initialize our board array.
            iBoardArray = new string[v1, v2];
            for ( int i = 0; i < v1; i++ )
            {
                for ( int j = 0; j < v2; j++ )
                {
                    iBoardArray[i, j] = " ";
                }
            }
            Console.WriteLine( "\nYou have created a game board of M: {0}, N: {1}, and K: {2}.", v1, v2, v3 );
        }

        // Print out our board.
        public void BoardPrint()
        {
            Console.WriteLine( "Here is the board:" );
            // Print [ ], [X], and [O] depending on size of board.
            for ( int i = 0; i < this.m; i++ )
            {
                Console.WriteLine( "\n" );
                for ( int j = 0; j < this.n; j++ )
                {
                    Console.Write( " [{0}] ", iBoardArray[i, j] );
                }
            }
            Console.WriteLine("\n");
        }

        // Check if we have any blank spaces left.
        // Return: If we do, return true.
        public bool bMovesLeft()
        {
            for ( int i = 0; i < this.m; i++ )
            {
                for ( int j = 0; j < this.n; j++ )
                {
                    if ( iBoardArray[i, j] == " " )
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        // Return a win, draw, or continue game status.
        // Return: Game status ID.
        public string GetStatus()
        {
            if ( !bMovesLeft() ) { return "draw"; }
            if ( iLatestMove[3] == 0 ) { return "continue"; }

            // We check for our win with search depth. We know for a fact our last
            // game board wasn't a win, so we take our latest move, cycle through
            // all of the direction pairs (you need to check both, TRUST me), 
            // and find out if our depth matches our required k value.

            // Coordinate modifiers, uses addition; N-S, W-E, NE-SW, SE-NW
            int[,,] mod =
            {
                {  { 0, 1 }, { 0, -1 } },
                {  { 1, 0 }, { -1, 0 } },
                {  { 1, 1 }, { -1, -1 } },
                {  { 1, -1 }, { -1, 1 } }
            };

            // First loop, loops through direction pairs.
            for ( int i = 0; i < 4; i++ )
            {
                // Second loop, loops through directions.
                for ( int k = 0; k < 2; k++ )
                {
                    DoSearch( iLatestMove[0], iLatestMove[1], mod[i, k, 0], mod[i, k, 1] );
                }

                // After we're done checking our pair, reset our temporary counter.
                iSearchDepth = iDepth;
                iDepth = 1;

                // If our search depth is large enough, return a win.
                if( iSearchDepth >= this.k )
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
        public int DoSearch(int x, int y, int a, int b)
        {
            try
            {
                if( iBoardArray[ x + a, y + b ] == sID )
                {
                    iDepth++;
                    DoSearch( x + a, y + b, a, b );
                }
            }
            catch
            {
                // Corner piece, invalid direction to search.
            }
            return iDepth;
        }

        // Add or remove a play into the board.
        // Pass: player #, x coord, y coord, add = true; remove = false;
        // Return: true if move succeeded, false if move failed.
        public bool ModifyBoard( int p, int x, int y, bool ar )
        {
            // If our spot is empty, and we want to add, add.
            if ( ar && ( iBoardArray[x, y] == " " ) )
            {
                switch ( p )
                {
                    case 1:
                        sID = iBoardArray[x, y] = "X";
                        break;
                    case 2:
                        sID = iBoardArray[x, y] = "O";
                        break;
                }

                // Setup our latest move variables for our win-condition search.
                iLatestMove[3] = ar ? 1 : 0;
                iLatestMove[0] = x; iLatestMove[1] = y; iLatestMove[2] = p;

                return true;
            }
            // If our spot is already empty, and we want to remove, return false!
            else if ( !ar && ( iBoardArray[x, y] == " " ) )
            {
                return false;
            }
            // If our spot is not empty, and we want to remove, remove!
            else if ( !ar && !( iBoardArray[x, y] == " " ) )
            {
                if( ( p == 1 ) && ( iBoardArray[x,y] == "X" ))
                {
                    iBoardArray[x, y] = " ";
                    return true;
                }
                else if ( ( p == 2 ) && ( iBoardArray[x, y] == "O" ) )
                {
                    iBoardArray[x, y] = " ";
                    return true;
                }
                return false;
            }

            // Return false for anything else. i.e. A spot is already taken.
            return false;

        }

    }
}

// Notes:
// I probably could have created a class for players, 
// To store player turn #,
// Player latest move,
// Player ID #,
// And player marker string.
// This would allow for infinite players.
