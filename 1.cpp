#include <iostream>
#include <windows.h>
#include "MMSystem.h"

using namespace std;

int main()
{
    cout << " Di atas awan \n ";
    cout << " BBBBBBB\n";
    cout << "C";
    cout << "D";
    cout << 3 + 9 << endl;
    PlaySound(TEXT("Mawar Jingga.wav"), NULL, SND_FILENAME | SND_SYNC);
    system("pause");
    return 0;
}
