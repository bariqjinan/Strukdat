#include <iostream>
#include <windows.h>
#include <string.h>
#include "MMSystem.h"
#include <winnt.h>

using namespace std;

struct lagu
{
    string judul;
    lagu *next;
};

typedef lagu *pointer;

int main()
{
    pointer first = new lagu;

    string ext = ".wav";

    cout << "Masukkan Judul Lagu: " << endl;
    getline(cin, first->judul);

    cout << "asjdsa" << endl;

    string judul = first->judul + ext;

    cout << first->judul << endl;

    cout << " Di atas awan \n ";
    cout << " BBBBBBB\n";
    cout << "C";
    cout << "D";
    cout << 3 + 9 << endl;
    PlaySoundA(judul.c_str(), NULL, SND_FILENAME | SND_SYNC);
    system("pause");
    return 0;
}
