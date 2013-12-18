#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>
#include <nrk_stats.h>
#include <string.h>
#include "mbed.h"
#include "basic_rf.h"
#include "bmac.h"

// Only require MAC address for address decode 
//#define MAC_ADDR	0x0001

nrk_task_type RX_TASK;
NRK_STK rx_task_stack[NRK_APP_STACKSIZE];
void rx_task (void);

nrk_task_type TX_TASK;
NRK_STK tx_task_stack[NRK_APP_STACKSIZE];
void tx_task (void);

void nrk_create_taskset ();

char tx_buf[RF_MAX_PAYLOAD_SIZE];
char rx_buf[RF_MAX_PAYLOAD_SIZE];
uint16_t tx_count;
uint16_t rx_count;

int main(void)

  {
	
			nrk_setup_ports();
			nrk_init();
			bmac_task_config();
			nrk_create_taskset();
		  nrk_start();
			return 0;

	}

void rx_task ()
{
  uint8_t i, len, rssi;
  int8_t val;
	char *local_rx_buf;
  nrk_time_t check_period;
  printf ("rx_task PID=%d\r\n", nrk_get_pid ());
  // init bmac on channel 24 
  bmac_init (12);  
  bmac_set_cca_thresh(DEFAULT_BMAC_CCA); 
  
  bmac_rx_pkt_set_buffer (rx_buf, RF_MAX_PAYLOAD_SIZE);
	rx_count = 0;

  while (1) {
    // Wait until an RX packet is received
		if (tx_count >= 300) {
			printf("Total Packet received : %d \r\n", rx_count);
		}
		val = bmac_wait_until_rx_pkt ();
		
    // Get the RX packet 
    nrk_led_set (ORANGE_LED);
    local_rx_buf = bmac_rx_pkt_get (&len, &rssi);
    //if( bmac_rx_pkt_is_encrypted()==1 ) nrk_kprintf( PSTR( "Packet Encrypted\r\n" ));
    if ((len > 0) && (rssi > 0)) {
			printf ("Got RX packet len=%d RSSI=%d \r\n", len, rssi);
			nrk_led_clr (ORANGE_LED);
			rx_count++;
		}
		// Release the RX buffer so future packets can arrive 
    bmac_rx_pkt_release ();	
		// this is necessary
    nrk_wait_until_next_period ();
  }
}


uint8_t ctr_cnt[4];

void tx_task ()
{
  uint8_t j, i, val, len;
  int8_t v;
  nrk_sig_t tx_done_signal;
  nrk_sig_mask_t ret;
  nrk_time_t r_period;
	
	
  while (!bmac_started ())
    nrk_wait_until_next_period ();

  tx_done_signal = bmac_get_tx_done_signal ();
  nrk_signal_register (tx_done_signal);

  ctr_cnt[0]=0; ctr_cnt[1]=0; ctr_cnt[2]=0; ctr_cnt[3]=0;
  tx_count = 0;
  while (1) {
		if (tx_count >= 300) {
		printf("Total packets sent : %d \r\n", tx_count);
		}
    // Build a TX packet
		if (tx_count < 300) {
			sprintf(tx_buf, "Hello World");
			nrk_led_set (BLUE_LED);
		   
			bmac_auto_ack_disable();

			val=bmac_tx_pkt(tx_buf, strlen(tx_buf));
			if(val==NRK_OK) tx_count++;		 
			else printf("NO ack or Reserve Violated! \r\n");		
		 
			printf("Tx task sent data!\r\n");
			nrk_led_clr (BLUE_LED);
			printf("tx_task PID=%d\r\n", nrk_get_pid ());
		}
		nrk_wait_until_next_period ();
  }
}

void nrk_create_taskset ()
{


  RX_TASK.task = rx_task;
  nrk_task_set_stk( &RX_TASK, rx_task_stack, NRK_APP_STACKSIZE);
  RX_TASK.prio = 2;
  RX_TASK.FirstActivation = TRUE;
  RX_TASK.Type = BASIC_TASK;
  RX_TASK.SchType = PREEMPTIVE;
  RX_TASK.period.secs = 1;
  RX_TASK.period.nano_secs = 0;
  RX_TASK.cpu_reserve.secs = 1;
  RX_TASK.cpu_reserve.nano_secs = 500 * NANOS_PER_MS;
  RX_TASK.offset.secs = 0;
  RX_TASK.offset.nano_secs = 0;
  nrk_activate_task (&RX_TASK);

  TX_TASK.task = tx_task;
  nrk_task_set_stk( &TX_TASK, tx_task_stack, NRK_APP_STACKSIZE);
  TX_TASK.prio = 2;
  TX_TASK.FirstActivation = TRUE;
  TX_TASK.Type = BASIC_TASK;
  TX_TASK.SchType = PREEMPTIVE;
  TX_TASK.period.secs = 1;
  TX_TASK.period.nano_secs = 0;
  TX_TASK.cpu_reserve.secs = 1;
  TX_TASK.cpu_reserve.nano_secs = 500 * NANOS_PER_MS;
  TX_TASK.offset.secs = 0;
  TX_TASK.offset.nano_secs = 0;
  nrk_activate_task (&TX_TASK); 

  printf ("Create done\r\n");
}