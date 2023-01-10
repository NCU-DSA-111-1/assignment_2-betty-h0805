#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

/* chess board
１２３４５６７８９
香桂銀金玉金銀桂香１
十飛十十十十十角十２
步步步步步步步步步３
十十十十十十十十十４
十十十十十十十十十５
十十十十十十十十十６
步步步步步步步步步７
十角十十十十十飛十８
香桂銀金玉金銀桂香９
*/
#define ROW 9
#define COL 9
#define TYPENUM 8

//prototype
void initChessboard();
void printChessboard();
void enterInfo(int turn);
int isInBoard(struct Point info);
void checkRule(int turn);
void updateBoard();
int isRightPlayer(int turn);
int isSelf(int turn);
int basicCheckSuccess(int turn);
void saveGame(char* filename);

enum PIECES {
	KYOU, KEI, GIN, KIN, GYOKU, HI, KAKU, FU, SPACE
};

enum CHCOLOR {
	BLUE = -1, WHITE, RED
};

typedef struct{
	unsigned int row; //row
	unsigned int col; //col
	int id; //which type of piece
	int color; //blue or red or white
}Chess;

struct Point {
	unsigned int col_start = 0; //start point
	unsigned int row_start= 0; //start point
	unsigned int col_end = 0; //end point
	unsigned int row_end = 0; //end point
};

Chess board[ROW][COL];
struct Point info;
int pieces[] = { KYOU,KEI,GIN,KIN,GYOKU,HI,KAKU,FU,SPACE };
//const char chessname[] = { '香','桂','銀','金','玉','飛','角','步','十' };
//const char* chessname[] = { "香","桂","銀","金","玉","飛","角","步","十" };
const char chessname[] = { "香桂銀金玉飛角步十" };
bool restart = 0;
bool continue_sign = 1; //game continue or not
bool illegal = 0; //the move is legal or not
bool notregret = 0; // regret or not

char start_status[100];
char filename[100];