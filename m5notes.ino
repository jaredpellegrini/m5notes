/*
 * @file    m5notes.ino
 * @author  Jared Pellegrini <jpellegrini@gmail.com>
 * @copyright Copyright 2024 Jared Pellegrini
 * @license Apache License, Version 2.0 - https://www.apache.org/licenses/LICENSE-2.0
 * @brief   Demonstration of playing notes and songs on the M5 Cardputer.
 * @version 0.1
 * @date    2024-03-26 (Created 2024-03-23)
 *
 * Targets the M5 Cardputer.
 *
 * Note Durations (at 120 bpm):
 *  
 * 1/2 note = 1000 ms
 * dotted 1/4 note = 750 ms
 * 1/4 note = 500 ms
 * dotted 1/8 note = 375 ms
 * 1/8 note = 250 ms
 * dotted 1/16 note = 187.5 ms
 * 1/16 note = 125 ms
 * staccato = half the length of the note
 * to raise a note by N octaves, multiply the frequency by 2^N
 * 
 * CHANGELOG
 * v0.1: First version.
 */

#include "M5Cardputer.h"
#include <map>

const uint8_t MAJOR_VERSION = 0;
const uint8_t MINOR_VERSION = 1;

// <octave><note>, <freq Hz>
std::map<String, uint16_t> note_freq = {
  {"3C", 130.8},
  {"3C#", 138.6},
  {"3Db", 138.6},
  {"3D", 146.8},
  {"3D#", 155.6},
  {"3Eb", 155.6},
  {"3E", 164.8},
  {"3F", 174.6},
  {"3F#", 185.0},
  {"3Gb", 185.0},
  {"3G", 196.0},
  {"3G#", 207.7},
  {"3Ab", 207.7},
  {"3A", 220.00},
  {"3A#", 233.1},
  {"3Bb", 233.1},
  {"3B", 246.9},
  {"4C", 261.63},
  {"4C#", 277.18},
  {"4Db", 277.18},
  {"4D", 293.66},
  {"4D#", 311.13},
  {"4Eb", 311.13},
  {"4E", 329.63},
  {"4F", 349.23},
  {"4F#", 369.99},
  {"4Gb", 369.99},
  {"4G", 392.00},
  {"4G#", 415.30},
  {"4Ab", 415.30},
  {"4A", 440.00},
  {"4A#", 466.16},
  {"4Bb", 466.16},
  {"4B", 493.88},
  {"5C", 523.25},
  {"5C#", 554.37},
  {"5Db", 554.37},
  {"5D", 587.33},
  {"5D#", 622.25 },
  {"5Eb", 622.25},
  {"5E", 659.25},
  {"5F", 698.46},
  {"5F#", 739.99},
  {"5Gb", 739.99},
  {"5G", 783.99},
  {"5G#", 830.61},
  {"5Ab", 830.61},
  {"5A", 880.00},
  {"5A#", 932.33},
  {"5Bb", 932.33},
  {"5B", 987.77},
  {"6C", 1046.50},
  {"6C#", 1108.73},
  {"6Db", 1108.73},
  {"6D", 1174.66},
  {"6D#", 1244.51 },
  {"6Eb", 1244.51},
  {"6E", 1318.51},
  {"6F", 1396.91},
  {"6F#", 1479.98},
  {"6Gb", 1479.98},
  {"6G", 1567.98},
  {"6G#", 1661.22},
  {"6Ab", 1661.22},
  {"6A", 1760.00},
  {"6A#", 1864.66},
  {"6Bb", 1864.66},
  {"6B", 1975.53}
};

uint8_t fontHeight;
uint8_t displayHeight;
uint8_t displayWidth;

bool playing = 0;

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextColor(GREEN);
  M5Cardputer.Display.setTextDatum(middle_center);
  M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
  M5Cardputer.Display.setTextSize(1);
  fontHeight = M5Cardputer.Display.fontHeight(&fonts::FreeSans9pt7b); // 9
  displayWidth = M5Cardputer.Display.width();
  displayHeight = M5Cardputer.Display.height();

  M5Cardputer.Display.setTextDatum(textdatum_t::top_center);
  M5Cardputer.Display.setTextColor(WHITE);
  M5Cardputer.Display.drawString("Notes & Music Example", displayWidth / 2, 0);
  M5Cardputer.Display.setTextColor(DARKGREEN);
  M5Cardputer.Display.drawString("by Jared Pellegrini", displayWidth / 2, fontHeight);

  M5Cardputer.Display.setTextDatum(textdatum_t::top_left);
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.drawString("[A]=Axel F", 0, 3 * fontHeight);
  M5Cardputer.Display.drawString("[E]=Edelweiss", 0, 4 * fontHeight);
  M5Cardputer.Display.drawString("[N]=N.G.G.Y.U.", 0, 5 * fontHeight);
  M5Cardputer.Display.drawString("[T]=Take On Me", 0, 6 * fontHeight);

  M5Cardputer.Display.setTextColor(WHITE);
  M5Cardputer.Display.setTextDatum(textdatum_t::bottom_right);
  M5Cardputer.Display.drawString("v" + String(MAJOR_VERSION) + "." + String(MINOR_VERSION) + " ", displayWidth, displayHeight);
}

void loop() {
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isChange() && !playing) {
    if (M5Cardputer.Keyboard.isKeyPressed('a')) {
      playAxelF();
    } else if (M5Cardputer.Keyboard.isKeyPressed('e')) {
      playEdelweiss();
    } else if (M5Cardputer.Keyboard.isKeyPressed('n')) {
      playNGGYU();
    } else if (M5Cardputer.Keyboard.isKeyPressed('t')) {
      playTakeOnMe();
    }
  }
}

void playTone(String note, uint32_t duration) {
  uint16_t freq = note_freq[note];
  M5Cardputer.Speaker.tone(freq, duration);
  delay(duration);
}

void playStaccatoTone(String note, uint32_t duration) {
  uint16_t freq = note_freq[note];
  M5Cardputer.Speaker.tone(freq, duration / 2);
  delay(duration);
}

void playAxelF() {
  playing = 1;

  playStaccatoTone("4F", 500);
  playTone("4Ab", 375);
  playTone("4F", 200);
  delay(50);
  playTone("4F", 125);
  playTone("4Bb", 250);
  playTone("4F", 250);
  playTone("4Eb", 250);

  playStaccatoTone("4F", 500);
  playTone("5C", 375);
  playTone("4F", 200);
  delay(50);
  playTone("4F", 125);
  playTone("5Db", 250);
  playTone("5C", 250);
  playTone("4Ab", 250);

  playStaccatoTone("4F", 250);
  playTone("5C", 250);
  playTone("5F", 250);
  playTone("4F", 125);
  playStaccatoTone("4Eb", 250);
  playTone("4Eb", 125);
  playTone("4C", 250);
  playTone("4G", 250);
  playTone("4F", 1250);
  delay(1000);

  playing = 0;
}

void playNGGYU() {
  playing = 1;

  playTone("4A", 125);
  playTone("4B", 125);
  playTone("5D", 125);
  playTone("4B", 125);

  playStaccatoTone("5F#", 375);
  playStaccatoTone("5F#", 375);
  playTone("5E", 750);

  playTone("4A", 125);
  playTone("4B", 125);
  playTone("5D", 125);
  playTone("4B", 125);

  playStaccatoTone("5E", 375);
  playStaccatoTone("5E", 375);
  playTone("5D", 375);
  playTone("5C#", 125);
  playTone("4B", 250);

  playTone("4A", 125);
  playTone("4B", 125);
  playTone("5D", 125);
  playTone("4B", 125);

  playTone("5D", 500);
  playTone("5E", 250);
  playTone("5C#", 375);
  playTone("4B", 125);
  playTone("4A", 200);
  delay(50);

  playTone("4A", 200);
  delay(50);
  playTone("4A", 200);
  delay(50);

  playTone("5E", 500);
  playTone("5D", 1000);

  playing = 0;
}

void playTakeOnMe() {
  // offically 169 bpm
  // so make 1/8 notes 175 instead of 250
  playing = 1;

  playTone("5F#", 150);
  delay(25);
  playStaccatoTone("5F#", 175);
  playStaccatoTone("5D", 175);
  playTone("4B", 175);
  delay(175);
  playTone("4B", 175);
  delay(175);
  playTone("5E", 175);

  delay(175);
  playTone("5E", 175);
  delay(175);
  playTone("5E", 175);
  playTone("5G#", 150);
  delay(25);
  playStaccatoTone("5G#", 175);
  playStaccatoTone("5A", 175);
  playStaccatoTone("5B", 175);

  playTone("5A", 150);
  delay(25);
  playStaccatoTone("5A", 175);
  playStaccatoTone("5A", 175);
  playTone("5E", 175);
  delay(175);
  playTone("5D", 175);
  delay(175);
  playTone("5F#", 175);

  delay(175);
  playTone("5F#", 175);
  delay(175);
  playTone("5F#", 175);
  playTone("5E", 150);
  delay(50);
  playStaccatoTone("5E", 175);
  playStaccatoTone("5F#", 175);
  playStaccatoTone("5E", 175);

  playing = 0;
}

void playEdelweiss() {
  // 80 bpm :: quarter note = 750ms

  uint32_t duration = 500;

  M5Cardputer.Speaker.tone(note_freq["5D"], duration*2);
  M5Cardputer.Speaker.tone(note_freq["3Bb"], duration*3);
  delay(duration); // E
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration*2);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration*2);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5F"], duration);
  delay(duration); // del

  M5Cardputer.Speaker.tone(note_freq["6C"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["3A"], duration*3);
  delay(duration); // weiss,
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["5F"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration*2);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["5F"], duration-50);
  delay(duration);

  M5Cardputer.Speaker.tone(note_freq["5Bb"], duration*2);
  M5Cardputer.Speaker.tone(note_freq["4D"], duration*3);
  delay(duration); // E
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration*2);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration*2);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5F"], duration);
  delay(duration); // del

  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["3G"], duration*3);
  delay(duration); // weiss,
  M5Cardputer.Speaker.tone(note_freq["4Eb"], duration);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration);
  delay(duration);

  M5Cardputer.Speaker.tone(note_freq["5D"], (duration*2)-50);
  M5Cardputer.Speaker.tone(note_freq["3F"], duration*3);
  delay(duration); // Ev
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration*2);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration*2);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5D"], duration-50);
  delay(duration); // 'ry

  M5Cardputer.Speaker.tone(note_freq["5D"], duration);
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["3G"], duration*3);
  delay(duration); // morn
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration);
  delay(duration); // ing
  M5Cardputer.Speaker.tone(note_freq["5F"], duration);
  delay(duration); // you

  M5Cardputer.Speaker.tone(note_freq["5G"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["4C"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["3C"], duration*3);
  delay(duration); // greet
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration-50);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4Bb"], duration-50);
  delay(duration);

  M5Cardputer.Speaker.tone(note_freq["5F"], duration*3);
  M5Cardputer.Speaker.tone(note_freq["3F"], duration*3);
  delay(duration); // me.
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4A"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4F"], duration);
  delay(duration);
  M5Cardputer.Speaker.tone(note_freq["5Eb"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4A"], duration-50);
  M5Cardputer.Speaker.tone(note_freq["4C"], duration);
  delay(duration);

}
