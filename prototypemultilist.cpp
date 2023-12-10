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

struct playlist
{
    string nama;
    lagu *laguHead;
    playlist *next;
    playlist *prev;
};

typedef lagu *pointer_lagu;
typedef playlist *pointer_playlist;

void download(string ur, string judul, playlist *currentPlaylist)
{
    string url = ur;
    string name = loc + currentPlaylist->nama + "\\" + judul + ext;
    const char *convu = url.c_str();
    if (S_OK == URLDownloadToFileA(NULL, url.c_str(), name.c_str(), 0, NULL))
    {
        cout << "Berhasil Ditambahkan ke Playlist " << currentPlaylist->nama << endl;
        // Add the song to the playlist's linked list
        lagu *pBaru = new lagu;
        pBaru->judul = judul;
        pBaru->next = NULL;
        pBaru->prev = NULL;

        if (currentPlaylist->laguHead == NULL)
        {
            currentPlaylist->laguHead = pBaru;
        }
        else
        {
            lagu *last = currentPlaylist->laguHead;
            while (last->next != NULL)
            {
                last = last->next;
            }
            last->next = pBaru;
            pBaru->prev = last;
        }
    }
    else
    {
        cout << "Gagal" << endl;
    }
}

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
    _mkdir(playlistDir.c_str());
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

void addSongToPlaylist(pointer_playlist currentPlaylist)
{
    cin.ignore();
    string folderPath;
    cout << "Masukkan path folder lagu: ";
    getline(cin, folderPath);

    cout << "Pilih lagu yang ingin ditambahkan ke playlist (input nomor lagu):" << endl;
    displaySongsFromFolder(folderPath);

    int selectedSong;
    cout << "Pilihan: ";
    cin >> selectedSong;

    DIR *dir;
    struct dirent *entry;

    dir = opendir(folderPath.c_str());
    if (dir == nullptr)
    {
        perror("Failed to open directory");
        return;
    }

    int i = 1;
    while ((entry = readdir(dir)) != nullptr)
    {
        string file_name = entry->d_name;

        // Check if the file has a .wav extension
        if (file_name.size() >= 4 &&
            file_name.substr(file_name.size() - 4) == ".wav")
        {
            if (i == selectedSong)
            {
                file_name = file_name.substr(0, file_name.size() - 4);
                download((folderPath + "\\" + entry->d_name), file_name, currentPlaylist);
                break;
            }
            i++;
        }
    }

    closedir(dir);
}

int main()
{
    pointer_playlist firstPlaylist = NULL;
    pointer_playlist currentPlaylist = NULL;

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
        cout << "4. Masukkan lagu ke playlist" << endl;
        cout << "5. Masukan Antrian" << endl;
        cout << "6. Tampilkan lagu" << endl;
        cout << "7. Hapus lagu pengguna" << endl;
        cout << "8. Hapus Antrian lagu" << endl;
        cout << "9. Stop lagu pengguna" << endl;
        cout << "10. Delete Playlist" << endl;
        cout << "11.Keluar Program" << endl;
        cout << "Masukkan Pilihan : ";
        cin >> pilih;

        switch (pilih)
        {
        case 1:
        {
            if (currentPlaylist == NULL)
            {
                cout << "Anda belum memilih playlist. Pilih playlist terlebih dahulu." << endl;
                break;
            }

            cin.ignore();
            string web, lagu;
            cout << "Mau Lagu apa?";
            getline(cin, lagu);
            cout << lagu << endl;

            cout << "Masukkin URL-nya";
            getline(cin, web);
            cout << web << endl;
            download(web, lagu, currentPlaylist);
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
        }
        break;
        case 3:
            // Existing code for playing a song
            break;
        case 4:
            addSongToPlaylist(currentPlaylist);
            break;
        case 5:
            break;
        case 6:
            displayPlaylists(firstPlaylist);
            break;
        case 7:
            // Placeholder for adding more features
            break;
        case 8:
            // Placeholder for adding more features
            break;
        case 9:
            // Placeholder for adding more features
            break;
        case 10:
            // Placeholder for adding more features
            break;
        case 11:
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
