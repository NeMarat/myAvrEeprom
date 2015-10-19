#include "myarveeprom.h"

MyEeprom::MyEeprom (uint32_t first_address, uint32_t max_address, uint16_t data_length, bool reload_pages) {
  /*if (first_address + data_length + 1 > max_address) {
      return null;
  }*/
  firstAddress = first_address;
  maxAddress = max_address;
  dataLength = data_length;
  rotateEeprom = true;

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
}

MyEeprom::MyEeprom(uint32_t first_address) {
  firstAddress = first_address;
  rotateEeprom = false;
}

uint8_t MyEeprom::readByte(uint16_t relative_address) {
  if (rotateEeprom == true) {
    read_byte(relative_address);
  } else { simple_read_byte(relative_address); }
}

uint8_t MyEeprom::read_byte(uint16_t relative_address) {
    while (!eeprom_is_ready());
    return(eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address)));
}

uint8_t MyEeprom::simple_read_byte(uint16_t relative_address) {
  while (!eeprom_is_ready());
  return(eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address)));
}

void MyEeprom::writeByte(uint8_t b, uint16_t relative_address) {
  if (rotateEeprom == true) {
    write_byte(b, relative_address);
  } else { simple_write_byte(b, relative_address); }
}

void MyEeprom::write_byte(uint8_t b, uint16_t relative_address) {
    cli();
    while (!eeprom_is_ready());
    uint16_t pCurrentOffset = currentOffset;
    if (firstAddress + (currentOffset + 1) * (dataLength + servisInfo) > maxAddress) {
        currentOffset = 0;
    } else { currentOffset++; }
    uint8_t tb = 0;
    for (uint16_t i = 0; i < dataLength; i++) {
        if (i == relative_address) {
            eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + i), b);
        } else {
          tb = eeprom_read_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo) + servisInfo + i));
          eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + i), tb);
        }
    }
    eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo)), P_VALID);
    eeprom_write_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo)), P_INVALID);
    sei();
}

void MyEeprom::simple_write_byte(uint8_t b, uint16_t relative_address) {
    cli();
    while (!eeprom_is_ready());
    if (eeprom_read_byte((uint8_t*)firstAddress) == P_INVALID) {
      eeprom_write_byte((uint8_t*)firstAddress, P_VALID);
    }
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address, b);
    sei();
}

uint16_t MyEeprom::getCurrentOffset() {
  return(currentOffset);
}

uint32_t MyEeprom::getCurrentFirstAddress() {
  return(firstAddress + currentOffset * (dataLength + servisInfo));
}

void MyEeprom::writeULong (uint32_t l, uint16_t relative_address) {
  if (rotateEeprom == true) {
    write_ulong(l, relative_address);
  } else {
    simple_write_ulong(l, relative_address);
  }
}

void MyEeprom::simple_write_ulong(uint32_t l, uint16_t relative_address) {
  cli();
  while (!eeprom_is_ready());  
    if (eeprom_read_byte((uint8_t*)firstAddress) == P_INVALID) {
      eeprom_write_byte((uint8_t*)firstAddress, P_VALID);
    }
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 0, *((byte*)&l + 0));
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 1, *((byte*)&l + 1));
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 2, *((byte*)&l + 2));
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 3, *((byte*)&l + 3));
    sei();
}

void MyEeprom::write_ulong(uint32_t l, uint16_t relative_address) {
  cli();
  while (!eeprom_is_ready());
    uint16_t pCurrentOffset = currentOffset;
    if (firstAddress + (currentOffset + 1) * (dataLength + servisInfo) > maxAddress) {
        currentOffset = 0;
    } else { currentOffset++; }
    uint8_t tb = 0;
    int32_t currentNAddress = firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo;
    int32_t currentOAddress = firstAddress + pCurrentOffset * (dataLength + servisInfo) + servisInfo;   
    for (uint16_t i = 0; i < dataLength; i++) {
        if (i == relative_address) {
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 0), *((byte*)&l + 0));
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 1), *((byte*)&l + 1));
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 2), *((byte*)&l + 2));
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 3), *((byte*)&l + 3));
            i+=4;
        } else {
          tb = eeprom_read_byte((uint8_t*)(currentOAddress + i));
          eeprom_write_byte((uint8_t*)(currentNAddress + i), tb);
        }
    }
    eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo)), P_VALID);
    eeprom_write_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo)), P_INVALID);
    sei();
}

uint32_t MyEeprom::readULong (uint16_t relative_address) {
  if (rotateEeprom == true) {
    return read_ulong (relative_address);
  } else {
    return simple_read_ulong (relative_address);
  }
}

uint32_t MyEeprom::simple_read_ulong (uint16_t relative_address){
  uint32_t l = 0;
  while (!eeprom_is_ready());
  *((byte*)&l + 0) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 0));
  *((byte*)&l + 1) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 1));
  *((byte*)&l + 2) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 2));
  *((byte*)&l + 3) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 3));
  return l;
}

uint32_t MyEeprom::read_ulong (uint16_t relative_address){
  uint32_t l = 0; 
  while (!eeprom_is_ready());
  *((byte*)&l + 0) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 0));
  *((byte*)&l + 1) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 1));
  *((byte*)&l + 2) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 2));
  *((byte*)&l + 3) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 3));
  return l;
}

void MyEeprom::writeUInt (uint16_t l, uint16_t relative_address) {
  if (rotateEeprom == true) {
    write_uint(l, relative_address);
  } else {
    simple_write_uint(l, relative_address);
  }
}

uint16_t MyEeprom::readUInt (uint16_t relative_address) {
  if (rotateEeprom == true) {
    return read_uint (relative_address);
  } else {
    return simple_read_uint (relative_address);
  }
}

uint16_t MyEeprom::simple_read_uint (uint16_t relative_address){
  uint16_t l = 0;
  while (!eeprom_is_ready());
  *((byte*)&l + 0) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 0));
  *((byte*)&l + 1) = eeprom_read_byte((uint8_t*)(firstAddress + servisInfo + relative_address + 1));
  return l;
}

uint16_t MyEeprom::read_ulong (uint16_t relative_address){
  uint16_t l = 0; 
  while (!eeprom_is_ready());
  *((byte*)&l + 0) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 0));
  *((byte*)&l + 1) = eeprom_read_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo + relative_address + 1));
  return l;
}

void MyEeprom::simple_write_uint(uint16_t l, uint16_t relative_address) {
  cli();
  while (!eeprom_is_ready());  
    if (eeprom_read_byte((uint8_t*)firstAddress) == P_INVALID) {
      eeprom_write_byte((uint8_t*)firstAddress, P_VALID);
    }
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 0, *((byte*)&l + 0));
    eeprom_write_byte((uint8_t*)firstAddress + servisInfo + relative_address + 1, *((byte*)&l + 1));
    sei();
}

void MyEeprom::write_uint(uint16_t l, uint16_t relative_address) {
  cli();
  while (!eeprom_is_ready());
    uint16_t pCurrentOffset = currentOffset;
    if (firstAddress + (currentOffset + 1) * (dataLength + servisInfo) > maxAddress) {
        currentOffset = 0;
    } else { currentOffset++; }
    uint8_t tb = 0;
    int32_t currentNAddress = firstAddress + currentOffset * (dataLength + servisInfo) + servisInfo;
    int32_t currentOAddress = firstAddress + pCurrentOffset * (dataLength + servisInfo) + servisInfo;   
    for (uint16_t i = 0; i < dataLength; i++) {
        if (i == relative_address) {
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 0), *((byte*)&l + 0));
            eeprom_write_byte((uint8_t*)(currentNAddress + i + 1), *((byte*)&l + 1));
            i+=2;
        } else {
          tb = eeprom_read_byte((uint8_t*)(currentOAddress + i));
          eeprom_write_byte((uint8_t*)(currentNAddress + i), tb);
        }
    }
    eeprom_write_byte((uint8_t*)(firstAddress + currentOffset * (dataLength + servisInfo)), P_VALID);
    eeprom_write_byte((uint8_t*)(firstAddress + pCurrentOffset * (dataLength + servisInfo)), P_INVALID);
    sei();
}