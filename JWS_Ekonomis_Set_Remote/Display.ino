//==================================================================================================================
void mainDisplay() {
  DateTime now = rtc.now();
  digitalWrite(Latch, LOW);
  clearDigitDisplay(2, LOW);
  slideShowJWS();
  printDate(now.day(), now.month(), now.year());
  printTime(now.hour(), now.minute());
  digitalWrite(Latch, HIGH);
}
//==================================================================================================================
void displayStandBy() {
  DateTime now = rtc.now();
  if (remote.decode(&valRemote)) {
    Key = keyChar(valRemote.value);
    remote.resume();
  }
  if (Key == 'P' or (now.hour() == 3 and now.minute() == 0)) {                               // Tombol Power
    Key = (char)0; standby = false;
  }
  tBlink.update();
  tStandBy.update();
}

//==================================================================================================================
void printDate(byte _tgl, byte _bln, int _thn) {
  int _date = (_tgl * 100) + _bln;
  printRibuan(_thn);
  printRibuan(_date);
}

//==================================================================================================================
void printTime(byte _jam, byte _mnt) {
  int wkt = (_jam * 100) + _mnt;
  printRibuan(wkt);
}

//==================================================================================================================
void clearDigitDisplay(int jumlahDigit, int aktif) {
  for (i = 0; i < jumlahDigit; i++) {
    if (aktif == LOW) {
      shiftOut(Data, Clock, MSBFIRST, lowChar[10]);
    }
  }
}

//==================================================================================================================
void clearAllDisplay() {
  digitalWrite(Latch, LOW);
  for (i = 0; i < 40; i++) {
    shiftOut(Data, Clock, MSBFIRST, lowChar[10]);
  }
  digitalWrite(Latch, HIGH);
}

//==================================================================================================================
void shiftLedJWS(int jws) {
  switch (jws) {
    case 0: shiftOut(Data, Clock, MSBFIRST, ledJWS[1]); break;
    case 1: shiftOut(Data, Clock, MSBFIRST, ledJWS[6]); break;
    case 2: shiftOut(Data, Clock, MSBFIRST, ledJWS[2]); break;
    case 3: shiftOut(Data, Clock, MSBFIRST, ledJWS[3]); break;
    case 4: shiftOut(Data, Clock, MSBFIRST, ledJWS[6]); break;
    case 5: shiftOut(Data, Clock, MSBFIRST, ledJWS[4]); break;
    case 6: shiftOut(Data, Clock, MSBFIRST, ledJWS[5]); break;
    case 7: shiftOut(Data, Clock, MSBFIRST, ledJWS[6]); break;
    case 8: shiftOut(Data, Clock, MSBFIRST, ledJWS[0]); break;
    case 9: shiftOut(Data, Clock, MSBFIRST, ledJWS[7]); break;
  }
}

//==================================================================================================================
void printRibuan(int nilai) {
  ribuan = nilai / 1000;
  ratusan = (nilai % 1000) / 100;
  puluhan = ((nilai % 1000) % 100) / 10;
  satuan = (((nilai % 1000) % 100) % 10) % 10;

  if (nilai >= 1000 && nilai <= 9999) {
    shiftData(satuan, LOW, false);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, false);
  }
  else if (nilai >= 100 && nilai <= 999) {
    shiftData(satuan, LOW, false);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(0, LOW, false);
  }
  else if (nilai >= 10 && nilai <= 99) {
    shiftData(satuan, LOW, false);
    shiftData(puluhan, LOW, false);
    shiftData(0, LOW, false);
    shiftData(0, LOW, false);
  }
  else {
    shiftData(satuan, LOW, false);
    shiftData(0, LOW, false);
    shiftData(0, LOW, false);
    shiftData(0, LOW, false);
  }
}

//==================================================================================================================
void printGeo(long nilai) {
  if (nilai >= 0) {
    ratusanribu = nilai / 100000;
    puluhanribu = (nilai % 100000) / 10000;
    ribuan = ((nilai % 100000) % 10000) / 1000;
    ratusan = (((nilai % 100000) % 10000) % 1000) / 100;
    puluhan = ((((nilai % 100000) % 10000) % 1000) % 100) / 10;
    satuan = ((((nilai % 100000) % 10000) % 1000) % 100) % 10;
  }
  else {
    ratusanribu = abs(nilai / 100000);
    puluhanribu = abs((nilai % 100000) / 10000);
    ribuan = abs(((nilai % 100000) % 10000) / 1000);
    ratusan = abs((((nilai % 100000) % 10000) % 1000) / 100);
    puluhan = abs(((((nilai % 100000) % 10000) % 1000) % 100) / 10);
    satuan = abs(((((nilai % 100000) % 10000) % 1000) % 100) % 10);

  }
  if (nilai >= 100000 && nilai <= 999999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, true);
    shiftData(puluhanribu, LOW, false);
    shiftData(ratusanribu, LOW, false);
  }

  else if (nilai >= 10000 && nilai <= 99999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, true);
    shiftData(puluhanribu, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai >= 1000 && nilai <= 9999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, true);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai >= 100 && nilai <= 999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai >= 10 && nilai <= 99) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai < 10 && nilai >= 0) {
    shiftData(satuan, LOW, true);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }

  else if (nilai <= -1 && nilai >= -10) {
    shiftData(satuan, LOW, true);
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai <= -10 && nilai >= -99) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai <= -100 && nilai >= -999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai <= -1000 && nilai >= -9999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, true);
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
  }
  else if (nilai <= -10000 && nilai >= -99999) {
    shiftData(satuan, LOW, true);
    shiftData(puluhan, LOW, false);
    shiftData(ratusan, LOW, false);
    shiftData(ribuan, LOW, true);
    shiftData(puluhanribu, LOW, false);
    shiftData(11, LOW, false);
  }
}

//==================================================================================================================
void printPuluhan(byte nilai) {
  puluhan = nilai / 10;
  satuan = nilai % 10;

  if (nilai >= 10 && nilai <= 99) {
    shiftData(satuan, LOW, false);
    shiftData(puluhan, LOW, false);
  }
  else {
    shiftData(satuan, LOW, false);
    shiftData(10, LOW, false);
  }
}
//==================================================================================================================
void shiftData(int nilai, int aktif, bool point) {
  if (aktif == LOW) {
    if (point) {
      switch (nilai) {
        case 0: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[0]); break;
        case 1: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[1]); break;
        case 2: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[2]); break;
        case 3: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[3]); break;
        case 4: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[4]); break;
        case 5: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[5]); break;
        case 6: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[6]); break;
        case 7: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[7]); break;
        case 8: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[8]); break;
        case 9: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[9]); break;
        case 10: shiftOut(Data, Clock, MSBFIRST, lowCharPoint[10]); break;
      }
    }
    else {
      switch (nilai) {
        case 0: shiftOut(Data, Clock, MSBFIRST, lowChar[0]); break;
        case 1: shiftOut(Data, Clock, MSBFIRST, lowChar[1]); break;
        case 2: shiftOut(Data, Clock, MSBFIRST, lowChar[2]); break;
        case 3: shiftOut(Data, Clock, MSBFIRST, lowChar[3]); break;
        case 4: shiftOut(Data, Clock, MSBFIRST, lowChar[4]); break;
        case 5: shiftOut(Data, Clock, MSBFIRST, lowChar[5]); break;
        case 6: shiftOut(Data, Clock, MSBFIRST, lowChar[6]); break;
        case 7: shiftOut(Data, Clock, MSBFIRST, lowChar[7]); break;
        case 8: shiftOut(Data, Clock, MSBFIRST, lowChar[8]); break;
        case 9: shiftOut(Data, Clock, MSBFIRST, lowChar[9]); break;
        case 10: shiftOut(Data, Clock, MSBFIRST, lowChar[10]); break;
        case 11: shiftOut(Data, Clock, MSBFIRST, lowChar[11]); break;
      }
    }
  }
}

//==================================================================================================================
void shiftText(char huruf) {
  switch (huruf) {
    case 'A': shiftOut(Data, Clock, MSBFIRST, textChar[0]); break;
    case 'I': shiftOut(Data, Clock, MSBFIRST, textChar[1]); break;
    case 'U': shiftOut(Data, Clock, MSBFIRST, textChar[2]); break;
    case 'u': shiftOut(Data, Clock, MSBFIRST, textChar[3]); break;
    case 'E': shiftOut(Data, Clock, MSBFIRST, textChar[4]); break;
    case 'O': shiftOut(Data, Clock, MSBFIRST, textChar[5]); break;
    case 'o': shiftOut(Data, Clock, MSBFIRST, textChar[6]); break;
    case 'J': shiftOut(Data, Clock, MSBFIRST, textChar[7]); break;
    case 'L': shiftOut(Data, Clock, MSBFIRST, textChar[8]); break;
    case 'N': shiftOut(Data, Clock, MSBFIRST, textChar[9]); break;
    case 'n': shiftOut(Data, Clock, MSBFIRST, textChar[10]); break;
    case 'H': shiftOut(Data, Clock, MSBFIRST, textChar[11]); break;
    case 'h': shiftOut(Data, Clock, MSBFIRST, textChar[12]); break;
    case 'Y': shiftOut(Data, Clock, MSBFIRST, textChar[13]); break;
    case 't': shiftOut(Data, Clock, MSBFIRST, textChar[14]); break;
    case 'Z': shiftOut(Data, Clock, MSBFIRST, textChar[15]); break;
    case 'r': shiftOut(Data, Clock, MSBFIRST, textChar[16]); break;
    case 'b': shiftOut(Data, Clock, MSBFIRST, textChar[17]); break;
    case 'd': shiftOut(Data, Clock, MSBFIRST, textChar[18]); break;
    case 'S': shiftOut(Data, Clock, MSBFIRST, textChar[19]); break;
    case 'g': shiftOut(Data, Clock, MSBFIRST, textChar[20]); break;
    case '`': shiftOut(Data, Clock, MSBFIRST, textChar[21]); break;
    case 'P': shiftOut(Data, Clock, MSBFIRST, textChar[22]); break;
    case 'M':
      shiftOut(Data, Clock, MSBFIRST, textChar[24]);
      shiftOut(Data, Clock, MSBFIRST, textChar[23]);
      break;
  }
}
//==================================================================================================================
void printText(byte nilai) {
  if (nilai == 0) {                                       // Text Jeda
    shiftText('A');
    shiftText('d');
    shiftText('E');
    shiftText('J');
  }
  else if (nilai == 1) {                                  // Text Mp3
    shiftData(3, LOW, false);
    shiftText('P');
    shiftText('M');
  }
  else if (nilai == 2) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftText('N');
    shiftText('O');
    shiftData(11, LOW, false);
  }
  else if (nilai == 3) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftText('F');
    shiftText('F');
    shiftText('O');
  }
  else if (nilai == 4) {                                  // Text Mp3
    shiftText('t');
    shiftText('t');
    shiftText('E');
    shiftText('S');
  }
  else if (nilai == 5) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftText('h');
    shiftText('U');
    shiftText('b');
    shiftText('U');
    shiftText('S');
    shiftData(11, LOW, false);
  }
  else if (nilai == 6) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftText('r');
    shiftText('U');
    shiftText('h');
    shiftText('U');
    shiftText('h');
    shiftText('d');
    shiftData(11, LOW, false);
  }
  else if (nilai == 7) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftText('r');
    shiftText('A');
    shiftText('H');
    shiftText('S');
    shiftText('A');
    shiftData(11, LOW, false);
  }
  else if (nilai == 8) {                                  // Text Mp3
    shiftText('b');
    shiftText('I');
    shiftText('r');
    shiftText('H');
    shiftText('g');
    shiftText('A');
    shiftText('M');
  }
  else if (nilai == 9) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftText('A');
    shiftText('Y');
    shiftText('S');
    shiftText('I');
    shiftData(10, LOW, false);
    shiftData(11, LOW, false);
  }
  else if (nilai == 10) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftText('t');
    shiftText('A');
    shiftText('M');
    shiftText('U');
    shiftText('J');
    shiftData(11, LOW, false);
  }
  else if (nilai == 11) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftText('N');
    shiftText('A');
    shiftText('Z');
    shiftText('d');
    shiftText('A');
    shiftData(11, LOW, false);
  }
  else if (nilai == 12) {                                  // Text Mp3
    shiftData(11, LOW, false);
    shiftText('I');
    shiftText('t');
    shiftText('A');
    shiftText('M');
    shiftData(10, LOW, false);
    shiftData(11, LOW, false);
  }
  else if (nilai == 13) {                                  // Text Zona
    shiftData(11, LOW, false);
    shiftData(10, LOW, false);
    shiftText('A');
    shiftText('N');
    shiftText('O');
    shiftText('Z');
    shiftData(10, LOW, false);
    shiftData(11, LOW, false);
  }
  else if (nilai == 14) {                                  // Text Selisi
    shiftData(11, LOW, false);
    shiftText('I');
    shiftText('S');
    shiftText('I');
    shiftText('L');
    shiftText('E');
    shiftText('S');
    shiftData(11, LOW, false);
  }
  else if (nilai == 15) {                                  // Text Lintang
    shiftText('t');
    shiftText('N');
    shiftText('I');
    shiftText('L');
  }
  else if (nilai == 16) {                                  // Text Bujur
    shiftText('r');
    shiftText('J');
    shiftText('U');
    shiftText('b');
  }
}
//==================================================================================================================
