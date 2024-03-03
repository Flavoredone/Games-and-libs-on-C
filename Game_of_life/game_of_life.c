#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define M 80
#define N 25

void output(int **a, int delay) {
    for (int i = 0; i < M + 2; i++) {
        mvaddch(0, i, '#');
        mvaddch(N + 1, i, '#');
    }
    printw("\n  Press <q> for quit, press <-> or <+> for change the delay. Current delay = %dms\n",
           delay / 1000);

    for (int i = 0; i < N + 2; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, M + 1, '#');
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            mvaddch(i + 1, j + 1, *(*(a + i) + j) == 0 ? ' ' : '*');
        }
    }
}

void input(int **a) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", (*(a + i) + j));
        }
    }
}

void life(int **a, int **b) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            *(*(b + i) + j) = 0;
            *(*(b + i) + j) += *(*(a + (i == 0 ? N - 1 : i - 1)) + j);
            *(*(b + i) + j) += *(*(a + (i == 0 ? N - 1 : i - 1)) + (j == M - 1 ? 0 : j + 1));
            *(*(b + i) + j) += *(*(a + i) + (j == M - 1 ? 0 : j + 1));
            *(*(b + i) + j) += *(*(a + (i == N - 1 ? 0 : i + 1)) + (j == M - 1 ? 0 : j + 1));
            *(*(b + i) + j) += *(*(a + (i == N - 1 ? 0 : i + 1)) + j);
            *(*(b + i) + j) += *(*(a + (i == N - 1 ? 0 : i + 1)) + (j == 0 ? M - 1 : j - 1));
            *(*(b + i) + j) += *(*(a + i) + (j == 0 ? M - 1 : j - 1));
            *(*(b + i) + j) += *(*(a + (i == 0 ? N - 1 : i - 1)) + (j == 0 ? M - 1 : j - 1));
            if ((*(*(a + i) + j) == 0) && (*(*(b + i) + j) == 3)) {
                *(*(b + i) + j) = 1;
                continue;
            }
            if ((*(*(a + i) + j) == 1) && ((*(*(b + i) + j) == 3) || (*(*(b + i) + j) == 2))) {
                *(*(b + i) + j) = 1;
                continue;
            }
            *(*(b + i) + j) = 0;
        }
    }
}

int main() {
    int **a, **b, delay = 500000;
    int **ptr;
    int c = ' ';
    initscr();
    noecho();
    nodelay(stdscr, 1);
    a = (int **)calloc(N, sizeof(int *));
    for (int i = 0; i < N; i++) {
        *(a + i) = (int *)calloc(M, sizeof(int));
    }
    b = (int **)calloc(N, sizeof(int *));
    for (int i = 0; i < N; i++) {
        *(b + i) = (int *)calloc(M, sizeof(int));
    }
    input(a);
    FILE *f = freopen("/dev/tty", "r", stdin);
    do {
        output(a, delay);
        life(a, b);
        ptr = a;
        a = b;
        b = ptr;
        c = getch();
        if ((c == '-') && (delay > 50000)) {
            delay -= 50000;
        }
        if (c == '+') {
            delay += 100000;
        }
        usleep(delay);
    } while (c != 'q');
    for (int i = 0; i < N; i++) {
        free(*(a + i));
    }
    free(a);
    for (int i = 0; i < N; i++) {
        free(*(b + i));
    }
    free(b);
    fclose(f);
    endwin();
    return 0;
}
