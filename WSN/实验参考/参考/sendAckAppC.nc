/*
 *@author Amirhosein Ataei
 *@date March 29 2020
 */
 
#include "sendAck.h"
configuration sendAckAppC{}
implementation
{
    components MainC, sendAckC as App;
    components new AMSenderC (AM_MY_MSG);
    components new AMReceiverC(AM_MY_MSG);
    components ActiveMessageC;
    components new TimerMilliC();
    components new FakeSensorC();

    App.Boot -> MainC.Boot;

    App.Receive -> AMReceiverC;
    App.AMSend -> AMSenderC;

    App.SplitControl -> ActiveMessageC;

    App.AMPacket -> AMSenderC;
    App.Packet -> AMSenderC;
    App.PacketAcknowledgements->ActiveMessageC;
    App.MilliTimer -> TimerMilliC;

    App.Read -> FakeSensorC;
}


