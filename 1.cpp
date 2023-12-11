#include <iostream>
#include <iomanip>
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
struct playlist
{
    string nama;
    lagu *laguHead;
    playlist *next;
    playlist *prev;
};
typedef playlist *pointer_playlist;
void createPlaylist(pointer_playlist &firstPlaylist, string playlistName)
{
    pointer_playlist newPlaylist = new playlist;
    newPlaylist->nama = playlistName;
    newPlaylist->laguHead = NULL;
    newPlaylist->next = NULL;
    newPlaylist->prev = NULL;

    if (firstPlaylist == NULL)
    {
        firstPlaylist = newPlaylist;
    }
    else
    {
        pointer_playlist last = firstPlaylist;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = newPlaylist;
        newPlaylist->prev = last;
    }

    // Create a directory for the new playlist
    string playlistDir = loc + playlistName;
    mkdir(playlistDir.c_str());
}
void displayPlaylists(pointer_playlist firstPlaylist)
{
    cout << "Daftar Playlist:" << endl;
    pointer_playlist current = firstPlaylist;
    int i = 1;
    while (current != NULL)
    {
        cout << i << ". " << current->nama << endl;
        current = current->next;
        i++;
    }
}

void bacaPlaylist()
{
    const char *folderPath = "Musik\\";

    // Buka direktori
    DIR *dir = opendir(folderPath);

    // Periksa apakah direktori berhasil dibuka
    if (dir)
    {
        cout << "Daftar Direktori di " << folderPath << ":\n";

        // Baca setiap entri dalam direktori
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            // Abaikan . dan ..
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            // Bangun path lengkap untuk entri
            string fullPath = folderPath;
            fullPath += "/";
            fullPath += entry->d_name;

            // Dapatkan informasi status file menggunakan stat
            struct stat fileStat;
            if (stat(fullPath.c_str(), &fileStat) == 0)
            {
                // Cek apakah entri adalah direktori dan dapat diakses (readable)
                if (S_ISDIR(fileStat.st_mode) && access(fullPath.c_str(), R_OK) == 0)
                {
                    // Cetak nama entri
                    cout << entry->d_name << "\n";
                }
            }
            else
            {
                cerr << "Gagal mendapatkan informasi status untuk " << entry->d_name << "\n";
            }
        }

        // Tutup direktori setelah selesai membaca
        closedir(dir);
    }
    else
    {
        // Tampilkan pesan kesalahan jika gagal membuka direktori
        std::cerr << "Gagal membuka direktori " << folderPath << "\n";
        // Keluar dengan kode error
    }
}
void displaySongsFromFolder(string folderPath)
{
    DIR *dir;
    struct dirent *entry;
    struct stat sb;

    dir = opendir(folderPath.c_str());
    if (dir == nullptr)
    {
        perror("Failed to open directory");
    }

    int i = 1;
    while ((entry = readdir(dir)) != nullptr)
    {
        string file_name = entry->d_name;

        // Check if the file has a .wav extension
        if (file_name.size() >= 4 &&
            file_name.substr(file_name.size() - 4) == ".wav")
        {
            file_name = file_name.substr(0, file_name.size() - 4);
            cout << i << ". " << file_name << endl;
            i++;
        }
    }

    closedir(dir);
}

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
void insertLastLagu(pointer_lagu &first, pointer_lagu &pBaru)
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
    pBaru->next = NULL;
}

void traversalLagu(pointer_lagu first)
{
    int i = 1;
    system("cls");
    pointer_lagu pBantu = first;
    if (pBantu == NULL)
    {
        cout << "Tidak ada data mahasiswa." << endl;
    }
    else
    {
        cout << "=================="
             << " LAGU "
             << "==================" << endl;
        cout << setfill('=') << setw(70) << "" << setfill(' ') << endl;
        cout << left << setw(20) << "JUDUL LAGU" << setw(30) << endl;
        cout << setfill('=') << setw(70) << "" << setfill(' ') << endl;

        while (pBantu != NULL)
        {
            cout << i << ". " << pBantu->judul << endl;
            pBantu = pBantu->next;
            i++;
        }
    }
}

void laguInit(pointer_lagu &first)
{
    pointer_lagu pBaru;
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
            pBaru = new lagu;
            pBaru->prev = NULL;
            file_name = file_name.substr(0, file_name.size() - 4);
            // cout << i + 1 << ". " << file_name << endl;

            pBaru->judul = file_name;
            if (first == NULL)
            {
                first = pBaru;
                cout << "cek " << endl;
            }
            else
            {
                insertLastLagu(first, pBaru);
                cout << "cek" << endl;
            }
            // cout << "cek " << endl;
            pBaru = pBaru->next;
            i++;
        }
    }
    closedir(dir);
    // cout << "cek" << endl;
    // pBaru->next = NULL;
}
// void playlistInit(pointer)

int main()
{

    pointer_lagu first = NULL, pBaru, pCari, pHapus;
    pointer_playlist firstPlaylist = NULL;
    pointer_playlist currentPlaylist = NULL;
    laguInit(first);

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
            cout << "Mau Lagu apa? ";
            getline(cin, lagu);
            cout << lagu << endl;
            cout << "Masukkin URL-nya: ";
            getline(cin, web);
            cout << web << endl;
            download(web, lagu);
        }
        break;
        case 2:
        {
            cin.ignore();
            string playlistName;
            cout << "Masukkan Nama Playlist Baru: ";
            getline(cin, playlistName);

            createPlaylist(firstPlaylist, playlistName);
            cout << "Playlist " << playlistName << " berhasil dibuat." << endl;

            break;
        }
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
            traversalLagu(first);
            bacaPlaylist();
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
