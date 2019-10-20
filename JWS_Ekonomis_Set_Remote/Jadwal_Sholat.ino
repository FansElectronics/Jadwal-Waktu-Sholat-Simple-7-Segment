//==================================================================================================================
void slideShowJWS() {
  get_float_time_parts(times[a], jamSholat, mntSholat);
  mntSholat += selisihWaktu;
  if (mntSholat >= 60) {
    mntSholat = 0;
    jamSholat++;
  }
  if (a == 8) {
    get_float_time_parts(times[0], jamSholat, mntSholat);
    mntSholat += selisihWaktu;
    if (mntSholat < 10) {
      mntSholat = 60 - mntSholat;
      jamSholat--;
    }
    else {
      mntSholat = mntSholat - 10 ;
    }
  }
  shiftLedJWS(a);
  printTime(jamSholat, mntSholat);
}
//==================================================================================================================
void displayMati(byte jeda) {
  clearAllDisplay();
  resetHoldTime();
  while (jeda >= mntTunggu) {
    tBlink.update(); tHold.update();
    if (digitalRead(ledBlink) == LOW) {
      clearAllDisplay();
    }
  }
}

//==================================================================================================================
void realTimeScanJWS() {
  for (i = 0; i < 7; i++) {
    DateTime now = rtc.now();
    if (i == 1 or i == 4) {
      i++;
    }
    get_float_time_parts(times[i], jamSholat, mntSholat);
    mntSholat += selisihWaktu;
    if (mntSholat >= 60) {
      mntSholat = 0;
      jamSholat++;
    }
    if (now.dayOfTheWeek() == 5 and i == 2 and jamSholat == now.hour() and mntSholat == now.minute()) {
      // "5" = Hari Jumat, "2" = Sholat Dhuhur = sholat Jum'at
      alarmJumat(2, jamSholat, mntSholat);
    }
    else if (jamSholat == now.hour() && mntSholat == now.minute()) {
      alarmJWS(i, jamSholat, mntSholat);
    }
  }
}
//==================================================================================================================
void alarmJWS(byte jws, byte _jam, byte _mnt) {
  e = 1;
  resetHoldTime();
  while (jedaAdzan >= mntTunggu) {
    DateTime now = rtc.now();
    v = now.second() % 2;
    digitalWrite(Latch, LOW);
    if (jws == 0) {                                        // Deteksi Sholat Subuh
      if (v == 0) {
        shiftLedJWS(jws);
        printTime(_jam, _mnt);
        printText(5);
      }
      else if (v == 1) {
        shiftLedJWS(7);
        clearDigitDisplay(12, LOW);
      }
    }
    else if (jws == 2) {                                                           // Deteksi Sholat Dhuhur
      if (v == 0) {
        shiftLedJWS(jws);
        printTime(_jam, _mnt);
        printText(6);
      }
      else if (v == 1) {
        shiftLedJWS(7);
        clearDigitDisplay(12, LOW);
      }
    }
    else if (jws == 3) {                                                           // Deteksi Sholat Ashar
      if (v == 0) {
        shiftLedJWS(jws);
        printTime(_jam, _mnt);
        printText(7);
      }
      else if (v == 1) {
        shiftLedJWS(7);
        clearDigitDisplay(12, LOW);
      }
    }
    else if (jws == 5) {                                                           // Deteksi Sholat Maghrib
      if (v == 0) {
        shiftLedJWS(jws);
        printTime(_jam, _mnt);
        printText(8);
      }
      else if (v == 1) {
        shiftLedJWS(7);
        clearDigitDisplay(12, LOW);
      }
    }
    else if (jws == 6) {                                                           // Deteksi Sholat Isya
      if (v == 0) {
        shiftLedJWS(jws);
        printTime(_jam, _mnt);
        printText(9);
      }
      else if (v == 1) {
        shiftLedJWS(7);
        clearDigitDisplay(12, LOW);
      }
    }
    printTime(now.hour(), now.minute());
    digitalWrite(Latch, HIGH);

    // Buzzer 10x Bunyi
    if (v == 0) {
      digitalWrite(buzzer, e);
    }
    else {
      digitalWrite(buzzer, LOW);
    }
    if (tDtk >= 20 or mntTunggu > 0) {
      e = 0;
    }
    tBlink.update();    
    tHold.update();
  }
  waitIqomah(jws);
}
//==================================================================================================================
void waitIqomah(byte jeda) {
  switch (jeda) {
    case 0 : e = 0; break;       // Waktu Tunggu Iqomah Subuh
    case 2 : e = 1; break;       // Waktu Tunggu Iqomah Dhuhur
    case 3 : e = 2; break;       // Waktu Tunggu Iqomah Ashar
    case 5 : e = 3; break;       // Waktu Tunggu Iqomah Maghrib
    case 6 : e = 4; break;       // Waktu Tunggu Iqomah Isya'
  }
  mnt = jedaIqomah[e]; dtk = 0;
  digitalWrite(ledBlink, LOW);
  while (1) {
    DateTime now = rtc.now();
    v = now.second() % 2;

    digitalWrite(Latch, LOW);
    clearDigitDisplay(13, LOW);
    printTime(mnt, dtk);
    digitalWrite(Latch, HIGH);

    if (mnt <= 0 and dtk < 10) {
      digitalWrite(buzzer, ON);
    }
    if (mnt == 0 and dtk < 0 ) {
      digitalWrite(buzzer, OFF);
      break;
    }
    delay(500);
    digitalWrite(buzzer, OFF);
    delay(500);

    if (mnt <= 0 and dtk <= 0 ) {
      digitalWrite(buzzer, OFF);
      break;
    }
    dtk--;
    if (dtk < 0) {
      mnt--;
      dtk = 59;
    }
  }
  displayMati(jedaMati);
}
//==================================================================================================================
void alarmJumat(byte jws, byte _jam, byte _mnt) {
  resetHoldTime();
  while (jedaJumat >= mntTunggu) {
    tHold.update();
    DateTime now = rtc.now();
    v = now.second() % 2;
    digitalWrite(Latch, LOW);
    if (v == 0) {
      printTime(_jam, _mnt);
      shiftLedJWS(jws);
      printText(10);
    }
    else if (v == 1) {
      clearDigitDisplay(13, LOW);
    }
    printTime(now.hour(), now.minute());
    digitalWrite(Latch, HIGH);
  }
  displayMati(jedaJumat);
}
//==================================================================================================================
