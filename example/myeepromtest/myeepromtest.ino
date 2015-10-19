#include "myarveeprom.h"

MyEeprom mem(300, 800, 8, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uint32_t t;
  for (int i = 310; i < 810; i++) {
    mem.writeULong(uint32_t(i)*i, 0);
    mem.writeULong(uint32_t(i)*100, 4);
    Serial.print("i      = "); Serial.println(i);
    Serial.print("Offset = "); Serial.println(mem.getCurrentOffset());
    Serial.print("i * i  = "); Serial.println(mem.readULong(0));
    Serial.print("i * 100= "); Serial.println(mem.readULong(4));
    Serial.print("Address= "); Serial.println(mem.getCurrentFirstAddress());
    Serial.println(" ");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
