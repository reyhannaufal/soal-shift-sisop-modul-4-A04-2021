# Laporan Soal Shift Modul 4

### Kelompok A04
- Ihsannur Rahman Qalbi 05111940000090
- Muhamad Fikri Sunandar 05111940000135
- Reyhan Naufal Rahman 05111940000171


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


## N0 2
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada
komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang
dirancang oleh Sei. Karena no 02 perlu enkripsi ROT13 dan Vigenere. Maka membuat dlu kodingan enkripsinya.
Algoritma Rot13. 
```C
char *encrypt_rot13(char *str)
{
    int i = 0;
    if (str == NULL)
    {
        return NULL;
    }
    if (!strcmp(str, ".") || !strcmp(str, ".."))
        return str;

    char *result = malloc(strlen(str));
    strcpy(result, str);
    if (result != NULL)
    {
        while (str[i] != '\0')
        {
            if (str[i] == '.')
            {
                break;
            }
            //Only increment alphabet characters
            if ((str[i] >= 97 && str[i] <= 122) || (str[i] >= 65 && str[i] <= 90))
            {
                if (str[i] > 109 || (str[i] > 77 && str[i] < 91))
                {
                    //Characters that wrap around to the start of the alphabet
                    result[i] -= 13;
                }
                else
                {
                    //Characters that can be safely incremented
                    result[i] += 13;
                }
            }
            i++;
        }
        while (str[i] != '\0')
        {
            result[i] = str[i];
            i++;
        }
    }
    result[i] = '\0';
    return result;
}

```
Algoritma ini bekerja dengan cara membagi dua 26 huruf latin menjadi dua (13 pertama dan 13 terakhir). Lalu, merotasi huruf 13 pertama menjadi 13 terakhir dan sebaliknya. Sehingga A menjadi N, B menjadi O (dan sebaliknya), dan seterusnya.

Algoritma enkripsi Vigenere.
```C
char KEY[] = "SISOP";

char *encrypt_vignere(char *str)
{
    int i, j;
    int msgLen = strlen(str);
    int keyLen = strlen(KEY);
    char newKey[msgLen];
    char *encryptedMsg = malloc(msgLen);

    if (!strcmp(str, ".") || !strcmp(str, ".."))
        return str;
    //generating new key
    for (i = 0, j = 0; i < msgLen; ++i, ++j)
    {
        if (j == keyLen)
            j = 0;

        newKey[i] = KEY[j];
    }

    newKey[i] = '\0';

    //encryption
    for (i = 0; i < msgLen; ++i)
    {
        if (str[i] == '.')
        {
            break;
        }
        encryptedMsg[i] = ((str[i] + newKey[i]) % 26) + 'A';
    }

    while (str[i] != '\0')
    {
        encryptedMsg[i] = str[i];
        i++;
    }

    encryptedMsg[i] = '\0';

    return encryptedMsg;
}
```
Algoritma ini bekerja dengan cara menggandakan keynya (dalam konteks ini "SISOP") menjadi sepanjang string yang akan dienkripsi (secara berulang, misal "SISOPSISOP..." sepanjang string). Setelah itu algoritma enkripsnya key[i] + string[i]/mod 26.

Algoritma dekripsi Vigenere.
```C
char *decrypt_vignere(char *str)
{
    int i, j;
    int msgLen = strlen(str);
    int keyLen = strlen(KEY);
    char newKey[msgLen];
    char *decryptedMsg = malloc(msgLen);

    if (!strcmp(str, ".") || !strcmp(str, ".."))
        return str;

    //generating new key
    for (i = 0, j = 0; i < msgLen; ++i, ++j)
    {
        if (j == keyLen)
            j = 0;

        newKey[i] = KEY[j];
    }

    newKey[i] = '\0';

    //decryption
    for (i = 0; i < msgLen; ++i)
    {
        if (str[i] == '.')
        {
            break;
        }
        decryptedMsg[i] = (((str[i] - newKey[i]) + 26) % 26) + 'A';
    }

    while (str[i] != '\0')
    {
        decryptedMsg[i] = str[i];
        i++;
    }
    decryptedMsg[i] = '\0';

    return decryptedMsg;
}

```
Algortima ini bekerja hampir mirip dengan enkripsinya tapi tinggal dibalik ajah. Dekripsi = (stringEnkripti[i] - key[i]) mod 26.

No 2a. 
Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori
tersebut akan menjadi direktori terencode beserta isinya dengan perubahan
nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash +
ROT13).

```C
else if (enc & (1 << 1))
        {
            sprintf(temp, "/%s", encrypt_rot13(encrypt_atbash(de->d_name)));
        }
```
Jika memenuhi seperti yang diminta oleh no 2a, yakni ada folder yang dibuat (mkdir) maka fungsi encryt_rot13(encrypti_atbash(string)) akan muncul.

No 2b.
Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori
tersebut akan menjadi direktori terencode beserta isinya dengan perubahan
nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere
Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
```C
else if (enc & (1 << 2))
        {
            sprintf(temp, "/%s", encrypt_vignere(encrypt_atbash(de->d_name)));
        }
```
Mirip dengan no 2a, jika ada rename dengan awalan "RX_" maka fungsi encrypt_vignere dan encrypt_atbash akan dipanggil dengan parameter string (yang akan dienkripsi).
Kendala:
No 2c, 2d, 2e belum bisa dikerjakan.
referensi: 
- https://en.wikipedia.org/wiki/ROT13
- https://www.javatpoint.com/vigenere-cipher

## NO3

Sin menambahkan fitur ke filesystem mereka dimana sebuah direktori dengan awalan "A_is_a" menjadi direktori spesial
```sh
int get_encryption_mode(char *path)
{
....
    if (strstr(tok, "A_is_a_") == tok)
        {
            mode |= 1 << 3;
        }
....
}

char *get_encryption_path(const char *path)
{
....
 else if (enc & (1 << 3))
        {
            // sprintf(fpath + n, "/%s", jawaban);
            sprintf(fpath + n, "/%s", get_special_directory_name(jawaban));
        }
....
}
```
Pada fungsi get_encryption_path, value enc didapat menggunakan fungsi get_encryption_mode.

Setelah direktori menjadi direktori special, fungsi get_special_directory_name dijalankan untuk merubah nama file yang terdapat didalam direktori menjadi lowercase dan memberi ekstensi berupa angka yang menunjukan perbedaan nama file setelah dan sebelum dirubah menjadi lowecase
```sh
char *get_file_name_only(char *path)
{
    char *filename = malloc(sizeof(char) * buffer_size);
    char *temppath = get_file_name(path);
    int n = strlen(temppath);
    int i = 0;
    for (; i < n; i++)
    {
        if (temppath[i] == '.')
        {
            break;
        }
    }
    snprintf(filename, i + 1, "%s", temppath);
    free(temppath);
    return filename;
}

char *get_extension_name(char *path)
{
    char *filename = malloc(sizeof(char) * buffer_size);
    char *temppath = get_file_name(path);
    int n = strlen(temppath);
    int i = 0;
    for (; i < n; i++)
    {
        if (temppath[i] == '.')
        {
            break;
        }
    }
    i++;
    sprintf(filename, "%s", temppath + i);
    free(temppath);
    return filename;
}

int get_lowercase_diff_decimal(char *path)
{
    int val = 0;

    int n = strlen(path);
    int k = n;

    int i = 0;

    for (; i < k; i++)
    {
        int diff = (path[i] == tolower(path[i]) ? 0 : 1);
        val <<= 1;
        val |= diff;
    }

    return val;
}

char *get_special_directory_name(char *path)
{
    char *filename = malloc(sizeof(char) * buffer_size);
    char *filename_only = get_file_name_only(path);
    char *extension_only = get_extension_name(path);

    //printf("%s\n", filename_only);
    //printf("%s\n", extension_only);

    int diff = get_lowercase_diff_decimal(filename_only);

    int i = 0;
    int n = strlen(filename_only);
    for (; i < n; i++)
    {
        filename_only[i] = tolower(filename_only[i]);
    }

    sprintf(filename, "%s.%s.%d", filename_only, extension_only, diff);

    free(filename_only);
    free(extension_only);

    return filename;
}
```
Untuk mendapatkan nama baru dari file didalam direktori spesial, pertama-tama nama file dan ekstensi dari file dipisahkan, sehingga nama file dapat dirubah tanpa memodifikasi nama ekstensi dari file tersebut. 
Kemudian didapatkan nilai perbedaan dari nama awal dengan nama akhir, nilai ini adalah nilai desimal dari bilangan biner yang merepresentasikan ada atau tidaknya perbedaan dari nama awal dan nama akhir dari file, jika ada maka digit biner akan bernilai 1 dan jika tidak maka akan bernilai 0. Contohnya CoNToH.tXt akan menjadi contoh.tXT yang di representasikan menjadi 101101 yang jika diubah menjadi desimal akan bernilai 45.
Setelah nilai perbedaan didapat, nama file akan dirubah menjadi lowecase, kemudian di concate dengan ekstensi dan nilai perbedaan yang didapatkan sebelumnya.

## NO4
Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.
- Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
- Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
- Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
- Sisanya, akan dicatat pada level INFO.
- Format untuk logging yaitu:
`[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`
Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

`INFO::28052021-10:00:00:CREATE::/test.txt`

`INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt`

Kami membuat 2 log pada system yaitu `warning` dan `info` 

```C
int log_info_command(char *command, const char *from, const char *to)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char mains[1000];
    if (to == NULL)
    {
        sprintf(mains, "INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
                tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }
    else
    {
        sprintf(mains, "INFO::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
                tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
    printf("%s", mains);
    FILE *foutput = fopen(log_path, "a");
    fputs(mains, foutput);
    fclose(foutput);
    return 1;
}
```

```C
int log_warning_command(char *command, const char *from, const char *to)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char mains[1000];
    if (to == NULL)
    {
        sprintf(mains, "WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s\n",
                tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from);
    }
    else
    {
        sprintf(mains, "WARNING::%02d%02d%02d-%02d:%02d:%02d:%s::%s::%s\n",
                tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year, tm.tm_hour, tm.tm_min, tm.tm_sec, command, from, to);
    }
    printf("%s", mains);
    FILE *foutput = fopen(log_path, "a");
    fputs(mains, foutput);
    fclose(foutput);
    return 1;
}
```
Jika terdapat tujuan logging, maka format logging  `:[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]` dengan kedua desc berisi `char* from` dan `char* to`. Jika terdapat tujuan logging `[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC]` dengan desc adalah `char* from`.

Lalu print hasil perulangan dalam bentuk `string`. Buka fileouput lalu akan memasukan string main kedalam `log`.
