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
typedef pointer_playlist ListPlaylist;

struct ElemenQueue
{
    string music;
    ElemenQueue *next;
};

typedef ElemenQueue *pointer_queue;

struct Queue
{
    pointer_queue head, tail;
};

Queue Q;

void createQueue(Queue &Q)
{
    Q.head = NULL;
    Q.tail = NULL;
}

void putarLagu(string Judul)
{

    PlaySoundA(Judul.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void deleteQueue(Queue &Q, pointer_queue &pHapus)
{
    if (Q.head == NULL && Q.tail == NULL)
    {
        pHapus = NULL;
    }
    else if (Q.head->next == NULL)
    {
        pHapus = Q.head;
        Q.head = NULL;
        Q.tail = NULL;
    }
    else
    {
        pHapus = Q.head;
        Q.head = Q.head->next;
        pHapus->next = NULL;
    }

    // delete pHapus; // Deallocate memory
    // pHapus = NULL; // Set pointer to NULL
}


void playNextFromQueue(Queue &Q)
{
    if (Q.head != NULL)
    {
        pointer_queue pHapus;

        cout << "Memutar lagu dari antrian: " << Q.head->music << endl;
        string Judul = loc + Q.head->music + ext;
        // PlaySoundA(NULL, NULL, 0);

        putarLagu(Judul);
        // Create a new thread to play music asynchronously
        deleteQueue(Q, pHapus);

        // delete pHapus;
    }
    else
    {
        cout << "Antrian lagu kosong." << endl;

        // Setel isPlaying menjadi false saat antrian kosong
    }
}
void createElement(pointer_queue &qBaru, string music)
{
    qBaru = new ElemenQueue;
    // cout << "Data: ";
    // cin >> pBaru->data;
    qBaru->music = music;
    qBaru->next = NULL;
}

void insertQueue(Queue &Q, pointer_queue qBaru)
{
    if (Q.head == NULL && Q.tail == NULL)
    {

        Q.head = qBaru;
        Q.tail = qBaru;
    }
    else
    {

        Q.tail->next = qBaru;
        Q.tail = qBaru;
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

void displayQueueInMenu(Queue Q)
{
    pointer_queue qBantu = NULL;
    if (Q.head == NULL)
    {
        cout << "Antrean lagu kosong" << endl;
    }
    else
    {
        qBantu = Q.head;
        cout << "Lagu dalam antrean : " << endl;

        cout << qBantu->music << "\t";
        cout << endl;
    }
}
void displayQueue(Queue Q)
{
    pointer_queue qBantu;
    int i = 1;
    if (Q.head == NULL)
    {
        cout << "Antrean Lagu Kosong" << endl;
    }
    else
    {
        qBantu = Q.head;
        cout << "Daftar Lagu Dalam Antrean : " << endl;
        do
        {
            cout << i << ". " << qBantu->music << endl;
            qBantu = qBantu->next;
        } while (qBantu != Q.tail->next);
        cout << endl;
    }
}

void stopLagu()
{
    PlaySoundA(NULL, NULL, 0);
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

void displayPlaylists(pointer_playlist firstPlaylist, pointer_lagu first)
{
    // Sebelum menampilkan, urutkan playlist berdasarkan urutan abjad
    sortByAlphabet(firstPlaylist);
    selectionSortLagu(first);

    cout << "Daftar Playlist : " << endl;
    pointer_playlist currentPlaylist = firstPlaylist;
    pointer_lagu currentLagu;
    int i = 1;
    int j;

    while (currentPlaylist != NULL)
    {
        cout << i << ". " << currentPlaylist->nama << " :" << endl;
        currentLagu = currentPlaylist->laguHead;
        j = 1;

        while (currentLagu != NULL)
        {
            cout << "    " << j << ". " << currentLagu->judul << endl;
            currentLagu = currentLagu->next;
            j++;
        }

        currentPlaylist = currentPlaylist->next;
        i++;
    }

    cout << endl;
}

void displaySongsFromFolder(string folderPath)
{
    DIR *dir;
    struct dirent *entry;
    struct stat sb;

    dir = opendir(folderPath.c_str());
    if (dir == nullptr)
    {
        perror("Gagal Untuk Membuka Direktori");
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
        cout << "Lagu Berhasil Ditambahkan!" << endl;
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
        cout << "Lagu Gagal Untuk Ditambahkan" << endl;
    }
}

void traversalLagu(pointer_lagu first)
{
    int i = 1;
    system("cls");
    pointer_lagu pBantu = first;
    if (pBantu == NULL)
    {
        cout << "Tidak Ada Daftar Lagu" << endl;
    }
    else
    {
        cout << "=================="
             << " .DAFTAR LAGU "
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
        perror("Gagal Untuk Membuka Direktori");
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

void tambahLagu(pointer_playlist &pInduk, const string &namaLagu)
{
    // Pemeriksaan ekstensi file
    if (namaLagu.size() >= 4 && namaLagu.substr(namaLagu.size() - 4) == ".wav")
    {
        string file_name = namaLagu.substr(0, namaLagu.size() - 4);
        lagu *laguBaru = new lagu;
        laguBaru->judul = file_name;
        laguBaru->next = nullptr;
        laguBaru->prev = nullptr;
        cout << "cek" << endl;

        if (pInduk->laguHead == nullptr)
        {
            pInduk->laguHead = laguBaru;
        }
        else
        {
            lagu *lastLagu = pInduk->laguHead;
            while (lastLagu->next != nullptr)
            {
                lastLagu = lastLagu->next;
            }
            lastLagu->next = laguBaru;
            laguBaru->prev = lastLagu;
        }
    }
}

void playlistInit(pointer_playlist &firstPlaylist)
{
    const char *folderPath = "Musik\\";

    // Buka direktori
    DIR *dir = opendir(folderPath);

    // Periksa apakah direktori berhasil dibuka
    if (dir)
    {
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
                    pointer_playlist pBaru = new playlist;
                    pBaru->nama = entry->d_name;
                    pBaru->laguHead = nullptr;
                    pBaru->next = nullptr;
                    pBaru->prev = nullptr;

                    // Tambahkan playlist ke dalam multilist
                    if (firstPlaylist == nullptr)
                    {
                        firstPlaylist = pBaru;
                    }
                    else
                    {
                        pointer_playlist lastPlaylist = firstPlaylist;
                        while (lastPlaylist->next != nullptr)
                        {
                            lastPlaylist = lastPlaylist->next;
                        }
                        lastPlaylist->next = pBaru;
                        pBaru->prev = lastPlaylist;
                    }

                    // Baca lagu di dalam folder dan tambahkan ke playlist
                    DIR *laguDir = opendir(fullPath.c_str());
                    if (laguDir)
                    {
                        struct dirent *laguEntry;
                        while ((laguEntry = readdir(laguDir)) != nullptr)
                        {

                            if (strcmp(laguEntry->d_name, ".") != 0 && strcmp(laguEntry->d_name, "..") != 0)
                            {
                                tambahLagu(pBaru, laguEntry->d_name);
                            }
                        }
                        closedir(laguDir);
                    }
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

void playMusicThread(string filePath)
{
    PlaySoundA(filePath.c_str(), NULL, SND_FILENAME);

    while (PlaySoundA(NULL, NULL, SND_ASYNC) == 0)
    {

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

                std::thread(playMusicThread, filePath).detach();

                laguDiputar = laguDiputar->next;
            }
        }
        else
        {
            cout << "Playlist kosong." << endl;
        }
    }
}

bool isInPlaylist(pointer_playlist firstPlaylist, string judul_lagu)
{
    pointer_playlist pPlaylist = firstPlaylist;

    while (pPlaylist != NULL)
    {
        pointer_lagu pCari = pPlaylist->laguHead;

        // Cari lagu dalam playlist
        while (pCari != NULL)
        {
            if (strcmp(pCari->judul.c_str(), judul_lagu.c_str()) == 0)
            {
                cout << "Lagu ada pada playlist " << pPlaylist->nama << endl;
                return true; // Lagu ditemukan dalam playlist
            }
            pCari = pCari->next;
        }

        pPlaylist = pPlaylist->next;
    }

    return false; // Lagu tidak ditemukan dalam playlist
}

void hapusDariPlaylist(pointer_playlist &firstPlaylist, string judul_lagu)
{
    pointer_playlist pPlaylist = firstPlaylist;

    while (pPlaylist != NULL)
    {
        pointer_lagu pCari = pPlaylist->laguHead;
        pointer_lagu pPrev = NULL;

        // Cari lagu dalam playlist
        while (pCari != NULL && pCari->judul != judul_lagu)
        {
            pPrev = pCari;
            pCari = pCari->next;
        }

        if (pCari != NULL)
        {
            // Hapus lagu dari multilinked list
            if (pPrev != NULL)
            {
                pPrev->next = pCari->next;
            }
            else
            {
                pPlaylist->laguHead = pCari->next;
            }

            // Hapus lagu dari memori
            delete pCari;

            cout << "Lagu " << judul_lagu << " berhasil dihapus dari playlist " << pPlaylist->nama << "." << endl;
        }

        pPlaylist = pPlaylist->next;
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
            cout << "cek" << endl;
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
void hapusLagu(pointer_lagu &first, pointer_playlist &firstPlaylist, string judul_lagu)
{
    bool isInAnyPlaylist = isInPlaylist(firstPlaylist, judul_lagu);

    pointer_lagu pCari = first;
    pointer_lagu pPrev = NULL;

    // Cari lagu yang ingin dihapus
    while (pCari != NULL && pCari->judul != judul_lagu)
    {
        pPrev = pCari;
        pCari = pCari->next;
    }

    if (pCari != NULL)
    {
        // Hapus lagu dari sistem file jika lagu berada dalam playlist
        if (isInAnyPlaylist)
        {
            string filePath = loc + pCari->judul + ext;
            cout << filePath << endl;
            if (remove(filePath.c_str()) != 0)
            {
                cerr << "Error: Gagal menghapus file lagu dari sistem file." << endl;
                return;
            }
        }

        // Hapus lagu dari linked list
        if (pPrev != NULL)
        {
            pPrev->next = pCari->next;
        }
        else
        {
            first = pCari->next;
        }

        // Hapus lagu dari multilinked list jika lagu berada dalam playlist
        if (isInAnyPlaylist)
        {
            hapusLaguDariPlaylist(first, firstPlaylist, pCari->judul);
        }

        // Hapus lagu dari memori
        delete pCari;

        cout << "Lagu " << judul_lagu << " berhasil dihapus." << endl;

        // Tampilkan daftar lagu setelah penghapusan
        cout << "Daftar Lagu setelah penghapusan:" << endl;
        pointer_lagu pLagu = first;
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

bool deleteFolder(const string &folderPath)
{
    try
    {
        // Menghapus seluruh isi folder secara rekursif
        filesystem::remove_all(folderPath);

        // Menghapus folder itu sendiri
        filesystem::remove(folderPath);

        cout << "Folder berhasil dihapus: " << folderPath << endl;
        return true;
    }
    catch (const exception &e)
    {
        cerr << "Gagal menghapus folder: " << e.what() << endl;
        return false;
    }
}

void deletePlaylistFolder(const string &playlistName, pointer_playlist &firstPlaylist)
{
    // Konversi string biasa ke wide string
    string playlistFolderPath = "Musik\\" + playlistName;

    // Attempt to remove the playlist folder and its contents
    if (deleteFolder(playlistFolderPath.c_str()))
    {
        // Remove the playlist entry from the multilist
        pointer_playlist currentPlaylist = firstPlaylist;
        while (currentPlaylist != nullptr)
        {
            if (currentPlaylist->nama == playlistName)
            {
                // Adjust pointers for the playlist entry
                if (currentPlaylist->prev != nullptr)
                {
                    currentPlaylist->prev->next = currentPlaylist->next;
                }
                else
                {
                    firstPlaylist = currentPlaylist->next;
                }

                if (currentPlaylist->next != nullptr)
                {
                    currentPlaylist->next->prev = currentPlaylist->prev;
                }

                // Free memory for the playlist entry
                delete currentPlaylist;

                cout << "Playlist folder " << playlistName << " successfully deleted." << endl;
                return;
            }

            currentPlaylist = currentPlaylist->next;
        }

        cerr << "Error: Playlist tidak ditemukan di multilist" << endl;
    }
    else
    {
        cerr << "Error: Gagal Untuk Menghapus Folder Playlist " << playlistName << ". Error code: " << GetLastError() << endl;
    }
}

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
