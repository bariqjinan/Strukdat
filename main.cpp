#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include "MMSystem.h"
#include <cstdio>
#include <filesystem>
#include <thread>
#include "Func.h"

using namespace std;

int main()
{
    string judul_lagu;
    int found;
    pointer_lagu first = NULL, pBaru, pCari, pHapus;
    pointer_playlist firstPlaylist = NULL;
    pointer_playlist currentPlaylist = NULL;
    pointer_queue qBaru, qHapus;
    createQueue(Q);
    laguInit(first);
    playlistInit(firstPlaylist);
    int pilih;

    const char *greenBackground = "\033[42m";
    const char *blackText = "\033[30m";
    const char *reset = "\033[0m";

    // Set warna untuk keseluruhan output
    cout << greenBackground << blackText;
    pointer_playlist playlistDiputar = NULL;

    do
    {
        system("cls");
        displayQueueInMenu(Q);
        cout << "|======================|" << endl;
        cout << "| Program Pemutar Lagu |" << endl;
        cout << "|======================|" << endl;
        cout << "Pilihan Menu" << endl;
        cout << "1. Tambah lagu" << endl;
        cout << "2. Tambah playlist" << endl;
        cout << "3. Putar lagu apa?" << endl;
        cout << "4. Tambahkan lagu ke playlist" << endl;
        cout << "5. Masukan Antrian" << endl;
        cout << "6. Tampilkan lagu dan playlist" << endl;
        cout << "7. Putar lagu di Antrian" << endl;
        cout << "8. Hapus lagu pengguna" << endl;
        cout << "9. Hapus Antrian lagu" << endl;
        cout << "10. Stop lagu pengguna" << endl;
        cout << "11. Hapus Playlist" << endl;
        cout << "12.Keluar Program" << endl;
        cout << "Masukkan Pilihan : ";
        cin >> pilih;
        switch (pilih)
        {
        case 1:
        {
            pBaru = new lagu;
            cin.ignore();
            string web, lagu;
            cout << "Lagu Apa yang Ingin Ditambahkan? ";
            getline(cin, pBaru->judul);
            cout << pBaru->judul << endl;
            cout << "Masukkan URL-nya : ";
            getline(cin, web);
            cout << web << endl;
            download(web, pBaru->judul, first, pBaru);
            system("pause");
            system("cls");
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
            system("pause");
            system("cls");
        }
        break;
        case 3:
        {
            traversalLagu(first);
            displayPlaylists(firstPlaylist, first);
            cin.ignore();
            string inputJudul;
            cout << "Masukkan Judul Lagu atau Nama Playlist: ";
            getline(cin, inputJudul);

            pointer_playlist foundPlaylist = NULL;
            cariplaylist(firstPlaylist, inputJudul, foundPlaylist);

            if (foundPlaylist != NULL)
            {
                playPlaylist(foundPlaylist);
            }
            else
            {
                pointer_lagu pCari;
                int found;
                cari(first, inputJudul, found, pCari);

                if (found)
                {
                    cout << "Lagu " << inputJudul << " ditemukan." << endl;
                    cout << "Memutar lagu " << inputJudul << endl;

                    string Judul = loc + pCari->judul + ext;
                    putarLagu(Judul);
                }
                else
                {
                    cout << "Lagu atau Playlist dengan judul " << inputJudul << " tidak ditemukan." << endl;
                }
            }
            break;
        }

        case 4:
        {
            cin.ignore();
            string playlistName;
            traversalLagu(first);
            displayPlaylists(firstPlaylist, first);
            cout << "Masukkan Nama Playlist: ";
            getline(cin, playlistName);

            // Cari playlist yang diinginkan
            pointer_playlist pPlaylist = firstPlaylist;
            while (pPlaylist != NULL && strcmp(pPlaylist->nama.c_str(), playlistName.c_str()) != 0)

            {
                pPlaylist = pPlaylist->next;
            }

            if (pPlaylist != NULL)
            {
                currentPlaylist = pPlaylist;
                cout << "Playlist " << playlistName << " dipilih." << endl;

                // Tambahkan lagu ke dalam playlist
                cout << "Masukkan Judul Lagu yang ingin ditambahkan: ";
                getline(cin, judul_lagu);
                tambahLaguKePlaylist(first, currentPlaylist, judul_lagu);
            }
            else
            {
                cout << "Playlist " << playlistName << " tidak ditemukan." << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 5:
        {
            traversalLagu(first);
            cin.ignore();
            cout << "Masukkan Judul Lagu yang akan diputar nanti: ";
            getline(cin, judul_lagu);
            cari(first, judul_lagu, found, pCari);
            if (found)
            {
                cout << "Lagu " << judul_lagu << " ditemukan." << endl;
                cout << "Memasukkan " << judul_lagu << " ke antrian." << endl;
                string Judul = loc + pCari->judul + ext;
                createElement(qBaru, judul_lagu);

                insertQueue(Q, qBaru);
            }
            system("pause");
            system("cls");
            break;
        }
        case 6:
        {
            traversalLagu(first);
            displayPlaylists(firstPlaylist, first);
            system("pause");
            system("cls");
            break;
        }
        case 7:
            playNextFromQueue(Q);
            system("pause");
            system("cls");
            break;
        case 8:
        {
            int pil2;

            cout << "|======================|" << endl;
            cout << "|      HAPUS LAGU      |" << endl;
            cout << "|======================|" << endl;
            cout << "1. Hapus dari Lokal" << endl;
            cout << "2. Hapus dari Playlist" << endl;
            cout << "Masukkan Pilihan Anda: ";
            cin >> pil2;
            switch (pil2)
            {
            case 1:
            {

                traversalLagu(first);
                cout << "Masukkan Lagu yang ingin dihapus:" << endl;
                cin.ignore();
                getline(cin, judul_lagu);
                hapusLagu(first, firstPlaylist, judul_lagu);
                system("pause");
                system("cls");
                break;
            }
            case 2:
            {

                cin.ignore();
                string playlistName;
                displayPlaylists(firstPlaylist, first);
                cout << "Masukkan Nama Playlist: ";
                getline(cin, playlistName);

                // Cari playlist yang diinginkan
                pointer_playlist pPlaylist = firstPlaylist;
                while (pPlaylist != NULL && pPlaylist->nama != playlistName)
                {
                    pPlaylist = pPlaylist->next;
                }

                if (pPlaylist != NULL)
                {
                    currentPlaylist = pPlaylist;
                    cout << "Playlist " << playlistName << " dipilih." << endl;

                    // Tampilkan lagu dalam playlist sebelum penghapusan
                    cout << "Daftar Lagu dalam Playlist " << currentPlaylist->nama << " sebelum penghapusan:" << endl;
                    pointer_lagu pLaguSebelum = currentPlaylist->laguHead;
                    int i = 1;
                    while (pLaguSebelum != NULL)
                    {
                        cout << i << ". " << pLaguSebelum->judul << endl;
                        pLaguSebelum = pLaguSebelum->next;
                        i++;
                    }

                    // Hapus lagu dari playlist
                    cout << "Masukkan Judul Lagu yang ingin dihapus: ";
                    getline(cin, judul_lagu);
                    hapusLaguDariPlaylist(first, currentPlaylist, judul_lagu);
                }
                else
                {
                    cout << "Playlist " << playlistName << " tidak ditemukan." << endl;
                }

                system("pause");
                system("cls");
                break;
            }
            default:
                cout << "Pilihan tidak tersedia!" << endl;
                break;
            }
            break;
        }
        case 9:
            cout << Q.head->music << " dihapus dari antrean" << endl;
            deleteQueue(Q, qHapus);
            system("pause");
            system("cls");
            break;
        case 10:
            stopLagu();
            break;
        case 11:
        {
            cin.ignore();
            string playlistName;
            displayPlaylists(firstPlaylist, first);
            cout << "Masukkan Nama Playlist yang ingin dihapus: ";
            getline(cin, playlistName);

            // Cari playlist yang diinginkan
            pointer_playlist pPlaylist = firstPlaylist;
            while (pPlaylist != NULL && pPlaylist->nama != playlistName)
            {
                pPlaylist = pPlaylist->next;
            }

            if (pPlaylist != NULL)
            {
                // Hapus folder playlist
                deletePlaylistFolder(playlistName, firstPlaylist);

                cout << "Playlist " << playlistName << " berhasil dihapus." << endl;
            }
            else
            {
                cout << "Playlist " << playlistName << " tidak ditemukan." << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 12:
            cout << "Program berhenti" << endl;
            cout << "Terima Kasih" << endl;
            break;
        default:
            cout << "Maaf pilihan Menu tidak tersedia" << endl;
            break;
        }
    } while (pilih != 12);
    cout << reset;
    return 0;
}
