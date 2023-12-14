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
#include <thread>

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
void sortByAlphabet(pointer_playlist &firstPlaylist)
{
    if (firstPlaylist == NULL || firstPlaylist->next == NULL)
    {
        // Tidak perlu mengurutkan jika playlist kosong atau hanya berisi satu elemen
        return;
    }

    pointer_playlist sorted = NULL;
    pointer_playlist current = firstPlaylist;

    while (current != NULL)
    {
        pointer_playlist next = current->next;

        if (sorted == NULL || sorted->nama.compare(current->nama) > 0)
        {
            // Sisipkan di awal jika sorted kosong atau nama current kurang dari sorted
            current->next = sorted;
            current->prev = NULL;
            if (sorted != NULL)
            {
                sorted->prev = current;
            }
            sorted = current;
        }
        else
        {
            // Cari tempat yang tepat untuk menyisipkan jika sorted tidak kosong
            pointer_playlist temp = sorted;
            while (temp->next != NULL && temp->next->nama.compare(current->nama) < 0)
            {
                temp = temp->next;
            }
            current->next = temp->next;
            if (temp->next != NULL)
            {
                temp->next->prev = current;
            }
            temp->next = current;
            current->prev = temp;
        }

        current = next;
    }

    // Update firstPlaylist menjadi sorted
    firstPlaylist = sorted;
}

void displayPlaylists(pointer_playlist firstPlaylist)
{
    // Sebelum menampilkan, urutkan playlist berdasarkan urutan abjad
    sortByAlphabet(firstPlaylist);

    cout << "Daftar Playlist : " << endl;
    pointer_playlist current = firstPlaylist;
    int i = 1;
    while (current != NULL)
    {
        cout << i << ". " << current->nama << endl;
        current = current->next;
        i++;
    }
}

// void bacaPlaylist()
// {
//     const char *folderPath = "Musik\\";

//     // Buka direktori
//     DIR *dir = opendir(folderPath);

//     // Periksa apakah direktori berhasil dibuka
//     if (dir)
//     {
//         cout << "Daftar Direktori di " << folderPath << ":\n";

//         // Baca setiap entri dalam direktori
//         struct dirent *entry;
//         while ((entry = readdir(dir)) != nullptr)
//         {
//             // Abaikan . dan ..
//             if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//             {
//                 continue;
//             }

//             // Bangun path lengkap untuk entri
//             string fullPath = folderPath;
//             fullPath += "/";
//             fullPath += entry->d_name;

//             // Dapatkan informasi status file menggunakan stat
//             struct stat fileStat;
//             if (stat(fullPath.c_str(), &fileStat) == 0)
//             {
//                 // Cek apakah entri adalah direktori dan dapat diakses (readable)
//                 if (S_ISDIR(fileStat.st_mode) && access(fullPath.c_str(), R_OK) == 0)
//                 {
//                     // Cetak nama entri
//                     cout << entry->d_name << "\n";
//                 }
//             }
//             else
//             {
//                 cerr << "Gagal mendapatkan informasi status untuk " << entry->d_name << "\n";
//             }
//         }

//         // Tutup direktori setelah selesai membaca
//         closedir(dir);
//     }
//     else
//     {
//         // Tampilkan pesan kesalahan jika gagal membuka direktori
//         std::cerr << "Gagal membuka direktori " << folderPath << "\n";
//         // Keluar dengan kode error
//     }
// }
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

void download(string ur, string judul, pointer_lagu &first, pointer_lagu &pBaru)
{
    string url = ur;
    string name = loc + judul + ext;
    const char *convu = url.c_str();
    if (S_OK == URLDownloadToFileA(NULL, url.c_str(), name.c_str(), 0, NULL))
    {
        pBaru->next = NULL;
        pBaru->prev = NULL;
        cout << "Berhasil Ditambahkan" << endl;
        if (first == NULL)
        {
            first = pBaru;
        }
        else
        {
            insertLastLagu(first, pBaru);
        }
    }
    else
    {
        cout << "Gagal" << endl;
    }
}

void selectionSortLagu(pointer_lagu &first)
{
    pointer_lagu p = first;
    while (p != NULL)
    {
        pointer_lagu min = p;
        pointer_lagu q = p->next;
        while (q != NULL)
        {
            if (strcmp(q->judul.c_str(), min->judul.c_str()) < 0)
            {
                min = q;
            }
            q = q->next;
        }
        swap(p->judul, min->judul);
        p = p->next;
    }
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

        // Panggil fungsi selectionSortLagu sebelum melakukan traversal
        selectionSortLagu(first);

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
    // int i = 0;
    selectionSortLagu(first);
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
                // cout << "cek " << endl;
            }
            else
            {
                insertLastLagu(first, pBaru);
                // cout << "cek" << endl;
            }
            // cout << "cek " << endl;
            pBaru = pBaru->next;
            // i++;
        }
    }
    closedir(dir);
    // cout << "cek" << endl;
    // pBaru->next = NULL;
}

void cari(pointer_lagu first, string &judul_lagu, int &found, pointer_lagu &pCari)
{
    found = 0;
    pCari = first;

    while (found == 0 && pCari != NULL)
    {
        if (pCari->judul == judul_lagu)
        {
            found = 1;
        }
        else
        {
            pCari = pCari->next;
        }
    }
}

void cariplaylist(pointer_playlist firstPlaylist, string &playlistName, pointer_playlist &foundPlaylist)
{
    foundPlaylist = firstPlaylist;
    while (foundPlaylist != NULL && foundPlaylist->nama != playlistName)
    {
        foundPlaylist = foundPlaylist->next;
    }
}

void playlistInit(pointer_playlist &firstPlaylist)
{
    pointer_playlist pBaru;
    const char *folderPath = "Musik\\";

    // Buka direktori
    DIR *dir = opendir(folderPath);

    // Periksa apakah direktori berhasil dibuka
    if (dir)
    {
        // cout << "Daftar Direktori di " << folderPath << ":\n";

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
                    pBaru = new playlist;
                    pBaru->nama = entry->d_name;
                    pBaru->laguHead = NULL;
                    pBaru->prev = NULL;
                    pBaru->next = NULL;

                    if (firstPlaylist == NULL)
                    {
                        firstPlaylist = pBaru;
                        cout << "cek" << endl;
                    }
                    else
                    {
                        cout << "cek" << endl;
                        pointer_playlist last = firstPlaylist;
                        while (last->next != NULL)
                        {
                            last = last->next;
                        }
                        last->next = pBaru;
                        pBaru->prev = last;
                    }
                    // pBaru = pBaru->next;

                    // cout << entry->d_name << "\n";
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
        cerr << "Gagal membuka direktori " << folderPath << "\n";
        // Keluar dengan kode error
    }
}
void putarLagu(string Judul)
{

    PlaySoundA(Judul.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void tambahLaguKePlaylist(pointer_lagu &first, pointer_playlist &currentPlaylist, string judul_lagu)
{
    int found;
    pointer_lagu pCari;

    cari(first, judul_lagu, found, pCari);

    if (found)
    {
        cout << "Lagu " << judul_lagu << " ditemukan." << endl;

        pointer_lagu laguBaru = new lagu;
        laguBaru->judul = pCari->judul;
        laguBaru->next = NULL;
        laguBaru->prev = NULL;

        if (currentPlaylist->laguHead == NULL)
        {
            currentPlaylist->laguHead = laguBaru;
        }
        else
        {
            pointer_lagu last = currentPlaylist->laguHead;
            while (last->next != NULL)
            {
                last = last->next;
            }
            last->next = laguBaru;
            laguBaru->prev = last;
        }

        string sourcePath = loc + laguBaru->judul + ext;
        string destinationPath = loc + currentPlaylist->nama + "\\" + laguBaru->judul + ext;

        string playlistFolderPath = loc + currentPlaylist->nama;
        mkdir(playlistFolderPath.c_str());

        ifstream source(sourcePath, ios::binary);
        ofstream destination(destinationPath, ios::binary);

        if (!source || !destination)
        {
            cerr << "Error: Gagal membuka file untuk penyalinan." << endl;
            return;
        }

        destination << source.rdbuf();

        if (!destination)
        {
            cerr << "Error: Gagal menulis file tujuan setelah penyalinan." << endl;
            return;
        }

        cout << "Lagu " << judul_lagu << " berhasil ditambahkan ke dalam playlist " << currentPlaylist->nama << "." << endl;

        cout << "Daftar Lagu dalam Playlist " << currentPlaylist->nama << ":" << endl;
        pointer_lagu pLagu = currentPlaylist->laguHead;
        int i = 1;
        while (pLagu != NULL)
        {
            cout << i << ". " << pLagu->judul << endl;
            pLagu = pLagu->next;
            i++;
        }
    }
    else
    {
        cout << "Lagu " << judul_lagu << " tidak ditemukan." << endl;
    }
}

// ini keknya terlalu panjang
void playPlaylist(pointer_playlist foundPlaylist)
{
    if (foundPlaylist != NULL)
    {
        cout << "Playlist ditemukan: " << foundPlaylist->nama << endl;

        lagu *laguInPlaylist = foundPlaylist->laguHead;
        int laguIndex = 1;

        while (laguInPlaylist != NULL)
        {
            cout << laguIndex << ". Lagu dalam playlist: " << laguInPlaylist->judul << endl;
            laguInPlaylist = laguInPlaylist->next;
            laguIndex++;
        }

        if (foundPlaylist->laguHead != NULL)
        {
            lagu *laguDiputar = foundPlaylist->laguHead;

            while (laguDiputar != NULL)
            {
                string judulLagu = laguDiputar->judul;
                string filePath = loc + foundPlaylist->nama + "\\" + judulLagu + ext;

                cout << "Memutar lagu dari playlist: " << judulLagu << endl;

                if (PlaySoundA(filePath.c_str(), NULL, SND_FILENAME) == 0)
                {
                    cerr << "Error memutar suara. Kode kesalahan: " << GetLastError() << endl;
                }

                while (PlaySoundA(NULL, NULL, SND_ASYNC) == 0)
                {
                }

                laguDiputar = laguDiputar->next;
            }
        }
        else
        {
            cout << "Playlist kosong." << endl;
        }
    }
}

void hapusLaguDariPlaylist(pointer_lagu &first, pointer_playlist &currentPlaylist, string judul_lagu)
{
    if (currentPlaylist == NULL || currentPlaylist->laguHead == NULL)
    {
        cout << "Playlist kosong." << endl;
        return;
    }

    pointer_lagu pCari = currentPlaylist->laguHead;
    pointer_lagu pPrev = NULL;

    // Cari lagu yang ingin dihapus dalam playlist
    while (pCari != NULL && pCari->judul != judul_lagu)
    {
        pPrev = pCari;
        pCari = pCari->next;
    }

    if (pCari != NULL)
    {
        // Hapus lagu dari sistem file
        string filePath = loc + currentPlaylist->nama + "\\" + judul_lagu + ext;
        if (remove(filePath.c_str()) != 0)
        {
            cerr << "Error: Gagal menghapus file lagu dari sistem file." << endl;
            return;
        }

        // Hapus lagu dari linked list
        if (pPrev != NULL)
        {
            pPrev->next = pCari->next;
            if (pCari->next != NULL)
            {
                pCari->next->prev = pPrev;
            }
        }
        else
        {
            currentPlaylist->laguHead = pCari->next;
            if (pCari->next != NULL)
            {
                pCari->next->prev = NULL;
            }
        }

        // Hapus lagu dari memori
        delete pCari;

        cout << "Lagu " << judul_lagu << " berhasil dihapus dari playlist " << currentPlaylist->nama << "." << endl;

        // Tampilkan daftar lagu dalam playlist setelah penghapusan
        cout << "Daftar Lagu dalam Playlist " << currentPlaylist->nama << " setelah penghapusan:" << endl;
        pointer_lagu pLagu = currentPlaylist->laguHead;
        int i = 1;
        while (pLagu != NULL)
        {
            cout << i << ". " << pLagu->judul << endl;
            pLagu = pLagu->next;
            i++;
        }
    }
    else
    {
        cout << "Lagu " << judul_lagu << " tidak ditemukan dalam playlist." << endl;
    }
}

void deletePlaylistFolder(const string &playlistName)
{
    // Construct the path to the playlist folder
    string playlistFolderPath = "Musik\\" + playlistName;

    // Attempt to remove the playlist folder
    if (RemoveDirectoryA(playlistFolderPath.c_str()) != 0)
    {
        cout << "Playlist folder " << playlistName << " successfully deleted." << endl;
    }
    else
    {
        cerr << "Error: Failed to delete playlist folder " << playlistName << ". Error code: " << GetLastError() << endl;
    }
}

int main()
{
    string judul_lagu;
    int found;
    pointer_lagu first = NULL, pBaru, pCari, pHapus;
    pointer_playlist firstPlaylist = NULL;
    pointer_playlist currentPlaylist = NULL;
    laguInit(first);
    playlistInit(firstPlaylist);
    int pilih;

    pointer_playlist playlistDiputar = NULL;

    do
    {
        system("cls");
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
            pBaru = new lagu;
            cin.ignore();
            string web, lagu;
            cout << "Mau Lagu apa? ";
            getline(cin, pBaru->judul);
            cout << pBaru->judul << endl;

            cout << "Masukkin URL-nya: ";
            getline(cin, web);
            cout << web << endl;
            download(web, pBaru->judul, first, pBaru);
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
        {
            traversalLagu(first);
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
            displayPlaylists(firstPlaylist);
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

                // Tambahkan lagu ke dalam playlist
                cout << "Masukkan Judul Lagu yang ingin ditambahkan: ";
                getline(cin, judul_lagu);
                tambahLaguKePlaylist(first, currentPlaylist, judul_lagu);
            }
            else
            {
                cout << "Playlist " << playlistName << " tidak ditemukan." << endl;
            }
            break;
        }
        case 5:
            break;
        case 6:
        {
            traversalLagu(first);
            displayPlaylists(firstPlaylist);
            system("pause");
            system("cls");
            break;
        }
        case 7:
        {
            cin.ignore();
            string playlistName;
            displayPlaylists(firstPlaylist);
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
            break;
        }
        case 8:
            break;
        case 9:
            break;
        case 10:
        {
            cin.ignore();
            string playlistName;
            displayPlaylists(firstPlaylist);
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
                deletePlaylistFolder(playlistName);

                // Hapus playlist dari linked list
                if (pPlaylist->prev != NULL)
                {
                    pPlaylist->prev->next = pPlaylist->next;
                }
                else
                {
                    firstPlaylist = pPlaylist->next;
                }

                if (pPlaylist->next != NULL)
                {
                    pPlaylist->next->prev = pPlaylist->prev;
                }

                // Hapus playlist dari memori
                delete pPlaylist;

                cout << "Playlist " << playlistName << " berhasil dihapus." << endl;
            }
            else
            {
                cout << "Playlist " << playlistName << " tidak ditemukan." << endl;
            }
            break;
        }
        case 11:
            cout << "Program berhenti" << endl;
            cout << "Terima Kasih" << endl;
            break;
        default:
            cout << "Maaf pilihan Menu tidak tersedia" << endl;
            break;
        }
        // system("PAUSE");5

        // system("cls");
    } while (pilih != 11);
    return 0;
}