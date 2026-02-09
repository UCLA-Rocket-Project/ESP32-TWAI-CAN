#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp>

#include <stdio.h>

#define CAN_TX 11
#define CAN_RX 12

void setup() {
    Serial.begin(115200);
    delay(2000);

    printf("HELLO\n");

    twai_general_config_t cfg = {
        .mode           = TWAI_MODE_NO_ACK,
        .tx_io          = (gpio_num_t)CAN_TX,
        .rx_io          = (gpio_num_t)CAN_RX,
        .clkout_io      = TWAI_IO_UNUSED,
        .bus_off_io     = TWAI_IO_UNUSED,
        .tx_queue_len   = 10,
        .rx_queue_len   = 10,
        .alerts_enabled = TWAI_ALERT_ALL,
        .clkout_divider = 0,
    };

    // or override everything in one command;
    // It is also safe to use .begin() without .end() as it calls it internally
    if(ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 10, 10, nullptr, &cfg, nullptr)) {
        printf("CAN bus started!\n");
    } else {
        printf("CAN bus failed!\n");
    }
}

void loop() {
    // queue 3 requests
    CanFrame frm = {};
    frm.self     = 1;

    for(int i = 0; i < 3; ++i) {
        frm.identifier = i;
        ESP32Can.writeFrame(frm);
    }

    delay(1000);

    if(ESP32Can.clearReceiveQueue() && ESP32Can.clearTransmitQueue()) {
        printf("Cleared the queue\n");
    } else {
        printf("Could not clear the queue\n");
    }

    if(!ESP32Can.readFrame(frm, 1000)) {
        printf("Frames cleared!\n");
    } else {
        printf("There are still frames\n");
    }

    delay(5000);
}