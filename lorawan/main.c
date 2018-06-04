#include <string.h>

#include "thread.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

//#include "hts221.h"
//#include "hts221_params.h"
#include "sx127x.h"          /* SX1272/6 device driver API */
#include "sx127x_params.h"   /* SX1272/6 device driver initialization parameters */

//#include "cayenne_lpp.h"

#include "board.h"

//#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
//static kernel_pid_t sender_pid;
//static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

/* Declare globally the loramac descriptor */
//static semtech_loramac_t loramac;

/* Declare globally the sensor device descriptor */
//static hts221_t hts221;

/* Cayenne LPP buffer */
//static cayenne_lpp_t lpp;

/* Device and application informations required for OTAA activation */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0x7A, 0xDA, 0x81, 0x32, 0x6C, 0x08, 0x96 };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0xB6, 0xE7 };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xA0, 0x95, 0xE1, 0x38, 0xF8, 0xF9, 0x53, 0xB5, 0xAE, 0xCC, 0x63, 0xA0, 0x18, 0xE5, 0x94, 0x8D };
sx127x_t sx127x;


int main(void)
{
    /*if (hts221_init(&hts221, &hts221_params[0]) != HTS221_OK) {
        puts("Sensor initialization failed");
        return 1;
    }
    if (hts221_power_on(&hts221) != HTS221_OK) {
        puts("Sensor initialization power on failed");
        return 1;
    }
    if (hts221_set_rate(&hts221, hts221.p.rate) != HTS221_OK) {
        puts("Sensor continuous mode setup failed");
        return 1;
    }*/

    /* initialize the loramac stack */
    sx127x_setup(&sx127x, &sx127x_params[0]);
    semtech_loramac_init(&sx127x);


   // semtech_loramac_init(&loramac);

    /* use a fast datarate so we don't use the physical layer too much */
    semtech_loramac_set_dr(5);

    /* set the LoRaWAN keys */
    semtech_loramac_set_deveui(deveui);
    semtech_loramac_set_appeui(appeui);
    semtech_loramac_set_appkey(appkey);

    /* start the OTAA join procedure */
    puts("Starting join procedure");
    if (semtech_loramac_join( LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }

    puts("---Join procedure succeeded---");
     while(1)
    {
        printf("==Sending LPP data==\n");
        char *message = "Hello World!";
        semtech_loramac_rx_data_t rx_data;
        switch(semtech_loramac_send(LORAMAC_TX_CNF, 10,(uint8_t *)message, strlen(message), &rx_data))
        {
                case SEMTECH_LORAMAC_RX_DATA:
                printf("Data received: %s, port: %d\n",
                       (char *)rx_data.payload, rx_data.port);
                break;

            case SEMTECH_LORAMAC_TX_DONE:
                printf("TX done");
                break; 
            case SEMTECH_LORAMAC_NOT_JOINED:
                printf("Failed: not joined");
                break;

        }

        /* send the message every 20 seconds 
        semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);
         Wait until the send cycle has completed */
        //semtech_loramac_recv();
        //cayenne_lpp_reset(&lpp);
        printf("Data sent ===00\n");
        xtimer_sleep(60);
        printf("==After sleep 60==\n");
    }
    printf("==Out of loop==\n");
    /* this should never be reached */
    return 0;


}
