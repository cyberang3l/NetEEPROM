NetEEPROM
=========

Arduino library that reads stores network settings to EEPROM.
Forked from https://github.com/gregington/NetEEPROM.

This library stores to EEPROM:
* the MAC address of the interface
* whether DHCP should be used to configure IP settings
* if DHCP is not used, then the manual IP settings for the interface

The library only stores the network configuration, and it can be used
in combination with ethercard or another library for network configuration.
If network configuration has not been stored in EEPROM then the following
configuration will apply:
* A random MAC address in the form DE:AD:BE:xx:xx:xx
  (where xx is a random number between 0 and 255) will be generated
  and stored in EEPROM.
* The static IP address 192.168.1.200/24 will be configured and this
  configuration will be stored in EEPROM

Usage
-----

To use the the NetEEPROM library the sketch will need to contain the
following includes:

    #include <EEPROM.h>
    #include <NetEEPROM.h>

To set a MAC address with DHCP configuration, use the following code:

    byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
    NetEeprom.writeDhcpConfig(mac);

To set a MAC address with manual IP configuration, use the following code:

    byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
    byte ip[4] = { 192, 168, 1, 100 };
    byte dns[4] = { 8, 8, 8, 8 };
    byte gw[4] = { 192, 168, 1, 1 };
    byte subnet[4] = { 255, 255, 255, 0 };
    NetEeprom.writeManualConfig(mac, ip, dns, gw, subnet);

To set up an ENC28J60 Ethernet adapter using EtherCard with configuration from EEPROM, use the
following lines:

    byte mymac[6];
    // init will read the mac address stored in EEPROM
    // and store it in mymac array.
    NetEeprom.init(mymac);
    while (ether.begin(sizeof Ethernet::buffer, mymac, CHIP_SELECT_PIN) == 0)
    {
        Serial.println( "Failed to access Ethernet controller");
        delay(5000);
    }

    // If the stored configuration is using dhcp
    // then run ether.dhcpSetup()
    if (NetEeprom.isDhcp())
    {
        Serial.println("Try DHCP");
        if (!ether.dhcpSetup())
            Serial.println( "DHCP failed");
    }
    else
    {
        // Else, configure the static IP
        byte myip[4];
        byte gwip[4];
        byte dnsip[4];
        byte netmask[4];
        Serial.println("Static IP");
        NetEeprom.readIp(myip);
        NetEeprom.readGateway(gwip);
        NetEeprom.readDns(dnsip);
        NetEeprom.readSubnet(netmask);
        ether.staticSetup(myip, gwip, dnsip, netmask);
    }

