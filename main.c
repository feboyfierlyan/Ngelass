#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_USERS 100
#define MAX_CLASSES 6
#define MAX_FLOORS 11
#define MAX_TIMESLOTS 3
#define DATA_FILE "data.txt"

struct Class
{
  char time_slot[20];
  char date[20];
  int is_booked;
  char booked_by[20];
} classes[MAX_FLOORS][MAX_CLASSES][MAX_TIMESLOTS];

struct User
{
  char username[20];
  char password[20];
} users[MAX_USERS];

int user_count = 0;

// Fungsi menunggu pengguna menekan enter
void pressEnterToContinue()
{
  printf("\nTekan Enter untuk kembali ke menu...");
  while (getchar() != '\n')
    ;
  getchar();
}

// Fungsi membersihkan layar
void clearScreen()
{
  system("cls");
}

// Fungsi untuk memuat data dari file
void loadDataFromFile()
{
  FILE *file = fopen(DATA_FILE, "r");
  if (file == NULL)
  {
    return; // File tidak ada, tidak perlu memuat data
  }

  // Memuat data pengguna
  while (fscanf(file, "%s %s", users[user_count].username, users[user_count].password) != EOF)
  {
    user_count++;
  }

  // Memuat data booking kelas
  while (!feof(file))
  {
    char booked_by[50];
    char date[20];
    int floor, room, slot;
    char time_slot[20];

    // Membaca data booking kelas dari file
    if (fscanf(file, "%s %s %d %d %d %s", booked_by, date, &floor, &room, &slot, time_slot) == 6)
    {
      // Menyimpan data booking ke dalam struktur
      strcpy(classes[floor][room][slot].booked_by, booked_by);
      strcpy(classes[floor][room][slot].date, date);
      strcpy(classes[floor][room][slot].time_slot, time_slot);
      classes[floor][room][slot].is_booked = 1; // Set kelas sebagai di-booking
    }
  }

  fclose(file);
}

// Fungsi untuk menyimpan data ke file
void saveDataToFile()
{
  FILE *file = fopen(DATA_FILE, "w");
  if (file == NULL)
  {
    printf("Tidak dapat menyimpan data.\n");
    return;
  }
  for (int i = 0; i < user_count; i++)
  {
    fprintf(file, "%s %s\n", users[i].username, users[i].password);
  }
  fclose(file);
}

// Fungsi untuk menyimpan data kelas ke file
void saveClassDataToFile()
{
  FILE *file = fopen(DATA_FILE, "a"); // Membuka file dalam mode append
  if (file == NULL)
  {
    printf("Tidak dapat menyimpan data kelas.\n");
    return;
  }
  for (int floor = 0; floor < MAX_FLOORS; floor++)
  {
    for (int room = 0; room < MAX_CLASSES; room++)
    {
      for (int slot = 0; slot < MAX_TIMESLOTS; slot++)
      {
        if (classes[floor][room][slot].is_booked)
        {
          fprintf(file, "Booking: %s %s %d %d.%02d %s\n",
                  classes[floor][room][slot].booked_by,
                  classes[floor][room][slot].date,
                  floor + 1,
                  room + 1,
                  slot + 1,
                  classes[floor][room][slot].time_slot);
        }
      }
    }
  }
  fclose(file);
}

// Fungsi untuk registrasi user
void registerUser()
{
  clearScreen(); // Bersihkan layar sebelum registrasi
  if (user_count >= MAX_USERS)
  {
    printf("User maksimal tercapai.\n");
    pressEnterToContinue();
    return;
  }
  printf("Registrasi User:\n");
  printf("Masukkan Username: ");
  scanf("%s", users[user_count].username);
  printf("Masukkan Password: ");
  scanf("%s", users[user_count].password);
  user_count++;
  saveDataToFile(); // Simpan data ke file setelah registrasi
  printf("Registrasi berhasil.\n");
  pressEnterToContinue();
}

// Fungsi untuk login user
int loginUser()
{
  clearScreen(); // Bersihkan layar sebelum login
  char username[20], password[20];
  printf("Login User:\n");
  printf("Username: ");
  scanf("%s", username);
  printf("Password: ");
  scanf("%s", password);

  for (int i = 0; i < user_count; i++)
  {
    if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
    {
      printf("Login berhasil.\n");
      pressEnterToContinue();
      return i;
    }
  }
  printf("Login gagal. Username atau password salah.\n");
  pressEnterToContinue();
  return -1;
}

// Fungsi untuk booking kelas
void bookClass(int user_id)
{
  clearScreen(); // Bersihkan layar sebelum proses booking
  printf("Booking Kelas:\n");

  int floor, room, slot;
  printf("Pilih Lantai (1-11): ");
  scanf("%d", &floor);
  floor -= 1; // Ubah agar sesuai dengan indeks array

  printf("Pilih Kelas (1-6): ");
  scanf("%d", &room);
  room -= 1; // Ubah agar sesuai dengan indeks array

  printf("Pilih Waktu:\n");
  printf("1. 08:00-09:40\n");
  printf("2. 09:40-11:20\n");
  printf("3. 13:00-14:40\n");
  scanf("%d", &slot);
  slot -= 1; // Ubah agar sesuai dengan indeks array

  if (classes[floor][room][slot].is_booked)
  {
    printf("Kelas sudah di-booking.\n");
  }
  else
  {
    classes[floor][room][slot].is_booked = 1;
    strcpy(classes[floor][room][slot].booked_by, users[user_id].username);
    printf("Masukkan tanggal (dd-mm-yyyy): ");
    scanf("%s", classes[floor][room][slot].date);
    strcpy(classes[floor][room][slot].time_slot, slot == 0 ? "08:00-09:40" : (slot == 1 ? "09:40-11:20" : "13:00-14:40"));
    printf("Booking berhasil.\n");
    saveClassDataToFile(); // Simpan data kelas ke file setelah booking berhasil
  }

  pressEnterToContinue(); // Tunggu enter untuk kembali ke menu setelah booking berhasil
}

// Fungsi untuk membatalkan booking kelas
void cancelBooking(int user_id)
{
  clearScreen(); // Bersihkan layar sebelum membatalkan booking
  printf("Membatalkan Booking Kelas:\n");

  int floor, room, slot;
  printf("Pilih Lantai (1-11): ");
  scanf("%d", &floor);
  floor -= 1; // Ubah agar sesuai dengan indeks array

  printf("Pilih Kelas (1-6): ");
  scanf("%d", &room);
  room -= 1; // Ubah agar sesuai dengan indeks array

  printf("Pilih Waktu:\n");
  printf("1. 08:00-09:40\n");
  printf("2. 09:40-11:20\n");
  printf("3. 13:00-14:40\n");
  scanf("%d", &slot);
  slot -= 1; // Ubah agar sesuai dengan indeks array

  if (classes[floor][room][slot].is_booked && strcmp(classes[floor][room][slot].booked_by, users[user_id].username) == 0)
  {
    classes[floor][room][slot].is_booked = 0; // Batalkan booking
    strcpy(classes[floor][room][slot].booked_by, "N/A");
    printf("Booking dibatalkan.\n");
  }
  else
  {
    printf("Kelas tidak ada booking atas nama Anda.\n");
  }

  pressEnterToContinue(); // Tunggu enter untuk kembali ke menu setelah membatalkan booking
}

void viewClassStatusByFloor()
{
  clearScreen(); // Bersihkan layar sebelum menampilkan status kelas
  int floor;

  printf("Pilih Lantai (1-11): ");
  int result = scanf("%d", &floor);

  // Jika pengguna menekan Enter tanpa memasukkan angka
  if (result == 0)
  {
    // Mengosongkan input buffer
    while (getchar() != '\n')
      ;
    return; // Kembali ke menu
  }

  floor -= 1; // Ubah agar sesuai dengan indeks array

  if (floor < 0 || floor >= MAX_FLOORS)
  {
    printf("Lantai tidak valid. Silakan coba lagi.\n");
    pressEnterToContinue();
    return;
  }

  printf("\nStatus Kelas di Lantai %d:\n", floor + 1);
  for (int room = 0; room < MAX_CLASSES; room++)
  {
    for (int slot = 0; slot < MAX_TIMESLOTS; slot++)
    {
      // Mengatur warna output
      if (classes[floor][room][slot].is_booked)
      {
        printf("\033[31m"); // Merah untuk kelas yang sudah dibooking
      }
      else
      {
        printf("\033[32m"); // Hijau untuk kelas yang kosong
      }
      printf("%d. SAW %d.%02d [%s] - [%s - %s] - [%s]\n",
             room + 1,
             floor + 1,
             slot + 1,
             classes[floor][room][slot].is_booked ? classes[floor][room][slot].booked_by : "Kosong",
             classes[floor][room][slot].date,
             classes[floor][room][slot].is_booked ? "Booked" : "Kosong",
             classes[floor][room][slot].time_slot);
      printf("\033[0m"); // Reset warna output
    }
  }

  pressEnterToContinue(); // Tunggu enter untuk kembali ke menu setelah menampilkan status kelas
}

int main()
{
  loadDataFromFile(); // Memuat data dari file saat aplikasi dimulai

  int choice, user_id = -1;

  while (1)
  {
    clearScreen(); // Bersihkan layar di setiap iterasi

    // Menampilkan menu yang sesuai berdasarkan status login
    if (user_id == -1)
    {
      printf("=== Menu Utama ===\n");
      printf("1. Registrasi\n");
      printf("2. Login\n");
      printf("3. Melihat Status Kelas\n");
      printf("0. Keluar\n");
    }
    else
    {
      printf("=== Menu Utama ===\n");
      printf("1. Booking Kelas\n");
      printf("2. Membatalkan Booking Kelas\n");
      printf("3. Melihat Status Kelas\n");
      printf("0. Keluar\n");
    }

    printf("Pilih opsi: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      if (user_id == -1)
        registerUser();
      else
        bookClass(user_id);
      break;
    case 2:
      if (user_id == -1)
        user_id = loginUser();
      else
        cancelBooking(user_id);
      break;
    case 3:
      viewClassStatusByFloor();
      break;
    case 0:
      return 0;
    default:
      printf("Opsi tidak valid. Silakan coba lagi.\n");
      pressEnterToContinue();
    }
  }

  return 0;
}
