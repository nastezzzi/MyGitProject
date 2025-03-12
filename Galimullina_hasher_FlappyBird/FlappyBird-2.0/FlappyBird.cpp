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
const int BONUS_SIZE = 1; // Размер бонуса

int birdY = WINDOW_HEIGHT / 2;
int birdVelocity = 0;
int score = 0;
bool gameOver = false;

struct Pipe {
    int top;
    int bottom;
    int x;
};

struct Bonus {
    int x;
    int y;
};

// Массив для хранения труб и бонусов
Pipe pipes[3];
Bonus bonus;

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
                // Рисуем бонус
                if (x == bonus.x && y == bonus.y) {
                    printw("*");  // Бонус
                }
                if (!pipePrinted && !(x == bonus.x && y == bonus.y)) {
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
    if (birdY >= WINDOW_HEIGHT || birdY < 0) {
        gameOver = true;  // Игра заканчивается, если птица выходит за границы
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

    // Проверка на столкновение с бонусом
    if (bonus.x == 5 && bonus.y == birdY) {
        score += 5;  // Добавляем очки за бонус
        bonus.x = -1;  // Убираем бонус после сбора
    }

    // Генерация бонусов
    if (bonus.x == -1) {  // Если бонус исчез, создаём новый
        bonus.x = WINDOW_WIDTH + rand() % 10;
        bonus.y = rand() % WINDOW_HEIGHT;
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

    // Инициализация бонуса
    bonus.x = -1;  // Бонус ещё не создан

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
