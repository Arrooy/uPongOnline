#include "Comm/Comm.h"

void Comm::setBall(Ball * ball)
{
    ball_ = ball;
}

void Comm::checkTxNotReceived()
{

    // Send pending ACK msg
    if(delayedPacketId != -1 && millis () - delayedAck >= DELAYED_ACK)
    {
        PRINTF_1("Sending ACK of packet %d\n", delayedPacketId);
        //Send the ACK msg.
        uint8_t buff[2] = {3, (uint8_t)delayedPacketId};
        sendMsg(buff, 2, false);    
        delayedPacketId = -1;
    }
    
    // Check waiting msg to be re tx
    std::vector<WaitAckMsg>::iterator it = waitingMsgs.begin();

    while(it != waitingMsgs.end()) {
        
        if(millis() - it->elapsedTime >= RETRANSMISSION_TIMEOUT) {
            PRINTLN("Retransmission packet. Did not recv the ack...");
            PRINTF_2("Info is %d Payload header is %d\n", it->packetId, it->payload[0]);
            
            //Retransmission needed!
            LoRa.beginPacket();
            LoRa.write(it->packetId);
            LoRa.write(it->payload, it->size);
            LoRa.endPacket();
            it->elapsedTime = millis();
        }
        else ++it;
    }
}

void Comm::checkNewMessages()
{
    int packetSize = LoRa.parsePacket();
    if(packetSize == 0) return;

    if (LoRa.available()) {
        
        uint8_t newPacketId = LoRa.read();

        // received a packet
        PRINTF_1("Received packet id %d\n", (int)newPacketId);

        // read packet
        byte header = LoRa.read();
        PRINTF_1("Packet head is %d\n", (int)header);
        switch( header )
        {
            // Lost a point msg 
            case 0:
            {
                byte score = LoRa.read();
                PRINTF_1("Score msg. Its %d\n", score);
                ball_->updateOtherScore((int)score);
                if((int)score == 0 && *state_ == PLAYING) *state_ = END;
            }
            break;
            // Portal ball
            case 1:
            {
                PRINTLN("Portal msg");
                int vals[4];
                int i = 0;
                while (LoRa.available()) {
                    vals[i++] = (int)LoRa.read();
                }
                vals[2] = vals[2] - MAX_DX;
                vals[3] = vals[3] - MAX_DY;
                
                PRINTF_4("Vals are: %d %d %d %d\n",vals[0],vals[1],vals[2], vals[3]);
                ball_->show();
                ball_->setVals(vals);   
            }
            break;
            // Start game
            case 2:
            {
                PRINTLN("Start msg");
                *state_ = BEGIN_PLAYING;
            }
            break;

            // Packet ACK
            case 3:
            {
                PRINT("ACK msg in... ");
                byte ackPacketId = LoRa.read();

                // Look for the ackPacket id and delete it from the wait list.
                std::vector<WaitAckMsg>::iterator it = waitingMsgs.begin();

                while(it != waitingMsgs.end()) {
                    if(ackPacketId == it->packetId){
                        // free ram pls
                        free(it->payload);
                        it->payload = NULL;
                        PRINT(" ACK confirmed.");
                        it = waitingMsgs.erase(it);
                    }
                    else ++it;
                }
                // Add enter.
                PRINTLN("");
            }
            break;
            default:
                PRINTLN("\nUNKNOWN HEADER FOUND!\n");
        }
        
        // Dont include ack!
        if(header < 3)
        {
            delayedPacketId = newPacketId;
            delayedAck = millis();
        }

    }
}

void Comm::portal(int x, int y, int dx, int dy)
{   
    // Cant send negative numbers. Offset in tx the number with MAX_DX,MAX_DY.
    // Later in rx, the reverse operation is applied.
    dx = dx + MAX_DX;
    dy = dy + MAX_DY;
    PRINTLN("Sending PORTAL lora msg!");
    PRINTF_4("Vals are: %d %d %d %d\n",x,y,dx,dy);
    uint8_t buff[5] = {1,(uint8_t)x, (uint8_t)y, (uint8_t)dx, (uint8_t)dy};
    sendMsg(buff, 5);
}

void Comm::lostPoint(uint8_t newScore)
{
    PRINTLN("Sending LOST POINT lora msg!");
    uint8_t buff[2] = {0, newScore};
    sendMsg(buff, 2);
}

void Comm::beginGame()
{
    sendMsg(2);
}

void Comm::sendMsg(uint8_t byte)
{
    sendMsg(&byte,sizeof(byte));
}

void Comm::sendMsg(uint8_t *buffer, size_t size, bool expectACK)
{

    // Dont tx if there is any open conversation
    while(!waitingMsgs.empty())
    {
        PRINTLN("Waiting conversaciones abiertas...");
        delay(100);
    }

    // Wait lora ready to send.    
    LoRa.beginPacket();
    LoRa.write(packetId);
    LoRa.write(buffer, size);
    LoRa.endPacket();
    
    if(expectACK)
    {
        PRINTF_1("Adding to wait ack list packet id %d\n",packetId);
        WaitAckMsg waitAck = {
            .elapsedTime = millis(),
            .packetId = packetId,
            .payload = nullptr,
            .size = size
        };

        // Copy packet payload.
        size_t packetSizeInBytes = sizeof(uint8_t) * waitAck.size;
        waitAck.payload = (uint8_t *) malloc(packetSizeInBytes);
        memcpy(waitAck.payload, buffer, packetSizeInBytes);

        // Add to waiting ack list
        waitingMsgs.push_back(waitAck);
    }
    packetId++;
}