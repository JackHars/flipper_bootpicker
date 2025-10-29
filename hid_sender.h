#pragma once

#include <furi.h>
#include <furi_hal_usb_hid.h>
#include "profiles.h"

typedef struct HidSender HidSender;

// HID sender lifecycle
HidSender* hid_sender_alloc();
void hid_sender_init(HidSender* hid); // Initialize USB HID mode (call after alloc, before use)
void hid_sender_free(HidSender* hid);

// USB HID connection management
bool hid_sender_is_connected(HidSender* hid);
void hid_sender_ensure_usb_mode();

// Key sending
void hid_sender_send_key(HidSender* hid, uint16_t key);
void hid_sender_send_f12(HidSender* hid);
void hid_sender_send_esc(HidSender* hid);

// Profile execution
void hid_sender_execute_profile(HidSender* hid, uint8_t arrow_count, bool send_second_enter);

// HID keyboard key codes
#define HID_KEYBOARD_F12 0x45
#define HID_KEYBOARD_ESC 0x29
#define HID_KEYBOARD_DOWN_ARROW 0x51
#define HID_KEYBOARD_UP_ARROW 0x52
#define HID_KEYBOARD_RETURN 0x28

