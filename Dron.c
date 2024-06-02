#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 20
#define MAX_DRONES 5
#define MAX_WAGONS 50

typedef struct {
    int x, y;
    bool has_pumpkin;
} Tile;

typedef struct {
    int x, y;
    int num_wagons;
} Drone;

Tile field[FIELD_WIDTH][FIELD_HEIGHT];
Drone drones[MAX_DRONES];
int total_pumpkins = 0;

void initialize_field() {
    srand(time(NULL));
    for (int i = 0; i < FIELD_WIDTH; i++) {
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            field[i][j].x = i;
            field[i][j].y = j;
            field[i][j].has_pumpkin = rand() % 5 == 0;
            if (field[i][j].has_pumpkin) {
                total_pumpkins++;
            }
        }
    }
}

void print_field() {
    for (int i = 0; i < FIELD_WIDTH; i++) {
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            if (field[i][j].has_pumpkin) {
                printf("T ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void move_drone(Drone* drone, int dx, int dy) {
    int new_x = drone->x + dx;
    int new_y = drone->y + dy;
    if (new_x >= 0 && new_x < FIELD_WIDTH && new_y >= 0 && new_y < FIELD_HEIGHT) {
        drone->x = new_x;
        drone->y = new_y;
        if (field[new_x][new_y].has_pumpkin) {
            field[new_x][new_y].has_pumpkin = false;
            drone->num_wagons++;
            total_pumpkins--;
        }
    }
}

void auto_pilot(Drone* drone) {
    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;
    move_drone(drone, dx, dy);
}

void cooperative_auto_pilot() {
    for (int i = 0; i < MAX_DRONES; i++) {
        auto_pilot(&drones[i]);
    }
}

void handle_collision() {
    for (int i = 0; i < MAX_DRONES - 1; i++) {
        for (int j = i + 1; j < MAX_DRONES; j++) {
            if (drones[i].x == drones[j].x && drones[i].y == drones[j].y) {
                printf("Столкновение дронов! Аварийная ситуация.\n");
            }
        }
    }
}

int main(void) {
    initialize_field();

    int num_drones = 0;
    while (num_drones < MAX_DRONES) {
        printf("Введите позицию дрона %d (x y): ", num_drones + 1);
        scanf("%d %d", &drones[num_drones].x, &drones[num_drones].y);
        drones[num_drones].num_wagons = 0;
        num_drones++;
    }

    while (total_pumpkins > 0) {
        print_field();
        printf("Собрано тыкв: %d\n", total_pumpkins);

        int dx, dy;
        printf("Управление дроном (dx dy): ");
        scanf("%d %d", &dx, &dy);

        for (int i = 0; i < num_drones; i++) {
            move_drone(&drones[i], dx, dy);
        }

        cooperative_auto_pilot();
        handle_collision();
    }

    printf("Сбор урожая завершен! Всего собрано тыкв: %d\n", total_pumpkins);

    return 0;
}
