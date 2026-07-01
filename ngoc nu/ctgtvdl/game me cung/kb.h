
#ifndef MAZE_H
#define MAZE_H

#include <vector>

// Cau truc mot o trong luoi logic cua me cung
struct Cell {
    int r, c;
    bool visited = false;
    bool walls[4] = { true, true, true, true }; // 0: Tren, 1: Phai, 2: Duoi, 3: Trai
};

// Cau truc toa do
struct Point {
    int r, c;
};

class MazeGame {
private:
    int width, height;             // Kich thuoc logic (so o)
    int renderW, renderH;          // Kich thuoc hien thi thuc te tren Terminal
    std::vector<std::vector<Cell>> cells;
    std::vector<std::vector<char>> grid;     // Ma tran ky tu ASCII de in ra man hinh
    Point start, end;
    Point player;

    // Dinh huong di chuyen tuong ung: Tren (0), Phai (1), Duoi (2), Trai (3)
    int dr[4] = { -1, 0, 1, 0 };
    int dc[4] = { 0, 1, 0, -1 };

    void convertToASCII();

public:
    MazeGame(int h, int w);
    void generateMaze();
    void display(Point current_player);
    void playManual();
    int solveBFS(bool visualize = true);
    int solveDFS(bool visualize = true);
    void compareAlgorithms();
};

// Ham bo tro da nen tang de bat phim khong can Enter
int getKeyStroke();

#endif // MAZE_H