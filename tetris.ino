#include <stdlib.h>
#include <stdio.h>

#define B_X 8
#define B_Y 20

struct Player {
    char x;
    char y;
    unsigned char color;
    unsigned int shape;
    unsigned char rotation;
    unsigned char bag[28];
    unsigned char countBag;
};

void initPlayer(struct Player* p);
void fillBag(unsigned char* bag);
void shuffle(unsigned char *array, size_t n);
void pickShape(struct Player* p);
unsigned char collisionChecker(struct Player* p);
int moveDown(struct Player* p);
void rotate(struct Player* p);
void checkCompletedRows();
void render(struct Player* p);

unsigned char board[B_Y][B_X] = {0};
unsigned int shapes[7][4] = {{0x0F00, 0x4444, 0x0F00, 0x4444},
                             {0x8E00, 0x6440, 0x0E20, 0x44C0},
                             {0x2E00, 0x4460, 0x0E80, 0xC440},
                             {0xCC00, 0xCC00, 0xCC00, 0xCC00},
                             {0x06C0, 0x8C40, 0x6C00, 0x4620},
                             {0x0C60, 0x4C80, 0xC600, 0x2640},
                             {0x4E00, 0x4640, 0x0E40, 0x4C40}};
double aspect_ratio = 16.0 / 9;
int width = 1280;
int height = int(width / aspect_ratio);
double scale = height / (double(B_Y * 10));
double offset = (width - (B_X * 10 * scale)) / 2.0;

int score = 0;


void setup() {
  // put your setup code here, to run once:

}

// Verifica se a peça do jogador está colidindo com outra
unsigned char collisionChecker(struct Player* p) {
    int b = 0x8000;
    for (int i = p->y; i < p->y + 4; i++) {
        for (int j = p->x; j < p->x + 4; j++, b = b>>1) {
            if ((p->shape & b) && (j < 0 || j > B_X - 1 || i < 0 || i > B_Y - 1 || board[i][j])) { return 1; }
        }
    }
    return 0;
}

int moveDown(struct Player* p) {
    p->y++;

    if (collisionChecker(p)) {
        p->y--;
        int b = 0x8000;
        for (int i = p->y; i < p->y + 4; i++) {
            for (int j = p->x; j < p->x + 4; j++, b = b>>1) {
                if (p->shape & b) { board[i][j] = p->color + 1; }
            }
        }
        pickShape(p);
        checkCompletedRows();
        render(p);
        return 1;
    }
    render(p);
    return 0;
}

// Rotaciona as peças do jogador
void rotate(struct Player* p) {
    p->rotation++;
    if (p->rotation > 3) { p->rotation = 0; }
    
    p->shape = shapes[p->color][p->rotation];
    if (collisionChecker(p)) {
        p->rotation--;
        if (p->rotation > 3) { p->rotation = 3; }
        p->shape = shapes[p->color][p->rotation];
    } else { render(p); }
}

// Verifica por linhas completadas e atualiza o score do jogador
void checkCompletedRows() {
    int rowsCompleted = 0;
    for (int i = B_Y - 1; i >= 0; i--) {
        int isCompleted = 1;
        for (int j = 0; j < B_X; j++) {
            if (!board[i][j]) { isCompleted = 0; break; }
        }
        if (isCompleted)
        {
            rowsCompleted++;
            for (int j = 0; j < B_X; j++) { board[i][j] = 0; }
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < B_X; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for (int j = 0; j < B_X; j++) { board[0][j] = 0; }
            i++;
            score += 100 + 100 * (rowsCompleted-1);
        }
    }
    printf("Score: %d\n", score);
}

// Seleciona a próxima peça do jogador (e testa para ver se perdeu o jogo)
void pickShape(struct Player* p) {
    p->countBag--;
    p->rotation = 0;
    
    p->shape = shapes[p->bag[p->countBag]][p->rotation];

    p->color = p->bag[p->countBag];
    if (!p->countBag) { fillBag(p->bag); p->countBag = 28; }

    p->y = 0;
    if (p->shape == shapes[4][0] || p->shape == shapes[5][0] || p->shape == shapes[0][0]) { p->y--; }

    if (p->shape & 0x1111) { p->x = (B_X - 4) / 2; }
    else if (p->shape & 0x2222) { p->x = (B_X - 3) / 2; }
    else { p->x = (B_X / 2) - 1; }}

void loop() {
  // put your main code here, to run repeatedly:

}

