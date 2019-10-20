//==================================================================================================================
char keyChar(long data) {
  switch (data) {
    case 3636629159   : return '0'; break;       // Angka 0
    case 133245066    : return '1'; break;       // Angka 1
    case 3142997633   : return '2'; break;       // Angka 2
    case 3436156858   : return '3'; break;       // Angka 3
    case 3555943334   : return '4'; break;       // Angka 4
    case 255518173    : return '5'; break;       // Angka 5
    case 1625950912   : return '6'; break;       // Angka 6
    case 1830144222   : return '7'; break;       // Angka 7
    case 1446098477   : return '8'; break;       // Angka 8
    case 838088718    : return '9'; break;       // Angka 9
    case 2073673018   : return '.'; break;       // Titik .
    case 436340559    : return '+'; break;       // Minus +
    case 2246546505   : return '-'; break;       // Minus -
    case 599379270    : return 'P'; break;       // Tombol Power
    case 1496431332   : return 'O'; break;       // OK
    case 1081617514   : return 'U'; break;       // Atas
    case 1363854958   : return 'D'; break;       // Bawah
    case 2790879903   : return 'L'; break;       // Kiri
    case 2508642459   : return 'R'; break;       // Kanan
    case 3034408699   : return 'C'; break;       // Recall
    case 1332690195   : return 'E'; break;       // Exit
    case 2539807222   : return '>'; break;       // CH +
    case 3465568816   : return '<'; break;       // CH -


    default : break;
  }
  digitalWrite(buzzer, ON);
  delay(100);
  digitalWrite(buzzer, OFF);
}

//==================================================================================================================
void readAllEEPROM() {
  jedaIqomah[0] = eeprom_read_byte((uint8_t*)0);    // EEPROM Jeda Subuh
  jedaIqomah[1] = eeprom_read_byte((uint8_t*)1);    // EEPROM Jeda Dhuhur
  jedaIqomah[2] = eeprom_read_byte((uint8_t*)2);    // EEPROM Jeda Ashar
  jedaIqomah[3] = eeprom_read_byte((uint8_t*)3);    // EEPROM Jeda Maghrib
  jedaIqomah[4] = eeprom_read_byte((uint8_t*)4);    // EEPROM Jeda Isya
  jedaJumat     = eeprom_read_byte((uint8_t*)5);    // EEPROM Jeda Jumat
  jedaMati      = eeprom_read_byte((uint8_t*)6);    // EEPROM Jeda Mati
  jedaAdzan      = eeprom_read_byte((uint8_t*)9);    // EEPROM Jeda Mati
  selisihWaktu  = eeprom_read_byte((uint8_t*)7);    // EEPROM selesi Waktu / kalibrasi
  zonaWaktu     = eeprom_read_byte((uint8_t*)8);    // EEPROM Zona Waktu
  lintang       = eeprom_read_float((float*)10);    // EEPROM Lintang
  bujur         = eeprom_read_float((float*)15);    // EEPROM Bujur
  adzan         = eeprom_read_byte((uint8_t*)30);   // EEPROM Automatis adzan
}
//==================================================================================================================
void inputSetting(int maxDigit) {
  if (remote.decode(&valRemote)) {
    Key = keyChar(valRemote.value);
    remote.resume();
  }
  if (n < maxDigit) {
    if (Key) {
      if ((Key >= 48 and Key <= 57)) {
        charBuffer[n] = Key;
        n++;
      }
    }
  }
}
//==================================================================================================================
void readVarRTC() {
  DateTime now = rtc.now();
  v = now.second() % 2;
  jam = now.hour();
  mnt = now.minute();
  tgl = now.day();
  bln = now.month();
  thn = now.year();
}
//==================================================================================================================
void clearBuffer() {
  for (i = 0; i < 10; i++) {
    charBuffer[i] = 0; floatBuff[i] = 0;
  }
  n = 0; a = 0;
}
//==================================================================================================================
void menuSetting() {
  DateTime now = rtc.now();
  readVarRTC();
  //************************************************************************************* Setting Time
setTIME:
  inputSetting(4);
  tBlink.update();
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setBujur;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setDATE;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    now = rtc.now();
    val = String(charBuffer).toInt();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), jam, mnt, now.second()));
    if (val < 10) {
      jam = 0; mnt = val;
    }
    else if (val < 100) {
      jam = 0; mnt = val;
    }
    else {
      jam = val / 100; mnt = val % 100;
    }
  }
  else {
    readVarRTC();
    jam = now.hour(); mnt = now.minute();
  }
  digitalWrite(Latch, LOW);
  clearDigitDisplay(14, LOW);
  printTime(jam, mnt);
  digitalWrite(Latch, HIGH);
  goto setTIME;
  //************************************************************************************* Setting Date
setDATE:
  inputSetting(8);
  tBlink.update();
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setTIME;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaSubuh;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (!access) {
    readVarRTC();
    tgl = now.day(); bln = now.month(); thn = now.year();
  }
  else {
    val = String(charBuffer).toInt();
    if (val < 10 or val < 100 or val < 1000 or val < 1000) {
      tgl = 0; bln = 0; thn = val;
    }
    else if (val < 100000 or val < 1000000) {
      tgl = 0; bln = val / 10000; thn = val % 10000;
    }
    else {
      tgl = val / 1000000; bln = (val % 1000000) / 10000 ; thn = val % 10000;
    }
    now = rtc.now();
    rtc.adjust(DateTime(thn, bln, tgl, now.hour(), now.minute(), now.second()));
  }
  digitalWrite(Latch, LOW);
  clearDigitDisplay(4, LOW);
  printDate(tgl, bln, thn);
  clearDigitDisplay(4, LOW);
  digitalWrite(Latch, HIGH);
  goto setDATE;
  //************************************************************************************* Setting Jeda Subuh

setJedaSubuh:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setDATE;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaDhuhur;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)0, val);
  }
  else {
    val = jedaIqomah[0];
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(5);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaSubuh;
  //********************************************************************************** Setting Jeda Dhuhur

setJedaDhuhur:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaSubuh;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaAshar;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)1, val);
  }
  else {
    val = jedaIqomah[1];
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(6);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaDhuhur;
  //********************************************************************************** Setting Jeda Ashar

setJedaAshar:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaDhuhur;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaMaghrib;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)2, val);
  }
  else {
    val = jedaIqomah[2];
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(7);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaAshar;
  //********************************************************************************** Setting Jeda Maghrib

setJedaMaghrib:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaAshar;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaIsya;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)3, val);
  }
  else {
    val = jedaIqomah[3];
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(8);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaMaghrib;
  //********************************************************************************** Setting Jeda Isya

setJedaIsya:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaMaghrib;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaJumat;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)4, val);
  }
  else {
    val = jedaIqomah[4];
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(9);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaIsya;

  //********************************************************************************** Setting Jeda Jumat

setJedaJumat:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaIsya;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaMati;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)5, val);
  }
  else {
    val = jedaJumat;
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(10);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaJumat;
  //**********************************************************************************  Setting Jeda Mati

setJedaMati:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaJumat;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setJedaAdzan;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)6, val);
  }
  else {
    val = jedaMati;
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(12);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaMati;
  //**********************************************************************************  Setting Jeda Adzan

setJedaAdzan:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaMati;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setSelisihWaktu;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)9, val);
  }
  else {
    val = jedaAdzan;
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(11);
  printText(0);
  digitalWrite(Latch, HIGH);
  goto setJedaAdzan;

  //**********************************************************************************  Setting Selisih Waktu

setSelisihWaktu:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setJedaAdzan;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setZonaWaktu;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)7, val);
  }
  else {
    val = selisihWaktu;
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(14);
  printText(4);
  digitalWrite(Latch, HIGH);
  goto setSelisihWaktu;
  //**********************************************************************************  Setting Zona waktu

setZonaWaktu:
  readAllEEPROM();
  inputSetting(2);
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; Key = 0; goto setSelisihWaktu;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; Key = 0; goto setLintang;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; Key = 0; goto endSetting;
  }
  Key = 0;                                             // Hapus Data Key

  if (access) {
    val = String(charBuffer).toInt();
    eeprom_write_byte((uint8_t*)8, val);
  }
  else {
    val = zonaWaktu;
  }
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printRibuan(val);
  printText(13);
  printText(4);
  digitalWrite(Latch, HIGH);
  goto setZonaWaktu;
  //**********************************************************************************  Setting Lintang

setLintang:
  readAllEEPROM();
  if (remote.decode(&valRemote)) {
    Key = keyChar(valRemote.value);
    remote.resume();
  }
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true; titik = false; ttk = 0; val = 0;;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; titik = false; Key = 0; goto setZonaWaktu;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; titik = false; Key = 0; goto setBujur;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; titik = false; Key = 0; goto endSetting;
  }
  else if (Key == '.') {                               // Tombol Exit
    titik = true; charBuffer[n] = Key; n++; Key = 0;
  }

  if (access) {
    if (!titik) {
      if (n < 6) {
        if (Key) {
          if (Key >= 48 and Key <= 57 or Key == '-' or Key == '.') {
            charBuffer[n] = Key;
            n++;
          }
        }
      }
      val = String(charBuffer).toInt();
    }
    else {
      if (a < 2) {
        if (Key) {
          if (Key >= 48 and Key <= 57) {
            charBuffer[n] = Key;
            floatBuff[a] = Key;
            a++; n++;
          }
        }
      }
      ttk = String(floatBuff).toInt();
      String(floatBuff) = "";
    }
    eeprom_write_float((float*)10, String(charBuffer).toFloat());
  }
  else {
    val = lintang;
    ttk = abs((lintang - val) * 100);
  }
  Key = 0;                                             // Hapus Data Key
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printText(15);
  printPuluhan(ttk);
  printGeo(val);
  printText(4);
  digitalWrite(Latch, HIGH);
  goto setLintang;
  //**********************************************************************************  Setting Bujur
setBujur:
  readAllEEPROM();
  if (remote.decode(&valRemote)) {
    Key = keyChar(valRemote.value);
    remote.resume();
  }
  if (Key == 'C') {                                    // Tombol Delete
    clearBuffer(); access = true; titik = false; ttk = 0; val = 0;;
  }
  else if (Key == 'U') {                               // Tombol Up
    access = false; titik = false; Key = 0; goto setLintang;
  }
  else if (Key == 'D') {                               // Tombol Down
    access = false; titik = false; Key = 0; goto setTIME;
  }
  else if (Key == 'E') {                               // Tombol Exit
    access = false; titik = false; Key = 0; goto endSetting;
  }
  else if (Key == '.') {                               // Tombol Exit
    titik = true; charBuffer[n] = Key; n++; Key = 0;
  }

  if (access) {
    if (!titik) {
      if (n < 6) {
        if (Key) {
          if (Key >= 48 and Key <= 57 or Key == '-' or Key == '.') {
            charBuffer[n] = Key;
            n++;
          }
        }
      }
      val = String(charBuffer).toInt();
    }
    else {
      if (a < 2) {
        if (Key) {
          if (Key >= 48 and Key <= 57) {
            charBuffer[n] = Key;
            floatBuff[a] = Key;
            a++; n++;
          }
        }
      }
      ttk = String(floatBuff).toInt();
      String(floatBuff) = "";
    }
    eeprom_write_float((float*)15, String(charBuffer).toFloat());
  }
  else {
    val = bujur;
    ttk = abs((bujur - val) * 100);
  }
  Key = 0;                                             // Hapus Data Key
  digitalWrite(Latch, LOW);
  shiftLedJWS(7);
  printText(16);
  printPuluhan(ttk);
  printGeo(val);
  printText(4);
  digitalWrite(Latch, HIGH);
  goto setBujur;
  //**********************************************************************************  End Setting
endSetting:
  setting = false;
}
//==================================================================================================================
