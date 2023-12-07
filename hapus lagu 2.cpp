#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main() {
    cout << "Masukkan judul lagu yang akan dihapus (tanpa ekstensi): ";
    string judul;
    getline(cin, judul);

    string fileName = judul + ".wav";

    char currentPath[FILENAME_MAX];
    getcwd(currentPath, sizeof(currentPath));

    chdir(currentPath);

    if (remove(fileName.c_str()) == 0) {
        cout << "File berhasil dihapus.\n";
    } else {
        cerr << "Error dalam menghapus file: " << fileName << "\n";
    }

    return 0;
}
