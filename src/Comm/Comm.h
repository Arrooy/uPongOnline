#ifndef COMM_ADRIA
#define COMM_ADRIA

#include <SPI.h>
#include <LoRa.h>
#include <vector>

#include "main.h"
#include "Ball/Ball.h"
#include "GameState.h"

// Time to wait (in ms) before sending again a packet. (only if no ack is recv)
#define RETRANSMISSION_TIMEOUT 750

// The ack msg are sent with an async delay. Now 50 ms.
#define DELAYED_ACK 50

// Fordward declarations
class Ball;

const long frequency = 915E6;  // LoRa Frequency

const int csPin    = 18;       // LoRa radio chip select
const int resetPin = 14;       // LoRa radio reset
const int irqPin   = 26;       // LoRa radio IRQ pin

struct WaitAckMsg{
    uint32_t elapsedTime;
    uint8_t  packetId;
    uint8_t* payload;
    size_t  size;
};

class Comm {
    Ball *ball_;
    GameState *state_;
    uint8_t packetId {0};
    std::vector<WaitAckMsg> waitingMsgs;

    uint32_t delayedAck;
    int delayedPacketId {-1};

public:
    Comm(GameState *state) : state_(state){      
        LoRa.setPins(csPin, resetPin, irqPin);

        if (!LoRa.begin(frequency)) {
            PRINTLN("LoRa init failed. Check your connections.");
            while (true);
        }
    };

    void setBall(Ball * ball);
    void lostPoint(uint8_t newScore);
    void portal(int x, int y, int dx, int dy);
    void beginGame();
    
    void checkNewMessages();
    void checkTxNotReceived();

    // Send msg wrapper
    void sendMsg(uint8_t *buffer, size_t size, bool expectACK=true);
    void sendMsg(uint8_t byte);
};
#endif