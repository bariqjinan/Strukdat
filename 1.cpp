#include <iostream>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include "MMSystem.h"
#include <winnt.h>
#include <pthread.h>

using namespace std;

struct lagu
{
    string judul;
    lagu *next;
};

void readDirectory()
{
    DIR *dir;
    struct dirent *entry;
    struct stat sb;

    dir = opendir("Musik\\");
    if (dir == nullptr)
    {
        perror("Failed to open directory");
    }
    int i = 0;
    while ((entry = readdir(dir)) != nullptr)
    {
        // // Check if the entry is a file or directory
        // if (stat(entry->d_name, &sb) == -1)
        // {
        //     perror("Failed to get file information");
        //         }

        string file_name = entry->d_name;

        // Check if the file has a .wav extension
        if (file_name.size() >= 4 &&
            file_name.substr(file_name.size() - 4) == ".wav")
        {
            file_name = file_name.substr(0, file_name.size() - 4);
            cout << i + 1 << ". " << file_name << endl;
            i++;
        }

        // if (S_ISREG(sb.st_mode))
        // {
        //     cout << entry->d_name << " is a regular file." << endl;
        // }
        // else if (S_ISDIR(sb.st_mode))
        // {
        //     cout << entry->d_name << " is a directory." << endl;
        // }
        // else
        // {
        //     cout << entry->d_name << " is an unknown type." << endl;
        // }
    }

    closedir(dir);
}
typedef lagu *pointer;

int main()
{
    pointer first = new lagu;

    int pilih;

    do
    {
        cout << "|======================|" << endl;
        cout << "| Program Pemutar Lagu |" << endl;
        cout << "|======================|" << endl;
        cout << "Pilihan Menu" << endl;
        cout << "1. Tambah lagu" << endl;
        cout << "2. Tambah playlist" << endl;
        cout << "3. Putar lagu apa?" << endl;
        cout << "4. Masukan Antrian" << endl;
        cout << "5. Tampilkan lagu" << endl;
        cout << "6. Hapus lagu pengguna" << endl;
        cout << "7. Hapus Antrian lagu" << endl;
        cout << "8. Stop lagu pengguna" << endl;
        cout << "9. Delete Playlist" << endl;
        cout << "10.Keluar Program" << endl;
        cout << "Masukkan Pilihan : ";
        cin >> pilih;
        switch (pilih)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
        {
            cin.ignore();
            cout << "Masukkan Judul Lagu: " << endl;
            getline(cin, first->judul);
            string loc = "Musik\\";
            string ext = ".wav";

            string Judul = loc + first->judul + ext;
            cout << Judul << endl;
            PlaySoundA(Judul.c_str(), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 4:
            break;
        case 5:

            readDirectory();
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            cout << "Program berhenti" << endl;
            cout << "Terima Kasih" << endl;
            break;
        default:
            cout << "Maaf pilihan Menu tidak tersedia" << endl;
            break;
        }
        system("PAUSE");
        system("cls");
    } while (pilih != 10);
}
