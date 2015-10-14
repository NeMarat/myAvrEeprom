#include "myarveeprom.h"

MyEeprom::MyEeprom (uint32_t first_address, uint32_t max_address, uint16_t data_length, bool reload_pages) {
  /*if (first_address + data_length + 1 > max_address) {
      return null;
  }*/
  firstAddress = first_address;
  maxAddress = max_address;
  dataLength = data_length;

  if (reload_pages == false) { currentOffset = 0; }
  else {
      bool found = false;
      currentOffset = 0;
      while (!eeprom_is_ready());
      while ((first_address + currentOffset * (data_length + servisInfo) < max_address) && (!found)) {
          if (eeprom_read_byte((uint8_t*)(first_address + currentOffset * (data_length + servisInfo))) == P_VALID) {
              found = true;
          } else {
              currentOffset++;
          }
      }
      if (!found) { currentOffset = 0; }
  }
};

uint8_t MyEeprom::read_byte(uint16_t relative_address) {
    while (!eeprom_is_ready());
    return(eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address)));
}

void MyEeprom::write_byte(uint8_t b, uint16_t relative_address) {
    while (!eeprom_is_ready());
    uint16_t pCurrentOffset = currentOffset;
    if (firstAddress + (currentOffset + 1) * (dataLength + servisInfo) > maxAddress) {
        currentOffset = 0;
    } else { currentOffset++; }
    uint8_t tb = 0;
    for (uint16_t i = 0; i < dataLength; i++) {
        if (i == relative_address) {
            cli();
            eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + i), b);
            sei();
        } else {
          tb = eeprom_read_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo) + servisInfo + i));
          cli();
          eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + i), tb);
          sei();
        }
    }
    cli();
    eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo)), P_VALID);
    eeprom_write_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo)), P_INVALID);
    sei();
}

uint16_t MyEeprom::getCurrentOffset() {
  return(currentOffset);
}

uint32_t MyEeprom::getCurrentFirstAddress() {
  return(firstAddress + currentOffset * (dataLength + servisInfo));
}


