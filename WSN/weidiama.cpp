//RADIO_COUNT_TO_LEDS_H
#ifndef RADIO_COUNT_TO_LEDS_H
#define RADIO_COUNT_TO_LEDS_H

typedef nx_struct radio_count_msg {
  nx_uint16_t counter;
  nx_uint16_t sender;
  nx_uint16_t receiver;
  string localtime;
  string some;
   
} radio_count_msg_t;

enum {
  AM_RADIO_COUNT_MSG = 6,
};

#endif


 
#include "Timer.h"
#include "RadioCountToLeds.h"
 

module RadioCountToLedsC @safe() {
  uses {
    interface Leds;
    interface Boot;
    interface AMPacket;
    interface Receive;
    interface AMSend;
    interface Timer<TMilli> as MilliTimer;
    interface SplitControl as AMControl;
    interface Packet;
    
  }
}
implementation {

  message_t packet;

  bool locked;
  uint16_t counter = 0;
  uint16_t S[100]={0};
  uint16_t R[100]={0};
  
  event void Boot.booted() {
    call AMControl.start();
  }

  event void AMControl.startDone(error_t err) {
    if (err == SUCCESS) {
      call MilliTimer.startPeriodic(250);
      dbg("RadioCountToLedsC", "AMControl start is done.\n");
    }
    else {
      call AMControl.start();
    }
  }

  event void AMControl.stopDone(error_t err) {
    // do nothing
  }
  
  event void MilliTimer.fired() {
    counter++;
    dbg("RadioCountToLedsC", "RadioCountToLedsC: timer fired, counter is %hu.\n", counter);
    if (locked) {
      return;
    }
    else {
      radio_count_msg_t* rcm = (radio_count_msg_t*)call Packet.getPayload(&packet, sizeof(radio_count_msg_t));
      if (rcm == NULL) {
	return;
      }

      rcm->counter = counter;
/*AM_BROADCAST_ADDR*/
      radio_count_msg_t temp;
      temp.counter=counter;
      temp.sender=rand(1,5);
      temp.receiver=rand(1,5);
      temp.some="dhwjswswmxm";
      temp.localtime=sim_time_string();
      if (call AMSend.send(2, &temp, sizeof(radio_count_msg_t)) == SUCCESS) {
	dbg("RadioCountToLedsC", "RadioCountToLedsC: Source: %hhu, Destination: %hhu, counter: %hhu, neirong: %s, shijian:%s.\n",
		temp.sender, temp.receiver,temp.counter,temp.some,temp.localtime);	
	locked = TRUE;
      }
    }
  }

  event message_t* Receive.receive(message_t* bufPtr, 
				   void* payload, uint8_t len) {
radio_count_msg_t* rcm = (radio_count_msg_t*)payload;
succ++;
 drop=counter-succ;
    dbg("RadioCountToLedsC", "RadioCountToLedsC: From: %hhu To:%hhu,  neirong: %s, fasongshijian:%s, jieshoushijian:%s.\n", temp.sender, temp.receiver,temp.some,temp.localtime,sim_time_string());

    if (len != sizeof(radio_count_msg_t)) {return bufPtr;}
    else {
      radio_count_msg_t* rcm = (radio_count_msg_t*)payload;
      if (rcm->counter & 0x1) {
	call Leds.led0On();
      }
      else {
	call Leds.led0Off();
      }
      if (rcm->counter & 0x2) {
	call Leds.led1On();
      }
      else {
	call Leds.led1Off();
      }
      if (rcm->counter & 0x4) {
	call Leds.led2On();
      }
      else {
	call Leds.led2Off();
      }
      return bufPtr;
    }
  }

  event void AMSend.sendDone(message_t* bufPtr, error_t error) {
    if (&packet == bufPtr) {
      locked = FALSE;
    }
  }

}
