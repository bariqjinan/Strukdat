#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>

int main()
{
    // Tentukan path folder yang ingin di-list
    const char *folderPath = "Musik\\";

    // Buka direktori
    DIR *dir = opendir(folderPath);

    // Periksa apakah direktori berhasil dibuka
    if (dir)
    {
        std::cout << "Daftar Direktori di " << folderPath << ":\n";

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
            std::string fullPath = folderPath;
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
                    std::cout << entry->d_name << "\n";
                }
            }
            else
            {
                std::cerr << "Gagal mendapatkan informasi status untuk " << entry->d_name << "\n";
            }
        }

        // Tutup direktori setelah selesai membaca
        closedir(dir);
    }
    else
    {
        // Tampilkan pesan kesalahan jika gagal membuka direktori
        std::cerr << "Gagal membuka direktori " << folderPath << "\n";
        return 1; // Keluar dengan kode error
    }

    return 0; // Program selesai dengan sukses
}
