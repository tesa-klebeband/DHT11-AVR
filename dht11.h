#ifndef DHT11_H
#define DHT11_H

#define DHTDDR DDRC
#define DHTPORT PORTC
#define DHTIN PINC
#define DHTPIN 0

#define HUMIDITY 1
#define TEMP 0

#include <util/delay.h>

// Usage: uint8_t temperature = DHT11_readData(TEMP);
//        uint8_t humidity = DHT11_readData(HUMIDITY);

uint8_t DHT11_readData(bool datatype)
{
    const uint8_t RAW_PIN = 1 << DHTPIN;
    DHTDDR |= RAW_PIN;

    // Read Start
    DHTPORT &= ~RAW_PIN;                        // Pull low for 18ms
    _delay_ms(18);
    DHTPORT |= RAW_PIN;                         // Pull high to wait for data
    _delay_us(40);                              // Max time to respond
    DHTDDR &= ~RAW_PIN;                         // Set pin to input
    DHTPORT |= RAW_PIN;            
    if (DHTIN & RAW_PIN) return -1;             // Line should be low
    _delay_us(80);                              // Time before DHT pulls line high
    if (!(DHTIN & RAW_PIN)) return -1;          // Line should be high
    _delay_us(80);                              // Time before DHT starts with data transmission

    uint8_t data[5] = {0};
    for (int u = 0; u < 5; u++)                 // Read 5 bytes (4 data bytes + checksum)       
    {
        uint8_t byte_data = 0;
        for (int i = 0; i < 8; i++) {           // Read every bit and store in one byte
            uint8_t timer = 0;
            while(!(DHTIN & RAW_PIN)) {         // Wait for line to be pulled high
                _delay_us(1);
                if (timer > 100) {              // Timeout if not pulled low in 100us
                    return -1;
                }
                timer++;
            }
            _delay_us(30);                      // Time of 0-bit response +1
            if (DHTIN & RAW_PIN) {              // If line is still high, then the bit is a 1
                byte_data |= (1 << 7 - i);
            }
            timer = 0;
            while(DHTIN & RAW_PIN) {            // Wait for line to be pulled low 
                _delay_us(1);
                if (timer > 100) {              // Timeout
                    return -1;
                }
                timer++;
            }
        }
        data[u] = byte_data;                    // Store each recieved byte in the array
    }

    DHTDDR |= RAW_PIN;                          // Set pin to output
    DHTPORT |= RAW_PIN;                         // Pull high (default state)

    // Calculate checksum from last 8 bits of all data added together and check
    if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) == data[4]) {
        if (datatype) {
            return data[0];                     // Return humidity
        }
        return data[2];                         // Return temperature
    }

    return -1;                                  // Checksum doesn't match
}

#endif
