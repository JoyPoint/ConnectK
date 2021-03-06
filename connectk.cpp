#include "StdAfx.h"
#include "ConnectK.h"

using namespace std;

// constructor
ConnectK::ConnectK()
	: M(0), N(0), K(0), G(FALSE),
	CKPlayer(PLAYER_A, BoardState(BoardConfig()))
{
	board_ = 0;
}

// destructor
ConnectK::~ConnectK()
{
	for ( int i = 0; i < M; i++ )
		delete [] board_[i];

	delete [] board_;
}

/////////////////////////////////////////////////////////////////////////////////////////
// newGame(int pM, int pN, int pK, bool pG, char pmark)
//
// Purpose:		This function is to allocate memory and initialize structures necessary 
//				to play the game defined by the parameters, M, N, K and G passed by the
//				GUI (or other driving program to this class).  In this sample code, we are
//				using a two dimensional dynamic array of characters to represent the board_
//				and the symbols X, O, and BLANK (defined in the ConnectK.h file) to keep
//				track of game progress.
//
//	Parameters:	pM		number of rows in the game board_
//				pN		number of columns in the game board_
//				pK		number in a row to win the game
//				pG		Gravity on/off. Determines whether or not game pieces fall to the 
//						lowest available position in a column (as in Connect 4), or remain
//						in the position places (as in Tic-tac-toe).
//				pmark	this is the mark that the computer player has been assigned.
//						it will be either 'X' or 'O'.  It is not actually necessary to use
//						this variable, but you may find it useful to know which player will
//						move first, prior to nextMove() being called.
//
// Returns:		nothing.
//
// NOTE:		This function can be called at any time from the GUI.  Make sure it 
//				can re-allocate memory and re-initialize variables, game after game.
//
//				The board_ layout used by the GUI has the origin located at the top left corner
//				of the board_.  For example:
//
//					N columns
//
//				  0  1  2..N-1
//               +-----------+
//              0|	|  |  |	 |
//               |--+--+--+--|
//				1|	|  |  |	 |
//     M rows    |--+--+--+--|
//				2|  |  |  |	 |
//             ..|--+--+--+--|
//			  M-1|  |  |  |  |
//               +-----------+
//
void ConnectK::newGame(int pM, int pN, int pK, bool pG, char pmark)
{	
	// re-initialize all variables

	// delete the previous game board_ if it exists
	if ( board_ != NULL )
	{
		for ( int i = 0; i < M; i++ )
			delete [] board_[i];
		board_ = 0;	
		delete [] board_;
	}

	// p is for parameter
	M = pM;
	N = pN;
	K = pK;
	G = pG;
	computerMark = pmark;
	playerMark = (pmark == 'X') ? Mark::A : Mark::B;
	player = (pmark == 'X') ? PLAYER_A : PLAYER_B;

	BoardConfig config;
	config.rows = M;
	config.cols = N;
	config.kLength = K;
	config.gravityFlag = G;

	// initialize the board_ to all blank characters (see ConnectK.h for definitions)
	if ( M > 0 && N > 0 )
	{
		board_ = new CharArray[M];
		board = BoardState(config);
		for (int i = 0; i < M; i++)
		{
			board_[i] = new char[N];
			for (int j = 0; j < N; j++)
				board_[i][j] = BLANK;
		}
	}
	else
		cerr << "Error: bad array size." << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////
// nextMove(int &row, int &col)
//
// Purpose:		This function receives the move made by the human player (or other AI
//				player), records it, and returns a move based on an AI search routine.
//
// Parameters:	&row	this holds the row coordinate of the move made by the human player
//				&col	this holds the column coordinate of the move made by the human player
//
// Returns:		&row	place the row value of the move made by your AI search in this variable
//				&col	place the column value of the move made by your AI in this variable
//
// NOTE:		Remember to do all initialization in newGame().  This function must be ready to 
//				return a move when given one, and will determine the time taken by your program
//				in tournament competition.  Make your search as efficient as possible.
//				
void ConnectK::nextMove(int &row, int &col)
{
	// If x and y are not -1 then we need to record the move made by the human player
	// If x and y are -1, then this is the first move of the game, the AI moves first.
	if( ( row != -1 ) && ( col != -1 ) )
	{
		if (computerMark == X)
			board_[row][col] = O;
		else
			board_[row][col] = X;
	}

	// Now we need to have an AI routine to determine the next move to make.
	// In this case, we are just looking for an empty square on the board_,
	// and returning that move.  Hardly an effective AI routine!
	// You will need many supporting functions to create an effective AI competitor.
	// Call them from this routine, but remember that this function is the only interface
	// to the GUI there is and the next move your program makes must be assigned to the variables
	// row and col.
	for (int rows = M - 1; rows >= 0; rows--) // we want to look for blank positions at the bottom of each column
	{
		for (int cols = 0; cols < N; cols++) // columns are left to right
		{
			if (board_[rows][cols] == BLANK)
			{
				// record the move made by the AI
				board_[rows][cols] = computerMark;

				// return the move made by the AI
				row = rows;
				col = cols;
				return;
			}
		}
	}
}

Cell ConnectK::getMove(unsigned int deadline) {
	int row = 0;
	int col = 0;
	for (int rows = M - 1; rows >= 0; rows--) // we want to look for blank positions at the bottom of each column
	{
		for (int cols = 0; cols < N; cols++) // columns are left to right
		{
			if (board_[rows][cols] == BLANK)
			{
				// record the move made by the AI
				board_[rows][cols] = computerMark;

				// return the move made by the AI
				row = rows;
				col = cols;
				updateBoard(Cell(row, col), playerMark);
				return Cell(row, col);
			}
		}
	}
	updateBoard(Cell(0, 0), playerMark);
	return Cell(0, 0);
}