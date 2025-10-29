#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <storage/storage.h>
#include <furi_hal_usb.h>
#include <furi_hal_usb_hid.h>

#include "profiles.h"
#include "hid_sender.h"
#include "scenes/bootpicker_scene.h"
#include "views/bootpicker_main_view.h"

#define TAG "BootPicker"

// Lightweight trace helper for RPC/qFlipper logs
#define TRACE_POINT(id) FURI_LOG_I(TAG, "TRACE %s:%d [%s]", __FILE_NAME__, __LINE__, id)

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    
    // Views
    BootPickerMainView* main_view;
    VariableItemList* variable_item_list;
    
    // HID components
    HidSender* hid_sender;
    FuriTimer* f12_timer;
    bool is_spamming;
    
    // Profile selection (0-8)
    uint8_t selected_profile_index;
    
    // Settings
    uint32_t spam_interval_ms;
    bool auto_start_spam;
    bool use_esc_key;  // false = F12, true = ESC
    bool send_second_enter;  // Send second ENTER 1 second after profile execution
} BootPickerApp;

typedef enum {
    BootPickerViewMain,
    BootPickerViewVariableItemList,
} BootPickerView;

