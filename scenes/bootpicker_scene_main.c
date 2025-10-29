#include "../bootpicker_i.h"
#include <furi.h>
#include <gui/elements.h>

#undef TAG
#define TAG "BootPicker:SceneMain"

enum {
    BootPickerMainEventExecuteProfile,
    BootPickerMainEventOpenSettings,
};

void bootpicker_scene_main_on_enter(void* context) {
    BootPickerApp* app = context;
    
    FURI_LOG_I(TAG, "scene_main_on_enter called");
    
    if(!app) {
        FURI_LOG_E(TAG, "NULL app in scene_main_on_enter");
        return;
    }
    if(!app->view_dispatcher) {
        FURI_LOG_E(TAG, "NULL view_dispatcher in scene_main_on_enter");
        return;
    }
    if(!app->main_view) {
        FURI_LOG_E(TAG, "NULL main_view in scene_main_on_enter");
        return;
    }
    
    TRACE_POINT("main_on_enter");
    FURI_LOG_I(TAG, "Entering main scene");
    
    // Set up main view
    TRACE_POINT("before_switch_to_main_view");
    FURI_LOG_I(TAG, "About to switch to main view");
    view_dispatcher_switch_to_view(app->view_dispatcher, BootPickerViewMain);
    FURI_LOG_I(TAG, "Switched to main view");
    TRACE_POINT("switch_to_main_view");
    
    // Update view with current profile
    const Profile* profile = profile_get(app->selected_profile_index);
    if(profile) {
        bootpicker_main_view_set_profile_name(app->main_view, profile->name);
    }
    TRACE_POINT("main_profile_set");
    
    // Initialize USB HID mode (with delay for enumeration)
    FURI_LOG_I(TAG, "Initializing USB HID...");
    hid_sender_init(app->hid_sender);
    FURI_LOG_I(TAG, "USB HID initialization complete");
    
    // Start F12 spam if auto-start enabled (only if not already in Done state)
    if(app->auto_start_spam && !app->is_spamming && !app->is_done_state) {
        app->is_spamming = true;
        bootpicker_main_view_set_status(app->main_view, "Spamming F12...");
        bootpicker_main_view_set_spamming(app->main_view, true);
        furi_timer_start(app->f12_timer, furi_ms_to_ticks(app->spam_interval_ms));
        FURI_LOG_I(TAG, "Started F12 spam timer");
        TRACE_POINT("f12_timer_started");
    } else if(!app->is_spamming && !app->is_done_state) {
        bootpicker_main_view_set_status(app->main_view, "Ready");
        bootpicker_main_view_set_spamming(app->main_view, false);
    }
}

bool bootpicker_scene_main_on_event(void* context, SceneManagerEvent event) {
    BootPickerApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case BootPickerMainEventOpenSettings:
            FURI_LOG_I(TAG, "Settings event");
            
            // Stop spam timer
            if(app->is_spamming) {
                app->is_spamming = false;
                furi_timer_stop(app->f12_timer);
            }
            
            scene_manager_next_scene(app->scene_manager, BootPickerSceneSettings);
            consumed = true;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Exit application
        consumed = false; // Let view dispatcher handle exit
    }
    
    return consumed;
}

void bootpicker_scene_main_on_exit(void* context) {
    BootPickerApp* app = context;
    
    if(!app) {
        return;
    }
    
    FURI_LOG_I(TAG, "Exiting main scene");
    
    // Stop spam timer
    if(app->f12_timer && app->is_spamming) {
        app->is_spamming = false;
        furi_timer_stop(app->f12_timer);
    }
}
