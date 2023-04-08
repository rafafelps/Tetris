#include <MD_KeySwitch.h>
#include <MD_MAX72xx.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW// PAROLA_HW
#define MAX_DEVICES 4
#define CLK_PIN  13  // or SCK
#define DATA_PIN 11  // or MOSI
#define CS_PIN  10  // or SS
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE,CS_PIN, MAX_DEVICES);

const int potmeter = 3;
int coord[200][2];
int leng = 1;
int prev;
int point[5][2] {{10,5},{3,1},{25,3},{15,7},{11,3}};

int ch1 = 5;
int ch2 = 8;
int ch3 = 9;
int ch4 = 4;
int aa = 6;
int bb = 19;
int cc = 2;
int dd = 1;
int ee = 0;
int ff = 7;
int gg = 3;
int dot = 25;
int delay1=3;

int blue = 12;
int green = 18;

int screens = 1;
int record;
int points;
int screenpick = 0;
int diff = 0;
int speedx;
int supermode;
int doublepoints;
long counter;


void slide()
{
  for(int x = 99; x > 0; x--)
  {
    coord[x][0] = coord[x-1][0];
    coord[x][1] = coord[x-1][1];
  }
  return;
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
    prev = 0;
    return 0;
  }
  if(analogRead(VRy) < 120)
  {
    prev = 1;
    return 1;
  }
  if(analogRead(VRx) > 900)
  {
    prev = 2;
    return 2;
  }
  if(analogRead(VRx) < 120)
  {
    prev = 3;
    return 3;
  }
  if(analogRead(SW) == 0)
  {
    return 5;
  }
  return prev;
}


void setup() 
{
  mx.begin();
  coord[1][0] = coord[0][0] =  5;
  coord[1][1] = coord[0][1] =  5;
//   pinMode(ch1, OUTPUT);
//  pinMode(ch2, OUTPUT);
//  pinMode(ch3, OUTPUT);
//  pinMode(ch4, OUTPUT);
//  pinMode(aa, OUTPUT);
//  pinMode(bb, OUTPUT);
//  pinMode(cc, OUTPUT);
//  pinMode(dd, OUTPUT);
//  pinMode(ee, OUTPUT);
//  pinMode(ff, OUTPUT);
//  pinMode(gg, OUTPUT);
//  pinMode(dot, OUTPUT);
//  pinMode(blue, OUTPUT);
//  pinMode(green, OUTPUT);
}

void led(int x, int y)
{
  mx.setPoint(y-1, x-1, 1);
  return;
}



void loop() 
{

  while (1) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 32; j++) {
        mx.setPoint(i, j, 1);
        delay(5);
      }
    }
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 32; j++) {
        mx.setPoint(i, j, 0);
        delay(5);
      }
    }  
  }
  return;
  while(screens == 1)
  {
    supermode = 0;
    doublepoints = 0;
    digitalWrite(blue,0);
    digitalWrite(green,0);
    mx.clear();
    if (points > record)
    {
      record = points;
    }
    coord[1][0] = coord[0][0] =  5;
    coord[1][1] = coord[0][1] =  5;
    while(screens == 1)
    {
      diff = analogRead(potmeter) / 100;
      if (Joystick() == 5)
      {
        screens = 0;
        speedx = (12-diff)*9;
        screenpick = 0;
        leng = 1;
        points = 0;
      }
      if (analogRead(0) > 1000)
      {
        screenpick++;
        if(screenpick > 2) screenpick = 0;
        mx.clear();
        delay(1000);
      }
      if (analogRead(0) < 150)
      {
        screenpick--;
        if(screenpick < 0) screenpick = 2;
        mx.clear();
        delay(1000);
      }
      switch(screenpick)
      {
        case 0: 
        printNumber(points,0);
        led(30,3);
        led(30,4);
        led(30,5);
        led(30,6);
        led(29,3);
        led(29,4);
        led(27,5);
        led(27,4);
        led(26,3);
        led(26,6);
        led(25,3);
        led(25,6);
        led(24,4);
        led(24,5);
        led(22,5);
        led(22,3);
        led(22,4);
        led(22,6);
        led(20,3);
        led(20,4);
        led(20,5);
        led(20,6);
        led(19,4);
        led(18,5);
        led(17,3);
        led(17,4);
        led(17,5);
        led(17,6);
        led(15,3);
        led(14,3);
        led(13,3);
        led(14,6);
        led(14,5);
        led(14,4);
        break;

        case 1:
        printNumber(record,0);
        led(30,3);
        led(30,4);
        led(30,5);
        led(30,6);
        led(29,3);
        led(29,4);
        led(28,5);
        led(27,6);
        led(25,3);
        led(25,4);
        led(25,5);
        led(25,6);
        led(25,7);
        led(24,7);
        led(23,7);
        led(24,5);
        led(23,5);
        led(24,3);
        led(23,3);
        led(21,4);
        led(21,5);
        led(20,6);
        led(19,6);
        led(20,3);
        led(19,3);
        break;

        case 2:
        printNumber(diff,0);
        led(30,3);
        led(30,4);
        led(30,5);
        led(30,6);
        led(29,3);
        led(29,6);
        led(28,5);
        led(28,4);
        led(26,3);
        led(26,4);
        led(26,5);
        led(26,6);
        led(24,3);
        led(24,4);
        led(24,5);
        led(24,6);
        led(23,5);
        led(23,3);
        led(22,3);
        led(20,3);
        led(20,4);
        led(20,5);
        led(20,6);
        led(19,3);
        led(19,5);
        led(18,3);
        break;
        
      }
    }
    
  }
  for (int x = 0; x < speedx; x++)
  {
    printNumber(points,0);
  }
  slide();
  counter++;
  if(counter == 40)
  {
    supermode = 0;
    doublepoints = 0;
    digitalWrite(blue,0);
    digitalWrite(green,0);
  }
  switch(Joystick())
  {
    case 0:
    coord[0][1] = coord[1][1]+1; //y+1
    break;
    case 1:
    coord[0][1] = coord[1][1]-1; //y-1
    break;
    case 2:
    coord[0][0] = coord[1][0]+1; //x+1
    break;
    case 3:
    coord[0][0] = coord[1][0]-1; //x-1
    break;
  }
  for (int x = 0; x < 5; x++)
  {
     if (coord[0][0] == point[x][0] && coord[0][1] == point[x][1])
    {
      if (random(0,diff+1) == 0)
      {
        supermode = 1;
        counter = 0;
        digitalWrite(blue,1);
      }
      if (random(0,9) == 0)
      {
        doublepoints = 1;
        counter = 0;
        digitalWrite(green,1);
      }
      leng++;
      points += 5;
      if(doublepoints == 1) points += 5;
      if (speedx > (10-diff)*3 + 10)
      {
        speedx--;
      }
      point[x][0] = random(1,32);
      point[x][1] = random(1,8);
    }
 }
  if (supermode == 1)
  {
    if(coord[0][0] == 0)
    {
      coord[0][0] = 32;
    }
    if(coord[0][0] == 33)
    {
      coord[0][0] = 1;
    }
    if(coord[0][1] == 0)
    {
      coord[0][1] = 8;
    }
    if(coord[0][1] == 9)
    {
      coord[0][1] = 1;
    }
  }
  mx.clear();
  for (int x = 0; x < leng; x++)
  {
    led(coord[x][0],coord[x][1]);
  }
  for (int x = 0; x < 5; x++)
  {
    led(point[x][0],point[x][1]);
  }
  if (supermode == 0)
  {
     if (coord[0][0] == 0 || coord[0][0] == 33 || coord[0][1] == 0 || coord[0][1] == 9)
     {
      screens = 1;
     }
  }
  
 
  for(int x = 1; x < leng; x++)
  {
    if (coord[0][0] == coord[x][0] && coord[0][1] == coord[x][1])
    {
      screens = 1;
    }
  }
}

void printNumber(long number,int place)
{
  int num1 = number / 1000;
  int num2 = number / 100 - num1 * 10;
  int num3 = number / 10  - num1 * 100 - num2 * 10;
  int num4 = number       - num1 * 1000- num2 * 100 - num3 * 10;

  //charachter 1
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
  digitalWrite(dot,0);
  
  digitalWrite(ch1,0);
  digitalWrite(ch2,1);
  digitalWrite(ch3,1);
  digitalWrite(ch4,1);

  if(place==1)
  {
    digitalWrite(dot,1);
  }

  switch(num1)
  {
    case 0: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,0);
    break;

    case 1:
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 2: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,0);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 3: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 4: 
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 5: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 6: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 7: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 8: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 9: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

    default: 
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;
  }

  delay(delay1);

  
  //charachter 2
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
  digitalWrite(dot,0);

  digitalWrite(ch1,1);
  digitalWrite(ch2,0);
  digitalWrite(ch3,1);
  digitalWrite(ch4,1);

  if(place==2)
  {
    digitalWrite(dot,1);
  }

  switch(num2)
  {
    case 0: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,0);
    break;

    case 1:
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 2: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,0);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 3: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 4: 
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 5: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 6: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 7: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 8: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 9: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

    default:
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;
  }

  delay(delay1);

  
  //charachter 3
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
  digitalWrite(dot,0);

  digitalWrite(ch1,1);
  digitalWrite(ch2,1);
  digitalWrite(ch3,0);
  digitalWrite(ch4,1);

  if(place==3)
  {
    digitalWrite(dot,1);
  }

  switch(num3)
  {
    case 0: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,0);
    break;

    case 1:
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 2: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,0);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 3: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 4: 
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 5: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 6: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 7: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 8: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 9: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

    default:
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;
  }



  delay(delay1);

  
  //charachter 4
  digitalWrite(dot,0);
  
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
  
  digitalWrite(ch1,1);
  digitalWrite(ch2,1);
  digitalWrite(ch3,1);
  digitalWrite(ch4,0);
  if(place==4)
  {
    digitalWrite(dot,1);
  }

  switch(num4)
  {
    case 0: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,0);
    break;

    case 1:
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 2: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,0);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 3: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,1);
    break;

     case 4: 
  digitalWrite(aa,0);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 5: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 6: 
  digitalWrite(aa,1);
  digitalWrite(bb,0);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 7: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;

     case 8: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,1);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

     case 9: 
  digitalWrite(aa,1);
  digitalWrite(bb,1);
  digitalWrite(cc,1);
  digitalWrite(dd,1);
  digitalWrite(ee,0);
  digitalWrite(ff,1);
  digitalWrite(gg,1);
    break;

    default:
  digitalWrite(aa,0);
  digitalWrite(bb,0);
  digitalWrite(cc,0);
  digitalWrite(dd,0);
  digitalWrite(ee,0);
  digitalWrite(ff,0);
  digitalWrite(gg,0);
    break;
  }
  delay(delay1);
}
