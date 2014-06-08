#include <EtherCard.h>
#include <EEPROM.h>
#include <NetEEPROM.h>

/* Choose CS - Chip Select (SS - Slave Select) Pin
 * Different microcontrollers use a different pin
 */
#if defined(__AVR_ATmega2560__)
#define CHIP_SELECT_PIN 53
#else
#define CHIP_SELECT_PIN 10
#endif

/* Array to store ethernet interface ip address
 * Will be read by NetEEPROM
 */
static byte myip[4];
/* Array to store gateway ip address
 * Will be read by NetEEPROM
 */
static byte gwip[4];
/* Array to store DNS IP Address
 * Will be read by NetEEPROM
 */
static byte dnsip[4];
/* Array to store netmask
 * Will be read by NetEEPROM
 */
static byte netmask[4];


/* Array to store ethernet interface mac address
 * Will be read by NetEEPROM
*/
static byte mymac[6];

/* TCP/IP send and receive buffer */
byte Ethernet::buffer[700];

/* Example of how to configure the network interface with EtherCard from configuration stored in EEPROM. */
void setup()
{
    /* Instantiate network from stored network paramters in EEPROM.
     * If not in EEPROM then a random MAC will be generated and stored in EEPROM
     * in the form DE:AD:BE:xx:xx:xx where xx is a random number from 0 to 255.
     * If the network is not configured in EEPROM, then The static IP address
     * will be used 192.168.1.200/24 and stored in EEPROM.
     */
    NetEeprom.init(mymac);
    ether.begin(sizeof Ethernet::buffer, mymac, CHIP_SELECT_PIN);

    /* If DHCP is in use, configure ethercard for DHCP. */
    if (NetEeprom.isDhcp())
    {
        ether.dhcpSetup();
    }
    /* Otherwise, read the static IP configuration from EEPROM and configure
     * the network with these settings.
     */
    else
    {
        NetEeprom.readIp(myip);
        NetEeprom.readGateway(gwip);
        NetEeprom.readDns(dnsip);
        NetEeprom.readSubnet(netmask);
        ether.staticSetup(myip, gwip, dnsip, netmask);
    }

}

void loop()
{
}

