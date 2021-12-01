//Will Harger
#include <iostream>
using namespace std;

class Player {
public:
	char playerName[10];
	char piece;
};

void MakeBoard(char board[][10]){
	int rows = 6;
	int columns = 7;
	for(int i = 1; i <= rows; i++){
		cout << "|";
		for(int y = 1; y <= columns; y++){
			if(board[i][y] != 'X' && board[i][y] != 'O')
				board[i][y] = '*';
			cout << board[i][y];
		}
		cout << "|" << endl;
	}
}

int PlayerDrop(char board[][10], Player activePlayer){
	int drop;
	do{
		cout << activePlayer.playerName << "'s Turn ";
		cout << "Please enter a column between 1 and 7: ";
		cin  >> drop;
		while ( board[1][drop] == 'X' || board[1][drop] == 'O' ){
			cout << "That column is full, please enter a new column: ";
			cin  >> drop;
		}
	}while (drop < 1 || drop > 7);
	return drop;
}

void CheckPlacement(char board[][10], Player activePlayer, int drop){
	int depth = 6;
	int turn = 0;
	do{
		if(board[depth][drop] != 'X' && board[depth][drop] != 'O'){
			board[depth][drop] = activePlayer.piece;
			turn = 1;
		}
		else
		--depth;
	}while(turn != 1);
}


bool CheckWin(char board[][10], Player activePlayer){
	char z = activePlayer.piece;
	int win = 0;
	bool x = false;
	for(int i = 8; i >= 1; --i){
		for(int y = 9; y >= 1; --y){
			if(board[i][y] == z && board[i-1][y+1] == z && board[i-2][y+2] == z && board[i-3][y+3] == z){
				win = 1;
			}
			if (board[i][y] == z && board[i][y+1] == z && board[i][y+2] == z && board[i][y+3] == z){
				win = 1;
			}
			if(board[i][y] == z && board[i-1][y-1] == z && board[i-2][y-2] == z && board[i-3][y-3] == z){
				win = 1;
			}
			if(board[i][y] == z && board[i][y-1] == z && board[i][y-2] == z && board[i][y-3] == z){
				win = 1;
			}
			if(board[i][y] == z && board[i-1][y] == z && board[i-2][y] == z && board[i-3][y] == z){	
				win = 1;
			}
		}
	}
	if(win == 1){
		x = true;
	}
	return x;
}
bool IsFull(char board[][10]){
	bool full = false;
	int x = 0;
	for (int i = 1; i <= 7; ++i){
		if (board[1][i] != '*')
			++x;
	}
	if(x == 7){
		full = true;
	}
	return full;
}
void PlayerWon(Player activePlayer){
	cout << endl << activePlayer.playerName << " You Win!" << endl;
}
char Replay(char board[][10]){
	char replay;
	cout << "Would you like to restart? Yes(y) No(n): ";
	cin  >> replay;
	if (replay == 'y')
	{
		for(int i = 1; i <= 6; i++)
		{
			for(int y = 1; y <= 7; y++)
			{
				board[i][y] = '*';
			}
		}
	}
	else{
		cout << "Goodbye!" << endl;
		}
	return replay;
}
int main(){
	int numberofplayers;
	cout << "Let's Play Connect 4" << endl << endl;
	cout << "How many Players?" << endl;
	cin >> numberofplayers;
	Player playerOne; 
	Player playerTwo;
	char board[9][10]; 
	int drop;
	bool win = false;
	bool full = false;
	char again = 'y';
	cout << "Player One enter your name: ";
	cin  >> playerOne.playerName;
	playerOne.piece = 'X';
	cout << "Player Two enter your name: ";
	cin  >> playerTwo.playerName;
	playerTwo.piece = 'O';
	MakeBoard(board);
	do{
		drop = PlayerDrop(board, playerOne);
		CheckPlacement(board, playerOne, drop);
		MakeBoard(board);
		win = CheckWin(board, playerOne);
		if (win){
			PlayerWon(playerOne);
			again = Replay(board);
			if (again == 'n'){
				break;
			}
		}
		drop = PlayerDrop(board, playerTwo);
		CheckPlacement(board, playerTwo, drop);
		MakeBoard(board);
		win = CheckWin(board, playerTwo);
		if (win){
			PlayerWon(playerTwo);
			again = Replay(board);
			if (again == 'n'){
				break;
			}
		}
		full = IsFull(board);
		if (full){
			cout << "The board is full, draw!" << endl;
			again = Replay(board);
		}
	}while (again != 'n');
	return 0;
}