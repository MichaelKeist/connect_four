#include <ncurses.h>
#include <iostream>
using namespace std;
//a function for printing out a message when somebody wins (player variable is inverted as this is called after turnswap)
void win_message(int player, int maxx){
	//loop to clear earlier writing
	move(0, maxx/2);
	for(int i = 0; i < 18; i++){
		addstr(" ");
	}
	if(player == 2){
		move(0, maxx/2 - 11);
		attron(COLOR_PAIR(1));
		addstr("THE RED PLAYER HAS WON!");
	}
	else if(player == 1){
		move(0, maxx/2 - 11);	
		attron(COLOR_PAIR(2));
		addstr("THE BLUE PLAYER HAS WON!");
	}
}
//a simpler version of the listen function used after the game is ended but before the window has been closed
bool end_listen(int* y_pos, int* x_pos){
	int chr;
	chr = getch();
	if(chr == 113 or chr == 27){
		return false;
	}
	else if(chr == 97){
		*x_pos -= 2;	
	}
	else if(chr == 100){
		*x_pos += 2;
	}
	else if(chr == 119){
		*y_pos -= 1;
	}
	else if(chr == 115){
		*y_pos += 1;
	}
	return true;
}
//function to swap whose turn it is
void turn_swap(int* player){
	if(*player == 1){
		*player = 2;
	}
	else if(*player == 2){
		*player = 1;
	}
}
//function to check if someone won
bool win_check(int board[6][7], int player){
	//first need to change player, since this function is called after player swap (can't use turn_swap since that affects global)
	if(player == 1){
		player = 2;
	}
	else if(player == 2){
		player = 1;
	}
	int d_sum;
	int r_sum;
	int dr_sum;
	int dl_sum;
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 7; j++){
			if(board[i][j] == player){
				d_sum = 0;
				r_sum = 0;
				dr_sum = 0;
				dl_sum = 0;
				for(int c = 0; c < 4; c++){
					if(board[i + c][j] == player & i+c < 6){
						d_sum += board[i + c][j];
					}
					if(board[i][j + c] == player & j+c < 7){
						r_sum += board[i][j + c];
					}
					if(board[i + c][j + c] == player & i+c < 6 & j+c < 7){
						dr_sum += board[i + c][j + c];
					}
					if(board[i + c][j - c] == player & i+c < 6 & j-c >= 0){
						dl_sum += board[i + c][j - c];
					}
				}
			}
			if(d_sum == player * 4 or r_sum == player * 4 or dr_sum == player * 4 or dl_sum == player * 4){
				return true;	
			}
		}	
	}	
	return false;
}
//function for initializing board state, returns a 6x7 matrix filled with zeros
void initialize_board(int board[6][7]){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 7; j++){
			board[i][j] = 0;
		}
	}
}
//function for printing the current board state to stdscr
int print_board(int y, int x, int board[6][7]){
	wmove(stdscr, y, x);
	for(int i = 0; i < 6; i++){
		attrset(A_NORMAL);
		addch('|');
		addch(' ');
		for(int j = 0; j < 7; j++){
			if(board[i][j] == 0){
				attrset(A_NORMAL);
				addch('O');
				addch(' ');
			}
			else if(board[i][j] == 1){
				addch('X' | COLOR_PAIR(1));
				addch(' ');
			}
			else if(board[i][j] == 2){
				addch('X' | COLOR_PAIR(2));
				addch(' ');
			}
		}
	addch('|');
	wmove(stdscr, y + i + 1, x);
	}
	wmove(stdscr, y + 6, x);
	for(int i = 0; i < 17; i++){
		addch('-');	
	}
	return 0;
}
//function for listening and responding to keypresses
bool listen(int* y_pos, int* x_pos, int* player, int board[6][7], int matrix_pos[2]){
	move(*y_pos, *x_pos);
	int chr;
	chr = getch();
	if(chr == 113 or chr == 27){
		return false;
	}
	else if(chr == 10){
		if(*player == 1){
			if((matrix_pos[0] == 5 or board[matrix_pos[0] + 1][matrix_pos[1]] != 0) and board[matrix_pos[0]][matrix_pos[1]] == 0){
				move(0,0);
				addstr("                 ");
				board[matrix_pos[0]][matrix_pos[1]] = 1;
				win_check(board, 1);
				turn_swap(player);
			}else{
				move(0,0);
				addstr("invalid move");
			}
		}else{
			if((matrix_pos[0] == 5 or board[matrix_pos[0] + 1][matrix_pos[1]] != 0) and board[matrix_pos[0]][matrix_pos[1]] == 0){
				move(0,0);
				addstr("                 ");
				board[matrix_pos[0]][matrix_pos[1]] = 2;
				win_check(board, 2);
				turn_swap(player);
			}else{
				move(0,0);
				addstr("invalid move");	
			}
		}
	}
	else if(chr == 97){
		*x_pos -= 2;	
		matrix_pos[1] -= 1;
	}
	else if(chr == 100){
		*x_pos += 2;
		matrix_pos[1] += 1;
	}
	else if(chr == 119){
		*y_pos -= 1;
		matrix_pos[0] -= 1;
	}
	else if(chr == 115){
		*y_pos += 1;
		matrix_pos[0] += 1;
	}
	return true;
}
//function to print whose turn it is
void turn(int y = 0, int x = 0, int player = 1){
	move(y,x);
	//next loop required to erase earlier writing
	for(int i = 0; i < 18; i++){
		addstr(" ");
	}
	move(y, x);
	if(player == 1){
		attron(COLOR_PAIR(1));
		addstr("RED PLAYER'S TURN");
	}
	if(player == 2){
		attron(COLOR_PAIR(2));
		addstr("BLUE PLAYER'S TURN");
	}
	attron(A_NORMAL);
}
//function for keeping cursor and matrix position within bounds
int boundary(int* y_pos, int* x_pos, int matrix_pos[2], int maxy, int maxx){
	if(*y_pos < maxy/2 - 6){
		*y_pos = maxy/2 - 6;
		matrix_pos[0] = 0;
	}
	else if(*y_pos > maxy/2 - 1){
		*y_pos = maxy/2 - 1;
		matrix_pos[0] = 5;
	}
	else if(*x_pos < maxx/2 - 6){
		*x_pos = maxx/2 - 6;
		matrix_pos[1] = 0;
	}
	else if(*x_pos > maxx/2 + 6){
		*x_pos = maxx/2 + 6;
		matrix_pos[1] = 6;
	}
	return 0;
}
//main function
int main(){
	//initializing the board matrix and several variables needed to keep track of the game
	int board[6][7];
	int player = 1;
	int maxy = 0;
	int maxx = 0;
	int y_pos = 0;
	int x_pos = 0;
	bool user = true;
	bool winner = false;
	int chr;
	int matrix_pos[2] = {3, 3};
	initialize_board(board);
	//setting up the ncurses window
	initscr();
	cbreak();
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	keypad(stdscr,true);
	getmaxyx(stdscr, maxy, maxx);
	print_board(maxy/2 - 6, maxx/2 - 8, board);
	y_pos = maxy/2 - 3;
	x_pos = maxx/2;
	wrefresh(stdscr);
	//primary gameplay loop
	while(user & ~winner){
		boundary(&y_pos, &x_pos, matrix_pos, maxy, maxx);
		turn(0, maxx/2 - 8, player);
		move(y_pos, x_pos);
		wrefresh(stdscr);
		user = listen(&y_pos, &x_pos, &player, board, matrix_pos);
		print_board(maxy/2 - 6, maxx/2 - 8, board);
		winner = win_check(board, player);
	}
	win_message(player, maxx);
	while(user){
		boundary(&y_pos, &x_pos, matrix_pos, maxy, maxx);
		move(y_pos, x_pos);
		wrefresh(stdscr);
		user = end_listen(&y_pos, &x_pos);
	}
	endwin();
	cout << "Final Board State:\n";
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 7; j++){
			cout << board[i][j];
			cout << " ";
		}	
		cout << "\n";
	}
	return 0;
}
