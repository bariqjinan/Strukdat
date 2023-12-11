#include <iostream>
#include <windows.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include "MMSystem.h"
#include <pthread.h>
#include <direct.h>

using namespace std;

const string loc = "C:\\Users\\lenovo\\OneDrive\\Desktop\\Projek Struktur Data\\Musik\\";
const string ext = ".wav";

struct Lagu
{
    string judul;
    Lagu* next;
    Lagu* prev;
};

struct Playlist
{
    string nama;
    Lagu* laguHead;
    Playlist* next;
    Playlist* prev;
};

typedef Lagu* PointerLagu;
typedef Playlist* PointerPlaylist;

// Global playlist head
PointerPlaylist globalPlaylist = nullptr;

void download(const string& ur, const string& judul, Playlist* currentPlaylist)
{
    string url = ur;
    string name = loc + currentPlaylist->nama + "\\" + judul + ext;
    const char* convu = url.c_str();
    if (S_OK == URLDownloadToFileA(nullptr, url.c_str(), name.c_str(), 0, nullptr))
    {
        cout << "Berhasil Ditambahkan ke Playlist " << currentPlaylist->nama << endl;
        // Add the song to the playlist's linked list
        Lagu* pBaru = new Lagu;
        pBaru->judul = judul;
        pBaru->next = nullptr;
        pBaru->prev = nullptr;

        if (currentPlaylist->laguHead == nullptr)
        {
            currentPlaylist->laguHead = pBaru;
        }
        else
        {
            Lagu* last = currentPlaylist->laguHead;
            while (last->next != nullptr)
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

void createGlobalPlaylist()
{
    if (globalPlaylist == nullptr)
    {
        PointerPlaylist newPlaylist = new Playlist;
        newPlaylist->nama = "GlobalPlaylist";
        newPlaylist->laguHead = nullptr;
        newPlaylist->next = nullptr;
        newPlaylist->prev = nullptr;
        globalPlaylist = newPlaylist;

        // Create a directory for the global playlist
        string globalPlaylistDir = loc + newPlaylist->nama;
        mkdir(globalPlaylistDir.c_str());
    }
}

void createPlaylist(PointerPlaylist& firstPlaylist, const string& playlistName)
{
    createGlobalPlaylist(); // Ensure that globalPlaylist is created

    PointerPlaylist newPlaylist = new Playlist;
    newPlaylist->nama = playlistName;
    newPlaylist->laguHead = nullptr;
    newPlaylist->next = nullptr;
    newPlaylist->prev = nullptr;

    if (firstPlaylist == nullptr)
    {
        firstPlaylist = newPlaylist;
    }
    else
    {
        PointerPlaylist last = firstPlaylist;
        while (last->next != nullptr)
        {
            last = last->next;
        }
        last->next = newPlaylist;
        newPlaylist->prev = last;
    }

    // Create a directory for the new playlist
    string playlistDir = loc + playlistName;
    mkdir(playlistDir.c_str());

    // Add the new playlist to the global playlist
    PointerPlaylist lastGlobal = globalPlaylist;
    while (lastGlobal->next != nullptr)
    {
        lastGlobal = lastGlobal->next;
    }
    lastGlobal->next = newPlaylist;
    newPlaylist->prev = lastGlobal;
}

void displayPlaylists(PointerPlaylist firstPlaylist)
{
    cout << "Daftar Playlist:" << endl;
    PointerPlaylist current = globalPlaylist;
    int i = 1;
    while (current != nullptr)
    {
        cout << i << ". " << current->nama << endl;
        current = current->next;
        i++;
    }
}

void displaySongsFromFolder(const string& folderPath)
{
    DIR* dir;
    struct dirent* entry;
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

void addSongToPlaylist(PointerPlaylist currentPlaylist)
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

    DIR* dir;
    struct dirent* entry;

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
    PointerPlaylist firstPlaylist = nullptr;
    PointerPlaylist currentPlaylist = nullptr;

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
        cout << "11. Keluar Program" << endl;
        cout << "Masukkan Pilihan : ";
        cin >> pilih;

        switch (pilih)
        {
            case 1:
            {
                if (currentPlaylist == nullptr)
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
                break;
            }
            case 2:
            {
                cin.ignore();
                string playlistName;
                cout << "Masukkan Nama Playlist Baru: ";
                getline(cin, playlistName);
                createPlaylist(firstPlaylist, playlistName);
                cout << "Playlist " << playlistName << " berhasil dibuat." << endl;

                // Set the newly created playlist as the current playlist
                currentPlaylist = firstPlaylist; // You may want to set it to the newly created playlist specifically
                break;
            }

            case 3:
                // Existing code for playing a song
                break;

            case 4:
            {
                if (firstPlaylist == nullptr)
                {
                    cout << "Anda belum membuat playlist. Silakan buat playlist terlebih dahulu." << endl;
                    break;
                }

                    // Display available playlists
                displayPlaylists(firstPlaylist);

                int selectedPlaylistIndex;
                cout << "Pilih playlist untuk menambahkan lagu (input nomor playlist): ";
                cin >> selectedPlaylistIndex;

                // Find the selected playlist
                PointerPlaylist selectedPlaylist = firstPlaylist;
                for (int i = 1; i < selectedPlaylistIndex; ++i)
                {
                    if (selectedPlaylist == nullptr)
                    {
                        cout << "Playlist tidak ditemukan." << endl;
                        break;
                    }
                    selectedPlaylist = selectedPlaylist->next;
                }

                     // Set the selected playlist as the current playlist
                currentPlaylist = selectedPlaylist;

    // Check if the current playlist is valid
                if (currentPlaylist != nullptr)
                {
                    addSongToPlaylist(currentPlaylist);
                }
                else
                {
                    cout << "Error: currentPlaylist is not initialized." << endl;
                }
                break;
            }

            case 5:
                break;

            case 6:
            {
                displayPlaylists(firstPlaylist);
                break;
            }
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
            {
                cout << "Program berhenti" << endl;
                cout << "Terima Kasih" << endl;
                break;
            }
            default:
            {
                cout << "Maaf pilihan Menu tidak tersedia" << endl;
                break;
            }
        }

    } while (pilih != 11);

    // Cleanup: Free allocated memory
    PointerPlaylist temp = firstPlaylist;
    while (temp != nullptr)
    {
        PointerPlaylist next = temp->next;
        delete temp;
        temp = next;
    }

    // You may also need to free the memory allocated for Lagu nodes if needed.
    return 0;
}
