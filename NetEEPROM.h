/**
 * Library that stores, reads and configures Arduino ethernet settings.
 * Both the MAC address and IP settings are stored and retrieved from EEPROM.
 * The network adapter can be configured to use DHCP or manual IP settings.
 * 24 bytes of EEPROM are required to store the configuration.
 */

/* The EEPROM offset at which the network configuration starts.
 * Sketches can override this by defining NET_EEPROM_OFFSET before
 * importing NetEEPROM.h.
 */
#ifndef NET_EEPROM_OFFSET
#define NET_EEPROM_OFFSET 0
#endif

/* The pin from which an analog reading will be taken to generate a random MAC. */
#ifndef NET_RANDOM_ANALOG_PIN
#define NET_RANDOM_ANALOG_PIN 0
#endif

#ifndef NET_EEPROM_H
#define NET_EEPROM_H

#include <Arduino.h>
#include <EEPROM.h>

/* Magic number that starts the network configuration */
#define NET_EEPROM_MAGIC 0x22

class NetEEPROM {

public:
    /* Reads the settings stored in EEPROM.
     * If no settings can be found in EEPROM then a random MAC address
     * will be generated and the adapter will be set to use the static IP
     * address 192.168.1.200/24.
     * The MAC address and the default IP address will be saved to EEPROM.
     */
    void init(uint8_t mac[]);

    /* Writes given six byte MAC address to EEPROM and specifies that DHCP should
     * be used for configuring the IP address.
     * Parameters:
     *    mac: the six byte MAC address
     */
    void writeDhcpConfig(byte mac[]);

    /* Writes the given six byte MAC address and four byte IP configuration.
     * Parameters:
     *       mac: the six byte MAC address
     *        ip: the four byte IP address
     *       dns: the four byte IP address of the DNS server
     *        gw: the four byte IP address of the gateway
     *    subnet: the four byte subnet mask
     */
    void writeManualConfig(byte mac[], byte ip[], byte dns[], byte gw[], byte subnet[]);

    /* Reads the MAC address from EEPROM.
     * Parameters:
     *   mac: array to be populated with the six byte MAC address
     */
    void readMac(byte mac[]);

    /* Checks whether the magic number exists in the expected EEPROM location.
     * If the magic number exists then it is likely that network settings have
     * been stored in EEPROM.
     */
    bool checkMagic();

    /* Returns whether the the IP address will be configured using DHCP.
     */
    bool isDhcp();

    /* Reads the configured IP address from EEPROM.
     * Parameters:
     *   ip: array to be populated with the four byte IP address
     */
    void readIp(byte ip[]);

    /* Reads the DNS server address from EEPROM.
     * Parameters:
     *   dns: array to be populated with the four byte DNS server IP.
     */
    void readDns(byte dns[]);

    /* Reads the gateway address from EEPROM.
     * Parameters:
     *   gw: array to be populated with the four byte gateway IP.
     */
    void readGateway(byte gw[]);

    /* Reads the subnet mask from EEPROM.
     * Parameters:
     *   subnet: array to be populated with the four byte subnet mask.
     */
    void readSubnet(byte subnet[]);

private:
    void generateRandomMac(byte mac[]);
    void readEEPROM(byte data[], int offset, int length);
    void writeEEPROM(byte data[], int offset, int length);
    void writeMac(byte mac[]);
};

extern NetEEPROM NetEeprom;

#endif
