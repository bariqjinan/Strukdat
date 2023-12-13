#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct lagu {
    string judul;
    lagu* next;
    lagu* prev;
};

typedef lagu *pointer_lagu;

void DisplayList(const pointer_lagu& first) {
    cout << "Daftar lagu:\n";
    const lagu* current = first;
    while (current != NULL) {
        cout << current->judul << "\n";
        current = current->next;
    }
}

void DeletepCari(pointer_lagu& first, const string& judul, const string& folderPath) {
    lagu* pCari = first;

    while (pCari != NULL && pCari->judul != judul) {
        pCari = pCari->next;
    }

    if (pCari == NULL) {
        cout << "Lagu yang akan dihapus tidak ditemukan." << endl;
        return;
    }

    string fileName = folderPath + "\\" + pCari->judul + ".wav";
    if (remove(fileName.c_str()) != 0) {
        cerr << "Error deleting file. Check if the file exists and has the correct permissions." << endl;
        return;
    }

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

void ReadListFromFile(pointer_lagu& first, const string& folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((folderPath + "\\*.wav").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                string judul = findFileData.cFileName;
                judul.erase(judul.size() - 4);  // Remove the ".wav" extension
                lagu* pBaru = new lagu{judul, NULL, NULL};

                if (first == NULL) {
                    first = pBaru;
                } else {
                    lagu* last = first;
                    while (last->next != NULL) {
                        last = last->next;
                    }
                    last->next = pBaru;
                    pBaru->prev = last;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    } else {
        cerr << "Unable to open directory: " << folderPath << "\n";
    }
}

int main() {
    pointer_lagu first = NULL;

    // Set your storage path
    string folderPath = "D:\\DATA AISHA\\UNPAD\\SEMESTER 3(B)\\libCurl\\Musik";

    // Membaca daftar lagu dari file
    ReadListFromFile(first, folderPath);

    cout << "Daftar lagu sebelum penghapusan:\n";
    DisplayList(first);

    cout << "Masukkan judul lagu yang akan dihapus: ";
    string judul;
    getline(cin, judul);

    DeletepCari(first, judul, folderPath);

    cout << "Daftar lagu setelah penghapusan:\n";
    DisplayList(first);

    // Membersihkan memori
    while (first != NULL) {
        lagu* temp = first;
        first = first->next;
        delete temp;
    }

    return 0;
}
