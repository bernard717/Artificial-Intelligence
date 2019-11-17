
#include<windows.h>
#include<iostream>
#include<time.h>
#include<algorithm>
#include<vector>
#include "connect4_game.h"
#include "connect4_heuristic.h"

using namespace std;

#pragma warning(disable: 4996)

vector<int> v;            // heuristic 값을 저장

int maxf(int x, int y) {
	if (x > y)return x;
	else return y;
}

// 이미 존재하는 플레이어의 돌로 부터 4목 완성 가능성 있는 값들을 count 
void winning_count(int board[][10], int player, int value[], int score[]) {

	int cnt, flag, temp;
	int i, j, l;
	int num[10][10] = { 0 };
	for (int i = 1; i < 8; i++) {
		for (int j = 1; j < 7; j++) {
			if (board[i][j] == 0) {
				for (int k = 0; j + k < 7; k++)
					num[i][j + k] = k + 1;
				break;
			}
		}
	}
	for (i = 1; i <= 6; i++) {
		for (j = 1; j <= 7; j++) {
			//가로
			flag = cnt = temp = 0;
			for (l = j; l > j - 4 && j > 3; l--) {
				if (board[l][i] != player && board[l][i] != 0) {
					flag = 1;
					break;
				}
				else if (board[l][i] == 0) {
					temp += (7 - num[l][i]);
				}
				else if (board[l][i] == player) {
					cnt++;
				}
			}
			if (flag == 0) {
				value[cnt]++;
				score[cnt] = temp;
			}
			//세로
			flag = cnt = temp = 0;
			for (l = i; l > i - 4 && i > 3; l--) {
				if (board[j][l] != player && board[j][l] != 0) {
					flag = 1;
					break;
				}
				else if (board[j][l] == player) {
					cnt++;
				}
			}
			if (flag == 0) value[cnt]++;
			//오른쪽 위 대각선
			flag = cnt = temp = 0;
			for (l = 0; l < 4 && i > 3 && j < 5; l++) {
				if (board[j + l][i - l] != player && board[j + l][i - l] != 0) {
					flag = 1;
					break;
				}
				else if (board[j + l][i - l] == 0) {
					temp += (7 - num[j + 1][i - l]);
				}
				else if (board[j + l][i - l] == player) {
					cnt++;
				}
			}
			if (flag == 0) {
				value[cnt]++;
				score[cnt] = temp;
			}
			//왼쪽 위 대각선
			flag = cnt = temp = 0;
			for (l = 0; l < 4 && i > 3 && j > 3; l++) {
				if (board[j - l][i - l] != player && board[j - l][i - l] != 0) {
					flag = 1;
					break;
				}
				else if (board[j - l][i - l] == 0) {
					temp += (7 - num[j - 1][i - l]);
				}
				else if (board[j - l][i - l] == player) {
					cnt++;
				}
			}
			if (flag == 0) {
				value[cnt]++;
				score[cnt] = temp;
			}
		}
	}
}

// 이긴 사람의 값을 반환
int win_check(int x, int y, int board[][10]) {

	int i, j, min;

	//가로
	j = -maxf(-7, -x - 3);
	for (i = maxf(1, x - 3); i <= j; i++) {
		if (board[i][y] * board[i + 1][y] * board[i + 2][y] * board[i + 3][y] == 1) {
			return 1;
		}
		else if (board[i][y] * board[i + 1][y] * board[i + 2][y] * board[i + 3][y] == 16) {
			return 2;
		}
	}
	//세로
	j = -maxf(-6, -y - 3);
	for (i = maxf(1, y - 3); i <= j; i++) {
		if (board[x][i] * board[x][i + 1] * board[x][i + 2] * board[x][i + 3] == 1) {
			return 1;
		}
		else if (board[x][i] * board[x][i + 1] * board[x][i + 2] * board[x][i + 3] == 16) {
			return 2;
		}
	}
	//오른쪽 대각선
	min = -maxf(-x, -y);
	for (i = maxf(-3, 1 - min); x + i + 3 < 8 && y + i + 3 < 7; i++) {
		if (board[x + i][y + i] * board[x + i + 1][y + i + 1] * board[x + i + 2][y + i + 2] * board[x + i + 3][y + i + 3] == 1) {
			return 1;
		}
		else if (board[x + i][y + i] * board[x + i + 1][y + i + 1] * board[x + i + 2][y + i + 2] * board[x + i + 3][y + i + 3] == 16) {
			return 2;
		}
	}
	//왼쪽 대각선
	min = -maxf(-(8 - x), -y);
	for (i = maxf(-3, 1 - min); x - i - 3 > 0 && y + i + 3 < 7; i++) {
		if (board[x - i][y + i] * board[x - i - 1][y + i + 1] * board[x - i - 2][y + i + 2] * board[x - i - 3][y + i + 3] == 1) {
			return 1;
		}
		else if (board[x - i][y + i] * board[x - i - 1][y + i + 1] * board[x - i - 2][y + i + 2] * board[x - i - 3][y + i + 3] == 16) {
			return 2;
		}
	}
	return 0;
}

// negamax with alpha beta prunning 알고리즘
int negamax_solver(int depth, int board[][10], int sign, int alpha, int beta, int *best_x, int put_x, int put_y, int turn, int count) {
	int immi_board[10][10] = { 0 };
	int top_x[8] = { 0 };
	int top_y[8] = { 0 };
	int my_value[4] = { 0 };
	int oppo_value[4] = { 0 };
	int my_score[4] = { 0 };
	int oppo_score[4] = { 0 };
	int value, best_value;
	int child_num = 0;
	int i, j, x, y, winner = 0;

	for (j = 1; j <= 7; j++) {
		for (i = 1; i <= 6; i++) {
			immi_board[j][i] = board[j][i];
			if (immi_board[j][i] == 0) {
				top_x[child_num] = j;
				top_y[child_num] = i;
				child_num++;
				break;
			}
		}
	}

	if (put_x != 0 && put_y != 0) {
		winner = win_check(put_x, put_y, board);
		if (winner == 1) {
			return sign * INFI;
		}
		else if (winner == 2) {
			return sign * -INFI;
		}
	}

	if (depth == 0) {
		winning_count(immi_board, 1, my_value, my_score);
		winning_count(immi_board, 2, oppo_value, oppo_score);
		return sign * (my_value[0] + my_value[1] * 7 + my_value[2] * 37 + my_value[3] * 127 + my_score[0] + my_score[1] + my_score[2]
			- oppo_value[0] - oppo_value[1] * 7 - oppo_value[2] * 37 - oppo_value[3] * 127 - oppo_score[0] - oppo_score[1] - oppo_score[2]);
	}

	best_value = -INFI;
	for (i = 0; i < child_num; i++) {
		x = top_x[i];
		y = top_y[i];
		if (sign == 1) immi_board[x][y] = 1;
		else immi_board[x][y] = 2;
		value = -negamax_solver(depth - 1, immi_board, -sign, -beta, -alpha, best_x, x, y, turn, count);
		immi_board[x][y] = 0;
		if (best_value < value) {
			best_value = value;
			v.push_back(best_value);
			if (turn == 1) {
				if (depth == MAXDEPTH - count) {
					*best_x = x;
					cout << ".";
				}
			}
			else {
				if (depth == MAXDEPTH - 1 - count) {
					*best_x = x;
					cout << ".";
				}
			}
		}
		alpha = maxf(alpha, value);
		if (alpha >= beta)
			break;
	}
	return best_value;

}

// 돌을 어디다가 둬야 하는지를 반환
int* get_position_by_heuristic(int turn) {

	int best_x, negamax_value;
	int what[4];
	cout << "진행 중.";
	clock_t begin, end;
	begin = clock();
	if (turn == 1)
		negamax_value = negamax_solver(MAXDEPTH, board, 1, -INFI, INFI, &best_x, 0, 0, 1, 0);
	else
		negamax_value = negamax_solver(MAXDEPTH - 1, board, 1, -INFI, INFI, &best_x, 0, 0, 2, 0);
	int count = 1;
	while ((negamax_value == INFI || negamax_value == -1 * INFI)  && MAXDEPTH - count > 0) {
		if(turn == 1)
			negamax_value = negamax_solver(MAXDEPTH - count, board, 1, -INFI, INFI, &best_x, 0, 0, 1, count);
		else
			negamax_value = negamax_solver(MAXDEPTH - count - 1, board, 1, -INFI, INFI, &best_x, 0, 0, 2, count);
		count++;
	}
	end = clock();
	what[0] = v.size();
	what[1] = best_x;
	what[2] = ((end - begin) / CLOCKS_PER_SEC);
	what[3] = negamax_value;
	v.clear();
	v.shrink_to_fit();

	return what;
}
