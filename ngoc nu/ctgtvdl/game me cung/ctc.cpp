

#include "kb.h"
#include <iostream>
#include <stack>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Cai dat ham bat phim khong cho Enter cho tung OS
int getKeyStroke() {
#if defined(_WIN32) || defined(_WIN64)
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

MazeGame::MazeGame(int h, int w) {
    height = h;
    width = w;
    renderH = h * 2 + 1;
    renderW = w * 2 + 1;
    start = { 0, 0 };
    end = { height - 1, width - 1 };
    player = start;
}

// Tao me cung ngau nhien bang DFS (Recursive Backtracker)
void MazeGame::generateMaze() {
    cells.assign(height, vector<Cell>(width));
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            cells[r][c].r = r;
            cells[r][c].c = c;
        }
    }

    stack<Point> st;
    cells[0][0].visited = true;
    st.push({ 0, 0 });

    while (!st.empty()) {
        Point curr = st.top();
        vector<int> neighbors;

        for (int i = 0; i < 4; ++i) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];
            if (nr >= 0 && nr < height && nc >= 0 && nc < width && !cells[nr][nc].visited) {
                neighbors.push_back(i);
            }
        }

        if (!neighbors.empty()) {
            int randDir = neighbors[rand() % neighbors.size()];
            int nr = curr.r + dr[randDir];
            int nc = curr.c + dc[randDir];

            cells[curr.r][curr.c].walls[randDir] = false;
            cells[nr][nc].walls[(randDir + 2) % 4] = false;

            cells[nr][nc].visited = true;
            st.push({ nr, nc });
        }
        else {
            st.pop();
        }
    }
    convertToASCII();
}

void MazeGame::convertToASCII() {
    grid.assign(renderH, vector<char>(renderW, '#'));

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            int gr = r * 2 + 1;
            int gc = c * 2 + 1;
            grid[gr][gc] = ' ';

            if (!cells[r][c].walls[1] && c + 1 < width) grid[gr][gc + 1] = ' ';
            if (!cells[r][c].walls[2] && r + 1 < height) grid[gr + 1][gc] = ' ';
        }
    }
    grid[start.r * 2 + 1][start.c * 2 + 1] = 'S';
    grid[end.r * 2 + 1][end.c * 2 + 1] = 'E';
}

// Hien thi me cung ASCII tren terminal
void MazeGame::display(Point current_player) {
    vector<vector<char>> tempGrid = grid;
    tempGrid[current_player.r * 2 + 1][current_player.c * 2 + 1] = 'P';

    for (int r = 0; r < renderH; ++r) {
        for (int c = 0; c < renderW; ++c) {
            cout << tempGrid[r][c] << " ";
        }
        cout << "\n";
    }
}
// Nguoi choi tu giai bang nut W/A/S/D
void MazeGame::playManual() {
    player = start;
    while (true) {
        system("clear || cls");
        cout << "=== CHE DO TU CHOI (W: Len, S: Xuong, A: Trai, D: Phai | Q: Thoat) ===\n\n";
        display(player);

        if (player.r == end.r && player.c == end.c) {
            cout << "\n Chuc mung! Ban da tim duoc loi thoat thanh cong!\n";
            cout << "Nhan phim bat ky de quay lai Menu chinh.";
            getKeyStroke();
            break;
        }

        char ch = getKeyStroke();
        if (ch == 'q' || ch == 'Q') break;

        int pr = player.r;
        int pc = player.c;

        if ((ch == 'w' || ch == 'W') && !cells[pr][pc].walls[0]) player.r--;
        else if ((ch == 'd' || ch == 'D') && !cells[pr][pc].walls[1]) player.c++;
        else if ((ch == 's' || ch == 'S') && !cells[pr][pc].walls[2]) player.r++;
        else if ((ch == 'a' || ch == 'A') && !cells[pr][pc].walls[3]) player.c--;
    }
}

// Tim duong bang BFS (Duong ngan nhat)
int MazeGame::solveBFS(bool visualize) {
    vector<vector<Point>> parent(height, vector<Point>(width, { -1, -1 }));
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<Point> q;

    q.push(start);
    visited[start.r][start.c] = true;

    bool found = false;
    while (!q.empty()) {
        Point curr = q.front();
        q.pop();

        if (curr.r == end.r && curr.c == end.c) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            if (!cells[curr.r][curr.c].walls[i]) {
                int nr = curr.r + dr[i];
                int nc = curr.c + dc[i];

                // Da sua: Dat dieu kien kiem tra bien len truoc de tranh loi SegFault
                if (nr >= 0 && nr < height && nc >= 0 && nc < width) {
                    if (!visited[nr][nc]) {
                        visited[nr][nc] = true;
                        parent[nr][nc] = curr;
                        q.push({ nr, nc });
                    }
                }
            }
        }
    }

    if (!found) return -1;

    int steps = 0;
    Point curr = end;
    vector<vector<char>> tempGrid = grid;

    while (!(curr.r == start.r && curr.c == start.c)) {
        int gr = curr.r * 2 + 1;
        int gc = curr.c * 2 + 1;
        if (!(curr.r == end.r && curr.c == end.c)) tempGrid[gr][gc] = '.';

        Point p = parent[curr.r][curr.c];
        int midR = (curr.r + p.r) * 2 + 1;
        int midC = (curr.c + p.c) * 2 + 1;
        tempGrid[midR][midC] = '.';

        curr = p;
        steps++;
    }

    if (visualize) {
        system("clear || cls");
        cout << "=== DUONG DI NGAN NHAT DUOC GIAI BANG BFS (Dau '.') ===\n\n";
        for (int r = 0; r < renderH; ++r) {
            for (int c = 0; c < renderW; ++c) cout << tempGrid[r][c] << " ";
            cout << "\n";
        }
        cout << "\n Do dai duong di cua BFS: " << steps << " buoc.\n";
        cout << "Nhan phim bat ky de tiep tuc.";
        getKeyStroke();
    }
    return steps;
}

// Tim duong bang DFS (Hien thi Backtrack)
int MazeGame::solveDFS(bool visualize) {
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    stack<Point> st;
    vector<Point> path;

    st.push(start);

    while (!st.empty()) {
        Point curr = st.top();

        if (!visited[curr.r][curr.c]) {
            visited[curr.r][curr.c] = true;
            path.push_back(curr);
        }

        if (curr.r == end.r && curr.c == end.c) break;

        bool hasNext = false;
        for (int i = 0; i < 4; ++i) {
            if (!cells[curr.r][curr.c].walls[i]) {
                int nr = curr.r + dr[i];
                int nc = curr.c + dc[i];
                if (nr >= 0 && nr < height && nc >= 0 && nc < width && !visited[nr][nc]) {
                    st.push({ nr, nc });
                    hasNext = true;
                    break;
                }
            }
        }

        if (!hasNext) {
            st.pop();
            if (!path.empty()) path.pop_back();
        }
    }

    vector<vector<char>> tempGrid = grid;
    for (size_t i = 0; i < path.size(); ++i) {
        int gr = path[i].r * 2 + 1;
        int gc = path[i].c * 2 + 1;
        if (i > 0 && i < path.size() - 1) tempGrid[gr][gc] = 'x';

        if (i < path.size() - 1) {
            int midR = (path[i].r + path[i + 1].r) * 2 + 1;
            int midC = (path[i].c + path[i + 1].c) * 2 + 1;
            tempGrid[midR][midC] = 'x';
        }
    }

    int steps = path.size() - 1;

    if (visualize) {
        system("clear || cls");
        cout << "=== DUONG DI DUOC GIAI BANG DFS (Dau 'x') ===\n\n";
        for (int r = 0; r < renderH; ++r) {
            for (int c = 0; c < renderW; ++c) cout << tempGrid[r][c] << " ";
            cout << "\n";
        }
        cout << "\nDo dai duong di tim thay boi DFS: " << steps << " buoc.\n";
        cout << "Nhan phim bat ky de tiep tuc.";
        getKeyStroke();
    }
    return steps;
}

// So sanh hieu nang BFS vs DFS
void MazeGame::compareAlgorithms() {
    int bfsSteps = solveBFS(false);
    int dfsSteps = solveDFS(false);

    system("clear || cls");
    cout << "============= BANG SO SANH: BFS vs DFS =============\n\n";
    cout << "  So buoc duong di ngan nhat (BFS): " << bfsSteps << " buoc.\n";
    cout << "  So buoc duong di dau tien tim thay (DFS): " << dfsSteps << " buoc.\n\n";

    if (bfsSteps < dfsSteps) {
        cout << "  Ket luan: BFS toi uu hon DFS " << (dfsSteps - bfsSteps) << " buoc di chuyen.\n";
    }
    else {
        cout << " Ket luan: Do dai duong di cua 2 thuat toan bang nhau.\n";
    }
    cout << "\nNhan phim bat ky de quay lai Menu.";
    getKeyStroke();
}