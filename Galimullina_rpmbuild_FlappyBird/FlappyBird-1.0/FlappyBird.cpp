#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

using namespace std;

// Константы игры
const int WINDOW_WIDTH = 40;
const int WINDOW_HEIGHT = 10;
const int PIPE_WIDTH = 3;
const int PIPE_GAP = 4;
const int GRAVITY = 1; // Сила гравитации
const int JUMP_STRENGTH = -2; // Сила прыжка

int birdY = WINDOW_HEIGHT / 2;
int birdVelocity = 0;
int score = 0;
bool gameOver = false;

struct Pipe {
    int top;
    int bottom;
    int x;
};

// Массив для хранения труб
Pipe pipes[3];

// Функция для отображения игрового поля
void draw() {
    clear();  // Очистить экран

    // Отображаем игру
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            // Рисуем птицу
            if (x == 5 && y == birdY) {
                printw("O");  // Птица
            }
            // Рисуем трубы
            else {
                bool pipePrinted = false;
                for (int i = 0; i < 3; ++i) {
                    if (x == pipes[i].x) {
                        if (y < pipes[i].top || y >= pipes[i].bottom) {
                            printw("#");  // Трубы
                        } else {
                            printw(" ");
                        }
                        pipePrinted = true;
                        break;
                    }
                }
                if (!pipePrinted) {
                    printw(" ");  // Пустое пространство
                }
            }
        }
        printw("\n");
    }

    // Выводим счёт
    printw("Score: %d\n", score);
    refresh();
}

// Функция для обновления состояния игры
void update() {
    // Движение птицы
    birdVelocity += GRAVITY;
    birdY += birdVelocity;

    // Проверка на падение за пределы
    if (birdY >= WINDOW_HEIGHT) {
        birdY = WINDOW_HEIGHT - 1;
    }
    if (birdY < 0) {
        birdY = 0;
    }

    // Двигаем трубы
    for (int i = 0; i < 3; ++i) {
        pipes[i].x--;

        // Если труба выходит за экран, перезапускаем её
        if (pipes[i].x < 0) {
            pipes[i].x = WINDOW_WIDTH;
            pipes[i].top = rand() % (WINDOW_HEIGHT / 2);  // Высота верхней трубы
            pipes[i].bottom = pipes[i].top + PIPE_GAP;  // Нижняя труба
        }

        // Проверка на столкновение с трубой
        if (pipes[i].x == 5) {
            if (birdY < pipes[i].top || birdY >= pipes[i].bottom) {
                gameOver = true;
            } else {
                if (pipes[i].x == 5) {
                    score++;
                }
            }
        }
    }
}

// Обработка ввода (прыжок)
void handleInput() {
    int ch = getch();  // Считываем ввод пользователя

    if (ch == 'w' || ch == KEY_UP) {  // Клавиша "w" или стрелка вверх
        birdVelocity = JUMP_STRENGTH;  // Птица прыгает
    }
}

int main() {
    srand(time(0));  // Инициализация генератора случайных чисел

    // Инициализация ncurses
    initscr();  // Инициализируем ncurses
    noecho();  // Отключаем отображение ввода
    cbreak();  // Отключаем буферизацию ввода
    timeout(100);  // Задержка в 100 миллисекунд для обновления экрана
    keypad(stdscr, TRUE);  // Разрешаем использование функциональных клавиш (например, стрелок)

    // Инициализация труб
    for (int i = 0; i < 3; ++i) {
        pipes[i].x = WINDOW_WIDTH + i * 15;  // Разные начальные позиции для труб
        pipes[i].top = rand() % (WINDOW_HEIGHT / 2);  // Высота верхней трубы
        pipes[i].bottom = pipes[i].top + PIPE_GAP;  // Нижняя труба
    }

    // Главный игровой цикл
    while (!gameOver) {
        draw();  // Отображаем игру
        handleInput();  // Обработка ввода (прыжок)
        update();  // Обновление состояния игры
    }

    // Когда игра заканчивается
    printw("Game Over!\n");
    refresh();
    getch();  // Ожидаем нажатие клавиши, чтобы завершить игру

    // Завершаем работу с ncurses
    endwin();
    return 0;
}
