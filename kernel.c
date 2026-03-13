#define VIDEO_MEMORY (char*)0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLUE 0x1F
#define GRAY_ON_BLACK 0x07
#define YELLOW_ON_BLACK 0x0E
#define GREEN 0x0A
#define CYAN 0x0B
#define RED_ON_BLACK 0x0C

// ============= ОБЪЯВЛЕНИЯ ФУНКЦИЙ =============
int str_compare(char* s1, char* s2);
int str_starts_with(char* s1, char* s2);
int str_compare_file(char* s1, char* s2);
void kprint(char* msg, char attr);
void kprint_char(char c, int x, int y, char attribute);
void port_byte_out(unsigned short port, unsigned char data);
unsigned char port_byte_in(unsigned short port);
void update_cursor(int x, int y);
void terminal_clear();
void scroll();
void execute_command(char* input);
void int_to_str(int num, char* str);
void cmd_date();
void beep();
void cmd_beep();
void snake_game();
void get_date(int* day, int* month, int* year);
int strlen(char* s);

// Объявления для файловой системы
typedef struct {
    char name[11];
    unsigned char attr;
    unsigned char reserved[10];
    unsigned short first_cluster;
    unsigned int size;
} FAT16_Entry;

FAT16_Entry* find_file(const char* filename);
void read_file(FAT16_Entry* file, unsigned char* buffer);
int create_file(const char* filename);

// Корневая директория
FAT16_Entry root_dir[512] = {0};

unsigned char file1_data[] = "Hello AXIS!\n";
unsigned char file2_data[] = "AXIS-OS v0.1\n";

int cursor_x = 0;
int cursor_y = 0;

char last_command[128];
char key_buffer[256]; 
int buffer_idx = 0;

// ============= ГРАФИЧЕСКАЯ ОБОЛОЧКА =============
#define GRAPHICS_MODE 0
#define TEXT_MODE 1
int g_mode = TEXT_MODE;
int g_width = 640;
int g_height = 480;

// Адрес фреймбуфера для Bochs
unsigned int* g_framebuffer = (unsigned int*)0xE0000000;

// Графический курсор
int g_cursor_x = 10;
int g_cursor_y = 10;
char g_input_buffer[256];
int g_buffer_idx = 0;
char g_last_command[128];

// ============= КАЛЬКУЛЯТОР (БЕЗ FLOAT) =============
int calc_mode = 0;
char calc_display[32];
int calc_result = 0;      // Целое число (умножено на 100 для 2 знаков после запятой)
int calc_memory = 0;
char calc_op = 0;
int calc_current = 0;
int calc_new_number = 1;

// ============= ПРОСТОЙ ШРИФТ 8x8 =============
unsigned char simple_font[128][8] = {
    // Пробел (32)
    [32] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    },
    
    // Цифры 0-9
    [48] = {0b00111100, 0b01100110, 0b01101110, 0b01111110, 0b01110110, 0b01100110, 0b00111100, 0b00000000}, // '0'
    [49] = {0b00011000, 0b00111000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b01111110, 0b00000000}, // '1'
    [50] = {0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00111000, 0b01100000, 0b01111110, 0b00000000}, // '2'
    [51] = {0b00111100, 0b01100110, 0b00000110, 0b00011100, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // '3'
    [52] = {0b00001100, 0b00011100, 0b00111100, 0b01101100, 0b01111110, 0b00001100, 0b00001100, 0b00000000}, // '4'
    [53] = {0b01111110, 0b01100000, 0b01111100, 0b00000110, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // '5'
    [54] = {0b00111100, 0b01100110, 0b01100000, 0b01111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // '6'
    [55] = {0b01111110, 0b01100110, 0b00000110, 0b00001100, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // '7'
    [56] = {0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // '8'
    [57] = {0b00111100, 0b01100110, 0b01100110, 0b00111110, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // '9'
    
    // Буквы A-Z (верхний регистр)
    [65] = {0b00111100, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'A'
    [66] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b00000000}, // 'B'
    [67] = {0b00111100, 0b01100110, 0b01100000, 0b01100000, 0b01100000, 0b01100110, 0b00111100, 0b00000000}, // 'C'
    [68] = {0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01111100, 0b00000000}, // 'D'
    [69] = {0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000, 0b01100000, 0b01111110, 0b00000000}, // 'E'
    [70] = {0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000}, // 'F'
    [71] = {0b00111100, 0b01100110, 0b01100000, 0b01101110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'G'
    [72] = {0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'H'
    [73] = {0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // 'I'
    [74] = {0b00011110, 0b00001100, 0b00001100, 0b00001100, 0b01101100, 0b01101100, 0b00111000, 0b00000000}, // 'J'
    [75] = {0b01100110, 0b01101100, 0b01111000, 0b01110000, 0b01111000, 0b01101100, 0b01100110, 0b00000000}, // 'K'
    [76] = {0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01111110, 0b00000000}, // 'L'
    [77] = {0b01100110, 0b01111110, 0b01111110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'M'
    [78] = {0b01100110, 0b01110110, 0b01111110, 0b01111110, 0b01101110, 0b01100110, 0b01100110, 0b00000000}, // 'N'
    [79] = {0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'O'
    [80] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000}, // 'P'
    [81] = {0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b00111100, 0b00000110}, // 'Q'
    [82] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01111000, 0b01101100, 0b01100110, 0b00000000}, // 'R'
    [83] = {0b00111100, 0b01100110, 0b01100000, 0b00111100, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // 'S'
    [84] = {0b01111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // 'T'
    [85] = {0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'U'
    [86] = {0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00000000}, // 'V'
    [87] = {0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b01111110, 0b01111110, 0b01100110, 0b00000000}, // 'W'
    [88] = {0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00111100, 0b01100110, 0b01100110, 0b00000000}, // 'X'
    [89] = {0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // 'Y'
    [90] = {0b01111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b01111110, 0b00000000}, // 'Z'
    
    // Строчные буквы a-z (упрощенно как заглавные)
    [97] = {0b00111100, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'a'
    [98] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b00000000}, // 'b'
    [99] = {0b00111100, 0b01100110, 0b01100000, 0b01100000, 0b01100000, 0b01100110, 0b00111100, 0b00000000}, // 'c'
    [100] = {0b01111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01111100, 0b00000000}, // 'd'
    [101] = {0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000, 0b01100000, 0b01111110, 0b00000000}, // 'e'
    [102] = {0b01111110, 0b01100000, 0b01100000, 0b01111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000}, // 'f'
    [103] = {0b00111100, 0b01100110, 0b01100000, 0b01101110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'g'
    [104] = {0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'h'
    [105] = {0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00111100, 0b00000000}, // 'i'
    [106] = {0b00011110, 0b00001100, 0b00001100, 0b00001100, 0b01101100, 0b01101100, 0b00111000, 0b00000000}, // 'j'
    [107] = {0b01100110, 0b01101100, 0b01111000, 0b01110000, 0b01111000, 0b01101100, 0b01100110, 0b00000000}, // 'k'
    [108] = {0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01100000, 0b01111110, 0b00000000}, // 'l'
    [109] = {0b01100110, 0b01111110, 0b01111110, 0b01111110, 0b01100110, 0b01100110, 0b01100110, 0b00000000}, // 'm'
    [110] = {0b01100110, 0b01110110, 0b01111110, 0b01111110, 0b01101110, 0b01100110, 0b01100110, 0b00000000}, // 'n'
    [111] = {0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'o'
    [112] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01100000, 0b01100000, 0b01100000, 0b00000000}, // 'p'
    [113] = {0b00111100, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b00111100, 0b00000110}, // 'q'
    [114] = {0b01111100, 0b01100110, 0b01100110, 0b01111100, 0b01111000, 0b01101100, 0b01100110, 0b00000000}, // 'r'
    [115] = {0b00111100, 0b01100110, 0b01100000, 0b00111100, 0b00000110, 0b01100110, 0b00111100, 0b00000000}, // 's'
    [116] = {0b01111110, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // 't'
    [117] = {0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00000000}, // 'u'
    [118] = {0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00000000}, // 'v'
    [119] = {0b01100110, 0b01100110, 0b01100110, 0b01111110, 0b01111110, 0b01111110, 0b01100110, 0b00000000}, // 'w'
    [120] = {0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00111100, 0b01100110, 0b01100110, 0b00000000}, // 'x'
    [121] = {0b01100110, 0b01100110, 0b01100110, 0b00111100, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // 'y'
    [122] = {0b01111110, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b01111110, 0b00000000}, // 'z'
    
    // Специальные символы
    ['.'] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000},
    [','] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00110000, 0b00000000},
    ['-'] = {0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    ['_'] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000},
    ['+'] = {0b00000000, 0b00011000, 0b00011000, 0b01111110, 0b00011000, 0b00011000, 0b00000000, 0b00000000},
    ['='] = {0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000},
    ['/'] = {0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b01000000, 0b00000000, 0b00000000},
    ['\\'] = {0b01100000, 0b00110000, 0b00011000, 0b00001100, 0b00000110, 0b00000010, 0b00000000, 0b00000000},
    ['|'] = {0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000},
    ['*'] = {0b00000000, 0b01100110, 0b00111100, 0b01111110, 0b00111100, 0b01100110, 0b00000000, 0b00000000},
    ['!'] = {0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00000000, 0b00000000},
    ['?'] = {0b00111100, 0b01100110, 0b00000110, 0b00001100, 0b00011000, 0b00000000, 0b00011000, 0b00000000},
    [':'] = {0b00000000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00011000, 0b00000000, 0b00000000},
    [';'] = {0b00000000, 0b00011000, 0b00011000, 0b00000000, 0b00011000, 0b00110000, 0b00000000, 0b00000000},
    ['"'] = {0b01100110, 0b01100110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    ['\''] = {0b00011000, 0b00011000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    ['('] = {0b00001100, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00001100, 0b00000000},
    [')'] = {0b00110000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00110000, 0b00000000},
    ['['] = {0b00111100, 0b00110000, 0b00110000, 0b00110000, 0b00110000, 0b00110000, 0b00111100, 0b00000000},
    [']'] = {0b00111100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00001100, 0b00111100, 0b00000000}
};

// ============= ГРАФИЧЕСКИЕ ФУНКЦИИ =============

// Рисование пикселя
void g_put_pixel(int x, int y, unsigned int color) {
    if (x >= 0 && x < g_width && y >= 0 && y < g_height) {
        g_framebuffer[y * g_width + x] = color;
    }
}

// Очистка экрана в графическом режиме
void g_clear_screen(unsigned int color) {
    for (int i = 0; i < g_width * g_height; i++) {
        g_framebuffer[i] = color;
    }
}

// Вывод символа в графическом режиме
void g_put_char(char c, int x, int y, unsigned int color) {
    if (c < 32 || c > 127) c = '?';
    
    unsigned char* glyph = simple_font[(int)c];
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (0x80 >> col)) {
                g_put_pixel(x + col, y + row, color);
            }
        }
    }
}

// Вывод строки в графическом режиме
void g_print(char* str, int x, int y, unsigned int color) {
    int i = 0;
    int start_x = x;
    
    while (str[i] != 0) {
        g_put_char(str[i], x, y, color);
        x += 8;
        i++;
        
        if (x + 8 > g_width) {
            x = start_x;
            y += 10;
        }
    }
}

// Рисование рамки
void g_draw_rect_outline(int x, int y, int w, int h, unsigned int color) {
    for (int i = 0; i < w; i++) {
        g_put_pixel(x + i, y, color);
        g_put_pixel(x + i, y + h - 1, color);
    }
    for (int j = 0; j < h; j++) {
        g_put_pixel(x, y + j, color);
        g_put_pixel(x + w - 1, y + j, color);
    }
}

// Рисование закрашенного прямоугольника
void g_draw_rect_filled(int x, int y, int w, int h, unsigned int color) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            g_put_pixel(x + i, y + j, color);
        }
    }
}

// Функция для преобразования целого числа (с 2 знаками после запятой) в строку
void int_to_str_with_frac(int num, char* str) {
    int int_part = num / 100;
    int frac_part = num % 100;
    if (frac_part < 0) frac_part = -frac_part;
    
    int i = 0;
    
    // Обработка отрицательного числа
    if (num < 0) {
        str[i++] = '-';
        int_part = -int_part;
    }
    
    // Преобразование целой части
    char temp[16];
    int j = 0;
    if (int_part == 0) {
        temp[j++] = '0';
    } else {
        int n = int_part;
        while (n > 0) {
            temp[j++] = '0' + (n % 10);
            n /= 10;
        }
    }
    
    // Переворачиваем целую часть
    while (j > 0) {
        str[i++] = temp[--j];
    }
    
    // Добавляем дробную часть
    str[i++] = '.';
    
    // Всегда показываем две цифры после запятой
    str[i++] = '0' + (frac_part / 10);
    str[i++] = '0' + (frac_part % 10);
    
    str[i] = 0;
}

// ============= ФУНКЦИИ КАЛЬКУЛЯТОРА =============

void g_draw_calculator() {
    g_clear_screen(0x000000);
    
    // Основная рамка калькулятора
    g_draw_rect_outline(100, 50, 440, 380, 0xFFFFFF);
    g_draw_rect_filled(102, 52, 436, 376, 0x333333);
    
    // Заголовок
    g_print("AXIS-OS Calculator", 200, 70, 0xFFFF00);
    
    // Дисплей
    g_draw_rect_outline(120, 100, 400, 50, 0x00FF00);
    g_draw_rect_filled(122, 102, 396, 46, 0x000000);
    
    // Отображаем текущее значение
    int_to_str_with_frac(calc_result, calc_display);
    g_print(calc_display, 130, 120, 0x00FF00);
    
    // Координаты для кнопок
    int bx = 120;  // начальный X
    int by = 170;  // начальный Y
    int bw = 70;   // ширина кнопки
    int bh = 50;   // высота кнопки
    int spacing = 10; // расстояние между кнопками
    
    // ============= СТРОКА 1: 7 8 9 / C =============
    // Кнопка 7
    g_draw_rect_outline(bx, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx+2, by+2, bw-4, bh-4, 0x666666);
    g_print("7", bx+30, by+20, 0xFFFFFF);
    
    // Кнопка 8
    g_draw_rect_outline(bx + bw + spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + bw + spacing + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("8", bx + bw + spacing + 30, by+20, 0xFFFFFF);
    
    // Кнопка 9
    g_draw_rect_outline(bx + 2*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 2*(bw + spacing) + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("9", bx + 2*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка /
    g_draw_rect_outline(bx + 3*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 3*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA0000);
    g_print("/", bx + 3*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка C (Clear)
    g_draw_rect_outline(bx + 4*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 4*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA5500);
    g_print("C", bx + 4*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // ============= СТРОКА 2: 4 5 6 * =============
    by += bh + spacing;
    
    // Кнопка 4
    g_draw_rect_outline(bx, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx+2, by+2, bw-4, bh-4, 0x666666);
    g_print("4", bx+30, by+20, 0xFFFFFF);
    
    // Кнопка 5
    g_draw_rect_outline(bx + bw + spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + bw + spacing + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("5", bx + bw + spacing + 30, by+20, 0xFFFFFF);
    
    // Кнопка 6
    g_draw_rect_outline(bx + 2*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 2*(bw + spacing) + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("6", bx + 2*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка *
    g_draw_rect_outline(bx + 3*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 3*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA0000);
    g_print("*", bx + 3*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка M (Memory save)
    g_draw_rect_outline(bx + 4*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 4*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA5500);
    g_print("M", bx + 4*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // ============= СТРОКА 3: 1 2 3 - =============
    by += bh + spacing;
    
    // Кнопка 1
    g_draw_rect_outline(bx, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx+2, by+2, bw-4, bh-4, 0x666666);
    g_print("1", bx+30, by+20, 0xFFFFFF);
    
    // Кнопка 2
    g_draw_rect_outline(bx + bw + spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + bw + spacing + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("2", bx + bw + spacing + 30, by+20, 0xFFFFFF);
    
    // Кнопка 3
    g_draw_rect_outline(bx + 2*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 2*(bw + spacing) + 2, by+2, bw-4, bh-4, 0x666666);
    g_print("3", bx + 2*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка -
    g_draw_rect_outline(bx + 3*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 3*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA0000);
    g_print("-", bx + 3*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // Кнопка R (Memory recall)
    g_draw_rect_outline(bx + 4*(bw + spacing), by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 4*(bw + spacing) + 2, by+2, bw-4, bh-4, 0xAA5500);
    g_print("R", bx + 4*(bw + spacing) + 30, by+20, 0xFFFFFF);
    
    // ============= СТРОКА 4: 0 . = + =============
    by += bh + spacing;
    
    // Кнопка 0 (шире)
    g_draw_rect_outline(bx, by, 2*bw + spacing, bh, 0xFFFFFF);
    g_draw_rect_filled(bx+2, by+2, 2*bw + spacing - 4, bh-4, 0x666666);
    g_print("0", bx + (bw + spacing/2), by+20, 0xFFFFFF);
    
    // Кнопка .
    g_draw_rect_outline(bx + 2*bw + 2*spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 2*bw + 2*spacing + 2, by+2, bw-4, bh-4, 0x666666);
    g_print(".", bx + 2*bw + 2*spacing + 30, by+20, 0xFFFFFF);
    
    // Кнопка =
    g_draw_rect_outline(bx + 3*bw + 3*spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 3*bw + 3*spacing + 2, by+2, bw-4, bh-4, 0x00AA00);
    g_print("=", bx + 3*bw + 3*spacing + 30, by+20, 0xFFFFFF);
    
    // Кнопка +
    g_draw_rect_outline(bx + 4*bw + 4*spacing, by, bw, bh, 0xFFFFFF);
    g_draw_rect_filled(bx + 4*bw + 4*spacing + 2, by+2, bw-4, bh-4, 0xAA0000);
    g_print("+", bx + 4*bw + 4*spacing + 30, by+20, 0xFFFFFF);
    
    // Инструкция
    g_print("Use keyboard: Numbers, + - * / . =, C, M, R", 120, 430, 0xAAAAAA);
    g_print("Press ESC to exit calculator", 120, 445, 0xAAAAAA);
}

void g_calc_handle_key(char key) {
    // Обработка цифр
    if (key >= '0' && key <= '9') {
        if (calc_new_number) {
            // Первая цифра: 5 -> 500 (5.00)
            calc_result = (key - '0') * 100;
            calc_new_number = 0;
        } else {
            // Добавляем цифру: 5 (500) + 3 -> 5300? Нет, нужно 5300 что означает 53.00
            // Но нам нужно 5.3 -> 530
            // Текущее число 500 (5.00) умножаем на 10 -> 5000 (50.00), потом добавляем 3*100 -> 300, итого 5300 (53.00) - это неправильно!
            
            // Правильно: 5.00 (500) + 3 -> 5.3 (530)
            calc_result = calc_result * 10 + (key - '0') * 100;
            // Но это даёт слишком большое число! Нужно делить на 10?
            
            // Давай сделаем по-другому:
            // Храним число как есть, без умножения на 100, а умножение делаем только при выводе
        }
    }
    // Обработка операций
    else if (key == '+' || key == '-' || key == '*' || key == '/' || 
             key == '=' || key == '\n') {
        
        if (key == '=' || key == '\n') {
            // Вычисление результата
            if (calc_op != 0) {
                switch (calc_op) {
                    case '+': 
                        calc_result = calc_current + calc_result; 
                        break;
                    case '-': 
                        calc_result = calc_current - calc_result; 
                        break;
                    case '*': 
                        calc_result = (calc_current * calc_result) / 100; 
                        break;
                    case '/': 
                        if (calc_result != 0) {
                            calc_result = (calc_current * 100) / calc_result; 
                        } else {
                            calc_result = 0;
                        }
                        break;
                }
                calc_op = 0;
            }
            calc_new_number = 1;
        } 
        else if (key == '+' || key == '-' || key == '*' || key == '/') {
            // Сохранение операции
            if (calc_op != 0 && !calc_new_number) {
                switch (calc_op) {
                    case '+': 
                        calc_result = calc_current + calc_result; 
                        break;
                    case '-': 
                        calc_result = calc_current - calc_result; 
                        break;
                    case '*': 
                        calc_result = (calc_current * calc_result) / 100; 
                        break;
                    case '/': 
                        if (calc_result != 0) {
                            calc_result = (calc_current * 100) / calc_result; 
                        } else {
                            calc_result = 0;
                        }
                        break;
                }
            }
            calc_current = calc_result;
            calc_op = key;
            calc_new_number = 1;
        }
    }
    // Обработка специальных команд
    else if (key == 'c' || key == 'C') {
        calc_result = 0;
        calc_current = 0;
        calc_op = 0;
        calc_new_number = 1;
    }
    else if (key == 'm' || key == 'M') {
        calc_memory = calc_result;
    }
    else if (key == 'r' || key == 'R') {
        calc_result = calc_memory;
        calc_new_number = 1;
    }
    
    // Обновляем дисплей
    g_draw_rect_filled(122, 102, 396, 46, 0x000000);
    int_to_str_with_frac(calc_result, calc_display);
    g_print(calc_display, 130, 120, 0x00FF00);
}

void cmd_calc() {
    if (g_mode != GRAPHICS_MODE) {
        kprint("Switch to graphics mode first: 'gui'\n", RED_ON_BLACK);
        return;
    }
    
    calc_mode = 1;
    calc_result = 0;
    calc_current = 0;
    calc_op = 0;
    calc_new_number = 1;
    calc_memory = 0;
    
    g_draw_calculator();
}

// Функция для переключения в графический режим через VBE
void vbe_set_mode() {
    port_byte_out(0x1CE, 0x4);
    port_byte_out(0x1CF, 0x00);
    
    for (volatile int i = 0; i < 1000; i++);
    
    port_byte_out(0x1CE, 0x1);
    __asm__ volatile (
        "movw $0x0280, %%ax\n"
        "out %%ax, %%dx"
        : 
        : "d" (0x1CF)
        : "ax"
    );
    
    port_byte_out(0x1CE, 0x2);
    __asm__ volatile (
        "movw $0x01E0, %%ax\n"
        "out %%ax, %%dx"
        : 
        : "d" (0x1CF)
        : "ax"
    );
    
    port_byte_out(0x1CE, 0x3);
    port_byte_out(0x1CF, 32);
    
    port_byte_out(0x1CE, 0x4);
    port_byte_out(0x1CF, 0x41);
    
    for (volatile int i = 0; i < 10000; i++);
}

// Команда для переключения в графику (КРАСИВЫЙ И РАБОЧИЙ ВАРИАНТ)
void cmd_gui() {
    kprint("Switching to graphics mode...\n", 0x0A);
    kprint("Framebuffer: 0xE0000000\n", 0x0A);
    
    vbe_set_mode();
    
    // Создаем красивый градиентный фон
    for (int y = 0; y < g_height; y++) {
        // Градиент от синего к голубому
        unsigned int blue = 200 - (y * 150 / g_height);
        unsigned int green = 50 + (y * 100 / g_height);
        unsigned int color = (green << 8) | blue;
        
        for (int x = 0; x < g_width; x++) {
            g_framebuffer[y * g_width + x] = color;
        }
    }
    
    // Рисуем красивое солнце
    int sun_x = 550, sun_y = 80, sun_r = 40;
    for (int y = sun_y - sun_r; y < sun_y + sun_r; y++) {
        for (int x = sun_x - sun_r; x < sun_x + sun_r; x++) {
            int dx = x - sun_x;
            int dy = y - sun_y;
            if (dx*dx + dy*dy <= sun_r*sun_r) {
                int dist = dx*dx + dy*dy;
                unsigned int color = 0xFFFF00 - (dist / 20);
                if (x >= 0 && x < g_width && y >= 0 && y < g_height) {
                    g_put_pixel(x, y, color);
                }
            }
        }
    }
    
    // Рисуем облака
    for (int cloud = 0; cloud < 3; cloud++) {
        int cloud_x = 100 + cloud * 200;
        int cloud_y = 100 + cloud * 20;
        for (int dy = 0; dy < 15; dy++) {
            for (int dx = 0; dx < 40; dx++) {
                int dist = (dx - 20)*(dx - 20) + (dy - 7)*(dy - 7);
                if (dist < 100) {
                    g_put_pixel(cloud_x + dx, cloud_y + dy, 0xFFFFFF);
                }
            }
        }
    }
    
    // Рисуем горы
    for (int y = 350; y < g_height; y++) {
        for (int x = 0; x < g_width; x++) {
            int mountain_height = 350 + ((x % 200) / 10);
            if (y > mountain_height) {
                unsigned int color = 0x228822;
                if (y > 400) color = 0x448844;
                if (y > 450) color = 0x66AA66;
                g_put_pixel(x, y, color);
            }
        }
    }
    
    // Рисуем звезды
    for (int i = 0; i < 30; i++) {
        int x = (i * 17) % g_width;
        int y = (i * 23) % 200;
        g_put_pixel(x, y, 0xFFFFFF);
    }
    
    // Заголовок с тенью
    g_print("AXIS-OS Graphics Mode", 202, 52, 0x000000);
    g_print("AXIS-OS Graphics Mode", 200, 50, 0xFFFFFF);
    
    g_print("Version 0.1a - Beautiful Edition", 210, 80, 0xFFFF00);
    
    // Простая рамка
    g_draw_rect_outline(0, 0, g_width, g_height, 0xFFFFFF);
    g_draw_rect_outline(2, 2, g_width-4, g_height-4, 0xAAAAAA);
    
    // Область для ввода команд
    g_draw_rect_filled(10, g_height - 70, g_width - 20, 40, 0x000000);
    g_draw_rect_outline(10, g_height - 70, g_width - 20, 40, 0x00FF00);
    
    g_print("root@axis:~#", 20, g_height - 55, 0x00FF00);
    
    // Инициализируем графический ввод
    g_cursor_x = 20 + 13*8;
    g_cursor_y = g_height - 55;
    g_buffer_idx = 0;
    g_input_buffer[0] = 0;
    calc_mode = 0;
    
    kprint("Graphics mode ready! Type commands directly on screen.\n", 0x0A);
    kprint("Press ESC to return to text mode.\n", 0x0A);
    
    g_mode = GRAPHICS_MODE;
}

void cmd_text() {
    g_mode = TEXT_MODE;
    terminal_clear();
    kprint("root@axis:~# ", CYAN);
}

void cmd_gtest() {
    if (g_mode != GRAPHICS_MODE) {
        kprint("Switch to graphics mode first: 'gui'\n", RED_ON_BLACK);
        return;
    }
    
    calc_mode = 0;
    cmd_gui();
}

void cmd_gcls() {
    if (g_mode != GRAPHICS_MODE) {
        kprint("Switch to graphics mode first: 'gui'\n", RED_ON_BLACK);
        return;
    }
    
    calc_mode = 0;
    cmd_gui();
}

// Функция для преобразования сканкода в символ
char scancode_to_ascii(unsigned char scancode) {
    static const char scancode_table[128] = {
        0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0,   ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    if (scancode < 128) {
        return scancode_table[scancode];
    }
    return 0;
}

// Выполнение команды в графическом режиме (ИСПРАВЛЕННАЯ С СКРОЛЛИНГОМ)
void g_execute_command(char* input) {
    // Проверяем специальные команды
    if (str_compare(input, "text")) {
        cmd_text();
        return;
    }
    
    if (str_compare(input, "calc")) {
        cmd_calc();
        return;
    }
    
    if (str_compare(input, "gtest") || str_compare(input, "gcls")) {
        cmd_gui();
        return;
    }
    
    // Стираем введенную команду
    for (int i = 0; i < g_buffer_idx; i++) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                g_put_pixel(g_cursor_x - (g_buffer_idx - i)*8 + col, 
                           g_cursor_y + row, 0x000000);
            }
        }
    }
    
    // Переходим на новую строку
    g_cursor_y += 10;
    g_cursor_x = 20;
    
    // Проверка на скроллинг - если вышли за пределы области ввода
    if (g_cursor_y > g_height - 80) {
        // Очищаем всю область вывода (выше строки ввода)
        for (int y = 20; y < g_height - 80; y++) {
            for (int x = 10; x < g_width - 10; x++) {
                g_put_pixel(x, y, 0x000000);
            }
        }
        g_cursor_y = 20;
    }
    
    // Обработка команд
    if (str_compare(input, "help")) {
        g_print("Available commands:", g_cursor_x, g_cursor_y, 0xFFFF00);
        g_cursor_y += 10;
        g_print("  help  - show this help", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  calc  - start calculator", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  gtest - restart graphics", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  text  - return to text mode", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  ver   - show version", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  beep  - make a beep sound", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  ls    - list files", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  cat   - show file content", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  touch - create file", g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
        g_print("  echo  - print text", g_cursor_x, g_cursor_y, 0xFFFFFF);
	    g_cursor_y += 10;
    }
    else if (str_compare(input, "ver")) {
        g_print("AXIS-OS v0.1a - Beautiful Edition", g_cursor_x, g_cursor_y, 0x00FF00);
        g_cursor_y += 10;
    }
    else if (str_compare(input, "beep")) {
        beep();
        g_print("Beep!", g_cursor_x, g_cursor_y, 0x00FF00);
        g_cursor_y += 10;
    }
    else if (str_compare(input, "ls")) {
        int found = 0;
        for (int i = 0; i < 512; i++) {
            if (root_dir[i].name[0] == 0) break;
            found = 1;
            char filename[12] = {0};
            int j;
            for (j = 0; j < 8; j++) {
                if (root_dir[i].name[j] == ' ') break;
                filename[j] = root_dir[i].name[j];
            }
            if (root_dir[i].name[8] != ' ') {
                filename[j++] = '.';
                for (int k = 8; k < 11; k++) {
                    if (root_dir[i].name[k] == ' ') break;
                    filename[j++] = root_dir[i].name[k];
                }
            }
            g_print(filename, g_cursor_x, g_cursor_y, 0xFFFFFF);
            g_cursor_y += 10;
            
            if (g_cursor_y > g_height - 80) {
                // Скроллинг при выводе ls
                for (int y = 20; y < g_height - 80; y++) {
                    for (int x = 10; x < g_width - 10; x++) {
                        g_put_pixel(x, y, 0x000000);
                    }
                }
                g_cursor_y = 20;
            }
        }
        if (!found) {
            g_print("No files found", g_cursor_x, g_cursor_y, 0xFFFF00);
            g_cursor_y += 10;
        }
    }
    else if (str_starts_with(input, "cat ")) {
        char* fname = &input[4];
        FAT16_Entry* f = find_file(fname);
        if (f) {
            unsigned char buf[512];
            read_file(f, buf);
            g_print((char*)buf, g_cursor_x, g_cursor_y, 0xFFFFFF);
            g_cursor_y += 10;
        } else {
            g_print("File not found", g_cursor_x, g_cursor_y, 0xFF0000);
            g_cursor_y += 10;
        }
    }
    else if (str_starts_with(input, "echo ")) {
        g_print(&input[5], g_cursor_x, g_cursor_y, 0xFFFFFF);
        g_cursor_y += 10;
    }
    else if (str_starts_with(input, "touch ")) {
        char* fname = &input[6];
        if (create_file(fname)) {
            g_print("File created: ", g_cursor_x, g_cursor_y, 0x00FF00);
            g_print(fname, g_cursor_x + 14*8, g_cursor_y, 0xFFFFFF);
            g_cursor_y += 10;
        } else {
            g_print("Error creating file", g_cursor_x, g_cursor_y, 0xFF0000);
            g_cursor_y += 10;
        }
    }
    else if (str_compare(input, "gui") || str_compare(input, "gmode")) {
        g_print("Already in graphics mode!", g_cursor_x, g_cursor_y, 0x00FF00);
        g_cursor_y += 10;
    }
    else if (input[0] != 0) {
        g_print("Unknown command: ", g_cursor_x, g_cursor_y, 0xFF0000);
        g_print(input, g_cursor_x + 17*8, g_cursor_y, 0xFF0000);
        g_cursor_y += 10;
    }
    
    // Финальная проверка скроллинга
    if (g_cursor_y > g_height - 80) {
        // Очищаем область вывода
        for (int y = 20; y < g_height - 80; y++) {
            for (int x = 10; x < g_width - 10; x++) {
                g_put_pixel(x, y, 0x000000);
            }
        }
        g_cursor_y = 20;
    }
    
    // Новый промпт
    g_print("root@axis:~#", g_cursor_x, g_cursor_y, 0x00FF00);
    g_cursor_x += 13 * 8;
    g_buffer_idx = 0;
    g_input_buffer[0] = 0;
}

// Обработка клавиш в графике
void g_handle_key(unsigned char scancode) {
    // ESC - выход в текстовый режим
    if (scancode == 0x01) {
        g_mode = TEXT_MODE;
        calc_mode = 0;
        terminal_clear();
        cursor_x = 0;
        cursor_y = 0;
        kprint("root@axis:~# ", CYAN);
        update_cursor(cursor_x, cursor_y);
        buffer_idx = 0;
        key_buffer[0] = 0;
        g_cursor_x = 10;
        g_cursor_y = 10;
        g_buffer_idx = 0;
        g_input_buffer[0] = 0;
        return;
    }
    
    if (scancode >= 0x80) return;
    
    char key = scancode_to_ascii(scancode);
    
    if (calc_mode) {
        if (key != 0) {
            g_calc_handle_key(key);
        }
        return;
    }
    
    if (key != 0) {
        if (key == '\n') {
            g_input_buffer[g_buffer_idx] = 0;
            
            for (int i = 0; i < 128; i++)
                g_last_command[i] = 0;
            for (int i = 0; i < g_buffer_idx && i < 127; i++)
                g_last_command[i] = g_input_buffer[i];
            
            g_execute_command(g_input_buffer);
        }
        else if (key == '\b') {
            if (g_buffer_idx > 0) {
                g_buffer_idx--;
                g_input_buffer[g_buffer_idx] = 0;
                g_cursor_x -= 8;
                
                for (int row = 0; row < 8; row++) {
                    for (int col = 0; col < 8; col++) {
                        g_put_pixel(g_cursor_x + col, g_cursor_y + row, 0x000000);
                    }
                }
            }
        }
        else {
            if (g_buffer_idx < 255) {
                g_input_buffer[g_buffer_idx] = key;
                g_put_char(key, g_cursor_x, g_cursor_y, 0xFFFFFF);
                g_buffer_idx++;
                g_input_buffer[g_buffer_idx] = 0;
                g_cursor_x += 8;
            }
        }
    }
}

// ============= ФУНКЦИИ ПОРТОВ =============
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "dN" (port));
}

void* memcpy(void* dest, const void* src, unsigned int n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (unsigned int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int strlen(char* s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

void int_to_str(int num, char* str) {
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    do {
        str[i++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);
    
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = 0;
    
    for (int j = 0; j < i/2; j++) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
}

// ============= ФУНКЦИИ ДЛЯ RTC =============
unsigned char cmos_read(unsigned char reg) {
    port_byte_out(0x70, reg);
    return port_byte_in(0x71);
}

unsigned char bcd_to_dec(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void get_date(int* day, int* month, int* year) {
    while (cmos_read(0x0A) & 0x80);
    
    *day = bcd_to_dec(cmos_read(0x07));
    *month = bcd_to_dec(cmos_read(0x08));
    *year = bcd_to_dec(cmos_read(0x09)) + 2000;
}

void cmd_date() {
    int day, month, year;
    
    get_date(&day, &month, &year);
    
    char num_str[4];
    
    int_to_str(day, num_str);
    if (day < 10) kprint("0", YELLOW_ON_BLACK);
    kprint(num_str, YELLOW_ON_BLACK);
    kprint(".", YELLOW_ON_BLACK);
    
    int_to_str(month, num_str);
    if (month < 10) kprint("0", YELLOW_ON_BLACK);
    kprint(num_str, YELLOW_ON_BLACK);
    kprint(".", YELLOW_ON_BLACK);
    
    int_to_str(year, num_str);
    kprint(num_str, YELLOW_ON_BLACK);
    kprint(" ", YELLOW_ON_BLACK);

    kprint("\n", YELLOW_ON_BLACK);
}

void update_cursor(int x, int y) {
    unsigned short pos = y * SCREEN_WIDTH + x;
    port_byte_out(0x3D4, 0x0E);
    port_byte_out(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
    port_byte_out(0x3D4, 0x0F);
    port_byte_out(0x3D5, (unsigned char)(pos & 0xFF));
}

// ============= ФУНКЦИИ ФАЙЛОВОЙ СИСТЕМЫ =============
FAT16_Entry* find_file(const char* filename) {
    for (int i = 0; i < 512; i++) {
        if (root_dir[i].name[0] == 0) break;
        if (str_compare_file(root_dir[i].name, (char*)filename)) {
            return &root_dir[i];
        }
    }
    return 0;
}

void read_file(FAT16_Entry* file, unsigned char* buffer) {
    if (!file) return;

    if (file->first_cluster == 2) {
        memcpy(buffer, file1_data, file->size);
    } else if (file->first_cluster == 3) {
        memcpy(buffer, file2_data, file->size);
    }
}

int create_file(const char* filename) {
    int free_index = -1;

    for (int i = 0; i < 512; i++) {
        if (root_dir[i].name[0] == 0) {
            free_index = i;
            break;
        }
    }

    if (free_index == -1) {
        kprint("Disk full\n", RED_ON_BLACK);
        return 0;
    }

    char fat_name[11];
    for (int i = 0; i < 11; i++) fat_name[i] = ' ';

    int name_i = 0;
    int ext_i = 8;

    for (int i = 0; filename[i] != 0; i++) {
        if (filename[i] == '.') {
            ext_i = 8;
            continue;
        }

        if (ext_i == 8) {
            if (name_i < 8) {
                fat_name[name_i++] = filename[i];
            }
        } else {
            if (ext_i < 11) {
                fat_name[ext_i++] = filename[i];
            }
        }
    }

    memcpy(root_dir[free_index].name, fat_name, 11);
    root_dir[free_index].attr = 0x20;
    root_dir[free_index].first_cluster = 0;
    root_dir[free_index].size = 0;

    return 1;
}

void beep() {
    unsigned char tmp = port_byte_in(0x61);
    port_byte_out(0x61, tmp | 0x03);
    for (volatile int i = 0; i < 3000000; i++);
    port_byte_out(0x61, tmp & 0xFC);
}

void cmd_beep() {
    kprint("Beep!\n", GREEN);
    beep();
}

// ============= ВИДЕО ФУНКЦИИ =============
void kprint_char(char c, int x, int y, char attribute) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

    char* video = VIDEO_MEMORY;
    int offset = (y * SCREEN_WIDTH + x) * 2;

    video[offset] = c;
    video[offset + 1] = attribute;
}

void kprint_at(char* msg, int x, int y, char attribute) {
    int i = 0;
    while (msg[i] != 0) {
        kprint_char(msg[i], x + i, y, attribute);
        i++;
    }
}

void terminal_clear() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        kprint_char(' ', i % SCREEN_WIDTH, i / SCREEN_WIDTH, GRAY_ON_BLACK);
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(0, 0);
}

void scroll() {
    char* video = VIDEO_MEMORY;

    for (int y = 1; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int from = (y * SCREEN_WIDTH + x) * 2;
            int to   = ((y - 1) * SCREEN_WIDTH + x) * 2;

            video[to]     = video[from];
            video[to + 1] = video[from + 1];
        }
    }

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        int offset = ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x) * 2;
        video[offset] = ' ';
        video[offset + 1] = GRAY_ON_BLACK;
    }

    cursor_y = SCREEN_HEIGHT - 1;
}

void kprint(char* msg, char attr) {
    int i = 0;
    while (msg[i] != 0) {
        if (msg[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= SCREEN_HEIGHT) {
                scroll();
            }
        } else if (msg[i] == '\b') {
            if (cursor_x > 13) {
                cursor_x--;
                kprint_char(' ', cursor_x, cursor_y, attr);
            }
        } else {
            kprint_char(msg[i], cursor_x, cursor_y, attr);
            cursor_x++;
            if (cursor_x >= SCREEN_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
            if (cursor_y >= SCREEN_HEIGHT) {
                scroll();
            }
        }
        i++;
    }
    update_cursor(cursor_x, cursor_y);
}

// ============= СТРОКОВЫЕ ФУНКЦИИ =============
int str_compare(char* s1, char* s2) {
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0) {
        if (s1[i] != s2[i]) return 0;
        i++;
    }
    return s1[i] == s2[i];
}

int str_compare_file(char* s1, char* s2) {
    for (int i = 0; i < 11; i++) {
        char c1 = s1[i];
        char c2 = s2[i];

        if (c1 == ' ') c1 = 0;
        if (c2 == ' ' || c2 == '.') c2 = 0;

        if (c1 >= 'a' && c1 <= 'z') c1 -= 32;
        if (c2 >= 'a' && c2 <= 'z') c2 -= 32;

        if (c1 != c2) return 0;
        if (c1 == 0) break;
    }
    return 1;
}

int str_starts_with(char* s1, char* s2) {
    int i = 0;
    while (s2[i] != 0) {
        if (s1[i] != s2[i]) return 0;
        i++;
    }
    return 1;
}

// ============= ИГРА ЗМЕЙКА =============
#define SNAKE_MAX_LENGTH 256
#define SNAKE_HEAD 0x0E
#define SNAKE_BODY 0x0A
#define SNAKE_FOOD 0x0C
#define SNAKE_BORDER 0x0A

int snake_x[SNAKE_MAX_LENGTH];
int snake_y[SNAKE_MAX_LENGTH];
int snake_length;
int snake_direction;
int new_direction;
int food_x, food_y;
int snake_score;
int game_running;
int game_started;

void snake_clear_area() {
    for (int y = 1; y < SCREEN_HEIGHT - 1; y++) {
        for (int x = 1; x < SCREEN_WIDTH - 1; x++) {
            kprint_char(' ', x, y, GRAY_ON_BLACK);
        }
    }
}

void snake_draw_borders() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        kprint_char('#', x, 0, SNAKE_BORDER);
        kprint_char('#', x, SCREEN_HEIGHT - 1, SNAKE_BORDER);
    }
    
    for (int y = 1; y < SCREEN_HEIGHT - 1; y++) {
        kprint_char('#', 0, y, SNAKE_BORDER);
        kprint_char('#', SCREEN_WIDTH - 1, y, SNAKE_BORDER);
    }
}

void snake_init() {
    snake_length = 3;
    snake_direction = 1;
    new_direction = 1;
    
    snake_x[0] = 40; snake_y[0] = 12;
    snake_x[1] = 40; snake_y[1] = 13;
    snake_x[2] = 40; snake_y[2] = 14;
    
    snake_score = 0;
    game_running = 1;
    game_started = 0;
    
    food_x = 20;
    food_y = 10;
}

void snake_draw() {
    for (int i = 1; i < snake_length; i++) {
        kprint_char('o', snake_x[i], snake_y[i], SNAKE_BODY);
    }
    kprint_char('O', snake_x[0], snake_y[0], SNAKE_HEAD);
    kprint_char('@', food_x, food_y, SNAKE_FOOD);
}

void snake_move() {
    if ((new_direction == 0 && snake_direction != 1) ||
        (new_direction == 1 && snake_direction != 0) ||
        (new_direction == 2 && snake_direction != 3) ||
        (new_direction == 3 && snake_direction != 2)) {
        snake_direction = new_direction;
    }
    
    int new_x = snake_x[0];
    int new_y = snake_y[0];
    
    switch(snake_direction) {
        case 0: new_y--; break;
        case 1: new_y++; break;
        case 2: new_x--; break;
        case 3: new_x++; break;
    }
    
    int ate_food = (new_x == food_x && new_y == food_y);
    
    for (int i = snake_length - 1; i > 0; i--) {
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i-1];
    }
    
    snake_x[0] = new_x;
    snake_y[0] = new_y;
    
    if (ate_food) {
        snake_length++;
        snake_score += 10;
        
        if (snake_length < SNAKE_MAX_LENGTH) {
            snake_x[snake_length - 1] = snake_x[snake_length - 2];
            snake_y[snake_length - 1] = snake_y[snake_length - 2];
        }
        
        food_x = 15 + (snake_score % 50);
        food_y = 3 + (snake_score % 18);
    }
}

int snake_check_collision() {
    if (snake_x[0] <= 0 || snake_x[0] >= SCREEN_WIDTH - 1 ||
        snake_y[0] <= 0 || snake_y[0] >= SCREEN_HEIGHT - 1) {
        return 1;
    }
    
    for (int i = 3; i < snake_length; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
            return 1;
        }
    }
    return 0;
}

void snake_show_score() {
    char score_text[20] = "Score: ";
    char num_str[4];
    int_to_str(snake_score, num_str);
    
    for (int i = 0; i < 20; i++) {
        kprint_char(' ', 60 + i, 0, GRAY_ON_BLACK);
    }
    
    kprint_at(score_text, 60, 0, SNAKE_HEAD);
    kprint_at(num_str, 67, 0, SNAKE_HEAD);
}

void snake_handle_key() {
    if (port_byte_in(0x64) & 0x01) {
        unsigned char scancode = port_byte_in(0x60);
        
        if (scancode < 0x80) {
            if (scancode == 0x48)
                new_direction = 0;
            else if (scancode == 0x50)
                new_direction = 1;
            else if (scancode == 0x4B)
                new_direction = 2;
            else if (scancode == 0x4D)
                new_direction = 3;
            else if (scancode == 0x01)
                game_running = 0;
        }
    }
}

void snake_game() {
    int old_cursor_x = cursor_x;
    int old_cursor_y = cursor_y;
    
    terminal_clear();
    
    kprint_at("=== SNAKE GAME ===", 30, 1, SNAKE_HEAD);
    kprint_at("Use arrows to move, ESC to exit", 20, 2, GRAY_ON_BLACK);
    kprint_at("Press SPACE to start...", 25, 4, CYAN);
    
    while (port_byte_in(0x64) & 0x01) {
        port_byte_in(0x60);
    }
    
    int start = 0;
    while (!start) {
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = port_byte_in(0x60);
            if (scancode == 0x39) {
                start = 1;
            }
        }
        for (volatile int i = 0; i < 1000; i++);
    }
    
    while (port_byte_in(0x64) & 0x01) {
        port_byte_in(0x60);
    }
    
    snake_clear_area();
    snake_draw_borders();
    snake_init();
    snake_draw();
    snake_show_score();
    
    kprint_at("Press any arrow to start moving...", 20, 22, CYAN);
    
    int first_move = 0;
    while (!first_move && game_running) {
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = port_byte_in(0x60);
            if (scancode < 0x80) {
                if (scancode == 0x48 || scancode == 0x50 || 
                    scancode == 0x4B || scancode == 0x4D) {
                    if (scancode == 0x48) new_direction = 0;
                    else if (scancode == 0x50) new_direction = 1;
                    else if (scancode == 0x4B) new_direction = 2;
                    else if (scancode == 0x4D) new_direction = 3;
                    
                    snake_direction = new_direction;
                    first_move = 1;
                }
            }
        }
        for (volatile int i = 0; i < 1000; i++);
    }
    
    for (int i = 0; i < 50; i++) {
        kprint_char(' ', 20 + i, 22, GRAY_ON_BLACK);
    }
    
    int move_counter = 0;
    int move_delay = 1000;
    
    while (game_running) {
        snake_handle_key();
        
        if (++move_counter >= move_delay) {
            move_counter = 0;
            
            kprint_char(' ', snake_x[snake_length-1], snake_y[snake_length-1], GRAY_ON_BLACK);
            
            snake_move();
            
            if (snake_check_collision()) {
                break;
            }
            
            snake_draw();
            snake_show_score();
        }
        
        for (volatile int i = 0; i < 1000; i++);
    }
    
    terminal_clear();
    kprint_at("GAME OVER!", 35, 11, SNAKE_FOOD);
    kprint_at(" Your score: ", 32, 12, GRAY_ON_BLACK);
    
    char score_str[4];
    int_to_str(snake_score, score_str);
    kprint_at(score_str, 44, 12, SNAKE_HEAD);
    
    kprint_at("     Press SPACE to return to OS...", 20, 14, CYAN);
    
    while (port_byte_in(0x64) & 0x01) {
        port_byte_in(0x60);
    }
    
    start = 0;
    while (!start) {
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = port_byte_in(0x60);
            if (scancode == 0x39) {
                start = 1;
            }
        }
        for (volatile int i = 0; i < 1000; i++);
    }
    
    while (port_byte_in(0x64) & 0x01) {
        port_byte_in(0x60);
    }
    
    terminal_clear();
    cursor_x = 0;
    cursor_y = 0;
    
    kprint("=============================================\n", GRAY_ON_BLACK);
    kprint("                     AXIS-OS                 \n", GREEN);
    kprint("(c) AXIS-OS Corporation. All rights reserved.\n", GRAY_ON_BLACK); 
    kprint("=============================================\n", GRAY_ON_BLACK);
    kprint("Welcome to AXIS-OS!\n", WHITE_ON_BLUE);
    kprint("Enter 'help' for commands\n", WHITE_ON_BLUE);
    kprint("root@axis:~# ", CYAN);
    
    update_cursor(cursor_x, cursor_y);
}

// ============= КОМАНДЫ =============
void execute_command(char* input) {
    if (str_compare(input, "cls")) {
        terminal_clear();
        kprint("root@axis:~# ", CYAN);
    } 
    else if (str_compare(input, "help")) {
        kprint("AXIS-OS commands:\n", CYAN);
        kprint(" help     - show commands\n", GRAY_ON_BLACK);
        kprint(" cls      - clear screen\n", GRAY_ON_BLACK);
        kprint(" ver      - system version\n", GRAY_ON_BLACK);
        kprint(" date     - show current date\n", GRAY_ON_BLACK);
        kprint(" reboot   - restart system\n", GRAY_ON_BLACK);
        kprint(" shutdown - power off system\n", GRAY_ON_BLACK);
        kprint(" echo     - print text\n", GRAY_ON_BLACK);
        kprint(" ls       - list files\n", GRAY_ON_BLACK);
        kprint(" touch    - create file\n", GRAY_ON_BLACK);
        kprint(" cat      - show file content\n", GRAY_ON_BLACK);
        kprint(" beep     - make a beep sound\n", GRAY_ON_BLACK);
        kprint(" snake    - play snake game\n", GRAY_ON_BLACK);
        kprint(" gui      - start graphics mode\n", GRAY_ON_BLACK);
        kprint(" gtest    - restart graphics\n", GRAY_ON_BLACK);
        kprint(" calc     - graphical calculator\n", GRAY_ON_BLACK);
        kprint("root@axis:~# ", CYAN);
    } 
    else if (str_compare(input, "date")) {
        cmd_date();
        kprint("root@axis:~# ", CYAN);
    }
    else if (str_compare(input, "ver")) {
        kprint("AXIS-OS v0.1a - Beautiful Edition\n", GREEN);
        kprint("root@axis:~# ", CYAN);
    } 
    else if (str_compare(input, "reboot")) {
        kprint("Rebooting...\n", RED_ON_BLACK);
        for (volatile int i = 0; i < 20000000; i++);
        port_byte_out(0x64, 0xFE);
        while(1) __asm__ volatile("hlt");
    }
    else if (str_compare(input, "shutdown")) {
        kprint("Shutting down AXIS-OS...\n", RED_ON_BLACK);
        while(1) __asm__ volatile("hlt");
    }
    else if (str_compare(input, "gui")) {
        cmd_gui();
    }
    else if (str_compare(input, "beep")) {
        cmd_beep();
        kprint("root@axis:~# ", CYAN);
    }
    else if (str_compare(input, "snake")) {
        snake_game();
    }
    else if (str_compare(input, "gtest")) {
        cmd_gtest();
    }
    else if (str_compare(input, "calc")) {
        cmd_calc();
    }
    else if (str_starts_with(input, "echo ")) {
        kprint(&input[5], GRAY_ON_BLACK);
        kprint("\n", GRAY_ON_BLACK);
        kprint("root@axis:~# ", CYAN);
    }
    else if (str_starts_with(input, "touch ")) {
        char* fname = &input[6];

        if (create_file(fname)) {
            kprint("File created\n", GREEN);
        } else {
            kprint("Error creating file\n", RED_ON_BLACK);
        }

        kprint("root@axis:~# ", CYAN);
    }
    else if (str_compare(input, "ls")) {
        for (int i = 0; i < 512; i++) {
            if (root_dir[i].name[0] == 0) break;
            for (int j = 0; j < 11; j++) {
                if (root_dir[i].name[j] == ' ') continue;
                char temp[2] = {root_dir[i].name[j], 0};
                kprint(temp, GRAY_ON_BLACK);
            }
            kprint("\n", GRAY_ON_BLACK);
        }
        kprint("root@axis:~# ", CYAN);
    }
    else if (str_starts_with(input, "cat ")) {
        char* fname = &input[4];
        FAT16_Entry* f = find_file(fname);
        if (f) {
            unsigned char buf[512];
            read_file(f, buf);
            kprint((char*)buf, GRAY_ON_BLACK);
            kprint("\n", GRAY_ON_BLACK);
        } else {
            kprint("File not found\n", RED_ON_BLACK);
        }
        kprint("root@axis:~# ", CYAN);
    }
    else if (input[0] == 0) {
        kprint("root@axis:~# ", CYAN);
    }
    else {
        kprint("Error: '", RED_ON_BLACK);
        kprint(input, RED_ON_BLACK);
        kprint("' is not recognized.\n", RED_ON_BLACK);
        kprint("root@axis:~# ", CYAN);
    }
}

// ============= ТАБЛИЦА СКАНКОДОВ =============
static char scancode_to_char[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '+', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void sleep(int ticks) {
    for (volatile int i = 0; i < ticks * 5000000; i++) {}
}

// ============= ГЛАВНАЯ ФУНКЦИЯ =============
void kernel_main() {
    terminal_clear();
    
    // Инициализация файловой системы
    memcpy(root_dir[0].name, "HELLO   TXT", 11);
    root_dir[0].attr = 0x20;
    root_dir[0].first_cluster = 2;
    root_dir[0].size = sizeof(file1_data) - 1;

    memcpy(root_dir[1].name, "INFO    TXT", 11);
    root_dir[1].attr = 0x20;
    root_dir[1].first_cluster = 3;
    root_dir[1].size = sizeof(file2_data) - 1;
    
    // Загрузочный звук
    unsigned char tmp = port_byte_in(0x61);
    
    port_byte_out(0x43, 0xB6);
    port_byte_out(0x42, (1193180 / 1000) & 0xFF);
    port_byte_out(0x42, ((1193180 / 1000) >> 8) & 0xFF);
    port_byte_out(0x61, tmp | 0x03);
    for (volatile int i = 0; i < 1000000; i++);
    
    port_byte_out(0x42, (1193180 / 800) & 0xFF);
    port_byte_out(0x42, ((1193180 / 800) >> 8) & 0xFF);
    for (volatile int i = 0; i < 1000000; i++);
    
    port_byte_out(0x42, (1193180 / 600) & 0xFF);
    port_byte_out(0x42, ((1193180 / 600) >> 8) & 0xFF);
    for (volatile int i = 0; i < 1500000; i++);
    
    port_byte_out(0x61, tmp & 0xFC);
    
    // Логотип
    kprint_at("     ___           ___                       ___           ___           ___       ", 0, 3, YELLOW_ON_BLACK);
    kprint_at("    /\\  \\         |\\__\\          ___        /\\  \\         /\\  \\         /\\  \\      ", 0, 4, YELLOW_ON_BLACK);
    kprint_at("   /::\\  \\        |:|  |        /\\  \\      /::\\  \\       /::\\  \\       /::\\  \\     ", 0, 5, YELLOW_ON_BLACK);
    kprint_at("  /:/\\:\\  \\       |:|  |        \\:\\  \\    /:/\\ \\  \\     /:/\\:\\  \\     /:/\\ \\  \\    ", 0, 6, YELLOW_ON_BLACK);
    kprint_at(" /::\\~\\:\\  \\      |:|__|__      /::\\__\\  _\\:\\~\\ \\  \\   /:/  \\:\\  \\   _\\:\\~\\ \\  \\   ", 0, 7, YELLOW_ON_BLACK);
    kprint_at("/:/\\:\\ \\:\\__\\ ____/::::\\__\\  __/:/\\/__/ /\\ \\:\\ \\ \\__\\ /:/__/ \\:\\__\\ /\\ \\:\\ \\ \\__\\  ", 0, 8, YELLOW_ON_BLACK);
    kprint_at("\\/__\\:\\/:/  / \\::::/~~/~    /\\/:/  /    \\:\\ \\:\\ \\/__/ \\:\\  \\ /:/  / \\:\\ \\ \\/__/  ", 0, 9, YELLOW_ON_BLACK);
    kprint_at("     \\::/  /   ~~|:|~~|     \\::/__/      \\:\\ \\:\\__\\    \\:\\  /:/  /   \\:\\ \\:\\__\\    ", 0, 10, YELLOW_ON_BLACK);
    kprint_at("     /:/  /      |:|  |      \\:\\__\\       \\:\\/:/  /     \\:\\/:/  /     \\:\\/:/  /    ", 0, 11, YELLOW_ON_BLACK);
    kprint_at("    /:/  /       |:|  |       \\/__/        \\::/  /       \\::/  /       \\::/  /     ", 0, 12, YELLOW_ON_BLACK);
    kprint_at("    \\/__/         \\|__|                     \\/__/         \\/__/         \\/__/      ", 0, 13, YELLOW_ON_BLACK);

    kprint_at("Kernel status: OK (32-bit Protected Mode)", 19, 19, 0x0A);
    kprint_at("Welcome to AXIS Operating System v0.1", 21, 16, WHITE_ON_BLUE);

    sleep(3);
    terminal_clear();
    
    kprint("=============================================\n", GRAY_ON_BLACK);
    kprint("                     AXIS-OS                 \n", GREEN);
    kprint("(c) AXIS-OS Corporation. All rights reserved.\n", GRAY_ON_BLACK); 
    kprint("=============================================\n", GRAY_ON_BLACK);

    kprint("Welcome to AXIS-OS!\n", WHITE_ON_BLUE);
    kprint("Enter 'help' for commands\n", WHITE_ON_BLUE);
    
    kprint("root@axis:~# ", CYAN); 
    update_cursor(cursor_x, cursor_y);

    while(1) {
        if (g_mode == TEXT_MODE) {
            // Текстовый режим
            if (port_byte_in(0x64) & 0x01) {
                unsigned char scancode = port_byte_in(0x60);
                
                if (scancode == 0x48) {
                    while (buffer_idx > 0) {
                        buffer_idx--;
                        kprint("\b", GRAY_ON_BLACK);
                    }

                    int i = 0;
                    while (last_command[i] != 0) {
                        key_buffer[i] = last_command[i];
                        char temp[2] = {last_command[i], 0};
                        kprint(temp, GRAY_ON_BLACK);
                        i++;
                    }

                    buffer_idx = i;
                    key_buffer[buffer_idx] = 0;
                    continue;
                }
                
                if (scancode < 0x80) {
                    char key = scancode_to_char[scancode];
                    if (key != 0) {
                        if (key == '\n') {
                            key_buffer[buffer_idx] = 0;
                            kprint("\n", GRAY_ON_BLACK);
                            execute_command(key_buffer);
                            
                            for (int i = 0; i < 128; i++)
                                last_command[i] = 0;
                            memcpy(last_command, key_buffer, buffer_idx + 1);

                            buffer_idx = 0;
                            key_buffer[0] = 0;
                        }
                        else if (key == '\b') {
                            if (buffer_idx > 0) {
                                buffer_idx--;
                                key_buffer[buffer_idx] = 0;
                                kprint("\b", GRAY_ON_BLACK);
                            }
                        } 
                        else {
                            if (buffer_idx < 255) {
                                key_buffer[buffer_idx] = key;
                                buffer_idx++;
                                key_buffer[buffer_idx] = 0;
                                
                                char temp[2] = {key, 0};
                                kprint(temp, GRAY_ON_BLACK);
                            }
                        }
                    }
                }
            }
        } else {
            // Графический режим
            if (port_byte_in(0x64) & 0x01) {
                unsigned char scancode = port_byte_in(0x60);
                g_handle_key(scancode);
            }
        }
        
        for(volatile int d = 0; d < 5000; d++);
    }
}
