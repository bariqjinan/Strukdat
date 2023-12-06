#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    // ini formatnya LokasiFile/Judul.tipeFile
    char file_link[]="D:/DATA AISHA/UNPAD/SEMESTER 3(B)/Struktur Data/PROGRAM/kelompok 1 hhe/Mawar Jingga.wav";
    if(remove(file_link)==0)
    {
        cout << " file sudah terhapus ";
    }
    else
    {
        cout << "yah gagal ";
    }
    return 0;
}
