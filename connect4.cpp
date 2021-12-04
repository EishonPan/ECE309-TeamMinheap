#include <iostream>
#include <vector>
using namespace std;

#define MAX_ROWS 6
#define MAX_COLUMNS 7
#define EMPTY_CHAR '*'

class Player {
public:
	string playerName;
	char piece;

	void win() {
		cout << endl << this -> playerName << " You Win!" << endl;
	}
	
	void getName(int player_id, char piece) {	// Read the name of Number #<player_id>, whose chess character is <piece>
		cout << "Player #" << player_id << ", who holds '" << piece << "', enter your name: ";
		cin >> this -> playerName;
		this -> piece = piece;
	}

	/*
		Interface for a `Player` object to play the game against other `Player` objects.
		`Player` puts a column number into <next_drop_column> as its next step.
		polymorphism & pure virtual function: 
			The actual behaviours of this function are depend on the subclass of `Player`.
			This function MUST be implemented by subclasses themselves.
	*/
	virtual void getNextDrop(char board[MAX_ROWS + 1][MAX_COLUMNS + 1], int &next_drop_column) = 0;
};

class ConsolePlayer: public Player {	// class for human player. Inherited from `Player`
	virtual void getNextDrop(char board[MAX_ROWS + 1][MAX_COLUMNS + 1], int &next_drop_column) {
		int drop;
		do {
			cout << this -> playerName << "'s Turn ";
			cout << "Please enter a column between 1 and 7: ";
			cin  >> drop;
			while (board[1][drop] == 'X' || board[1][drop] == 'O' ){
				cout << "That column is full, please enter a new column: ";
				cin  >> drop;
			}
		} while (drop < 1 || drop > 7);	// until the input is legal
		next_drop_column = drop;
	}
};

class ComputerPlayer: public Player {	// class for computer player. Inherited from `Player`
	virtual void getNextDrop(char board[MAX_ROWS + 1][MAX_COLUMNS + 1], int &next_drop_column) {	// computer player will find a position to drop
		cout << "\nInput Any Character (such as a/b/c) to let computer continue ..." << endl;

		char ch;
		cin >> ch;

		int ansColumn = -1, maxCnt = 0;
		bool hasSolution = false;

		// Computer Strategy: find the column, whose number of computer player's chess is maximum among all column, as its answer column for next step
		for(int j = 1; j <= MAX_COLUMNS; j++) {		
			int cnt = 0, blankCnt = 0;
			for(int i = 1; i <= MAX_ROWS; i++) {
				cnt += board[i][j] == this -> piece;
				blankCnt += board[i][j] == EMPTY_CHAR;
			}
			if (!hasSolution && blankCnt > 0 || hasSolution && blankCnt > 0 && cnt > maxCnt) {
				maxCnt = cnt;
				ansColumn = j;
				hasSolution = true;
			}
		}
		if (hasSolution) {
			next_drop_column = ansColumn;
			cout << "Player " << this -> playerName << " chose column " << ansColumn << endl;
		} else {
			next_drop_column = -1;
			cout << "Player " << this -> playerName << " found no available position" << endl;
		}
	}
};

class Connect4Game {
public:
	char board[MAX_ROWS + 1][MAX_COLUMNS + 1];
	vector<Player*> players;
	int current_player_id;
	const char PIECE_SEQUENCE[2] = {'X', 'O'};

	void cleanBoard() {
		for(int i = 1; i <= MAX_ROWS; i++)
			for(int j = 1; j <= MAX_COLUMNS; j++)
				board[i][j] = EMPTY_CHAR;
	}

	void printBoard() {
		for(int i = 1; i <= MAX_ROWS; i++) {
			cout << "|";
			for(int j = 1; j <= MAX_COLUMNS; j++)
				cout << board[i][j];
			cout << "|" << endl;
		}
		cout << endl;
	}

	bool isBoardFull() {
		int cnt = 0;
		for (int j = 1; j <= MAX_COLUMNS; j++) cnt += board[1][j] != EMPTY_CHAR;
		return cnt == MAX_COLUMNS;
	}

	bool checkWin(char z){
		int win = 0;
		bool x = false;
		for(int i = 8; i >= 1; --i){
			for(int y = 9; y >= 1; --y){
				if (board[i][y] == z && board[i-1][y+1] == z && board[i-2][y+2] == z && board[i-3][y+3] == z) win = 1;
				if (board[i][y] == z && board[i][y+1] == z && board[i][y+2] == z && board[i][y+3] == z) win = 1;
				if (board[i][y] == z && board[i-1][y-1] == z && board[i-2][y-2] == z && board[i-3][y-3] == z) win = 1;
				if (board[i][y] == z && board[i][y-1] == z && board[i][y-2] == z && board[i][y-3] == z) win = 1;
				if (board[i][y] == z && board[i-1][y] == z && board[i-2][y] == z && board[i-3][y] == z) win = 1;
			}
		}
		if(win == 1) x = true;
		return x;
	}

	void checkReplacement(char piece, int drop){
		int depth = 6;
		int turn = 0;
		do {
			if(this -> board[depth][drop] != 'X' && this -> board[depth][drop] != 'O') {
				this -> board[depth][drop] = piece;
				turn = 1;
			}
			else
			--depth;
		} while(turn != 1);
	}

	void handleRound() {
		int next_drop_columns;
		players[current_player_id] -> getNextDrop(this -> board, next_drop_columns);
		checkReplacement(players[current_player_id] -> piece, next_drop_columns);
		this -> printBoard();
	}

	int handleNewGame() {
		int numberOfPlayers = 2;
		cout << "Let's Play Connect 4" << endl;
		// cout << "How many Players?" << endl;
		// cin >> numberOfPlayers;

		players.clear();

		for(int i = 0; i < numberOfPlayers; i++) {
			char playerType;
			do {
				cout << "\nEnter the type of #" << i << " player. 'C' for Computer and 'H' for human " << endl;
				cin >> playerType;
			} while (playerType != 'C' && playerType != 'H');

			if (playerType == 'H') players.push_back(new ConsolePlayer());
			else players.push_back(new ComputerPlayer());
			players[i] -> getName(i, PIECE_SEQUENCE[i]); 
		}

		this -> cleanBoard();

		this -> current_player_id = 0;
		while (true) {
			this -> handleRound();
			bool isWin = this -> checkWin(players[current_player_id] -> piece);
			if (isWin) {
				players[current_player_id] -> win();
				break;
			}
			if (this -> isBoardFull()) {
				cout << "The board is full, draw!" << endl;
				break;
			}
			this -> current_player_id = (this -> current_player_id + 1) % players.size();
		}
		return 0; // exit normally
	}

	void run() {
		while (true) {
			this -> handleNewGame();
			char ifReplay;
			do {
				cout << "Would you like to restart? Yes(y) No(n): ";
				cin >> ifReplay;
			} while (ifReplay != 'y' && ifReplay != 'n');

			if (ifReplay == 'n') {
				cout << "Goodbye!" << endl;
				return ;
			}
		}
	}
};

int main() {
	Connect4Game game;
	game.run();
	return 0;
}