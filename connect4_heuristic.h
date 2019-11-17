#pragma once
#define MAXDEPTH 8
#define INFI 2147483647

int maxf(int x, int y);

void winning_count(int board[][10], int player, int value[], int score[]);

int win_check(int x, int y, int board[][10]);

int negamax_solver(int depth, int board[][10], int turn, int alpha, int beta, int *best_x, int put_x, int put_y, int what, int count);

int* get_position_by_heuristic(int turn);
