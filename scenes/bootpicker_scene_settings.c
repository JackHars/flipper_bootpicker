#include "../bootpicker_i.h"
#include <gui/modules/variable_item_list.h>
#include <furi.h>

#undef TAG
#define TAG "BootPicker:SceneSettings"

enum {
    SettingsItemBootKey,
    SettingsItemSpamInterval,
    SettingsItemAutoStart,
    SettingsItemSecondEnter,
    SettingsItemSave,
    SettingsItemCancel,
};

static void settings_item_callback(void* context, uint32_t index) {
    BootPickerApp* app = context;
    
    switch(index) {
    case SettingsItemSave:
        FURI_LOG_I(TAG, "Saving settings");
        // Settings are saved in app struct, no file I/O needed
        scene_manager_previous_scene(app->scene_manager);
        break;
        
    case SettingsItemCancel:
        FURI_LOG_I(TAG, "Canceling settings");
        scene_manager_previous_scene(app->scene_manager);
        break;
    }
}

static void settings_boot_key_callback(VariableItem* item) {
    BootPickerApp* app = variable_item_get_context(item);
    
    uint8_t index = variable_item_get_current_value_index(item);
    app->use_esc_key = (index == 1);
    variable_item_set_current_value_text(item, app->use_esc_key ? "ESC" : "F12");
}

static void settings_spam_interval_callback(VariableItem* item) {
    BootPickerApp* app = variable_item_get_context(item);
    
    uint8_t index = variable_item_get_current_value_index(item);
    app->spam_interval_ms = 200 + (index * 100); // 200ms to 2000ms in 100ms steps
    
    char text[16];
    snprintf(text, sizeof(text), "%lu ms", app->spam_interval_ms);
    variable_item_set_current_value_text(item, text);
}

static void settings_auto_start_callback(VariableItem* item) {
    BootPickerApp* app = variable_item_get_context(item);
    
    uint8_t index = variable_item_get_current_value_index(item);
    app->auto_start_spam = (index == 1);
    variable_item_set_current_value_text(item, app->auto_start_spam ? "Yes" : "No");
}

static void settings_second_enter_callback(VariableItem* item) {
    BootPickerApp* app = variable_item_get_context(item);
    
    uint8_t index = variable_item_get_current_value_index(item);
    app->send_second_enter = (index == 1);
    variable_item_set_current_value_text(item, app->send_second_enter ? "Yes" : "No");
}

void bootpicker_scene_settings_on_enter(void* context) {
    BootPickerApp* app = context;
    
    if(!app || !app->variable_item_list) {
        FURI_LOG_E(TAG, "NULL pointer in settings_on_enter");
        return;
    }
    TRACE_POINT("settings_on_enter");
    
    VariableItemList* vil = app->variable_item_list;
    
    FURI_LOG_I(TAG, "Entering settings scene");
    
    variable_item_list_reset(vil);
    TRACE_POINT("settings_reset_list");
    
    // Boot Key selection (F12 or ESC)
    VariableItem* item = variable_item_list_add(vil, "Boot Key", 2, 
        settings_boot_key_callback, app);
    variable_item_set_current_value_index(item, app->use_esc_key ? 1 : 0);
    variable_item_set_current_value_text(item, app->use_esc_key ? "ESC" : "F12");
    
    // Spam interval
    item = variable_item_list_add(vil, "Spam Interval", 19, 
        settings_spam_interval_callback, app);
    uint8_t interval_index = (app->spam_interval_ms - 200) / 100;
    variable_item_set_current_value_index(item, interval_index);
    char interval_text[16];
    snprintf(interval_text, sizeof(interval_text), "%lu ms", app->spam_interval_ms);
    variable_item_set_current_value_text(item, interval_text);
    
    // Auto-start spam
    item = variable_item_list_add(vil, "Auto-Start Spam", 2, 
        settings_auto_start_callback, app);
    variable_item_set_current_value_index(item, app->auto_start_spam ? 1 : 0);
    variable_item_set_current_value_text(item, app->auto_start_spam ? "Yes" : "No");
    
    // Second Enter
    item = variable_item_list_add(vil, "Second Enter", 2, 
        settings_second_enter_callback, app);
    variable_item_set_current_value_index(item, app->send_second_enter ? 1 : 0);
    variable_item_set_current_value_text(item, app->send_second_enter ? "Yes" : "No");
    
    // Actions
    variable_item_list_add(vil, "Save", 0, NULL, app);
    variable_item_list_add(vil, "Cancel", 0, NULL, app);
    
    variable_item_list_set_enter_callback(vil, settings_item_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, BootPickerViewVariableItemList);
    TRACE_POINT("settings_switched_view");
}

bool bootpicker_scene_settings_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void bootpicker_scene_settings_on_exit(void* context) {
    BootPickerApp* app = context;
    
    FURI_LOG_I(TAG, "Exiting settings scene");
    
    variable_item_list_reset(app->variable_item_list);
}
