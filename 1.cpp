#include <iostream>
#include <string>
using namespace std;

struct elementQueue
{
    int data;
    elementQueue *next;
};
typedef elementQueue *pointer;

struct queue
{
    pointer head;
    pointer tail;
};

queue q;

void initialization(queue &q)
{
    q.head = NULL;
    q.tail = NULL;
}

void createElement(pointer &pBaru)
{
    pBaru = new elementQueue;
    cout << "Masukkan angka : ";
    cin >> pBaru->data;
    pBaru->next = NULL;
}

void newQueue(queue &q, pointer pBaru)
{
    if (q.head == NULL && q.tail == NULL) {
        q.head = pBaru;
        q.tail = pBaru;
    } else {
        q.tail->next = pBaru;
        q.tail = pBaru;
    }
}

void deleteQueue(queue &q, pointer &pHapus)
{
    if (q.head == NULL && q.tail == NULL) {
        pHapus = NULL;
        cout << "Antrian kosong, tidak ada data yang bisa dihapus" << endl;
    } else if (q.head->next == NULL) {
        pHapus = q.head;
        q.head = NULL;
        q.tail = NULL;
        cout << "Dequeue berhasil" << endl;
    } else {
        pHapus = q.head;
        q.head = q.head->next;
        pHapus->next = NULL;
        cout << "Dequeue berhasil" << endl;
    }
}

int hitungQueue(queue q)
{
    pointer pBantu;
    int hasil = 0;
    if (q.head != NULL) {
        pBantu = q.head;
        do {
            hasil += pBantu->data;
            pBantu = pBantu->next;
        } while (pBantu != q.tail->next);
    }
    return hasil;
}

void cariNilaiEkstrim(queue q, int &max, int &min)
{
    pointer pBantu;
    max = 0;
    min = 0;

    if (q.head != NULL) {
        pBantu = q.head;
        min = pBantu->data;
        do {
            max = (max < pBantu->data) ? pBantu->data : max;
            min = (min > pBantu->data) ? pBantu->data : min;
            pBantu = pBantu->next;
        } while (pBantu != q.tail->next);
    }
}

void tampilkan(queue q)
{
    pointer pBantu;
    if (q.head == NULL) {
        cout << "Antrian kosong" << endl;
    } else {
        pBantu = q.head;
        do {
            cout << pBantu->data << " ";
            pBantu = pBantu->next;
        } while(pBantu != q.tail->next);
        cout << endl;
    }
}

int main()
{
    queue q;
    pointer pBaru, pHapus;
    int Pilihan, jumlahData, total, max, min;
    float rata;

    initialization(q);

    do {
        cout << "\nAntrian : ";
        tampilkan(q);
        cout << endl;
        cout << "-------------------------------" << endl;
        cout << "Program Antrian" << endl;
        cout << "-------------------------------" << endl;
        cout << "1. Enqueue ( Tambah Data)" << endl;
        cout << "2. Dequeue ( Hapus Data )" << endl;
        cout << "3. Jumlah Data" << endl;
        cout << "4. Rata-rata" << endl;
        cout << "5. Nilai terkecil dan terbesar" << endl;
        cout << "6. Keluar" << endl;
        cout << "-------------------------------" << endl;
        cout << "Masukkan pilihan : ";
        cin >> Pilihan;
        switch (Pilihan) {
            case 1:
                createElement(pBaru);
                newQueue(q, pBaru);
                cout << "Enqueue berhasil" << endl;
                break;
            case 2:
                deleteQueue(q, pHapus);
                break;
            case 3:
                total = hitungQueue(q);
                cout << "Jumlah Data = " << total << endl;
                break;
            case 4:
                total = hitungQueue(q);
                jumlahData = q.tail - q.head + 1;
                rata = total/jumlahData;
                cout << "Jumlah Data    = " << total << endl;
                cout << "Banyaknya Data = " << jumlahData << endl;
                cout << "Rata - Rata    = " << rata << endl;
                break;
            case 5:
                cariNilaiEkstrim(q, max, min);
                cout << "Nilai terbesar adalah\t: " << max << endl;
                cout << "Nilai terkecil adalah\t: " << min << endl;
                break;
            case 6:
                cout << "Keluar program.." << endl;
                break;
            default:
                cout << "Pilihan salah" << endl;
                break;
            }
        } while (Pilihan != 6);
    return 0;
}
