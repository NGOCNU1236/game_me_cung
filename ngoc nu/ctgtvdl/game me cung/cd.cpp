
#include "kb.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int size = 10; // Kich thuoc mac dinh
    MazeGame maze(size, size);
    maze.generateMaze();

    while (true) {
        system("clear || cls");
        cout << "================== GAME ME CUNG CLI ==================\n";
        cout << "  Do kho / Kich thuoc hien tai: " << size << " x " << size << "\n\n";
        cout << " 1. Tao me cung ngau nhien moi\n";
        cout << " 2. Hien thi me cung hien tai (ASCII)\n";
        cout << " 3. Tu choi (Di chuyen tu do bang nut W/A/S/D)\n";
        cout << " 4. Tu dong giai bang BFS (Tim duong ngan nhat)\n";
        cout << " 5. Tu dong giai bang DFS (Hien thi vet tich luy)\n";
        cout << " 6. So sanh chi tiet thuat toan: BFS vs DFS\n";
        cout << " 7. Dieu chinh do kho (Thay doi kich thuoc)\n";
        cout << " 0. Thoat tro choi\n";
        cout << "------------------------------------------------------\n";
        cout << "Nhap lua chon cua van (0 - 7): ";

        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            continue;
        }

        if (choice == 0) {
            cout << "\nCam on ban da choi game! Tam biet!\n";
            break;
        }

        switch (choice) {
        case 1:
            maze = MazeGame(size, size);
            maze.generateMaze();
            cout << "\n Da khoi tao xong me cung moi thanh cong! Nhan phim bat ky.";
            getKeyStroke();
            break;
        case 2:
            system("clear || cls");
            cout << "=== BAN DO ME CUNG HIEN TAI (S: Bat dau, E: Loi ra) ===\n\n";
            maze.display({ 0, 0 });
            cout << "\nNhan phim bat ky de quay lai Menu.";
            getKeyStroke();
            break;
        case 3:
            maze.playManual();
            break;
        case 4:
            maze.solveBFS(true);
            break;
        case 5:
            maze.solveDFS(true);
            break;
        case 6:
            maze.compareAlgorithms();
            break;
        case 7:
            cout << "\nNhap kich thuoc moi cho me cung (Khuyen dung tu 5 den 20): ";
            cin >> size;
            if (cin.fail() || size < 3) {
                size = 10;
                cout << "Kich thuoc khong hop le. Da tu dong doi ve 10x10.\n";
                clearInputBuffer();
            }
            maze = MazeGame(size, size);
            maze.generateMaze();
            cout << "Da ap dung kich thuoc moi! Nhan phim bat ky de tiep tuc.";
            getKeyStroke();
            break;
        default:
            break;
        }
    }
    return 0;
}