
#include <MsTimer2.h>

#define ROW 5
#define COL 4
#define PAG 2
#define GRD 32

//pin assignment
//enable pin
byte row[PAG][ROW] = {
                {14, 15, 16, 17, 18},
                {14, 15, 16, 17, 18},
//               {3, 4, 7, 11, 12},
//               {3, 4, 7, 11, 12},
              };
//pwm pin timer0, timer1
byte col[COL] = {5, 6, 9, 10};

//for debug
byte rp = 0;
byte rr = 0;
byte rc = 0;
byte rf = 0;

//matrix count
byte p = 0;
byte r = 0;

//counter, loop
int ca = 0;
int cf = 1;
int lp = 0;

//v-ram
byte v[PAG][COL][ROW];

//change rayout
byte xy[COL*PAG][PAG] = {{0,0},{0,1},{1,0},{1,1},{0,2},{0,3},{1,2},{1,3}};

//drops buffer
byte ix[COL*PAG] = {0,0,0,0,0,0,0,0};
byte ixf = 0;
byte ixe = 0;

//stage count
int cntstg[]={240,600,1600,1660};
//stage delay
int dlystg[]={2000,1000,0,7500};

//luminance correction table
byte lux[PAG][GRD]={
  {0,1,2,3,4,5,6,7,8,9,10,11,12,14,16,18,20,22,24,28,32,36,40,48,56,64,80,96,104,128,152,176},
  {255,253,251,249,247,246,244,242,240,236,232,228,224,218,212,206,
   200,192,184,176,168,156,144,128,112,96,80,64,48,32,16,0}
};
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15

byte ramp_updn[]={
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
  16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
  31,31,
  31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
  15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,
  0,0,0,0,0,0
};
byte szrp[]={sizeof(ramp_updn)};

//pattern table
byte id0[91][ROW]={
  {0 , 0, 0, 0, 0},   //0
  {1 , 0, 0, 0, 0},   //1
  {2 , 0, 0, 0, 0},   //2
  {4 , 0, 0, 0, 0},   //3
  {8 , 0, 0, 0, 0},   //4
  {12, 0, 0, 0, 0},   //5
  {16, 0, 0, 0, 0},   //6
  {20, 0, 0, 0, 0},   //7
  {24, 0, 0, 0, 0},   //8
  {31, 0, 0, 0, 0},   //9
  {31, 1, 0, 0, 0},   //10
  {31, 2, 0, 0, 0},   //11
  {31, 4, 0, 0, 0},   //12
  {31, 8, 0, 0, 0},   //13
  {31,12, 0, 0, 0},   //14
  {31,16, 0, 0, 0},   //15
  {31,20, 0, 0, 0},   //16
  {31,24, 0, 0, 0},   //17
  {31,31, 0, 0, 0},   //18
  {31,31, 1, 0, 0},   //19
  {31,31, 2, 0, 0},   //20
  {31,31, 4, 0, 0},   //21
  {31,31, 8, 0, 0},   //22
  {31,31,12, 0, 0},   //23
  {31,31,16, 0, 0},   //24
  {31,31,20, 0, 0},   //25
  {31,31,24, 0, 0},   //26
  {31,31,31, 0, 0},   //27
  {31,31,31, 1, 0},   //28
  {31,31,31, 2, 0},   //29
  {31,31,31, 4, 0},   //30
  {31,31,31, 8, 0},   //31
  {31,31,31,12, 0},   //32
  {31,31,31,16, 0},   //33
  {31,31,31,20, 0},   //34
  {31,31,31,24, 0},   //35
  {31,31,31,31, 0},   //36
  {31,31,31,31, 1},   //37
  {31,31,31,31, 2},   //38
  {31,31,31,31, 4},   //39
  {31,31,31,31, 8},   //40
  {31,31,31,31,12},   //41
  {31,31,31,31,16},   //42
  {31,31,31,31,20},   //43
  {31,31,31,31,24},   //44
  {31,31,31,31,31},   //45
  {24,31,31,31,31},   //46
  {20,31,31,31,31},   //47
  {16,31,31,31,31},   //48
  {12,31,31,31,31},   //49
  { 8,31,31,31,31},   //50
  { 4,31,31,31,31},   //51
  { 2,31,31,31,31},   //52
  { 1,31,31,31,31},   //53
  { 0,31,31,31,31},   //54
  { 0,24,31,31,31},   //55
  { 0,20,31,31,31},   //56
  { 0,16,31,31,31},   //57
  { 0,12,31,31,31},   //58
  { 0, 8,31,31,31},   //59
  { 0, 4,31,31,31},   //60
  { 0, 2,31,31,31},   //61
  { 0, 1,31,31,31},   //62
  { 0, 0,31,31,31},   //63
  { 0, 0,24,31,31},   //64
  { 0, 0,20,31,31},   //65
  { 0, 0,16,31,31},   //66
  { 0, 0,12,31,31},   //67
  { 0, 0, 8,31,31},   //68
  { 0, 0, 4,31,31},   //69
  { 0, 0, 2,31,31},   //70
  { 0, 0, 1,31,31},   //71
  { 0, 0, 0,31,31},   //72
  { 0, 0, 0,24,31},   //73
  { 0, 0, 0,20,31},   //74
  { 0, 0, 0,16,31},   //75
  { 0, 0, 0,12,31},   //76
  { 0, 0, 0, 8,31},   //77
  { 0, 0, 0, 4,31},   //78
  { 0, 0, 0, 2,31},   //79
  { 0, 0, 0, 1,31},   //80
  { 0, 0, 0, 0,31},   //81
  { 0, 0, 0, 0,24},   //82
  { 0, 0, 0, 0,20},   //83
  { 0, 0, 0, 0,16},   //84
  { 0, 0, 0, 0,12},   //85
  { 0, 0, 0, 0, 8},   //86
  { 0, 0, 0, 0, 4},   //87
  { 0, 0, 0, 0, 2},   //88
  { 0, 0, 0, 0, 1},   //89
  { 0, 0, 0, 0, 0},   //90
};

//pattern table
#if 1
byte id1[32][ROW]={
  {0,0,0,0,0},{1,1,1,1,1},{2,2,2,2,2},{4,4,4,4,4},{6,6,6,6,6},{8,8,8,8,8},{10,10,10,10,10},
  {12,12,12,12,12},{16,16,16,16,16},{20,20,20,20,20},{24,24,24,24,24},{28,28,28,28,28},{31,31,31,31,31},
  {1,0,0,0,0},{8,3,1,0,0},{12,8,3,1,0},{24,12,8,3,1},
  {1,3,8,12,24},{0,1,3,8,12},{0,0,1,3,8},{0,0,0,0,1},
  {31,0,31,0,31},{0,31,0,31,0},
  {31,0,0,31,31},{31,31,0,0,31},
  {31,0,0,0,31},{31,31,0,0,0},
  {31,0,0,0,0},{31,31,31,31,0},{0,0,0,31,31},{31,31,0,0,0},{0,31,31,31,31},
};
#endif

byte szid[]={sizeof(id0)/ROW-1,sizeof(id1)/ROW-1};


void setup(){

  Serial.begin(115200);

  for(byte i = 0; i < PAG; i++){
    for(byte ii = 0; ii < ROW; ii++){
      pinMode(row[i][ii],INPUT);
      digitalWrite(row[i][ii],LOW);
    }
  }
  for(byte i = 0; i < COL; i++){
    analogWrite(col[i], 0);
  }

  for(byte i = 0; i < PAG; i++){
    for(byte ii = 0; ii < COL; ii++){
      for(byte iii = 0; iii < ROW; iii++){
        v[i][ii][iii] = 0;
      }
    }
  }

  MsTimer2::set(1, Timer1ms);
  MsTimer2::start();

  TCCR0B &= B11110000;
  TCCR0B |= B00000001;

  TCCR1A = 0;
  TCCR1B = 0;
//  TCCR1A |= (1 << WGM11)|(1 << COM1A1)|(1 << COM1B1);//|(1 << COM1A0)|(1 << COM1B0);
//  TCCR1B |= (1 << WGM13)|(1 << WGM12)|(1 << CS10);
  TCCR1A |= B10100010;  //COM1A1,COM1B1,WGM11
  TCCR1B |= B00011001;  //WGM13,WGM12,CS10
  ICR1 = 255;
//  TCCR2B &= B11111000;
//  TCCR2B |= B00000001;

  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

}

void Timer1ms(){
//ISR(TIMER0_COMPA_vect) {

  pinMode(row[p][r],INPUT);
  digitalWrite(row[p][r],LOW);

  r++;
  if(r >= ROW){
     r = 0;
     p++;
     if(p >= PAG){
      p = 0;
     }
  }

  //v[p][c][0]
  for(byte i=0; i < COL; i++){
    analogWrite(col[i], lux[p][v[p][i][r]]);
  }

  digitalWrite(row[p][r],p);
  pinMode(row[p][r],OUTPUT);

}

#if 1
int getNum() {
  byte var = 'b';
  int num=0;

  // 'a' is for debug
  while (1) {
    if(Serial.available()){
        var = Serial.read();
        if(var == 0x0d){
          break;
        }
        int i = var - 0x30;
        if (i >= 0 && i <= 9){
          num = (num*10) + i;
        }
//        Serial.println(num);
    }
//    delay(100);
  }
    
  return num;
}
#endif

void loop(){

//stage initialize
if(lp == 0){
  ca = 0;
  cf = 1;
  ixe = COL*PAG;
  ix[ixe-1] = szid[0];
}

//stage 0
if(lp < cntstg[0]){
#if 1
  for(byte i=0; i < PAG; i++){
    for(byte ii=0; ii < COL; ii++){
      for(byte iii=0; iii < ROW; iii++){
        v[i][ii][iii] = ramp_updn[ca];
      }
    }
  }
  ca++;
  if(ca >= szrp[0]){
    ca = 0;
  }
  delay(dlystg[0]);
#endif

//stage 1
}else if(lp >= cntstg[0] && lp < cntstg[1]){
#if 1
//byte v[PAG][COL][ROW];
  for(byte i=0; i < 8; i++){
    for(byte ii=0; ii < ROW; ii++){
    v[xy[i][0]][xy[i][1]][ii] = id0[ca][ii];
    }
  }
  if(ca >= szid[0]){
    cf = -1;
    delay(20000);
  }
  if(ca <= 0){
    cf = 1;
    delay(20000);
  }
  ca += cf;
  delay(dlystg[1]);
#endif

//stage 2
}else if(lp >= cntstg[1] && lp < cntstg[2]){
#if 1
//byte v[PAG][COL][ROW];
  for(byte i=0; i < 8; i++){
    for(byte ii=0; ii < ROW; ii++){
      v[xy[i][0]][xy[i][1]][ii] = id0[ix[i]][ii];
    }
  }

//byte ix[8] = {0,0,0,0,0,0,0,0};
//byte ixf = 0;
//byte ixe = 0;
//byte id0[59][5]
// 0-29-58
// 0-45-90

  //up block
  if(ixf < COL*PAG && ix[ixf] == szid[0]>>1){
    ixf++;
  }//down block
  if(ixe < COL*PAG && ix[ixe] == szid[0]){
    ixe++;
  }//ramp up
  if(ixf < COL*PAG){
    ix[ixf]+=1;
  }//ramp down
  if(ixf > 1){
    ix[ixe]+=1;
  }
  if(ixe >= COL*PAG && ix[ixe-1] >= szid[0]){
    ixf = ixe = 0;
    for(byte i=0; i < COL*PAG; i++){
      ix[i] = 0;
    }
  }
  //delay(dlystg[2]);
  delay(lp>>1);
#endif

//stage 3
}else if(lp >= cntstg[2] && lp < cntstg[3]){
#if 1
//byte v[PAG][COL][ROW];
  for(byte i=0; i < COL*PAG; i++){
    for(byte ii=0; ii < ROW; ii++){
    v[xy[i][0]][xy[i][1]][ii] = id1[0x1f&(ca+i)][ii];
    }
  }
  ca++;
  if(ca >= GRD){
    ca = 0;
  }
  delay(dlystg[3]);
#endif

//end of stage
}
lp++;
if(lp > cntstg[3]){
  lp = 0;
}

//for debug;
#if 0
  if(rf == 0){
    rp = getNum();
    rf++;
  }else if(rf == 1){
    rr = getNum();
    rf++;
  }else if(rf == 2){
    rc= getNum();
    rf++;
  }else{
    v[rp][rr][rc] = getNum();
    rf = 0;
  }
#endif

  delay(200);



}
