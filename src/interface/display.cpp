#include "display.h"
#include "config/pinConfig.h"

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#include <ST7032_asukiaaa.h>
ST7032_asukiaaa lcd;

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

/**
 * 初期化
 */
void Display::init()
{
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.setContrast(40);
}

/**
 * 全文字書換で文字を表示
 * @param line1 1行目の文字列
 * @param line2 2行目の文字列
 */
void Display::print(const char line1[], const char line2[])
// TODO: 2行固定で実装しているので他のディスプレイを使用する場合修正が必要
{
  // 1行目を表示
  lcd.setCursor(0, 0);
  char buffer1[LCD_WIDTH + 1];
  snprintf(buffer1, sizeof(buffer1), "%-*s", LCD_WIDTH, line1); // 左寄せでLCD_WIDTH文字に拡張
  lcd.print(buffer1);

  // 2行目を表示
  lcd.setCursor(0, 1);
  char buffer2[LCD_WIDTH + 1];
  snprintf(buffer2, sizeof(buffer2), "%-*s", LCD_WIDTH, line2); // 左寄せでLCD_WIDTH文字に拡張
  lcd.print(buffer2);
}

/**
 * カーソルを指定して文字を表示
 * @param x X座標
 * @param y Y座標
 * @param text 表示する文字列
 */
void Display::printWithPosition(int x, int y, const char text[])
{
  lcd.setCursor(x, y);
  lcd.print(text);
}