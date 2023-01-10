#include "main.h"


int counter = 1;
int main(){
	printf("Enter file name:");
	scanf("%s",filename);
	
	initChessboard();
	printChessboard();
	
	int turn = -1; //X(BLUE) goes first
	while (1) {
		enterInfo(turn);
		if (notregret) {
			if (basicCheckSuccess(turn)) {
				checkRule(turn);
				if (illegal) {
					printf("illegal\n");
					illegal = 0;
					continue;
				}
				else {
					updateBoard();
					printChessboard();
					saveGame(filename);
					turn *= (-1); //change player
				}
			}
			else continue;
		}
		else  continue;
	}
	
	return 0;
}

void initChessboard() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			int chessid = SPACE;
			int chesscolor = WHITE;
			//red
			if (i <= 2) {
				//first row
				if (i == 0) {
					if (j <= 4) {
						chesscolor = RED;
						chessid = pieces[j];
					}
					else {
						chesscolor = RED;
						chessid = pieces[TYPENUM - j];
					}
				}
				//second row
				if (i == 1) {
					if (j == 1) {
						chesscolor = RED;
						chessid = PIECES::HI;
					}
					if (j == 7) {
						chesscolor = RED;
						chessid = PIECES::KAKU;
					}
				}
				//third row
				if (i == 2) {
					chesscolor = RED;
					chessid = PIECES::FU;
				}
			}
			//blue
			else if (i >= 6) {
				if (i == 6) {
					chesscolor = BLUE;
					chessid = PIECES::FU;
				}
				if (i == 7) {
					if (j == 1) {
						chesscolor = BLUE;
						chessid = PIECES::KAKU;
					}
					if (j == 7) {
						chesscolor = BLUE;
						chessid = PIECES::HI;
					}
				}
				if (i == 8) {
					if (j <= 4) {
						chesscolor = BLUE;
						chessid = pieces[j];
					}
					else {
						chesscolor = BLUE;
						chessid = pieces[TYPENUM - j];
					}
				}
			}
			board[i][j].row = i;
			board[i][j].col = j;
			board[i][j].id = chessid;
			board[i][j].color = chesscolor;
		}
	}
}

void printChessboard() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			//printf("%c", chessname[board[i][j].id]);
			if (board[i][j].color == BLUE) {
				printf("\033[34m%c%c\033[0m", chessname[2 * (board[i][j].id)], chessname[2 * (board[i][j].id) + 1]);
			}
			else if (board[i][j].color == RED) {
				printf("\033[31m%c%c\033[0m", chessname[2 * (board[i][j].id)], chessname[2 * (board[i][j].id) + 1]);
			}
			else {
				printf("%c%c", chessname[2 * (board[i][j].id)], chessname[2 * (board[i][j].id) + 1]);
			}
			
			//printf("%c", board[i][j].id);
		}
		printf("\n");
	}
}

void enterInfo(int turn) {
	switch (turn) {
	case -1: //blue
		printf("[X(BLUE)]請輸入你要移動的棋子（段 筋）：\n");
		scanf("%d %d", &info.col_start, &info.row_start);
		printf("[X(BLUE)]請輸入你要放置的位置（段 筋）：\n");
		scanf("%d %d", &info.col_end, &info.row_end);
		printf("是否確定要這樣下？（１：確定　０：悔棋）：\n");
		scanf("%d", &notregret);
		info.col_end -= 1;
		info.row_end -= 1;
		info.col_start -= 1;
		info.row_start -= 1;
		break;
	case 1: //red
		printf("[Y(RED)]請輸入你要移動的棋子（段 筋）：\n");
		scanf("%d %d", &info.col_start, &info.row_start);
		printf("[Y(RED)]請輸入你要放置的位置（段 筋）：\n");
		scanf("%d %d", &info.col_end, &info.row_end);
		printf("是否確定要這樣下？（１：確定　０：悔棋）：\n");
		scanf("%d", &notregret);
		info.col_end -= 1;
		info.row_end -= 1;
		info.col_start -= 1;
		info.row_start -= 1;
		break;
	}
}

int isInBoard(struct Point info) {
	if (info.col_start > COL-1 || info.col_start < 0) return 0;
	else if (info.row_start > COL-1 || info.row_start < 0) return 0;
	else if (info.col_end > COL-1 || info.col_end < 0) return 0;
	else if (info.row_end > COL-1 || info.row_end < 0) return 0;
	else return 1;
}

int isRightPlayer(int turn) {
	if (turn != board[info.row_start][info.col_start].color) return 0;
	else return 1;
}

int isMove() {
	if (info.col_end == info.col_start && info.row_end == info.row_start) return 0;
	else return 1;
}

int isSelf(int turn) {
	if (board[info.row_end][info.col_end].color == turn) return 1;
	else return 0;
}

int basicCheckSuccess(int turn) {
	if (!isInBoard(info)) {
		printf("[ERROR]Out of chess board!\n");
		return 0;
	}

	if (!isRightPlayer(turn)) {
		printf("[ERROR]Please move your own piece!\n");
		return 0;
	}
	
	if (!isMove()) {
		printf("[ERROR]You didn't move your piece!\n");
		return 0;
	}

	if (isSelf(turn)) {
		printf("[ERROR]Your own piece is at the destination!BBB\n");
		return 0;
	}

	else return 1;
}

void checkRule(int turn) {
	switch (board[info.row_start][info.col_start].id) {
	case FU: //步
		if (board[info.row_start][info.col_start].color == BLUE) {
			if (info.col_end != info.col_start) { //different column
				illegal = 1;
				printf("[ERROR]This is a illegal move for 步!\n");
			}
			else if (info.row_end != info.row_start - 1) { //didn't walk 1 step foward
				illegal = 1;
				printf("[ERROR]This is a illegal move for 步!\n");	
			}
		}
		else if (board[info.row_start][info.col_start].color == RED) {
			if (info.col_end != info.col_start) { //different column
				illegal = 1;
				printf("[ERROR]This is a illegal move for 步!\n");
			}
			else if (info.row_end != info.row_start + 1) { //didn't walk 1 step foward
				illegal = 1;
				printf("[ERROR]This is a illegal move for 步!\n");
			}
		}
		break;
	case KYOU: //香
		if (board[info.row_start][info.col_start].color == BLUE) {
			if (info.col_end != info.col_start) { //different column
				illegal = 1;
				printf("[ERROR]This is a illegal move for 香!\n");
			}
			else if (info.row_end > info.row_start) { //walk backwards
				illegal = 1;
				printf("[ERROR]This is a illegal move for 香!\n");
			}
			else { // check if there exists pieces between start point and end point
				for (int i = info.row_start; i > info.row_end + 1; i--) {
					if (board[i][info.col_end].color != WHITE) { 
						illegal = 1;
					}
				}
				if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
		}
		else if (board[info.row_start][info.col_start].color == RED) {
			if (info.col_end != info.col_start) { //different column
				illegal = 1;
				printf("[ERROR]This is a illegal move for 香!\n");
			}
			else if (info.row_end < info.row_start) { //walk backwards
				illegal = 1;
				printf("[ERROR]This is a illegal move for 香!\n");
			}
			else { // check if there exists pieces between start point and end point
				for (int i = info.row_start; i < info.row_end - 1; i++) {
					if (board[i][info.col_end].color != WHITE) {
						illegal = 1;
					}
				}
				if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
		}
		break;
	case KEI: //桂
		if (board[info.row_start][info.col_start].color == BLUE) {
			if (info.col_end == info.col_start - 1 || info.col_end == info.col_start + 1) {
				if (info.row_end == info.row_start - 2) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 桂!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 桂!\n");
			}
		}
		else if (board[info.row_start][info.col_start].color == RED) {
			if (info.col_end == info.col_start - 1 || info.col_end == info.col_start + 1) {
				if (info.row_end == info.row_start + 2) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 桂!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 桂!\n");
			}
		}
		break;
	case GIN: // 銀
		if (board[info.row_start][info.col_start].color == BLUE) { 
			if (info.row_end == info.row_start - 1) { //move foward
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) { 
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 銀!\n");
				}
			}
			else if (info.row_end == info.row_start + 1){ // move backward
				if (info.col_end == info.col_start - 1 || info.col_end == info.col_start + 1) { 
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 銀!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 銀!\n");
			}
		}
		else if (board[info.row_start][info.col_start].color == RED){
			if (info.row_end == info.row_start + 1) { //move foward
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) { 
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 銀!\n");
				}
			}
			else if (info.row_end == info.row_start - 1) { // move backward
				if (info.col_end == info.col_start - 1 || info.col_end == info.col_start + 1) { 
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 銀!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 銀!\n");
			}
		}
		break;
	case KIN : //金
		if (board[info.row_start][info.col_start].color == BLUE) {
			if (info.row_end == info.row_start - 1 || info.row_end == info.row_start) { //move foward & same row
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 金!\n");
				}
			}
			else if (info.row_end == info.row_start + 1) { // move backward
				if (info.col_end == info.col_start) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 金!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 金!\n");
			}
		}
		else if (board[info.row_start][info.col_start].color == RED) {
			if (info.row_end == info.row_start + 1 || info.row_end == info.row_start) { //move foward & same row
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 金!\n");
				}
			}
			else if (info.row_end == info.row_start - 1) { // move backward
				if (info.col_end == info.col_start) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 金!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 金!\n");
			}
		}
		break;
	case GYOKU : //玉
		if (board[info.row_start][info.col_start].color == BLUE) {
			if (info.row_end == info.row_start - 1 || info.row_end == info.row_start || info.row_end == info.row_start + 1) { //move foward & same row & move backward
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 玉!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 玉!\n");
			}
		}
		else if (board[info.row_start][info.col_start].color == RED) {
			if (info.row_end == info.row_start + 1 || info.row_end == info.row_start || info.row_end == info.row_start - 1) { //move foward & same row & backward
				if (info.col_end > info.col_start - 1 && info.col_end < info.col_start + 1) {
					illegal = 0;
				}
				else {
					illegal = 1;
					printf("[ERROR]This is a illegal move for 玉!\n");
				}
			}
			else {
				illegal = 1;
				printf("[ERROR]This is a illegal move for 玉!\n");
			}
		}
		break;
	case HI : //飛 
		if (info.col_end == info.col_start) { //same column 
			if (info.row_end > info.row_start) { // check if there is pieces between the start point and the end point
				for (int i = info.row_start; i < info.row_end; i++) {
					if (board[i][info.col_end].color != WHITE) {
						illegal = 1;
					}
				}if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
			else if (info.row_end < info.row_start){ // check if there is pieces between the start point and the end point
				for (int i = info.row_end; i < info.row_start; i++) {
					if (board[i][info.col_end].color != WHITE) {
						illegal = 1;
					}
				}if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
			else {
				illegal = 0;
			}
				
		}
		else if (info.row_end == info.row_start){ //same row
			if (info.col_end > info.col_start) { // check if there is pieces between the start point and the end point
				for (int i = info.col_start; i < info.col_end; i++) {
					if (board[info.row_end][i].color != WHITE) {
						illegal = 1;
					}
				}if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
			else if (info.col_end < info.col_start) { // check if there is pieces between the start point and the end point
				for (int i = info.col_end; i < info.col_start; i++) {
					if (board[info.row_end][i].color != WHITE) {
						illegal = 1;
					}
				}if (illegal) {
					printf("[ERROR]There exists pieces between you and your destination!\n");
				}
			}
			else {
				illegal = 0;
			}
		}
		else {
			illegal = 1;
			printf("[ERROR]This is a illegal move for 飛!\n");
		}
		break;
	case KAKU : //角 //還沒判斷中間是否有棋子
		if (board[info.row_end][info.col_end].color == WHITE) { //general
			if (info.row_end == info.col_end) { //up-left to down-right
				
				illegal = 0;
			}
			else if (info.row_end + info.col_end == ROW - 1) { //up-right to down-left

			}
			else {
				illegal = 1;
			}

		}
		break;
	}

}

void updateBoard() {
	board[info.row_end][info.col_end].color = board[info.row_start][info.col_start].color;
	board[info.row_end][info.col_end].id = board[info.row_start][info.col_start].id;
	board[info.row_start][info.col_start].color = WHITE;
	board[info.row_start][info.col_start].id = SPACE;

	info.col_start = 0;
	info.row_start = 0;
	info.col_end = 0;
	info.row_end = 0;

	illegal = 0;
}

void saveGame(char *filename) {
	FILE* fptr;
	char Filename[100];
	char status[100];
	sprintf(Filename, "%s.txt", filename);
	
	fptr = fopen(Filename,"a");
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			sprintf(status,"%c%c", chessname[2 * (board[i][j].id)], chessname[2 * (board[i][j].id) + 1]);
			fprintf(fptr, status);
		}
	}
	fprintf(fptr, "\n");
	fclose(fptr);
}
