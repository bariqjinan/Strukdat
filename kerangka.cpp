#include <iostream>
using namespace std;

int main() {
    int pilih;

	do{
		cout << "|======================|" << endl;
		cout << "| Program Pemutar Lagu |" << endl;
		cout << "|======================|" << endl;
		cout << "Pilihan Menu" << endl;
		cout << "1. Tambah lagu" << endl;
		cout << "2. Tambah playlist" << endl;
		cout << "3. Putar kagu apa?" << endl;
		cout << "4. Masukan Antrian" << endl;
		cout << "5. Tampilkan lagu" << endl;
		cout << "6. Hapus lagu pengguna" << endl;
		cout << "7. Hapus Antrian lagu" << endl;
		cout << "8. Stop lagu pengguna" << endl;
		cout << "9. Delete Playlist" << endl;
		cout << "10.Keluar Program" << endl;
		cout << "Masukkan Pilihan : "; cin >> pilih;
		switch(pilih){
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
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
	}while (pilih!=10);
}
