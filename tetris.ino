#include <MD_MAX72xx.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define HARDWARE 0

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // PAROLA_HW
#define MAX_DEVICES 4
#define CLK_PIN  13  // or SCK
#define DATA_PIN 11  // or MOSI
#define CS_PIN  10  // or SS
#define BUZZER 6

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


//Código da música do Tetris retirado de: https://github.com/robsoncouto/arduino-songs/blob/master/tetris/tetris.ino
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

// change this to make the song slower or faster
int tempo=160; 

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

int lastNote = 0; 

struct Pos {
	char x;
	char y;
};

struct Player {
    struct Pos pos;
    unsigned int shape;
    unsigned char rotation;
    unsigned char piece;
    unsigned char nextPiece;
};

struct Score {
    unsigned long score;
    char name[7];
};

void initPlayer();
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
void menuScores();
void displayDifficulty(unsigned char difficulty);
void(* resetFunc) (void) = 0;
void setLinesReq();
void advanceLevel();
unsigned short setSpeed();
unsigned short setDAS();
void playSong();
void sortPositions(unsigned int* scores);
void displayScore(struct Score* score);
unsigned char countDigits(unsigned long num);
void menuSaveScore();

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
    randomSeed(EEPROM.read(0));
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
        tone(BUZZER, 700);
        delay(200);
        noTone(BUZZER);
        tone(BUZZER, 800);
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

        menuSaveScore();
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

    unsigned char currentPiece = player.piece;
    if (currentInput == lastInput) {
        if (currentInput == DOWN) {
            if (!lock) { moveDown(); oldTime = millis(); }
            if (currentPiece != player.piece) { lock = 1; }
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
    EEPROM.update(0, random(256));

    player.shape = 0;
    player.rotation = 0;
    player.piece = 0;
    player.nextPiece = random(7);

    pickShape();
}

// Seleciona a próxima peça do jogador (e testa para ver se perdeu o jogo)
void pickShape() {
    player.rotation = 0;
    
    player.piece = player.nextPiece;
    player.nextPiece = random(7);
    player.shape = shapes[player.piece][player.rotation];

    player.pos.y = 0;
    if (player.piece == 0 || player.piece == 4 || player.piece == 5) { player.pos.y--; }

    if (player.shape & 0x1111) { player.pos.x = (B_X - 4) / 2; }
    else if (player.shape & 0x2222) { player.pos.x = (B_X - 3) / 2; }
    else { player.pos.x = (B_X / 2) - 1; }

    //Renderiza prox peça

    struct Pos tmp;
    for (int i = B_Y + 1; i < M_Y; i++) {
        for (int j = 0; j < B_X; j++) {
            tmp = {j, i};
            transformPos(&tmp);
            mx.setPoint(tmp.x, tmp.y, 0);
        }
    }

    char x_nextPiece;
    unsigned int shapeNextPiece = shapes[player.nextPiece][0];

    if (shapeNextPiece & 0x1111) { x_nextPiece = (B_X - 4) / 2; }
    else if (shapeNextPiece & 0x2222) { x_nextPiece = (B_X - 3) / 2; }
    else { x_nextPiece = (B_X / 2) - 1; }

    unsigned int b = 0x8000;

    for (int i = 25; i < 25 + 4; i++) {
        for (int j = x_nextPiece; j < x_nextPiece + 4; j++, b = b>>1) {
            if (shapeNextPiece & b) {
                tmp = {j, i};
                transformPos(&tmp);
                mx.setPoint(tmp.x, tmp.y, 1);
            }
        }
    }

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

        menuSaveScore();

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

        playSong();

        if (currentInput == DOWN) {
            if (!currentOption && menuDifficulty()) { return 1; }
            else if (currentOption) { menuScores(); }

            lcd.clear();
            lcd.setCursor(5,0);
            lcd.print("Tetris");

            if (!currentOption) {
                lcd.setCursor(6,1);
                lcd.print("Play");
            } else if (currentOption) {
                lcd.setCursor(5, 1);
                lcd.print("Scores");
            }

            delay(200);
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

        playSong();

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

void menuScores() {
    unsigned int scorePositions[10] = {1, 12, 23, 34, 45, 56, 67, 78, 89, 100};

    memoryValidator();
    sortPositions(scorePositions);

    unsigned char maxValidScorePosition = 0;
    struct Score scores[10];
    for (int i = 0; i < 10; i++) {
        EEPROM.get(scorePositions[i], scores[i].score);
        if (!scores[i].score) { maxValidScorePosition = i - 1; break; }
        for (int j = 0; j < 7; j++) {
            scores[i].name[j] = EEPROM.read(scorePositions[i] + j + 4);
        }
        maxValidScorePosition = i;
    }

    if (maxValidScorePosition > 10) { maxValidScorePosition = 0; }

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Scores");

    unsigned char currScore = 0;
    unsigned char anyScores = 0;
    if (!scores[currScore].score) {
        lcd.setCursor(3,1);
        lcd.print("No Scores!");
        
    } else {
        displayScore(&scores[currScore]);
        anyScores = 1;
    }

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();

        playSong();

        if (currentInput == UP) { return; }
        else if (currentInput == LEFT) {
            lcd.setCursor(0,1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15,1);
            lcd.write(RIGHT_ARROW);
            show = 1;

            currScore--;
            if (currScore > maxValidScorePosition) { currScore = maxValidScorePosition; }
            if (anyScores) {
                displayScore(&scores[currScore]);
            }

            delay(200);
        } else if (currentInput == RIGHT) {
            lcd.setCursor(0,1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15,1);
            lcd.write(RIGHT_ARROW);
            show = 1;

            currScore++;
            if (currScore > maxValidScorePosition) { currScore = 0; }
            if (anyScores) {
                displayScore(&scores[currScore]);   
            }

            delay(200);
        }

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

void playSong()
{
    // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  int thisNote = 0;
  
  for (thisNote = lastNote; thisNote < notes * 2 && joystick() == 0; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(BUZZER);

    lastNote = thisNote;
  }
  
  if (thisNote == notes * 2)
    {
        lastNote =0;
    }
}

void memoryValidator() {
    for (int i = 11; i <= 110; i += 11) {
        if (EEPROM.read(i) != '\0') {
            EEPROM.update(i, '\0');
            EEPROM.put(i - 10, (unsigned long)(0));
        }
    }
}

void sortPositions(unsigned int* scorePositions) {
    unsigned long a, b;
    for (int j = 0; j < 9; j++) {
        for (int i = 0; i < 9 - j; i++) {
            EEPROM.get(scorePositions[i], a);
            EEPROM.get(scorePositions[i+1], b);
            if (a < b) {
                unsigned int tmp = scorePositions[i+1];
                scorePositions[i+1] = scorePositions[i];
                scorePositions[i] = tmp;
            }
        }
    }
}

void displayScore(struct Score* score) {
    unsigned char length;
    for (length = 0; score->name[length] != '\0'; length++) {}

    lcd.setCursor(1,1);
    for (int i = 0; i < 14; i++) {
        lcd.print(" ");
    }

    unsigned char digits = countDigits(score->score);
    unsigned char totalLength = length + digits + 2;

    lcd.setCursor((16 - totalLength) / 2, 1);
    for (int i = 0; i < length; i++) {
        lcd.print(score->name[i]);
    }
    lcd.print(": ");
    lcd.print(score->score);
}

unsigned char countDigits(unsigned long num) {
    unsigned char count = 1;
    unsigned long comparison = 10;
    while (comparison <= num) {
        comparison *= 10;
        count++;
    }
    return count;
}

void menuSaveScore() {
    if (!score) { return; }

    unsigned int scorePositions[10] = {1, 12, 23, 34, 45, 56, 67, 78, 89, 100};

    memoryValidator();
    sortPositions(scorePositions);

    char writePos = 0;
    struct Score scores[10];
    for (int i = 0; i < 10; i++) {
        EEPROM.get(scorePositions[i], scores[i].score);
        if (!scores[i].score) { writePos = i; break; }
        for (int j = 0; j < 7; j++) {
            scores[i].name[j] = EEPROM.read(scorePositions[i] + j + 4);
        }
        writePos = i;
    }
    
    if (score < scores[writePos].score) { return; }

    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Player Name:");
    
    unsigned char cursor = 0;
    char currLetter = 'A';
    char name[7] = {'\0'};

    lcd.setCursor((16 - cursor) / 2 + cursor, 1);

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();

        if (currentInput == LEFT) {
            if (currLetter == 'A') { currLetter = '_'; }
            else if (currLetter == '_') { currLetter = 'Z'; }
            else { currLetter--; }
            lcd.print(currLetter);
            lcd.setCursor((16 - cursor) / 2 + cursor, 1);
            delay(400);
        } else if (currentInput == RIGHT) {
            if (currLetter == 'Z') { currLetter = '_'; }
            else if (currLetter == '_') { currLetter = 'A'; }
            else { currLetter++; }
            lcd.print(currLetter);
            lcd.setCursor((16 - cursor) / 2 + cursor, 1);
            delay(400);
        } else if (currentInput == UP) {
            if (!cursor) {
                if (cancelMenu()) { return; }
                else {
                    lcd.clear();
                    lcd.setCursor(2,0);
                    lcd.print("Player Name:");
                    lcd.setCursor((16 - cursor) / 2, 1);
                    for (int i = 0; name[i] != '\0'; i++) {
                        lcd.print(name[i]);
                    }
                    lcd.setCursor((16 - cursor) / 2 + cursor, 1);

                    delay(500);
                    continue;
                }
            }

            name[cursor--] = '\0';
            currLetter = name[cursor];

            lcd.setCursor(1,1);
            for (int i = 0; i < 12; i++) {
                lcd.print(" ");
            }

            lcd.setCursor((16 - cursor) / 2, 1);
            for (int i = 0; name[i] != '\0'; i++) {
                lcd.print(name[i]);
            }
            lcd.setCursor((16 - cursor) / 2 + cursor, 1);
            delay(200);
        } else if (currentInput == DOWN) {
            if (currLetter == '_') {
                if (!cursor) {
                    if (cancelMenu()) { return; }
                    else {
                        lcd.clear();
                        lcd.setCursor(2,0);
                        lcd.print("Player Name:");
                        lcd.setCursor((16 - cursor) / 2, 1);
                        for (int i = 0; name[i] != '\0'; i++) {
                            lcd.print(name[i]);
                        }
                        lcd.setCursor((16 - cursor) / 2 + cursor, 1);

                        delay(500);
                        continue;
                    }
                }
                if (confirmMenu(name)) {
                    EEPROM.put(scorePositions[writePos], score);
                    int i;
                    for (i = 0; name[i] != '\0'; i++) {
                        EEPROM.update(scorePositions[writePos] + i + 4, name[i]);
                    }
                    EEPROM.update(scorePositions[writePos] + i + 4, '\0');
                    return;
                } else {
                    lcd.clear();
                    lcd.setCursor(2,0);
                    lcd.print("Player Name:");
                    lcd.setCursor((16 - cursor) / 2, 1);
                    for (int i = 0; name[i] != '\0'; i++) {
                        lcd.print(name[i]);
                    }
                    lcd.setCursor((16 - cursor) / 2 + cursor, 1);

                    delay(500);
                    continue;
                }
            }
            
            name[cursor++] = currLetter;
            currLetter = 'A';

            if (cursor > 5) {
                if (confirmMenu(name)) {
                    EEPROM.put(scorePositions[writePos], score);
                    int i;
                    for (i = 0; name[i] != '\0'; i++) {
                        EEPROM.update(scorePositions[writePos] + i + 4, name[i]);
                    }
                    EEPROM.update(scorePositions[writePos] + i + 4, '\0');
                    return;
                } else {
                    cursor--;
                    lcd.clear();
                    lcd.setCursor(2,0);
                    lcd.print("Player Name:");
                    lcd.setCursor((16 - cursor) / 2, 1);
                    for (int i = 0; name[i] != '\0'; i++) {
                        lcd.print(name[i]);
                    }
                    lcd.setCursor((16 - cursor) / 2 + cursor, 1);
                }
            }

            lcd.setCursor((16 - cursor) / 2, 1);
            for (int i = 0; i < cursor; i++) {
                lcd.print(name[i]);
            }
            lcd.setCursor((16 - cursor) / 2 + cursor, 1);
            delay(400);
        }

        if (!show) { lcd.print(' '); lcd.setCursor((16 - cursor) / 2 + cursor, 1); }
        else { lcd.print(currLetter); lcd.setCursor((16 - cursor) / 2 + cursor, 1); }

        blinkArrows();
        delay(16);
    }
}

int cancelMenu() {
    unsigned char choice = 0;

    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Exit?");

    lcd.setCursor(7,1);
    lcd.print("No");

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();

        if (currentInput == LEFT || currentInput == RIGHT) {
            lcd.setCursor(0,1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15,1);
            lcd.write(RIGHT_ARROW);
            show = 1;

            choice++;
            if (choice > 1) { choice = 0; }

            lcd.setCursor(1,1);
            for (int i = 0; i < 12; i++) {
                lcd.print(" ");
            }

            if (!choice) {
                lcd.setCursor(7,1);
                lcd.print("No");
            } else {
                lcd.setCursor(6,1);
                lcd.print("Yes");
            }

            delay(200);
        } else if (currentInput == DOWN) { return choice; }

        blinkArrows();
        delay(16);
    }
}

int confirmMenu(char* name) {
    lcd.clear();

    unsigned char length;
    for (length = 0; name[length] != '\0'; length++) {}
    unsigned char totalLength = 7 + length;

    lcd.setCursor((16 - totalLength) / 2, 0);
    lcd.print("Is it ");
    for (int i = 0; name[i] != '\0'; i++) { lcd.print(name[i]); }
    lcd.print("?");

    lcd.setCursor(6,1);
    lcd.print("Yes");
    unsigned char choice = 1;

    delay(500);

    while (1) {
        unsigned char currentInput = joystick();

        if (currentInput == LEFT || currentInput == RIGHT) {
            lcd.setCursor(0,1);
            lcd.write(LEFT_ARROW);
            lcd.setCursor(15,1);
            lcd.write(RIGHT_ARROW);
            show = 1;

            choice++;
            if (choice > 1) { choice = 0; }

            lcd.setCursor(1,1);
            for (int i = 0; i < 12; i++) {
                lcd.print(" ");
            }

            if (!choice) {
                lcd.setCursor(7,1);
                lcd.print("No");
            } else {
                lcd.setCursor(6,1);
                lcd.print("Yes");
            }

            delay(200);
        } else if (currentInput == DOWN) { return choice; }

        blinkArrows();
        delay(16);
    }
}