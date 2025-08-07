#include "display.h"
#include "config/pinConfig.h"


#define LCD_WIDTH 16 

#include <ST7032.h>
ST7032 lcd;

// void Display::initLcd()
// {
//   delay(100);
//   writeCommand(0x38);
//   delay(20);
//   writeCommand(0x39);
//   delay(20);
//   writeCommand(0x14);
//   delay(20);
//   writeCommand(0x73);
//   delay(20);
//   writeCommand(0x52);
//   delay(20);
//   writeCommand(0x6C);
//   delay(20);
//   writeCommand(0x38);
//   delay(20);
//   writeCommand(0x01);
//   delay(20);
//   writeCommand(0x0C);
//   delay(20);
// }

// void Display::writeCommand(byte t_command)
// {
//   Wire.beginTransmission(LCD_ADDRESS);
//   Wire.write(0x00);
//   Wire.write(t_command);
//   Wire.endTransmission();
//   delay(10);
// }

// void Display::writeData(byte t_data)
// {
//   Wire.beginTransmission(LCD_ADDRESS);
//   Wire.write(0x40);
//   Wire.write(t_data);
//   Wire.endTransmission();
//   delay(1);
// }

// void Display::print(const char line1[], const char line2[])
// {
//   char buffer1[LCD_WIDTH + 1] = {0}; // +1 は null 終端
//   char buffer2[LCD_WIDTH + 1] = {0};

//   snprintf(buffer1, sizeof(buffer1), "%-*s", LCD_WIDTH, line1); // 左寄せ・スペース埋め
//   snprintf(buffer2, sizeof(buffer2), "%-*s", LCD_WIDTH, line2);

//   writeCommand(0x00 + 0x80);
//   for (int i = 0; i < strlen(buffer1); i++)
//   {
//     writeData(buffer1[i]);
//   }
//   writeCommand(0x40 + 0x80);
//   for (int i = 0; i < strlen(buffer2); i++)
//   {
//     writeData(buffer2[i]);
//   }
// }

void Display::initLcd()
{
  lcd.begin(16, 2);
  lcd.setContrast(40);
}

void Display::writeCommand(byte t_command)
{
  // Wire.beginTransmission(LCD_ADDRESS);
  // Wire.write(0x00); // Co = 0, RS = 0
  // Wire.write(t_command);
  // Wire.endTransmission();
  // delay(1);
}

void Display::writeData(byte t_data)
{
  // Wire.beginTransmission(LCD_ADDRESS);
  // Wire.write(0x40); // Co = 0, RS = 1
  // Wire.write(t_data);
  // Wire.endTransmission();
  // delay(1);
}

void Display::print(const char line1[], const char line2[])
{
  lcd.setCursor(0, 0); // 1行目先頭にカーソルを移動
  lcd.print(line1);
  lcd.setCursor(0, 1); // 2行目先頭にカーソルを移動
  lcd.print(line2);
}