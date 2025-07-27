#include "modules/AutoResponseModule.h"
#include "TestUtil.h"
#include "mesh/MeshService.h"
#include "mesh/NodeDB.h"
#include <unity.h>

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_auto_response(void)
{
    // Create a mock MeshService
    MeshService mockService;
    service = &mockService;

    // Create a mock NodeDB
    NodeDB mockNodeDB;
    nodeDB = &mockNodeDB;

    // Create an instance of the AutoResponseModule
    AutoResponseModule autoResponseModule;

    // Create a mock MeshPacket
    meshtastic_MeshPacket packet;
    packet.to = 1234;
    packet.from = 5678;
    packet.channel = 0;
    packet.decoded.portnum = meshtastic_PortNum_TEXT_MESSAGE_APP;
    std::string message = "hi";
    memcpy(packet.decoded.payload.bytes, message.c_str(), message.length());
    packet.decoded.payload.size = message.length();

    // Call the handleReceived method
    autoResponseModule.handleReceived(packet);

    // Check if a response was sent
    // TODO: How to check if a response was sent?
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(10);
    delay(2000);

    initializeTestEnvironment();
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_auto_response);
    exit(UNITY_END()); // stop unit testing
}

void loop() {}
