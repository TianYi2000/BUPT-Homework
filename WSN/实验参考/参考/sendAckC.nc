/*
 *@author Amirhosein Ataei
 *@date March 29 2020
 */

#include "sendAck.h"
#include "Timer.h"

module sendAckC
{
    uses
    {
        interface Boot;
        interface AMPacket;
        interface Packet;
        interface PacketAcknowledgements;
        interface AMSend;
        interface SplitControl;
        interface Receive;
        interface Timer<TMilli> as MilliTimer;
        interface Read<uint16_t>;

    }
}

implementation
{
    uint8_t counter = 0;
    message_t packet;

    task void sendReq();
    task void sendResp();

    task void sendReq()
    {
        my_msg_t* mess = (my_msg_t*)(call Packet.getPayload(&packet, sizeof(my_msg_t)));
        mess -> msg_type = REQ;
        mess -> msg_counter = counter;

        dbg("radio_send", "Try to send a request to node 2 at time %s \n", sim_time_string());

        call PacketAcknowledgements.requestAck(&packet);
        if(call AMSend.send(2,&packet,sizeof(my_msg_t))==SUCCESS)
        {
            dbg("radio_send", "Try to send a request to node 2 at time %s \n", sim_time_string());
            dbg("radio_pack", ">>>\n \t Payload length %huu \n", call Packet.payloadLength(&packet));
            dbg_clear("radio_pack","\t Source: %hhu \n ", call AMPacket.source(&packet));
            dbg_clear("radio_pack","\t Destination: %hhu \n ", call AMPacket.destination(&packet));
            dbg_clear("radio_pack","\t AM type: %hhu \n ", call AMPacket.type(&packet));
            dbg_clear("radio_pack","\t payload:\n ");
            dbg_clear("radio_pack","\t msg_type: %hhu \n ",mess->msg_type);
            dbg_clear("radio_pack","\t msg_counter: %hhu \n ",mess->msg_counter);
            dbg_clear("radio_pack","\t msg_data: %hhu \n ",mess->msg_data);
            dbg_clear("radio_send","\n ");
            dbg_clear("radio_pack","\n ");

        }
    }
    task void sendResp()
    {
        call Read.read();
    }

    event void Boot.booted()
    {
        dbg("boot", "application booted.\n");
        call SplitControl.start();

    }

    event void SplitControl.startDone(error_t err)
    {
        if(err==SUCCESS)
        {
            dbg("radio", "radio on.\n");
            if(TOS_NODE_ID == 1)
            {
                dbg("role", "node 1 start sending request... \n");
                call MilliTimer.startPeriodic(1000);
            }
    
        }
        else
        {
            call SplitControl.start();
        }
    }

    event void SplitControl.stopDone(error_t err){}

    event void MilliTimer.fired()
    {
        post sendReq();
        counter++;     
    }

    event void AMSend.sendDone(message_t* buf, error_t err)
    {
        if(&packet == buf && err == SUCCESS)
        {
            dbg("radio_send", "packet sent \n");
            }
            if(call PacketAcknowledgements.wasAcked(buf))
            {

				if(TOS_NODE_ID ==1)
				{
					call MilliTimer.stop();	
				} 
                dbg_clear("radio_ack","ACK received. ");
                call MilliTimer.stop();
                dbg("radio_ack","Sending message was stoped.\n");
            }
            else
            {
                dbg_clear("radio_ack", "but ACK was not received \n");
                
                if(TOS_NODE_ID ==2)
				{
					post sendReq();
				} 
                
                
            }
            dbg_clear("radio_send", "at time %s \n ", sim_time_string());
        
    } 

    event message_t* Receive.receive(message_t* buf, void* payload, uint8_t len) 
    {
        my_msg_t* mess = (my_msg_t*)payload;

        dbg("radio_rec", "message received at time: %s \n", sim_time_string());
        dbg("radio_pack", ">>>\n \t Payload length %huu \n", call Packet.payloadLength(buf));
        dbg_clear("radio_pack","\t Source: %hhu \n ", call AMPacket.source(buf));
        dbg_clear("radio_pack","\t Destination: %hhu \n ", call AMPacket.destination(buf));
        dbg_clear("radio_pack","\t AM type: %hhu \n ", call AMPacket.type(buf));
        dbg_clear("radio_pack","\t payload:\n ");
        dbg_clear("radio_pack","\t msg_type: %hhu \n ",mess->msg_type);
        dbg_clear("radio_pack","\t msg_counter: %hhu \n ",mess->msg_counter);
        dbg_clear("radio_pack","\t msg_data: %hhu \n ",mess->msg_data);
        dbg_clear("radio_send","\n ");
        dbg_clear("radio_pack","\n ");

        if(mess->msg_type == REQ)
        {
            post sendResp();
            counter=mess->msg_counter;
        }
    return buf;
    } 
    event void Read.readDone(error_t result, uint16_t data)
    {
        my_msg_t* mess = (my_msg_t*)(call Packet.getPayload(&packet,sizeof(my_msg_t)));
        mess->msg_type = RESP;
        mess->msg_counter = counter;
        mess->msg_data = data;
        dbg("radio_send", "Try to send a request to node 1 at time %s \n", sim_time_string());

        call PacketAcknowledgements.requestAck(&packet);
        if(call AMSend.send(1,&packet, sizeof(my_msg_t))==SUCCESS)
        {
            dbg("radio_send", "Try to send a request to node 1 at time %s \n", sim_time_string());
            dbg("radio_pack", ">>>\n \t Payload length %huu \n", call Packet.payloadLength(&packet));
            dbg_clear("radio_pack","\t Source: %hhu \n ", call AMPacket.source(&packet));
            dbg_clear("radio_pack","\t Destination: %hhu \n ", call AMPacket.destination(&packet));
            dbg_clear("radio_pack","\t AM type: %hhu \n ", call AMPacket.type(&packet));
            dbg_clear("radio_pack","\t payload:\n ");
            dbg_clear("radio_pack","\t msg_type: %hhu \n ",mess->msg_type);
            dbg_clear("radio_pack","\t msg_counter: %hhu \n ",mess->msg_counter);
            dbg_clear("radio_pack","\t msg_data: %hhu \n ",mess->msg_data);
            dbg_clear("radio_send","\n ");
            dbg_clear("radio_pack","\n ");           
        }
    }
}