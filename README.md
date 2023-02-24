# DHT11-AVR
Read from a DHT11 sensor using a bare metal AVR
## Usage
Connect your DHT11 to your AVR:
* pin 1 to 5v
* pin 2 to a data pin
* pin 4 to ground ( If there is no pin 4, connect pin 3 to ground )
After this, include the header and match DHTDDR, DHTPORT, DHTIN and DHTPIN to yours. To read data from the DHT11, call the read function as shown in the header file.
