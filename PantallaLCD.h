#ifndef PANTALLA_LCD_H
#define PANTALLA_LCD_H

// Librerias
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class PantallaLCD{
  private:
    LiquidCrystal_I2C lcd;
  
  public:
    PantallaLCD(): lcd(0x27, 16, 2){};

  void iniciarLCD() {
  Serial.println("Inicializando LCD...");
  lcd.init();
  delay(100);
  lcd.backlight();
  Serial.println("LCD inicializado.");
}

void mostrarMensaje(const String& mensaje, int col = 0, int fila = 0) {
  Serial.print("Mostrando mensaje: ");
  Serial.println(mensaje);
  lcd.setCursor(col, fila);
  lcd.print(mensaje);
  delay(10);
}
  void limpiar() {
    lcd.clear();
  }
};

#endif