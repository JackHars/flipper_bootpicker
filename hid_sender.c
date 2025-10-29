#include "hid_sender.h"
#include <furi_hal_usb.h>
#include <furi_hal_usb_hid.h>

#define TAG "BootPicker:HID"

struct HidSender {
    bool usb_mode_set;
};

HidSender* hid_sender_alloc() {
    HidSender* hid = malloc(sizeof(HidSender));
    if(!hid) {
        return NULL;
    }
    
    FURI_LOG_I(TAG, "HidSender allocated");
    hid->usb_mode_set = false;
    
    return hid;
}

void hid_sender_init(HidSender* hid) {
    if(!hid) return;
    
    FURI_LOG_I(TAG, "Initializing USB HID mode");
    
    // Set USB to HID mode
    hid_sender_ensure_usb_mode();
    hid->usb_mode_set = true;
    
    // Wait for USB enumeration to complete
    FURI_LOG_I(TAG, "Waiting for USB enumeration...");
    furi_delay_ms(1000);
    
    if(furi_hal_hid_is_connected()) {
        FURI_LOG_I(TAG, "HID connected successfully!");
    } else {
        FURI_LOG_W(TAG, "HID not showing as connected - check USB cable/settings");
    }
}

void hid_sender_free(HidSender* hid) {
    if(hid) {
        free(hid);
    }
}

void hid_sender_ensure_usb_mode() {
    // Ensure USB is in HID mode
    if(furi_hal_usb_is_locked()) {
        FURI_LOG_W(TAG, "USB is locked, cannot set HID mode");
        return;
    }

    FuriHalUsbInterface* usb_mode = furi_hal_usb_get_config();
    if(usb_mode != &usb_hid) {
        FURI_LOG_I(TAG, "Current USB mode is not HID, switching...");
        furi_hal_usb_set_config(&usb_hid, NULL);
        FURI_LOG_I(TAG, "USB HID mode set, waiting for re-enumeration");
        // Give time for USB re-enumeration to complete
        furi_delay_ms(500);
    } else {
        FURI_LOG_I(TAG, "USB already in HID mode");
    }
}

bool hid_sender_is_connected(HidSender* hid) {
    UNUSED(hid);
    return furi_hal_hid_is_connected();
}

void hid_sender_send_key(HidSender* hid, uint16_t key) {
    // Validate sender object
    if(!hid) {
        FURI_LOG_E(TAG, "NULL HidSender in send_key");
        return;
    }

    // Ensure USB is in HID mode (do this once when first needed)
    if(!hid->usb_mode_set) {
        hid_sender_ensure_usb_mode();
        hid->usb_mode_set = true;
    }

    // Check if HID is connected before sending
    if(!furi_hal_hid_is_connected()) {
        FURI_LOG_D(TAG, "HID not connected, skipping key 0x%02X", key);
        return;
    }
    
    // Press key
    furi_hal_hid_kb_press(key);
    furi_delay_ms(20);
    
    // Release key
    furi_hal_hid_kb_release(key);
    furi_delay_ms(20);
}

void hid_sender_send_f12(HidSender* hid) {
    hid_sender_send_key(hid, HID_KEYBOARD_F12);
}

void hid_sender_send_esc(HidSender* hid) {
    hid_sender_send_key(hid, HID_KEYBOARD_ESC);
}

void hid_sender_execute_profile(HidSender* hid, uint8_t arrow_count, bool send_second_enter) {
    if(!hid) {
        FURI_LOG_E(TAG, "NULL HidSender in execute_profile");
        return;
    }
    
    FURI_LOG_I(TAG, "Executing profile: %u DOWN arrows + ENTER", arrow_count);
    
    // Wait for boot menu to stabilize after F12 spam stops
    furi_delay_ms(500);
    
    // Send DOWN arrows
    for(uint8_t i = 0; i < arrow_count; i++) {
        hid_sender_send_key(hid, HID_KEYBOARD_DOWN_ARROW);
        furi_delay_ms(50);  // Small delay between arrows
    }
    
    // Small delay before ENTER
    furi_delay_ms(100);
    
    // Send ENTER
    FURI_LOG_I(TAG, "Sending ENTER");
    hid_sender_send_key(hid, HID_KEYBOARD_RETURN);
    
    // Send second ENTER after 1 second if enabled
    if(send_second_enter) {
        FURI_LOG_I(TAG, "Waiting 1 second before second ENTER");
        furi_delay_ms(1000);
        FURI_LOG_I(TAG, "Sending second ENTER");
        hid_sender_send_key(hid, HID_KEYBOARD_RETURN);
    }
    
    FURI_LOG_I(TAG, "Profile execution complete");
}

