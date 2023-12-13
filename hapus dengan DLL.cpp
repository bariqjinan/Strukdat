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

struct Lagu {
    string judul;
    Lagu* next;
    Lagu* prev;
};

typedef Lagu* plg;

void CreateElmt(plg& first, const string& judul) {
    Lagu* pBaru = new Lagu;
    pBaru->judul = judul;
    pBaru->next = NULL;
    pBaru->prev = NULL;

    if (first == NULL) {
        first = pBaru;
    } else {
        Lagu* last = first;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = pBaru;
        pBaru->prev = last;
    }

    // Simpan lagu ke file
    ofstream file(pBaru->judul + ".wav");
    if (file.is_open()) {
        file << "Judul Lagu: " << pBaru->judul << "\n";
        file.close();
        cout << "File " << pBaru->judul << ".wav berhasil dibuat." << endl;
    } else {
        cerr << "Unable to open file: " << pBaru->judul + ".wav" << "\n";
    }
}

void DisplayList(const plg& first) {
    cout << "Daftar lagu:\n";
    const Lagu* current = first;
    while (current != NULL) {
        cout << current->judul << "\n";
        current = current->next;
    }
}

void DeletepCari(plg& first, const string& judul) {
    Lagu* pCari = first;

    while (pCari != NULL && pCari->judul != judul) {
        pCari = pCari->next;
    }

    if (pCari == NULL) {
        cout << "Lagu yang akan dihapus tidak ditemukan." << endl;
        return;
    }

    string fileName = pCari->judul + ".wav";
    remove(fileName.c_str());

    if (pCari->prev == NULL) {
        first = pCari->next;
        if (first != NULL) {
            first->prev = NULL;
        }
    } else {
        pCari->prev->next = pCari->next;
        if (pCari->next != NULL) {
            pCari->next->prev = pCari->prev;
        }
    }

    pCari->next = NULL;
    pCari->prev = NULL;
    delete pCari;

    cout << "File dan lagu berhasil dihapus.\n";
}

void ReadListFromFile(plg& first, const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        string judul;
        while (getline(file, judul)) {
            CreateElmt(first, judul);
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << "\n";
    }
}

void SaveListToFile(const plg& first, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        const Lagu* current = first;
        while (current != NULL) {
            file << current->judul << "\n";
            current = current->next;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << "\n";
    }
}

int main() {
    plg first = NULL;

    // Membaca daftar lagu dari file
    ReadListFromFile(first, "daftar_lagu.txt");

    cout << "Daftar lagu sebelum penghapusan:\n";
    DisplayList(first);

    cout << "Masukkan judul lagu (kosongkan untuk selesai): ";
    string judul;
    getline(cin, judul);

    while (!judul.empty()) {
        CreateElmt(first, judul);
        SaveListToFile(first, "daftar_lagu.txt");  // Menyimpan daftar lagu ke file setiap kali ditambahkan
        cout << "Masukkan judul lagu (kosongkan untuk selesai): ";
        getline(cin, judul);
    }

    cout << "Masukkan judul lagu yang akan dihapus: ";
    getline(cin, judul);

    DeletepCari(first, judul);
    SaveListToFile(first, "daftar_lagu.txt");  // Menyimpan daftar lagu ke file setelah penghapusan

    cout << "Daftar lagu setelah penghapusan:\n";
    DisplayList(first);

    // Membersihkan memori
    while (first != NULL) {
        Lagu* temp = first;
        first = first->next;
        delete temp;
    }

    return 0;
}
