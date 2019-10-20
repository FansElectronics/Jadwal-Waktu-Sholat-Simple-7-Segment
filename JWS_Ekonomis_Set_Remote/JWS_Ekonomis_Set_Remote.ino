//==================================================================================================================
//  Program ini dibuat dengan tujuan amal, 100% digratiskan untuk membuat Jadwal Waktu Sholat.
//  Program mendukung pelestarian Masjid diseluruh dunia khususnya Indonesia ataupun sarana pendidikan.
//  Bagi anda yg mengunduh dan program ini disarankan untuk tidak menghapus catatan yg ada didalamnya.
//  Bagi anda yang ingin menjual belikan Program ini diwajibkan membayar royalty sebesar 2.5% kepada pembuat.
//  Dana dapat anda masukan pada kota amal masjid yg ada disekitar anda atau anda dapat mendonasikan kepada
//  kami sebagai dana untuk membuat JWS di masjid sekitar kami atau pengembangan produk Open Source lainnya.
//==================================================================================================================
//  Catatan & Program ditulis oleh:
//      Nama        : Irfan Indra Kurniawan
//      Organisasi  : Fans Electronics
//      Website     : www.fanselectronics.com
//      Email       : info@fanselectronics.com
//      GitHub      : github.com/FansElectronics
//
//      Project     : Jadwal Waktu Sholat Sederhana 1" Segment + Remote TV Setting
//      Version     : V1.2.1
//      Dibuat      : 1 Februari 2018
//      Modif       : 20 Oktober 2019 
//==================================================================================================================
#include <Timer.h>
#include <RTClib.h>
#include <PrayerTimes.h>
#include <IRremote.h>
#include <avr/eeprom.h>

//==================================================================================================================
#define Data      A1
#define Clock     7
#define Latch     6
#define ledBlink  A0
#define IR        11
#define buzzer    1
#define mp3_rx    0
#define mp3_tx    A2
#define bUP       2
#define bOK       3
#define bDN       4

#define ON        HIGH
#define OFF       LOW

//==================================================================================================================
byte jedaAdzan =  1;                       // Waktu adzan (Menit)
byte jedaIqomah[] = {1, 1, 1, 1, 2};       // Waktu tunggu iqomah (Menit) >> {subuh,dhuhur,ashar,maghrib,isya}
byte jedaMati = 2;                         // Waktu mati sholat (Menit)
byte jedaJumat = 30;                       // Waktu mati Jumat (Menit)
byte selisihWaktu = 2;                     // Selisi waktu sholat (Menit)
float lintang = -7.98;                     // Latitude
float bujur = 112.62;                      // Longitude
byte zonaWaktu = 7;                        // Zona Waktu

//==================================================================================================================
byte lowChar[]      = { B10000001, B10110111, B11000010, B10010010, B10110100, B10011000,
                        B10001000, B10110011, B10000000, B10010000, B11111111, B11111110
                      };
byte lowCharPoint[] = { B00000001, B00110111, B01000010, B00010010, B00110100, B00011000,
                        B00001000, B00110011, B00000000, B00010000, B01111111, B01111110
                      };
byte textChar[]     = { B10100000, B10110111, B10000101, B10001111, B11001000, B10000001, B10001110,
                        B10000111, B11001101, B10100001, B10101110, B10100100, B10101100, B10010100,
                        B11001100, B11000010, B11101001, B10001100, B10000110, B10011000, B10010000,
                        B11110111, B11100000, B11100001, B10110001
                      };
byte ledJWS[]       = { B00111110, B00111101, B00111011, B00110111, B00101111, B00011111, B00111111, B11111111};

char Key, charBuffer[10], floatBuff[4];
double times[sizeof(TimeName) / sizeof(char*)];
long val;
byte i, r, v, e, a, n;
int jamSholat, mntSholat, jam, mnt, dtk, tgl, bln , thn, ttk, tDtk, mntTunggu;
byte ratusanribu, puluhanribu, ribuan, ratusan, puluhan, satuan;
bool setting = false;
bool standby = false;
bool access  = false;
bool adzan   = false;
bool titik   = false;

//==================================================================================================================
RTC_DS3231 rtc;
IRrecv remote(IR);
decode_results valRemote;
Timer tBlink, tMain, tStandBy, tHold;

//==================================================================================================================
void setup() {
  rtc.begin();
  remote.enableIRIn();

  set_calc_method(ISNA);                                  // Methode perhitungan mengikuti ISNA
  set_asr_method(Shafii);                                 // Pendapat Imam Syafi'i
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);                                     // Sudut Waktu Subuh
  set_isha_angle(18);                                     // Sudut Waktu Isya

  pinMode(Data, OUTPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Latch, OUTPUT);
  pinMode(ledBlink, OUTPUT);
  pinMode(buzzer, OUTPUT);

  tBlink.oscillate(ledBlink, 500, LOW);
  tStandBy.every(5000, clearAllDisplay);
  tMain.every(1000, mainDisplay);
  tMain.every(800, realTimeScanJWS);
  tMain.every(2000, readAllEEPROM);
  tMain.every(5000, interruptSlideShow);
  tHold.every(1000, holdTime);

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2017,10,7,11,18,30));
  a = 8;
}

//==================================================================================================================
// LOOPING PROGRAM UTAMA //

void loop() {
  DateTime now = rtc.now();
  get_prayer_times(now.year(), now.month(), now.day(), lintang, bujur, zonaWaktu, times);

  if (remote.decode(&valRemote)) {
    Key = keyChar(valRemote.value);
    remote.resume();
  }

  if (Key == 'P' or (now.hour() == 0 and now.minute() == 0)) {                                    // Tombol Power
    standby = true; Key = 0;
    clearAllDisplay();
  }
  else if (Key == 'O') {                               // Tombol OK
    setting = true; Key = 0;
  }
  while (setting) {
    menuSetting();
  }

  while (standby) {
    displayStandBy();
  }
  if (!setting) {
    tBlink.update();
    tMain.update();
  }

}


//==================================================================================================================
void holdTime() {
  tDtk++;
  if (tDtk >= 60) {
    mntTunggu++; tDtk = 0;
  }
}
//==================================================================================================================
void resetHoldTime() {
  tDtk = 0; mntTunggu = 0;
}
//==================================================================================================================
void interruptSlideShow() {
  if (r > 5) {
    r = 0;
  }
  switch (r) {
    case 0 : a = 8; break;
    case 1 : a = 0; break;
    case 2 : a = 2; break;
    case 3 : a = 3; break;
    case 4 : a = 5; break;
    case 5 : a = 6; break;
  }
  r++;
}
//==================================================================================================================
