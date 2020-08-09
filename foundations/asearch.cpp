#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };
// kClosed means evaluated but ignored due to low score
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
// Delta movement allowed in each cell for the A* problem

std::string cellstring(State _input_state) {
  switch (_input_state) {
  case State::kObstacle:
    return "⛰️\t";
    break;
  case State::kPath:
    return "🚗\t";
    break;
  case State::kStart:
    return "🚦\t";
    break;
  case State::kFinish:
    return "🏁\t";
    break;
  case State::kEmpty:
    return "0\t";
    break;
  case State::kClosed:
    return "0\t";
    break;
  default:
    return "0\t";
    break;
  }
}

//-------------------------------------------------------------------
std::vector<State> parseline(std::string _loaded_file_line) {
  // variables created to parse
  std::vector<State> parsed_line;
  int aux;
  char aux_char;
  // opening file and streaming
  std::istringstream stream_line(_loaded_file_line);
  while (stream_line >> aux >> aux_char) {
    if (aux == 0) {
      parsed_line.push_back(State::kEmpty);
    } else if (aux == 1) {
      parsed_line.push_back(State::kObstacle);
    }
  }
  return parsed_line;
}

//-------------------------------------------------------------------
std::vector<std::vector<State>> readboard(std::string _path) {
  // variables declaration
  std::ifstream loaded_file;
  std::vector<State> parsed_line;
  std::vector<std::vector<State>> parsed_board;
  std::string get_loaded_file_line;
  // load stream
  loaded_file.open(_path);
  // checkout if file has been loaded
  if (loaded_file) {
    // get each line at a time
    while (getline(loaded_file, get_loaded_file_line)) {
      parsed_line = parseline(get_loaded_file_line);
      parsed_board.push_back(parsed_line);
    }
  } else {
    std::cout << "Cannot open the file! Check directory :)";
  }
  return parsed_board;
}

//-------------------------------------------------------------------
void printboard(std::vector<std::vector<State>> _board_to_print) {
  for (std::vector<State> _vector_iterator : _board_to_print) {
    for (State i : _vector_iterator) {
      std::cout << cellstring(i);
    }
    std::cout << "\n";
  }
}
//-------------------------------------------------------------------
int heuristic(int _x1, int _x2, int _y1, int _y2) {
  return abs(_x2 - _x1) + abs(_y2 - _y1);
}
//-------------------------------------------------------------------
void addtoopen(int _x, int _y, int _g, int _h,
               std::vector<std::vector<int>> &_open_nodes,
               std::vector<std::vector<State>> &_grid) {
  std::vector<int> node = {_x, _y, _g, _h};
  _open_nodes.push_back(node);
  _grid[_x][_y] = State::kClosed;
}
//-------------------------------------------------------------------
// Specific function to compare two nodes based on g+h score
bool compare(const std::vector<int> &_first_node,
             const std::vector<int> &_second_node) {
  int f_value_first_node = _first_node[2] + _first_node[3];
  int f_value_second_node = _second_node[2] + _second_node[3];
  return f_value_first_node > f_value_second_node;
}

//-------------------------------------------------------------------
void cellsort(std::vector<std::vector<int>> *_v) {
  std::sort(_v->begin(), _v->end(), compare);
}

//-------------------------------------------------------------------
bool checkvalidcell(int _x, int _y, std::vector<std::vector<State>> &_grid) {
  bool x_on_grid = _grid.size() > _x;
  bool y_on_grid = _grid[0].size() > _y;
  if (x_on_grid && y_on_grid)
    return _grid[_x][_y] == State::kEmpty;
  return false;
}

//-------------------------------------------------------------------
void neighborsexpand(const std::vector<int> &_current_node,
                     std::vector<std::vector<int>> &_open_list,
                     std::vector<std::vector<State>> &_grid,
                     const int _goal[]) {
  int x = _current_node[0];
  int y = _current_node[1];
  int h = _current_node[2];
  int g = _current_node[3];
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];
    // Check that the potential neighbor's x2 and y2 values are on the grid and
    // not closed.
    if (checkvalidcell(x2, y2, _grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = heuristic(x2, y2, _goal[0], _goal[1]);
      addtoopen(x2, y2, g2, h2, _open_list, _grid);
    }
  }
}

//-------------------------------------------------------------------
std::vector<std::vector<State>> search(std::vector<std::vector<State>> &_grid,
                                       int _start[], int _goal[]) {
  std::vector<std::vector<int>> open{};
  // list of open nodes empty at the begining
  addtoopen(_start[0], _start[1], 0,
            heuristic(_start[0], _start[1], _goal[0], _goal[1]), open, _grid);
  while (!open.empty()) {
    cellsort(&open);
    std::vector<int> current = open.back();
    open = {};
    int x = current[0];
    int y = current[1];
    _grid[x][y] = State::kPath;

    if (x == _goal[0] && y == _goal[1]) {
      _grid[_start[0]][_start[1]] = State::kStart;
      _grid[_goal[0]][_goal[1]] = State::kFinish;
      return _grid;
    }
    neighborsexpand(current, open, _grid, _goal);
  }
  std::cout << "No path found\n";
  return std::vector<std::vector<State>>{};
}

//-------------------------------------------------------------------
int main() {
  std::vector<std::vector<State>> parsed_board;
  std::vector<std::vector<State>> solution;
  int start[2]{0, 0};
  int goal[2]{4, 5};
  parsed_board =
      readboard("/home/jcorchado/programming/cpp_nano_degree/1.board");
  std::cout << "This is the orginal parsed map\n";
  printboard(parsed_board);
  std::cout << "This is the result of A* algorithm\n";
  solution = search(parsed_board, start, goal);
  printboard(solution);
}
