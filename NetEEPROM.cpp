#include "NetEEPROM.h"

#define MAC_OFFSET (NET_EEPROM_OFFSET + 1)
#define DHCP_OFFSET (MAC_OFFSET + 6)
#define IP_OFFSET (DHCP_OFFSET + 1)
#define DNS_OFFSET (IP_OFFSET + 4)
#define GW_OFFSET (DNS_OFFSET + 4)
#define SUBNET_OFFSET (GW_OFFSET + 4)

void NetEEPROM::init(uint8_t mac[]) {
    /* Check if NetEEPROM has already written any configuration
     * in the EEPROM. If not, generate a random mac address for
     * this host and write the static IP configuration.
     * The default static IP is 192.168.1.200/24, but you can
     * change it below.
     */
    if (checkMagic()) {
        readMac(mac);
    } else {
        generateRandomMac(mac);
        byte ip[] = {192, 168, 1, 200};
        byte gw[] = {192, 168, 1, 1};
        byte subnet[] = {255, 255, 255, 0};
        byte dns[] = {192, 168, 1, 1};
        writeManualConfig(mac, ip, gw, subnet, dns);
    }
}

void NetEEPROM::writeDhcpConfig(byte mac[]) {
    writeMac(mac);
    EEPROM.write(DHCP_OFFSET, 1);
}

void NetEEPROM::writeManualConfig(byte mac[], byte ip[], byte gw[], byte subnet[], byte dns[]) {
    writeMac(mac);
    EEPROM.write(DHCP_OFFSET, 0);
    writeEEPROM(ip, IP_OFFSET, 4);
    writeEEPROM(dns, DNS_OFFSET, 4);
    writeEEPROM(gw, GW_OFFSET, 4);
    writeEEPROM(subnet, SUBNET_OFFSET, 4);
}

bool NetEEPROM::isDhcp() {
    return EEPROM.read(DHCP_OFFSET) != 0;
}

void NetEEPROM::readMac(byte mac[]) {
    readEEPROM(mac, MAC_OFFSET, 6);
}

void NetEEPROM::readIp(byte ip[]) {
    readEEPROM(ip, IP_OFFSET, 4);
}

void NetEEPROM::readDns(byte dns[]) {
    readEEPROM(dns, DNS_OFFSET, 4);
}

void NetEEPROM::readGateway(byte gw[]) {
    readEEPROM(gw, GW_OFFSET, 4);
}

void NetEEPROM::readSubnet(byte subnet[]) {
    readEEPROM(subnet, SUBNET_OFFSET, 4);
}

bool NetEEPROM::checkMagic() {
    return EEPROM.read(NET_EEPROM_OFFSET) == NET_EEPROM_MAGIC;
}

void NetEEPROM::generateRandomMac(byte mac[]) {
    randomSeed(analogRead(NET_RANDOM_ANALOG_PIN));
    mac[0] = 0xDE;
    mac[1] = 0xAD;
    mac[2] = 0xBE;
    mac[3] = random(255);
    mac[4] = random(255);
    mac[5] = random(255);
}

void NetEEPROM::readEEPROM(byte data[], int offset, int length) {
    for (int i = 0; i < length; i++) {
        data[i] = EEPROM.read(offset + i);
    }
}

void NetEEPROM::writeEEPROM(byte data[], int offset, int length) {
    for (int i = 0; i < length; i++) {
        EEPROM.write(offset + i, data[i]);
    }
}

void NetEEPROM::writeMac(byte mac[]) {
    EEPROM.write(NET_EEPROM_OFFSET, NET_EEPROM_MAGIC);
    writeEEPROM(mac, MAC_OFFSET, 6);
}

NetEEPROM NetEeprom;
