#include <functional>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

random_device rd;
mt19937 rng(rd());

#define MATRIX_SIZE 4
#define BASE_NUM 2


vector<int> get_empty_cells(const vector<unsigned int>& matrix) {
  vector<int> to_return;
  for (int i = 0; i < matrix.size(); i++) {
    if (matrix[i] == 0) {
      to_return.push_back(i);
    }
  }

  return to_return;
}

bool gen_new_base(vector<unsigned int>& matrix) {
  vector<int> empty_cells = get_empty_cells(matrix);
  if (empty_cells.empty()) {
    return false;
  }
  
  uniform_int_distribution<int> uni(0, empty_cells.size()-1);
  matrix[empty_cells[uni(rng)]] = BASE_NUM;
  return true;
}

void move(vector<unsigned int>& matrix, bool vertical, bool down_right) {
  vector<unsigned int> cur_list;
  bool can_merge = true;
  auto merge = [&](unsigned int element) {
    if (cur_list.empty()) {
      cur_list.push_back(element);
    }
    else if (cur_list.back() == element && can_merge) {
      cur_list.back() += element;
      can_merge = false;
    }
    else {
      cur_list.push_back(element);
      can_merge = true;
    }
  };

  int start_index;
  int direction;

  std::function<void(int, int)> collapse = [&](int cur_index, int cnt) {
    if (cnt < MATRIX_SIZE) {
      if (matrix[cur_index] != 0) {
        merge(matrix[cur_index]);
        matrix[cur_index] = 0;
      }

      collapse(cur_index + direction, cnt + 1);
      
      if (cnt == (cur_list.size() - 1)) {
        matrix[cur_index] = cur_list.back();
        cur_list.pop_back();
      }
    }
  };

  int increment = vertical ? 1 : MATRIX_SIZE;
  int swap = MATRIX_SIZE + 1;
  int limit = MATRIX_SIZE - 1;

  for (int i = 0; i <= increment * limit; i += increment) {
    start_index = down_right ? i + (swap - increment) * limit : i;
    direction = down_right ? increment - swap : swap - increment;  

    collapse(start_index, 0);
  }
}

bool dead_end(const vector<unsigned int>& matrix) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      if (i < (MATRIX_SIZE - 1) && matrix[i * MATRIX_SIZE + j] == matrix[i * MATRIX_SIZE + j + MATRIX_SIZE]) {
        return false;
      }
      if (j < (MATRIX_SIZE - 1) && matrix[i * MATRIX_SIZE + j] == matrix[i * MATRIX_SIZE + j + 1]) {
        return false;
      }
    }
  }

  return true;
}

void print_matrix(const vector<unsigned int>& matrix) {
  cout << endl;
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      cout << matrix[i * MATRIX_SIZE + j] << "\t";
    }
    cout << endl << endl << endl;
  }
  cout << endl;
}

int main() {
  vector<unsigned int> matrix(MATRIX_SIZE*MATRIX_SIZE);
  gen_new_base(matrix);
  print_matrix(matrix);
  vector<unsigned int> prev_state = matrix;

  char input;
  while ((cin >> input)) {
    cin.clear();
    cin.ignore(512, '\n');
    switch (input) {
      case 'w': move(matrix, true, false); break;
      case 'a': move(matrix, false, false); break;
      case 's': move(matrix, true, true); break;
      case 'd': move(matrix, false, true); break;
    }
    
    if (matrix != prev_state) {
      bool not_full = gen_new_base(matrix);
      if (!not_full && dead_end(matrix)) {
        cout << "Game Over" << endl;
        break;
      }
    }

    print_matrix(matrix);
    prev_state = matrix;
  }
}
