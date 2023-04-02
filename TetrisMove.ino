#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // PAROLA_HW
#define MAX_DEVICES 4
#define CLK_PIN  13  // or SCK
#define DATA_PIN 11  // or MOSI
#define CS_PIN  10  // or SS

#define M_X 8
#define M_Y 32
#define B_X 8
#define B_Y 20

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define CLICK 5

struct Pos {
	char x;
	char y;
};

struct Player {
    struct Pos pos;
    unsigned int shape;
    unsigned char rotation;
    unsigned char piece;
    unsigned char bag[28];
    unsigned char countBag;
};

void initPlayer();
void fillBag(unsigned char* bag);
void shuffle(unsigned char *array, size_t n);
void pickShape();
unsigned char collisionChecker();
int moveDown();
void rotate();
void checkCompletedRows();
void render();
struct Pos transformPos(struct Pos* p);
int Joystick();

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE,CS_PIN, MAX_DEVICES);
unsigned char board[B_Y][B_X] = {0};
unsigned int shapes[7][4] = {{0x0F00, 0x4444, 0x0F00, 0x4444},
                             {0x8E00, 0x6440, 0x0E20, 0x44C0},
                             {0x2E00, 0x4460, 0x0E80, 0xC440},
                             {0xCC00, 0xCC00, 0xCC00, 0xCC00},
                             {0x06C0, 0x8C40, 0x6C00, 0x4620},
                             {0x0C60, 0x4C80, 0xC600, 0x2640},
                             {0x4E00, 0x4640, 0x0E40, 0x4C40}};

struct Player player;
unsigned int oldTime;
unsigned char lock = 0;


void setup() {
    mx.begin();
    randomSeed(analogRead(0));
    initPlayer();
    oldTime = millis();
    render();
    delay(500);
}

void loop() {
    // TODO: Mov Code

    unsigned char lock = 0;

    if (Joystick() == LEFT)
    {
        player.pos.x--;
        if (collisionChecker()) { player.pos.x++; }
        else { render(); }
    } else if (Joystick() == RIGHT)
    {
        player.pos.x++;
        if (collisionChecker()) { player.pos.x--; }
        else { render(); }
    } else if (Joystick() == UP)
    {
        rotate();
    } else if (Joystick() == DOWN)
    {
        lock = 0;
    }

    unsigned int currentPiece = player.countBag;

    if (Joystick() == DOWN)
    {
        if (!lock) { moveDown(); oldTime = millis(); }

        if (currentPiece != player.countBag) { lock = 1; }
    }

    if (millis() - oldTime >= 500) {
        moveDown();
        oldTime = millis();
    }

    delay(16);
}

int Joystick()
{
  // 0 = up
  // 1 = down
  // 3 = right
  // 2 = left
  // 4 = nothing
  // 5 = click
  const int VRx = 0;
  const int VRy = 1;
  const int SW = 2;
  if(analogRead(VRy) > 900)
  {
  //  prev = 0;
    return 0;
  }
  if(analogRead(VRy) < 120)
  {
  //  prev = 1;
    return 1;
  }
  if(analogRead(VRx) > 900)
  {
  //  prev = 2;
    return 2;
  }
  if(analogRead(VRx) < 120)
  {
  //  prev = 3;
    return 3;
  }
  if(analogRead(SW) == 0)
  {
    return 5;
  }
  //return prev;
}

// Dá valores iniciais para o jogador
void initPlayer() {
    player.shape = 0;
    player.rotation = 0;
    player.piece = 0;
    player.countBag = 28;

    fillBag(player.bag);
    pickShape();
}

// Preenche a seleção de peças que o jogador vai receber
void fillBag(unsigned char* bag) {
    unsigned char count = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++)
            bag[count++] = i;
    }
    for (int i = 0; i < 10; i++) { shuffle(bag, 28); }
}

// Aleatoriza os valores de um vetor
void shuffle(unsigned char *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

// Seleciona a próxima peça do jogador (e testa para ver se perdeu o jogo)
void pickShape() {
    player.countBag--;
    player.rotation = 0;
    
    player.piece = player.bag[player.countBag];
    player.shape = shapes[player.piece][player.rotation];

    if (!player.countBag) { fillBag(player.bag); player.countBag = 28; }

    player.pos.y = 0;
    if (player.shape == shapes[4][0] || player.shape == shapes[5][0] || player.shape == shapes[0][0]) { player.pos.y--; }

    if (player.shape & 0x1111) { player.pos.x = (B_X - 4) / 2; }
    else if (player.shape & 0x2222) { player.pos.x = (B_X - 3) / 2; }
    else { player.pos.x = (B_X / 2) - 1; }

    // Logica para jogo perdido
    if (collisionChecker()) {
        char tmpHeight = player.pos.y;
        for (int i = 0; i < 3; i++) {
            player.pos.y = tmpHeight;
            render();
            delay(500);

            player.pos.y = -10;
            render();
            delay(500);
        }

        delay(500);
        exit(0);
    }
}

// Verifica se a peça do jogador está colidindo com outra
unsigned char collisionChecker() {
    int b = 0x8000;
    for (int i = player.pos.y; i < player.pos.y + 4; i++) {
        for (int j = player.pos.x; j < player.pos.x + 4; j++, b = b>>1) {
            if ((player.shape & b) && (j < 0 || j > B_X - 1 || i < 0 || i > B_Y - 1 || board[i][j])) { return 1; }
        }
    }
    return 0;
}

// Move a peça para baixo e verifica/limpa fileiras completadas
int moveDown() {
    player.pos.y++;

    if (collisionChecker()) {
        player.pos.y--;
        int b = 0x8000;
        for (int i = player.pos.y; i < player.pos.y + 4; i++) {
            for (int j = player.pos.x; j < player.pos.x + 4; j++, b = b>>1) {
                if (player.shape & b) { board[i][j] = 1; }
            }
        }
        pickShape();
        checkCompletedRows();
        render();
        return 1;
    }
    render();
    return 0;
}

// Rotaciona as peças do jogador
void rotate() {
    player.rotation++;
    if (player.rotation > 3) { player.rotation = 0; }
    
    player.shape = shapes[player.piece][player.rotation];
    if (collisionChecker()) {
        player.rotation--;
        if (player.rotation > 3) { player.rotation = 3; }
        player.shape = shapes[player.piece][player.rotation];
    } else { render(); }
}

// Verifica por linhas completadas e atualiza o score do jogador
void checkCompletedRows() {
    for (int i = B_Y - 1; i >= 0; i--) {
        int isCompleted = 1;
        for (int j = 0; j < B_X; j++) {
            if (!board[i][j]) { isCompleted = 0; break; }
        }
        if (isCompleted) {
            for (int j = 0; j < B_X; j++) { board[i][j] = 0; }
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < B_X; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for (int j = 0; j < B_X; j++) { board[0][j] = 0; }
            i++;
        }
    }
}

// Renderiza o jogo na tela
void render() {
    // Desliga leds (se necessário)
    // TODO
    /*
    for (int i = 0; i < B_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            struct Pos tmp = {j, i};
            tmp = transformPos(&tmp);
            if (mx.getPoint(tmp.x, tmp.y)) {
                if (!board[i][j]) {
                    if (i)
                    mx.setPoint(tmp.x, tmp.y, 0);
                }
                if (!board[i][j] && !(player.shape & b))
                mx.setPoint(tmp.x, tmp.y, 0);
            } 
        }
    }*/
    for (int i = 0; i < B_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            struct Pos tmp = {j, i};
            tmp = transformPos(&tmp);
            mx.setPoint(tmp.x, tmp.y, 0);
        }
    }

    // Renderiza tabuleiro
    for (int i = 0; i < B_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            if (board[i][j]) {
                struct Pos tmp = {j, i};
                tmp = transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
            }
        }
    }

    // Renderiza jogador
    int b = 0x8000;
    for (int i = player.pos.y; i < player.pos.y + 4; i++) {
        for (int j = player.pos.x; j < player.pos.x + 4; j++, b = b>>1) {
            if (player.shape & b) {
                struct Pos tmp = {j, i};
                tmp = transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
            }
        }
    }

    // Renderiza paredes
    for (int i = 0 ; i < B_X; i++) {
        struct Pos tmp = {i, 20};
        tmp = transformPos(&tmp);
        mx.setPoint(tmp.x, tmp.y, 1);
    }
}

// Recebe a posição de uma matriz normal
// Retorna a posição correta para a matriz de leds
struct Pos transformPos(struct Pos* p) {
    struct Pos tmp;
    tmp.x = M_X - p->x - 1;
    tmp.y = M_Y - (16 * (p->y / (M_Y / MAX_DEVICES))) - M_X + p->y;
    return tmp;
}