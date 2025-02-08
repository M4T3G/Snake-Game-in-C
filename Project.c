#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 50
#define MAX_M 50

// Y�n belirleme i�in enum
typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

// Oyun tahtas� bilgisi
char board[MAX_N][MAX_M];
int rows, cols, food_count;
int move_count = 0;  // Hamle sayac�

// Y�lan yap�s�
typedef struct {
    int x, y;  // Y�lan�n ba�� koordinat�
} SnakePart;

SnakePart snake[MAX_N * MAX_M];
int snake_size;

// Y�n de�i�imleri
int dx[] = {-1, 1, 0, 0};  // UP, DOWN, LEFT, RIGHT
int dy[] = {0, 0, -1, 1};

// Tahtay� ekrana basma fonksiyonu
void print_board() {
    int i, j;
    system("CLS");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("Hamle Sayisi: %d\n", move_count);
    printf("Kalan Yemek Sayisi: %d\n", food_count);
}

// Rastgele yemek yerle�tirme fonksiyonu
void place_food() {
    int placed = 0;
    while (placed < food_count) {
        int x = rand() % rows;
        int y = rand() % cols;
        if (board[x][y] == ' ') {
            board[x][y] = '0';
            placed++;
        }
    }
}

// Oyun tahtas�n� haz�rlama
void initialize_game() {
    int i, j;
    // Kullan�c�dan boyut ve yemek say�s� al�nmas�
    printf("Oyun tahtasi boyutlarini giriniz (satir, sutun): ");
    scanf("%d %d", &rows, &cols);
    
    if (rows > MAX_N || cols > MAX_M) {
        printf("Boyutlar izin verilen maksimum boyutu asmamali (50x50).\n");
        exit(1);
    }

    printf("Yemek sayisini giriniz: ");
    scanf("%d", &food_count);

    if (food_count >= rows * cols) {
        printf("Yemek sayisi tahtaya sigmaz!\n");
        exit(1);
    }

    // Tahtay� bo� karakterle doldur
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            board[i][j] = ' ';
        }
    }

    // Y�lan�n ba�lang�� pozisyonu
    snake_size = 1;
    snake[0].x = rows / 2;
    snake[0].y = cols / 2;
    board[snake[0].x][snake[0].y] = '1';

    // Rastgele yemek yerle�tir
    place_food();
}

// Y�lan� hareket ettir
int move_snake(Direction dir) {
    int i, j;
    int new_x = snake[0].x + dx[dir];
    int new_y = snake[0].y + dy[dir];

    // Tahta s�n�rlar�n� kontrol et
    if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols) {
        printf("Kaybettiniz! Yilan sinirdan cikti.\n");
        return 0;
    }

    // Yemek yeme durumu
    int grow = (board[new_x][new_y] == '0');

    // Y�lan�n ba� pozisyonunu g�ncelle
    for (i = snake_size; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    snake[0].x = new_x;
    snake[0].y = new_y;

    // B�y�tme
    if (grow) {
        snake_size++;
        food_count--;  // Kalan yemek say�s�n� azalt
        printf("Yemek yendi! Kalan yemek sayisi: %d\n", food_count);
        board[new_x][new_y] = ' ';  // Yemek kaybolur
    }

    // Tahtay� g�ncelle
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (board[i][j] != '0')
                board[i][j] = ' ';
        }
    }

    // Y�lan par�alar�n� tahtaya yerle�tir
    for (i = 0; i < snake_size; i++) {
        board[snake[i].x][snake[i].y] = '1' + i;
    }

    move_count++;  // Hamle say�s�n� art�r

    print_board();

    // T�m yemekler bitti�inde oyunu kazanma durumu
    if (food_count == 0) {
        printf("Tebrikler! Tum yemekleri yediniz ve kazandiniz!\n");
        return 0;
    }

    return 1;
}

// Ana oyun d�ng�s�
void play_game() {
    char input;
    while (1) {
        printf("Yon giriniz (U: yukari, D: asagi, L: sol, R: sag): ");
        scanf(" %c", &input);

        Direction dir;
        switch (input) {
            case 'U': case 'u': dir = UP; break;
            case 'D': case 'd': dir = DOWN; break;
            case 'L': case 'l': dir = LEFT; break;
            case 'R': case 'r': dir = RIGHT; break;
            default: printf("Gecersiz yon!\n"); continue;
        }

        if (!move_snake(dir)) break;
    }
}

int main() {
    srand(time(NULL));
    initialize_game();
    print_board();
    play_game();

    printf("Oyun Bitti. Toplam Hamle: %d, Yilan Buyuklugu: %d, Kalan Yemek: %d\n", 
           move_count, snake_size, food_count);
    return 0;
}

