#include <iostream>
#include <random>
#include <vector>
#include <windows.h>
#include <fstream>

using namespace std;

class Game {
public:
	Game(const int& height,const int& width,const bool& setting) : height_(height), width_(width) {
		
		if (setting) {
			field.assign(height_, vector<bool>(width_));
			random_init();
		}
		else
		{
			ifstream f("field.txt");
			if (f) {
				int i = 0;
				char ch;
				field.resize(1, vector<bool>(0));
				while (f.get(ch)) {
					if (ch == '*') {
						field[i].push_back(1);
					}
					else if(ch == ' '){
						field[i].push_back(0);
					}

					if (ch == '\n') {
						field.resize(i + 2, vector<bool>(0));
						++i;
					}
				}
				height_ = i + 1;
				width_ = field[i].size();
			}	
			else
				throw("File (field.txt) not found");
			f.close();
		}
	}

	void startGame() {
		while (true)
		{
			print_field();
			next_step();
			Sleep(250);
		}
	}

private:
	void random_init() {
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<> distribution(1, 100);

		for (int i = 0; i < width_; i++) {
			for (int j = 0; j < height_; j++) {
				int num = distribution(generator);
				if (num % 2 == 0)
					field[i][j] = true;
				else
					field[i][j] = false;
			}
		}
	}

	void print_field() {
		system("cls");
		for (int i = 0; i < width_; i++) {
			for (int j = 0; j < height_; j++) {

				if (field[i][j] == true)
					cout << '*';
				else
					cout << ' ';

				cout << ' ';
			}
			cout << endl;
		}
	}

	vector<vector<int>> get_array_of_neighbor(const int& x, const int& y) {

		vector<vector<int>> result;

		result.assign(8, vector<int>(2));

		int ind = 0;

		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (!(i == x && j == y))
				{
					result[ind][0] = i;
					result[ind][1] = j;
					++ind;
				}

			}
		}
		return result;
	}

	int get_count_live_nb(const int& x,const int& y) {
		int x_, y_;
		int count = 0;
		auto neighbors = get_array_of_neighbor(x, y);
		for (int i = 0; i < 8; ++i) {

			x_ = neighbors[i][0];
			y_ = neighbors[i][1];

			if (!(x_ < 0 || y_ < 0) && !(x_ >= width_ || y_ >= height_) && (field[x_][y_] == true))
				count++;
		}
		return count;
	}
	void next_step() {
		int count_live_nb;

		for (int i = 0; i < width_; i++) {
			for (int j = 0; j < height_; j++) {
				count_live_nb = get_count_live_nb(i, j);
				

				if (field[i][j] == true && count_live_nb == 2 || count_live_nb == 3)
					field[i][j] = true;

				if (field[i][j] == true && count_live_nb > 3)
					field[i][j] = false;

				if (field[i][j] == true && count_live_nb < 2)
					field[i][j] = false;

				if (field[i][j] == false && count_live_nb == 3)
					field[i][j] = true;
			}
		}
	}
	int height_; 
	int width_;
	vector<vector<bool>> field;
};


int main(int argc, char* argv[])
{
	int M, N;
	cin >> M >> N;

	Game live(M, N, true);
	live.startGame();
	return 0;
}

