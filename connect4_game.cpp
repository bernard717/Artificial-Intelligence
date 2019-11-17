
#include<windows.h>
#include<iostream>
#include "connect4_heuristic.h"
#include "connect4_game.h"

using namespace std;

#pragma warning(disable: 4996)

int board[10][10], len[10];
int turn = 1;
int dy[4] = { 0, -1, -1, -1 }, dx[4] = { -1, -1, 0, 1 };

void print_board() {
	for (int i = 6; i >= 1; i--) {
		cout << "|";
		for (int j = 1; j <= 7; j++) {
			if (board[j][i] == 0) cout << "  |";
			else if (board[j][i] == 1) cout << "○|";
			else cout << "●|";
		}
		cout << endl;
	}
	cout << "";
	for (int i = 1; i <= 7; i++) cout << "  " << i;
	cout << endl << endl;
}

void put_piece(int x, bool num) {
	board[x][++len[x]] = num ? 1 : 2;
}

void deput_piece(int x) {
	board[x][len[x]--] = 0;
}

int putable_both_side(int x) {
	int cnt1 = 0;
	for (int i = 0; i < 4; i++) {
		bool flag = false;
		int cnt = 1;
		for (int j = -1; j >= -2; j--) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 6 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = false;
				break;
			}
			cnt++;
		}
		for (int j = 1; j <= 2; j++) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 6 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = flag;
				else flag = false;
				break;
			}
			cnt++;
		}
		if (flag && cnt >= 3) cnt1++;
	}
	return cnt1;
}

bool double_three(int x) {
	int cnt1 = 0;
	for (int i = 0; i < 4; i++) {
		bool flag = false;
		int cnt = 1;
		for (int j = -1; j >= -2; j--) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 6 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = false;
				break;
			}
			cnt++;
		}
		for (int j = 1; j <= 2; j++) {
			int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
			if (target_x < 1 || target_x > 6 || target_y < 1 || target_y > 7) break;
			if (board[target_x][target_y] != board[x][len[x]]) {
				if (board[target_x][target_y - 1] != 0 && board[target_x][target_y] == 0) flag = true;
				else flag = flag;
				break;
			}
			cnt++;
		}
		if (flag && cnt >= 3) cnt1++;
	}
	if (cnt1 >= 2) return true;
	return false;
}

int get_unlimit_length_connected(int x, int i) {
	int cnt = 1;
	for (int j = -1; j >= -3; j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] != 0 && board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	for (int j = 1; j <= 3; j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] != 0 && board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	return cnt;
}

int get_overflow_length_connected(int x, int i) {
	int cnt = 1;
	bool flag = false;
	for (int j = -1; j >= -3; j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] == 0 && !flag) {
			flag = true;
			cnt++;
		}
		else if (board[target_x][target_y] == board[x][len[x]]) cnt++;
		else break;
	}
	flag = false;
	for (int j = 1; j <= 3; j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7) break;
		if (board[target_x][target_y] == 0 && !flag) {
			flag = true;
			cnt++;
		}
		else if (board[target_x][target_y] == board[x][len[x]]) cnt++;
		else break;
	}
	return cnt;
}

int get_current_length_connected(int x, int i) {
	int cnt = 1;
	for (int j = -1; j >= -3; j--) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7 || board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	for (int j = 1; j <= 3; j++) {
		int target_x = x + dx[i] * j, target_y = len[x] + dy[i] * j;
		if (target_x < 1 || target_x>6 || target_y < 1 || target_y>7 || board[target_x][target_y] != board[x][len[x]]) break;
		cnt++;
	}
	return cnt;
}

int get_max_length_connected(int x) {
	int maxv = 0;
	for (int i = 0; i < 4; i++) {
		int result = get_current_length_connected(x, i);
		if (maxv < result) maxv = result;
	}
	return maxv;
}

void game_over(int winner) {


	cout << "게임 종료" << endl << endl;
	print_board();

	if (winner == 1) cout << "○ 승리!";
	else cout << "● 승리!";

	Sleep(3000);
}

int main() {
	int is_first, placeholder;
	cout << "공격 순서 (선공: 1, 후공: 2) : ";
	cin >> is_first;

	system("cls");
	if (is_first == 1) {
		cout << endl << endl;
		print_board();

		// 선공이면 무조건 5에 놓고 시작함 

		put_piece(5, true);
		system("cls");

		cout << "마지막 놓은 위치 : 5" << endl << endl;
	}
	else cout << endl << endl;

	while (true) {
		int position, time, max_value, size;
		int *what;

		print_board();
		cout << "● 상대 돌을 놓을 위치(1~7) : ";
		cin >> position;
		if (position < 1 || position > 7 || len[position] >= 6) {
			system("cls");
			cout << "잘못된 위치의 입력" << endl << endl;
			continue;
		}

		put_piece(position, false);
		system("cls");

		//게임이 끝났는지 판별 

		if (get_max_length_connected(position) >= 4) {
			game_over(2);
			return 0;
		}

		cout << "마지막 놓은 위치 : " << position << endl;
		cout << endl;
		print_board();

		what = get_position_by_heuristic(is_first);
		size = what[0];
		position = what[1];
		time = what[2];
		max_value = what[3];

		put_piece(position, true);
		system("cls");

		// 게임이 끝났는지 판별 

		if (get_max_length_connected(position) >= 4) {
			game_over(1);
			return 0;
		}

		cout << "마지막 놓은 위치 : " << position << endl << endl;
		cout << "이전 소요 시간 : " << time << endl << endl;
		cout << "heurisitc에 의해 계산된 값들 총 " << size << "개 중 가장 큰 값이 " << max_value << "이기 때문에 이 수를 두었다." << endl;

	}
	return 0;
}
