# Strukdat
PREQUISITE:
harus memanggil library -lwinmm untuk bisa memutar musik
harus memanggil dirent.h dan yang lainnya untuk bisa membaca file yang ada folder


MENU : 
1. Tambah Lagu
 - *Pointer* 
- program dimulai dengan inisialisasi SDL, program memuat file musik ke dalam suatu variabel. File musik yang dimuat berupa path yang diberikan oleh pengguna
2. Tambah Playlist 
- Menggunakan linked list untuk menyimpan playlist, pada setiap node pada linked list menyimpan informasi tentang playlist dan lagu-lagu di dalamnya. Struktur data untuk lagu: data untuk menyimpan informasi lagu (judul, artis, durasi, pointer untuk next). Struktur data untuk Playlist: data untuk menyimpan nama playlist dan pointer ke lagu pertama di dalamnya (nama playlist, song* next). Pake multi linked list.
3. Putar lagu apa? 
Program menggunakan library tambahan yaitu  SDL dan SDL_mixer, yang digunakan agar program dapat memutar musik. Selain itu terdapat beberapa fungsi diantaranya untuk menginisialisasi Pustaka SDL, memuat file musik yang dimainkan serta memutar musik . Hal pertama yang diproses adalah menampilkan daftar lagu yang ada di storage, kemudian diminta input oleh user lagu mana yang akan diputar.
- *Pointer* 
- pilih
4. Masukkan antrian 
- *Queue* 
Program menambah antrian lagu digunakan untuk menyusun daftar putar lagu yang akan diputar secara berurutan sesuai dengan keinginan pengguna Fitur utama program ini mencakup: penambahan lagu ke antrian, pengguna dapat menambahkan lagu baru ke dalam antrian. Setiap lagu yang ditambahkan akan ditempatkan di akhir antrian.
- Masukkan antrian, setelah lagu ini apa
5. Tampilkan lagu : 
Pengguna bisa melihat list lagu yang telah ditambahkan
6. Hapus lagu 
Pengguna bisa menghapus lagu, ketika lagu dihapus filenya pun akan ikut dihapus
7. Hapus Antrian
Antrian lagu akan dihapus yang paling luar, sesuai dengan konsep First In First Out
8 . Stop lagu
Pengguna bisa menghentikan lagu yang diputar  
- *Queue* ( tampilkan next antrian) 
- *Linked list* ( kalau gada antrian lagi ) 
9. Delete playlist
Pengguna bisa menghapus playlist yang ada, foldernya pun akan ikut dihapus
- *linked list* 

Catatan :
- Setiap playlist punya file
- Buat kasih tau lagi play : bisa pake yang queue daftar antrian 
- Lagu bisa diputar secara realtime

Pointer : opsi pertama +
 opsi kedua  + opsi ketiga ( fungsi searching + if kalau antrian kosong  ) 
multi list
