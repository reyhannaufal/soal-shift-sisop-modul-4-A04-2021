# Laporan Soal Shift Modul 4

### Kelompok A04
- Ihsannur Rahman Qalbi 05111940000090
- Muhamad Fikri Sunandar 05111940000135
- Reyhan Naufal Rahman 05111940000171

### Daftar isi
1. [NO 1](#NO1)
2. [NO 2](#NO2)
3. [NO 3](#NO3)
4. [NO 4](#NO4)

## N0 1
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :
	
NOTE : 
Semua file yang berada pada direktori harus ter-encode menggunakan Atbash cipher(mirror).
Misalkan terdapat file bernama kucinglucu123.jpg pada direktori DATA_PENTING
`“AtoZ_folder/DATA_PENTING/kucinglucu123.jpg” → “AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”`
Note : filesystem berfungsi normal layaknya linux pada umumnya, Mount source (root) filesystem adalah directory /home/[USER]/Downloads, dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di-encode.

Referensi : https://www.dcode.fr/atbash-cipher

- Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]
- Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

### Penyelesaian
Mendefinisikan directory path dan log path. Pada hal ini yaitu
```C
static const char *dirpath = "/home/ubuntu/downloads";
static const char *log_path = "/home/ubuntu/SinSeiFS.log";
```
Lalu untuk proses enkripsi dan deskripsi kami menggunakan fungsi `fuse` dari `xmp_ggetattr`
```C
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[buffer_size];
    sprintf(fpath, "%s%s", dirpath, get_encryption_path(path));

    printf("file_attr : %s\n", fpath);

    int res;

    is_decrypting = false;

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}
```
Fungsi melakukan deskripsi dan akan di concatenate dengan directory path yang telah didefiniskan diatas. Lalu kami menggunakan `fuse` dari `xmp_readdir` yang berfungsi melakukan encode agar terlihat pada terminal.

Lalu untuk melakukan enkripsi pada soal no 1, kami melakukannya dengan fungsi encrypt_atbash.
```C
char *encrypt_atbash(char *str)
{
    int i = 0;
    if (!strcmp(str, ".") || !strcmp(str, ".."))
        return str;

    char *output = malloc(buffer_size);

    while (str[i] != '\0')
    {
        if (str[i] == '.')
        {
            break;
        }
        /*
            Upercase letter start from 65 - 90
            Lowecase letter start from 97-122
        */
        if (!((str[i] >= 0 && str[i] < 65) || (str[i] > 90 && str[i] < 97) || (str[i] > 122 && str[i] <= 127)))
        {
            // 65 + 90 - 65 = 90 rubah ke belakang A -> Z
            if (str[i] >= 'A' && str[i] <= 'Z')
                output[i] = 'Z' + 'A' - str[i];
            if (str[i] >= 'a' && str[i] <= 'z')
                output[i] = 'z' + 'a' - str[i];
        }
        else
        {
            output[i] = str[i];
        }

        i++;
    }

    while (str[i] != '\0')
    {
        output[i] = str[i];
        i++;
    }

    output[i] = '\0';

    return output;
}
```
Jika file tersebut merupakan file hidden maka akan diexclude dan akan langsung direturn. Lalu ASCII code kapital dimulai dari 65 - 90, sedangkan untuk ASCII code huruf kecil dimulai dari 97 - 122. Kami melakukan exclude untuk yang tidak memenuhi ASCII code yang telah dilakukan. Lalu kami akan mengurangi nya dengan ` output[i] = 'Z' + 'A' - str[i];` ` // 65 + 90 - 65 = 90 rubah ke belakang A -> Z`. Lalu seperti itulah cara kerja enkripsi atbash kami.
