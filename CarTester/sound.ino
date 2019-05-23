//http://themushroomkingdom.net/stuff/sheet_smb_over1.jpg
//http://themushroomkingdom.net/stuff/sheet_smb_over2.jpg
//http://themushroomkingdom.net/stuff/sheet_smb_over3.jpg

// (c) Vitaliy Sorokovikov 2017
// mailto:ramer-mobile@yandex.ru

#define TEMPO 10
#define NOTELATENCY 5
#define DURATION_BITS 2
#define NOTE_BITS 6

uint16_t note[] = {0,262,330,392,415,440,466,494,523,587,622,659,698,740,784,880,1047}; // Hz
byte duration[] = {20,15,30,60}; // 1/10 of necessary tempo
uint32_t timeplayer = millis();
byte melodyphrase = 0;
byte phraseposition = 0;

//mario phrases data
//byte m[] = {75,139,75,64,72,139,206,195,136,64,67,128,130,64,69,64,71,64,70,133,3,11,14,143,76,78,64,75,64,72,73,71,128,136,64,67,128,130,64,69,64,71,64,70,133,3,11,14,143,76,78,64,75,64,72,73,71,128,128,78,77,76,138,75,64,68,69,72,64,69,72,73,128,78,77,76,138,75,64,80,64,80,144,128,128,78,77,76,138,75,64,68,69,72,64,69,72,73,128,138,64,73,128,200,192,128,78,77,76,138,75,64,68,69,72,64,69,72,73,128,78,77,76,138,75,64,80,64,80,144,128,128,78,77,76,138,75,64,68,69,72,64,69,72,73,128,138,64,73,128,200,192,72,136,72,64,72,137,75,136,69,195,72,136,72,64,72,73,75,192,192,72,136,72,64,72,137,75,136,69,195,75,139,75,64,72,139,206,195,136,64,67,128,130,64,69,64,71,64,70,133,3,11,14,143,76,78,64,75,64,72,73,71,128,136,64,67,128,130,64,69,64,71,64,70,133,3,11,14,143,76,78,64,75,64,72,73,71,128,75,136,67,128,132,69,140,76,197,7,15,15,15,14,12,75,136,69,195,75,136,67,128,132,69,140,76,197,71,140,76,12,11,9,72,131,67,193,75,136,67,128,132,69,140,76,197,7,15,15,15,14,12,75,136,69,195,75,136,67,128,132,69,140,76,197,71,140,76,12,11,9,72,131,67,193,72,136,72,64,72,137,75,136,69,195,72,136,72,64,72,73,75,192,192,72,136,72,64,72,137,75,136,69,195,75,139,75,64,72,139,206,195,75,136,67,128,132,69,140,76,197,7,15,15,15,14,12,75,136,69,195,75,136,67,128,132,69,140,76,197,71,140,76,12,11,9,72,131,67,193,0}; // whole melody
byte mI[] = {75,139,75,64,72,139,206,195,0};
byte mA[] = {136,64,67,128,130,64,69,64,71,64,70,133,3,11,14,143,76,78,64,75,64,72,73,71,128,0};
byte mBa[] = {128,78,77,76,138,75,64,68,69,72,64,69,72,73,0};
byte mBb[] = {128,78,77,76,138,75,64,80,64,80,144,128,0};
byte mBc[] = {128,138,64,73,128,200,192,0};
byte mCa[] = {72,136,72,64,72,137,75,136,69,195,0};
byte mCb[] = {72,136,72,64,72,73,75,192,192,0};
byte mCc[] = {75,139,75,64,72,139,206,195,0};
byte mDa[] = {75,136,67,128,132,69,140,76,197,0};
byte mDb[] = {7,15,15,15,14,12,75,136,69,195,0};
byte mDc[] = {71,140,76,12,11,9,72,131,67,193,0};

//mario phrases order: I A A Ba Bb Ba Bc Ba Bb Ba Bc Ca Cb Ca Cc A A Da Db Da Dc Da Db Da Dc Ca Cb Ca Cc Da Db Da Dc
byte* melody[] = {mI,mA,mA,mBa,mBb,mBa,mBc,mBa,mBb,mBa,mBc,mCa,mCb,mCa,mCc,mA,mA,mDa,mDb,mDa,mDc,mDa,mDb,mDa,mDc,mCa,mCb,mCa,mCc,mDa,mDb,mDa,mDc,0};

void loop_player() {
  if (playing && millis() > timeplayer) {
    play(melody[melodyphrase]);
    if (melody[melodyphrase] == 0) {playing = false; melodyphrase = 0; phraseposition = 0;}
  } 
}
void play(byte *phrase) {
  //[duration|  note  ]
  //[_2_bits_|_6_bits_]
  if (phrase[phraseposition] == 0) {melodyphrase++; phraseposition = 0; return;}
  byte n = (phrase[phraseposition]); n <<= DURATION_BITS; n >>= DURATION_BITS; 
  byte d = (phrase[phraseposition]); d >>= NOTE_BITS;
  if (n == 0) { // pause
    timeplayer = millis() + (duration[d] * TEMPO);
  } else { // play only first 100 ms of note = stacatto
    tone(PIN_TONE, note[n], duration[d] * NOTELATENCY);
    timeplayer = millis() + duration[d] * TEMPO;
  }
  phraseposition++;
}