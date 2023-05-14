#include <MD_MAX72xx.h>
#include <LiquidCrystal_I2C.h>

#define HARDWARE 0

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // PAROLA_HW
#define MAX_DEVICES 4
#define CLK_PIN  13  // or SCK
#define DATA_PIN 11  // or MOSI
#define CS_PIN  10  // or SS
#define BUZZER 8

#define B_X 8
#define B_Y 20
#define M_X 8
#define M_Y 32

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define CLICK 5

#define LEFT_ARROW 0
#define RIGHT_ARROW 1

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
void transformPos(struct Pos* input);
int joystick();
void playNote(int t);
void blinkArrows();
int menuTetris();
int menuDifficulty();
void displayDifficulty(unsigned char difficulty);
void(* resetFunc) (void) = 0;
void setLinesReq();
void advanceLevel();
unsigned short setSpeed();
unsigned short setDAS();

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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
unsigned int newTime;
unsigned char lock = 0;
unsigned char lastInput = 0;
unsigned char show = 0;
unsigned int linesCleared = 0;
unsigned char startLevel = 1; //marca se é o nível inicial ou não
unsigned short linesRequired;
unsigned short previousLines;
unsigned short level = 0;
unsigned long score = 0;

byte leftArrow[] = {
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b01111,
    0b00111,
    0b00011,
    0b00001
};

byte rightArrow[] = {
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11110,
    0b11100,
    0b11000,
    0b10000
};

void setup() {
    pinMode(BUZZER, OUTPUT);
    mx.begin();
    lcd.init();
    lcd.backlight();
    lcd.createChar(LEFT_ARROW, leftArrow);
    lcd.createChar(RIGHT_ARROW, rightArrow);
    lcd.clear();
    randomSeed(analogRead(2));
    oldTime = millis();

    lcd.setCursor(6,0);
    lcd.print("Game");
    lcd.setCursor(5,1);
    lcd.print("Tetris");

    while (1) {
        if (joystick() == DOWN) {
            if (menuTetris()) { break; }
            else {
                lcd.clear();
                lcd.setCursor(6,0);
                lcd.print("Game");
                lcd.setCursor(5,1);
                lcd.print("Tetris");
            }
        }

        blinkArrows();
        delay(16);
    }
    lcd.clear();

    // Start Game //

    initPlayer();

    // Renderiza paredes
    for (int i = 0 ; i < B_X; i++) {
        struct Pos tmp = {i, 20};
        transformPos(&tmp);
        mx.setPoint(tmp.x, tmp.y, 1);
    }

    render();
    delay(500);
}

void loop() {

    lcd.setCursor(0,0);
    lcd.print("Lvl:");
    lcd.setCursor(4,0);
    lcd.print(level);
    lcd.setCursor(0,1);
    lcd.print("Score:");
    lcd.setCursor(6,1);
    lcd.print(score);
    lcd.setCursor(7,0);
    lcd.print("Lines:");
    lcd.setCursor(13,0);
    lcd.print(linesCleared);

    unsigned short speed;

    setLinesReq();
    speed = setSpeed();

    if (score >= 999999) //Jogo ganho
    {
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("You Win!");
        delay(500);
        resetFunc();
    }

    unsigned char currentInput = joystick();

    if (currentInput != lastInput) {
        if (currentInput == LEFT) {
            playNote(600);
            player.pos.x--;
            if (collisionChecker()) { player.pos.x++; }
            else { render(); }
            delay(setDAS());
        }
        else if (currentInput == RIGHT) {
            playNote(600);
            player.pos.x++;
            if (collisionChecker()) { player.pos.x--; }
            else { render(); }
            delay(setDAS());
        }
        else if (currentInput == UP) {
            playNote(392);
            rotate();
        }
        else if (currentInput == DOWN) {
            playNote(200);
            lock = 0;
        }
    }

    unsigned int currentPiece = player.countBag;
    if (currentInput == lastInput) {
        if (currentInput == DOWN) {
            if (!lock) { moveDown(); oldTime = millis(); }
            if (currentPiece != player.countBag) { lock = 1; }
        }
        if (currentInput == RIGHT) {
            player.pos.x++;
            if (collisionChecker()) { player.pos.x--; }
            else { render(); }
        }
        if (currentInput == LEFT) {
            player.pos.x--;
            if (collisionChecker()) { player.pos.x++; }
            else { render(); }
        }
    }

    newTime = millis();
    lastInput = currentInput;

    if (newTime - oldTime >= speed) {
        moveDown();
        oldTime = newTime;
    }

    delay(16);
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
    if (player.piece == 0 || player.piece == 4 || player.piece == 5) { player.pos.y--; }

    if (player.shape & 0x1111) { player.pos.x = (B_X - 4) / 2; }
    else if (player.shape & 0x2222) { player.pos.x = (B_X - 3) / 2; }
    else { player.pos.x = (B_X / 2) - 1; }

    // Logica para jogo perdido
    if (collisionChecker()) {
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("You Lost!");

        // Som para jogo perdido
        tone(BUZZER, 294);
        delay(200);
        noTone(BUZZER);
        tone(BUZZER, 110);
        delay(200);
        noTone(BUZZER);

        for (int i = 0; i < M_Y; i++) {
            for (int j = 0; j < M_X; j++) {
                struct Pos tmp = {j, i};
                transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
                delay(10);
            }
        }

        for (int i = M_Y - 1; i >= 0; i--) {
            for (int j = M_X - 1; j >= 0; j--) {
                struct Pos tmp = {j, i};
                transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 0);
                delay(10);
            }
        }

        delay(500);
        resetFunc();
    }
}

// Verifica se a peça do jogador está colidindo com outra
unsigned char collisionChecker() {
    unsigned int b = 0x8000;
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
    if (joystick()==DOWN)
    {
        score++;
    }
    if (collisionChecker()) {
        player.pos.y--;
        unsigned int b = 0x8000;
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
    int rowsCompleted = 0;
    for (int i = B_Y - 1; i >= 0; i--) {
        int isCompleted = 1;
        for (int j = 0; j < B_X; j++) {
            if (!board[i][j]) { isCompleted = 0; break; }
        }
        if (isCompleted) {
            rowsCompleted++;
            linesCleared++;
            // Linha completa some
            for (int k = i; k > i-rowsCompleted; k--)
            {
                for (int j = B_X/2; j < B_X; j++) 
                {
                    struct Pos tmp;
                    tmp = {j, k};
                    transformPos(&tmp);
                    mx.setPoint(tmp.x, tmp.y, 0);

                    tmp = {7-j,k};
                    transformPos(&tmp);
                    mx.setPoint(tmp.x, tmp.y, 0);
                    delay (50);
                }
            }
            // Peças caem
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < B_X; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for (int j = 0; j < B_X; j++) { board[0][j] = 0; }
            i++;

            // Som para pontuação
            tone(BUZZER, 392);
            delay(100);
            noTone(BUZZER);
            tone(BUZZER, 528);
            delay(100);
            noTone(BUZZER);
        }
    }

    //Incrementa a pontuação, 4 linhas ou mais aumentam a mesma quantidade de pontos
    switch (rowsCompleted)
    {
        case 0:
        break;

        case 1:
        score += 40 * (level + 1);
        break;

        case 2:
        score += 100 * (level + 1);
        break;

        case 3:
        score += 300 * (level + 1);
        break;

        default:
        score += 1200 * (level + 1);
    }
    
    advanceLevel();

}

// Renderiza o jogo na tela
void render() {
    // TODO: Desligar leds (se necessário)
    struct Pos tmp;
    for (int i = 0; i < B_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            tmp = {j, i};
            transformPos(&tmp);
            mx.setPoint(tmp.x, tmp.y, 0);
        }
    }

    // Renderiza tabuleiro
    for (int i = 0; i < B_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            if (board[i][j]) {
                tmp = {j, i};
                transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
            }
        }
    }

    // Renderiza jogador
    unsigned int b = 0x8000;
    for (int i = player.pos.y; i < player.pos.y + 4; i++) {
        for (int j = player.pos.x; j < player.pos.x + 4; j++, b = b>>1) {
            if (player.shape & b) {
                tmp = {j, i};
                transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
            }
        }
    }
}


#if HARDWARE
// Converte coordenadas para funcionar com a matriz de leds (hardware)
void transformPos(struct Pos* input) {
    char tmp = input->x;
    input->x = 7 - tmp;
    input->y = 31 - input->y;
}

// Retorna uma direção do joystick (hardware)
int joystick() {
    const int VRx = 0;
    const int VRy = 1;
    const int SW = 2;

    if (analogRead(VRy) > 900) { return DOWN; }
    if (analogRead(VRy) < 120) { return UP; }
    if (analogRead(VRx) > 900) { return LEFT; }
    if (analogRead(VRx) < 120) { return RIGHT; }
    if (!analogRead(SW)) { return CLICK; }

    return 0;
}

#else
// Converte coordenadas para funcionar com a matriz de leds (simulador)
void transformPos(struct Pos* input) {
    input->x = 7 - input->x;
    input->y = 24 - (16 * (input->y / 8)) + input->y;
}

// Retorna uma direção do joystick (simulador)
int joystick() {
    const int VRx = 0;
    const int VRy = 1;
    const int SW = 2;

    if (analogRead(VRy) > 900) { return UP; }
    if (analogRead(VRy) < 120) { return DOWN; }
    if (analogRead(VRx) > 900) { return LEFT; }
    if (analogRead(VRx) < 120) { return RIGHT; }
    if (!analogRead(SW)) { return CLICK; }

    return 0;
}

#endif

// Toca uma nota t
void playNote(int t) {
    tone(BUZZER, t);
    delay(10);
    noTone(BUZZER);
}

void blinkArrows() {
    newTime = millis();
    if (newTime - oldTime >= 800) {
        oldTime = newTime;
        if (show) {
            show = 0;
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(15, 1);
            lcd.print(" ");
        } else {
            show = 1;
            lcd.setCursor(0, 1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15, 1);
            lcd.write(RIGHT_ARROW);
        }
    }
}

int menuTetris() {
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Tetris");
    lcd.setCursor(6,1);
    lcd.print("Play");
    unsigned char currentOption = 0;

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();
        if (currentInput == DOWN) {
            if (menuDifficulty()) { return 1; }
            else {
                lcd.clear();
                lcd.setCursor(5,0);
                lcd.print("Tetris");
                lcd.setCursor(6,1);
                lcd.print("Play");
            }
        } else if (currentInput == LEFT || currentInput == RIGHT) {
            currentOption++;
            if (currentOption > 1) { currentOption = 0; }

            lcd.setCursor(0,1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15,1);
            lcd.write(RIGHT_ARROW);
            show = 1;

            lcd.setCursor(2,1);
            for (int i = 0; i < 12; i++) {
                lcd.print(" ");
            }

            if (currentOption == 0) {
                lcd.setCursor(6,1);
                lcd.print("Play");
            } else {
                lcd.setCursor(5,1);
                lcd.print("Scores");
            }

            delay(200);
        } else if (currentInput == UP) { return 0; }

        blinkArrows();
        delay(16);
    }
}

int menuDifficulty() {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Difficulty");
    lcd.setCursor(6,1);
    lcd.print("Easy");
    unsigned char difficulty = 0;

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();
        if (currentInput == DOWN) {
            if (difficulty == 0) { level = 0; } // Easy
            else if (difficulty == 1) { level = 5; } // Medium
            else { level = 10; } // Hard
            return 1;
        } else if (currentInput == LEFT) {
            // Scroll leftToright
            difficulty--;
            if (difficulty > 2) { difficulty = 2; }

            displayDifficulty(difficulty);

            delay(200);
        } else if (currentInput == RIGHT) {
            // Scroll rightToleft
            difficulty++;
            if (difficulty > 2) { difficulty = 0; }

            displayDifficulty(difficulty);

            delay(200);
        } else if (currentInput == UP) { return 0; }

        blinkArrows();
        delay(16);
    }
}

void displayDifficulty(unsigned char difficulty) {
    lcd.setCursor(0,1);
    lcd.write(LEFT_ARROW);
    lcd.setCursor(15,1);
    lcd.write(RIGHT_ARROW);
    lcd.setCursor(2,1);
    show = 1;
    for (int i = 0; i < 12; i++) { lcd.print(" "); }

    if (difficulty == 0) { lcd.setCursor(6,1); lcd.print("Easy"); }
    else if (difficulty == 1) { lcd.setCursor(5,1); lcd.print("Medium"); }
    else { lcd.setCursor(6,1); lcd.print("Hard"); }
}

void setLinesReq()
{
    if (startLevel)
    {
        linesRequired = min(level*10+10, max(100,level*10-50));
        previousLines = linesRequired;
    } else {
        linesRequired = previousLines + 10;
    }
}

void advanceLevel()
{
    if (linesCleared >= linesRequired)
    {
        level++;
        previousLines = linesRequired;
        startLevel = 0;
    }
}

unsigned short setSpeed()
{
    unsigned short framesgrid;

    if (level < 9)
    {
        framesgrid = 48 - level*5;
    } else if (level == 9)
    {
        framesgrid = 6;
    } else if (level <= 12)
    {
        framesgrid = 5;
    } else if (level <= 15)
    {
        framesgrid = 4;
    } else if (level <= 18)
    {
        framesgrid = 3;
    } else if (level <= 28)
    {
        framesgrid = 2;
    } else 
    {
        framesgrid = 1;
    }


    return (1/(60/framesgrid))*1000;
}

unsigned short setDAS()
{
    unsigned short framesgrid;

    if (level <= 2)
    {
        framesgrid = 29;
    }
    else if (level <= 7 && level%2==0)
    {
        framesgrid = 36 - (level/2)*7;
    } else if (level <= 7 && level%2==1)
    {
        framesgrid = 32 - ((level-1)/2)*7;
    } else if (level == 8)
    {
        framesgrid = 7;
    } else if (level == 9)
    {
        framesgrid = 5;
    } else if (level <= 12)
    {
        framesgrid = 4;
    } else if (level <= 15)
    {
        framesgrid = 3;
    }
    else if (level <= 18)
    {
        framesgrid = 2;
    } else 
    {
        framesgrid = 1;
    }


    return (1/(60/framesgrid))*1000;
}