#ifndef __MYARVEEPROM__
#define __MYARVEEPROM__

#include <avr/eeprom.h>
#include <avr/interrupt.h>

enum {P_VALID = 17, P_INVALID};

class MyEeprom {
    public:
      MyEeprom (uint32_t first_address, uint32_t max_address, uint16_t data_length, bool reload_pages = false);
      void write_byte(uint8_t b, uint16_t relative_address);
      uint8_t read_byte(uint16_t relative_address);
      uint16_t getCurrentOffset();

    private:
        uint16_t dataLength;
        uint32_t firstAddress;
        uint32_t maxAddress;
        uint16_t currentOffset;
};

#endif // __MYARVEEPROM__

