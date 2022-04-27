
// authot : Seif El_Den Mohamed (Sonic_AR)

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define ULL unsigned long long
enum{
	BLACK , WHITE , EMPTY
};

enum{
	A8 = 0 , B8 , C8 , D8 , E8 , F8 , G8 , H8,
	A7 , B7 , C7 , D7 , E7 ,  F7 , G7 , H7,
	A6 , B6 , C6 , D6 , E6 ,  F6 , G6 , H6,
	A5 , B5 , C5 , D5 , E5 ,  F5 , G5 , H5,
	A4 , B4 , C4 , D4 , E4 ,  F4 , G4 , H4,
	A3 , B3 , C3 , D3 , E3 ,  F3 , G3 , H3,
	A2 , B2 , C2 , D2 , E2 ,  F2 , G2 , H2,
	A1 , B1 , C1 , D1 , E1 ,  F1 , G1 , H1,NO_SQ
};

// while -->>  king = 1 , queen = 2 , rook = 3 , bishop = 4 , knight = 5 , pawn = 6
// black -->>  king = 7 , queen = 8 , rook = 9 , bishop = 10 , knight = 11 ,pawn = 12
enum{
	WKI = 1 , WQU , WRO , WBI , WKN , WPA , BKI , BQU , BRO , BBI , BKN , BPA
};

// variables init

bool white_move;
int board[64];
bool moved[64];
ULL moves_bitboard = 0; 

bool is_black(int index){
	return board[index] >= 7 && board[index] <= 12;
}

bool is_white(int index){
	return board[index] >= 1 && board[index] <= 6;
}

bool is_empty(int index){
	return board[index] == 0 ? 1 : 0;
}

bool x_y_on_board(int x , int y){
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}
// this function to initialize the real bitboard values

void init_board(){
	white_move = true;
	memset(board , 0 , sizeof(board));
	memset(moved , 0 , sizeof(moved));
	board[A8] = WRO ;
	board[B8] = WKN ;
	board[C8] = WBI ;
	board[D8] = WQU ; 
	board[E8] = WKI ;
	board[F8] = WBI ;
	board[G8] = WKN ;
	board[H8] = WRO ;
	
	for(int i = 0; i < 8 ; i++){
		board[A7 + i] = WPA;
	}
	
	board[A1] = BRO ;
	board[B1] = BKN ;
	board[C1] = BBI ;
	board[D1] = BQU ; 
	board[E1] = BKI ;
	board[F1] = BBI ;
	board[G1] = BKN ;
	board[H1] = BRO ;
	
	for(int i = 0; i < 8 ; i++){
		board[A2 + i] = BPA;
	}
	
	/////// testing area //////////
	// board[D6] = BKI;
}

// this function to print the real bitboard

void print_board(){
	printf("   ");
	for(char c = 'A' ; c <= 'H' ; c++){
		printf("%c  " , c);
	}
	printf("\n");
	for(int i = 0; i < 8 ; i++){
		printf("%d  " , 8 - i);
		for(int j = 0; j < 8 ; j++){
			printf("%d " , board[i* 8 + j]);
			if(board[i * 8 + j] < 10)printf(" ");
		}
		printf("\n");
	}
}

// this function for white pawn move

void calculate_white_pawn_moves(int index){
	if(is_black(index + 9)){
		moves_bitboard |= (1ULL << (index + 9));
	}
	if(is_black(index + 7)){
		moves_bitboard |= (1ULL << (index + 7));
	}
	if(is_empty(index + 8)){
		moves_bitboard |= (1ULL << (index + 8));
	}else{
		return;
	}
	// this for pawn did not move before he can take 2 steps toward
	if(index >=  A7 && index <= H7){
		if(is_empty(index + 16)){
			moves_bitboard |= (1ULL << (index + 16));
		}
	}
}

// this function for white knight move

void calculate_white_knight_moves(int index){
	int KN_xd[] = {2 , -2 , 1 , -1};
	int KN_yd[] = {2 , -2 , 1 , -1};
	int rank = index / 8, file = index % 8, new_rank = 0 , new_file = 0;
	for(int i = 0; i < 4 ; i++){
		for(int j = 0; j < 4 ; j++){
			if(abs(KN_xd[i]) == abs(KN_yd[j]))
				continue;
			new_rank = KN_xd[i] + rank;
			new_file = KN_yd[j] + file;
			if(x_y_on_board(new_rank , new_file)){
				if(!is_white(new_rank * 8 + new_file))
					moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
			}
		}
	}
	
}

// this function to calculate white bishop moves

void calculate_white_bishop_moves(int index){
	int rank = index / 8 , file = index % 8;
	int new_rank = rank , new_file = file;
	while(1){
		--new_rank , --new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		--new_rank , ++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank , --new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank , ++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
}

// this function for calculating white rook moves

void calculate_white_rook_moves(int index){
	int rank = index / 8 , file = index % 8;
	int new_rank = rank , new_file = file;
	while(1){
		--new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		--new_rank ;
		if(x_y_on_board(new_rank , new_file)){
			if(is_white(new_rank * 8 + new_file)){
				break;
			}
			if(is_black(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
}

// this function for white king moves

void calculate_white_king_moves(int index){
	int rank = index / 8 , file = index % 8;
	int KI_xd[] = {0 , 0 , 1 , -1 , 1 , 1 , -1 , -1};
	int KI_yd[] = {1 , -1 , 0 , 0 , 1 , -1 , 1 , -1};
	for(int i = 0 ; i < 8 ; i++){
		if(x_y_on_board(rank + KI_xd[i] , file + KI_yd[i]) && (!is_white((rank + KI_xd[i]) * 8) + (file + KI_yd[i]))){
			moves_bitboard |= (1ULL << ((rank + KI_xd[i]) * 8 + (file + KI_yd[i])));
		}
	}
}

// this function to calc black pawn moves

void calculate_black_pawn_moves(int index){
	if(is_white(index - 9)){
		moves_bitboard |= (1ULL << (index - 9));
	}
	if(is_white(index - 7)){
		moves_bitboard |= (1ULL << (index - 7));
	}
	if(is_empty(index - 8)){
		moves_bitboard |= (1ULL << (index - 8));
	}else{
		return;
	}
	// this for pawn did not move before he can take 2 steps forward
	if(index >=  A2 && index <= H2){
		if(is_empty(index - 16)){
			moves_bitboard |= (1ULL << (index - 16));
		}
	}	
}

// this function to calculate black knight moves

void calculate_black_knight_moves(int index){
	int KN_xd[] = {2 , -2 , 1 , -1};
	int KN_yd[] = {2 , -2 , 1 , -1};
	int rank = index / 8, file = index % 8, new_rank = 0 , new_file = 0;
	for(int i = 0; i < 4 ; i++){
		for(int j = 0; j < 4 ; j++){
			if(abs(KN_xd[i]) == abs(KN_yd[j]))
				continue;
			new_rank = KN_xd[i] + rank;
			new_file = KN_yd[j] + file;
			if(x_y_on_board(new_rank , new_file)){
				if(!is_black(new_rank * 8 + new_file))
					moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
			}
		}
	}	
}

// this is to calculate black bishop moves

void calculate_black_bishop_moves(int index){
	int rank = index / 8 , file = index % 8;
	int new_rank = rank , new_file = file;
	while(1){
		--new_rank , --new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		--new_rank , ++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank , --new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank , ++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
}

// this is for black rook moves

void calculate_black_rook_moves(int index){
	int rank = index / 8 , file = index % 8;
	int new_rank = rank , new_file = file;
	while(1){
		--new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_file;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		++new_rank;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	new_rank = rank  , new_file = file;
	while(1){
		--new_rank ;
		if(x_y_on_board(new_rank , new_file)){
			if(is_black(new_rank * 8 + new_file)){
				break;
			}
			if(is_white(new_rank * 8 + new_file)){
				moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
				break;
			}
			moves_bitboard |= (1ULL << (new_rank * 8 + new_file));
		}else{
			break;
		}
	}
	
}

// this is for black king moves

void calculate_black_king_moves(int index){
	int rank = index / 8 , file = index % 8;
	int KI_xd[] = {0 , 0 , 1 , -1 , 1 , 1 , -1 , -1};
	int KI_yd[] = {1 , -1 , 0 , 0 , 1 , -1 , 1 , -1};
	for(int i = 0 ; i < 8 ; i++){
		if(x_y_on_board(rank + KI_xd[i] , file + KI_yd[i]) && (!is_black((rank + KI_xd[i]) * 8) + (file + KI_yd[i]))){
			moves_bitboard |= (1ULL << ((rank + KI_xd[i]) * 8 + (file + KI_yd[i])));
		}
	}
}

// this function for recgonize the piece and distribute every piece for its function

void distr(int index){
	moves_bitboard = 0;
	// thia is white pawn move
	if(board[index] == WPA){
		calculate_white_pawn_moves(index);
	}
	// this is white knight move
	if(board[index] == WKN){
		calculate_white_knight_moves(index);
	}
	//this is white bishop move
	if(board[index] == WBI){
		calculate_white_bishop_moves(index);
	}
	// this is white rook move
	if(board[index] == WRO){
		calculate_white_rook_moves(index);
	}
	// this is for white queen moves
	if(board[index] == WQU){
		calculate_white_rook_moves(index);
		calculate_white_bishop_moves(index);
	}
	// this is for white king moves
	if(board[index] == WKI){
		calculate_white_king_moves(index);
	}
	// this is for black pawn moves
	if(board[index] == BPA){
		calculate_black_pawn_moves(index);
	}
	// this is for black knight moves
	if(board[index] == BKN){
		calculate_black_knight_moves(index);
	}
	// this is for black bishop moves
	if(board[index] == BBI){
		calculate_black_bishop_moves(index);
	}
	// this is for black rook moves
	if(board[index] == BRO){
		calculate_black_rook_moves(index);
	}
	// this is for black queen
	if(board[index] == BQU){
		calculate_black_rook_moves(index);
		calculate_black_bishop_moves(index);
	}
	// this is for black king moves
	if(board[index] == BKI){
		calculate_black_king_moves(index);
	}
}



// this function for print moves bitboard

void print_moves_bitboard(){
	ULL index = 1;
	printf("  ");
	for(char c = 'A' ; c <= 'H' ; c++){
		printf("%c " , c);
	}
	printf("\n");
	for(int i = 0; i < 8 ; i++){
		printf("%d " , 8 - i);
		for(int j = 0; j < 8 ; j++){
			printf("%d ", (int)(moves_bitboard & index ? 1 : 0));
			index <<= 1;
		}
		printf("\n");
	}
}
bool valid_selection(int index){
	if(board[index] >= 1 &&board[index] <= 6 && white_move)return true;
	if(board[index] >= 7 && board[index] <= 12 && (!white_move))return true;
	return false;
}
void print_notes(){
	printf("//////////////////// notes ////////////////////// \n");
	printf("white is UP ,black is DOWN you should start with white\n");
	printf("choose capital letters like from A to H and letters after it from 1 to 8\n");
	printf("inputs examples choose smth like C7 to start because it's white then moves will come out\n");
	printf("choose position you want to put this piece on it\n\n\n");
}
int main(int argc, char **argv)
{
	init_board();
	print_notes();
	while(1){
		printf("choose place to put it on fly like A1 C1 etc...\n");
		print_board();
		char in[2] , in2[2];
		scanf("%s" , in);
		int index = (int)(in[0] - 'A') + (8 - (in[1] - '0')) * 8; 
		if(in[0] < 'A' || in[0] > 'H' || in[1] < '1' || in[1] > '8'){
			printf("this is NO_SQ Enter smth else\n");
			continue;
		}
		if(board[index] == 0){
			printf("this square in Empty choose smth else\n");
			continue;
		}
		if(!valid_selection(index)){
			if(white_move){
				printf("this is white move you choosed black try again\n");
			}
			else{
				printf("this is black move you choosed white try again\n");
			}
			continue;
		}
		distr(index);
		print_moves_bitboard();
		printf("choose pos to put this piece in\n");
		scanf("%s" , in2);
		int index2 = (int)(in2[0] - 'A') + (8 - (in2[1] - '0')) * 8; 
		if(in2[0] < 'A' || in2[0] > 'H' || in2[1] < '1' || in2[1] > '8'){
			printf("this is NO_SQ try again\n");
			continue;
		}
		if((moves_bitboard & (1ULL << index2)) == 0){
			printf("you can't choose this position try again \n");
			continue;
		}
		board[index2] = board[index];
		board[index] = 0;
		white_move ^= 1;
	}
	return 0;
}

