#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include "MMSystem.h"
#include <winnt.h>
#include <pthread.h>

using namespace std;

string loc = "Musik\\";
string ext = ".wav";
struct lagu
{
    string judul;
    lagu *next;
    lagu *prev;
};
void download(string ur, string judul)
{
    string url = ur;
    string name = loc + judul + ext;
    const char *convu = url.c_str();
    if (S_OK == URLDownloadToFileA(NULL, url.c_str(), name.c_str(), 0, NULL))
    {
        cout << "Berhasil Ditambahkan" << endl;
    }
    else
    {
        cout << "Gagal" << endl;
    }
}

typedef lagu *pointer_lagu;
void insertLast(pointer_lagu &first, pointer_lagu &pBaru)
{
    pointer_lagu last;
    if (first == NULL)
    {
        first = pBaru;
    }
    else
    {
        last = first;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = pBaru;
        pBaru->prev = last;
    }
}

void readDirectory()
{
    // pointer_lagu first = NULL, pBaru;
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

        string file_name = entry->d_name;

        // Check if the file has a .wav extension
        if (file_name.size() >= 4 &&
            file_name.substr(file_name.size() - 4) == ".wav")
        {
            // pBaru->prev = NULL;
            file_name = file_name.substr(0, file_name.size() - 4);
            cout << i + 1 << ". " << file_name << endl;

            // pBaru->judul = file_name;
            // if (first == NULL)
            // {
            //     first = pBaru;
            // }
            // else
            // {
            //     insertLast(first, pBaru);
            // }
            // pBaru = pBaru->next;
            i++;
        }
    }
    closedir(dir);
    // pBaru->next = NULL;
}

int main()
{

    pointer_lagu first = new lagu, pBaru, pCari, pHapus;

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
        {
            cin.ignore();
            string web, lagu;
            cout << "Mau Lagu apa?";
            getline(cin, lagu);
            cout << lagu << endl;
            cout << "Masukkin URL-nya";
            getline(cin, web);
            cout << web << endl;
            download(web, lagu);
        }
        break;
        case 2:
            break;
        case 3:
        {
            cin.ignore();
            cout << "Masukkan Judul Lagu: " << endl;
            getline(cin, first->judul);

            string Judul = loc + first->judul + ext;
            cout << Judul << endl;
            PlaySoundA(Judul.c_str(), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 4:
            break;
        case 5:
        {
            readDirectory();
        }
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
