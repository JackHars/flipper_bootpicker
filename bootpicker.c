#include "bootpicker_i.h"
#include <furi.h>
#include <gui/gui.h>

#define TAG "BootPicker"

static void f12_timer_callback(void* context) {
    BootPickerApp* app = context;
    if(app && app->hid_sender && app->is_spamming) {
        if(app->use_esc_key) {
            hid_sender_send_esc(app->hid_sender);
        } else {
            hid_sender_send_f12(app->hid_sender);
        }
    }
}

static bool bootpicker_custom_event_callback(void* context, uint32_t custom_event) {
    BootPickerApp* app = context;
    if(!app || !app->scene_manager) {
        return false;
    }
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

static bool bootpicker_back_event_callback(void* context) {
    BootPickerApp* app = context;
    if(!app || !app->scene_manager) {
        return false;
    }
    return scene_manager_handle_back_event(app->scene_manager);
}

static bool bootpicker_main_view_input_callback(InputEvent* event, void* context) {
    BootPickerApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event->type == InputTypeShort) {
        const Profile* profile = NULL;
        
        switch(event->key) {
        case InputKeyLeft:
            // Execute Profile 0 (Windows - just ENTER)
            FURI_LOG_I(TAG, "LEFT: Executing Profile 0 (Windows)");
            app->selected_profile_index = 0;
            profile = profile_get(0);
            if(profile) {
                // Stop spam
                if(app->is_spamming) {
                    app->is_spamming = false;
                    furi_timer_stop(app->f12_timer);
                    bootpicker_main_view_set_spamming(app->main_view, false);
                }
                bootpicker_main_view_set_profile_name(app->main_view, profile->name);
                bootpicker_main_view_set_status(app->main_view, "Executing...");
                hid_sender_execute_profile(app->hid_sender, profile->arrow_count, app->send_second_enter);
                bootpicker_main_view_set_status(app->main_view, "Done!");
                // Stop spamming after execution - don't resume
            }
            consumed = true;
            break;
            
        case InputKeyUp:
            // Execute Profile 1 (Ubuntu - DOWN ENTER)
            FURI_LOG_I(TAG, "UP: Executing Profile 1 (Ubuntu)");
            app->selected_profile_index = 1;
            profile = profile_get(1);
            if(profile) {
                if(app->is_spamming) {
                    app->is_spamming = false;
                    furi_timer_stop(app->f12_timer);
                    bootpicker_main_view_set_spamming(app->main_view, false);
                }
                bootpicker_main_view_set_profile_name(app->main_view, profile->name);
                bootpicker_main_view_set_status(app->main_view, "Executing...");
                hid_sender_execute_profile(app->hid_sender, profile->arrow_count, app->send_second_enter);
                bootpicker_main_view_set_status(app->main_view, "Done!");
                if(app->auto_start_spam) {
                    furi_delay_ms(1000);
                    app->is_spamming = true;
                    bootpicker_main_view_set_status(app->main_view, "Spamming F12...");
                    bootpicker_main_view_set_spamming(app->main_view, true);
                    furi_timer_start(app->f12_timer, furi_ms_to_ticks(app->spam_interval_ms));
                }
            }
            consumed = true;
            break;
            
        case InputKeyRight:
            // Execute Profile 2 (DOWN DOWN ENTER)
            FURI_LOG_I(TAG, "RIGHT: Executing Profile 2");
            app->selected_profile_index = 2;
            profile = profile_get(2);
            if(profile) {
                if(app->is_spamming) {
                    app->is_spamming = false;
                    furi_timer_stop(app->f12_timer);
                    bootpicker_main_view_set_spamming(app->main_view, false);
                }
                bootpicker_main_view_set_profile_name(app->main_view, profile->name);
                bootpicker_main_view_set_status(app->main_view, "Executing...");
                hid_sender_execute_profile(app->hid_sender, profile->arrow_count, app->send_second_enter);
                bootpicker_main_view_set_status(app->main_view, "Done!");
                if(app->auto_start_spam) {
                    furi_delay_ms(1000);
                    app->is_spamming = true;
                    bootpicker_main_view_set_status(app->main_view, "Spamming F12...");
                    bootpicker_main_view_set_spamming(app->main_view, true);
                    furi_timer_start(app->f12_timer, furi_ms_to_ticks(app->spam_interval_ms));
                }
            }
            consumed = true;
            break;
            
        case InputKeyDown:
            // Execute Profile 8 (HIVE - 8 DOWNs ENTER)
            FURI_LOG_I(TAG, "DOWN: Executing Profile 8 (HIVE)");
            app->selected_profile_index = 8;
            profile = profile_get(8);
            if(profile) {
                if(app->is_spamming) {
                    app->is_spamming = false;
                    furi_timer_stop(app->f12_timer);
                    bootpicker_main_view_set_spamming(app->main_view, false);
                }
                bootpicker_main_view_set_profile_name(app->main_view, profile->name);
                bootpicker_main_view_set_status(app->main_view, "Executing...");
                hid_sender_execute_profile(app->hid_sender, profile->arrow_count, app->send_second_enter);
                bootpicker_main_view_set_status(app->main_view, "Done!");
                if(app->auto_start_spam) {
                    furi_delay_ms(1000);
                    app->is_spamming = true;
                    bootpicker_main_view_set_status(app->main_view, "Spamming F12...");
                    bootpicker_main_view_set_spamming(app->main_view, true);
                    furi_timer_start(app->f12_timer, furi_ms_to_ticks(app->spam_interval_ms));
                }
            }
            consumed = true;
            break;
            
        case InputKeyOk:
            // Execute currently selected profile (cycle through all 0-8)
            FURI_LOG_I(TAG, "OK: Executing Profile %u", app->selected_profile_index);
            profile = profile_get(app->selected_profile_index);
            if(profile) {
                if(app->is_spamming) {
                    app->is_spamming = false;
                    furi_timer_stop(app->f12_timer);
                    bootpicker_main_view_set_spamming(app->main_view, false);
                }
                bootpicker_main_view_set_profile_name(app->main_view, profile->name);
                bootpicker_main_view_set_status(app->main_view, "Executing...");
                hid_sender_execute_profile(app->hid_sender, profile->arrow_count, app->send_second_enter);
                bootpicker_main_view_set_status(app->main_view, "Done!");
                // Cycle to next profile
                app->selected_profile_index = (app->selected_profile_index + 1) % profiles_get_count();
                if(app->auto_start_spam) {
                    furi_delay_ms(1000);
                    app->is_spamming = true;
                    bootpicker_main_view_set_status(app->main_view, "Spamming F12...");
                    bootpicker_main_view_set_spamming(app->main_view, true);
                    furi_timer_start(app->f12_timer, furi_ms_to_ticks(app->spam_interval_ms));
                }
            }
            consumed = true;
            break;
            
        default:
            break;
        }
    } else if(event->type == InputTypeLong) {
        switch(event->key) {
        case InputKeyOk:
            // Open settings
            scene_manager_handle_custom_event(app->scene_manager, 1); // OpenSettings
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static BootPickerApp* bootpicker_app_alloc() {
    TRACE_POINT("alloc_start");
    BootPickerApp* app = malloc(sizeof(BootPickerApp));
    if(!app) {
        FURI_LOG_E(TAG, "Failed to malloc BootPickerApp");
        return NULL;
    }
    TRACE_POINT("malloc_ok");
    
    // Initialize settings with defaults
    app->selected_profile_index = 0;  // Windows
    app->spam_interval_ms = 300;      // 300ms
    app->auto_start_spam = true;      // Auto-start enabled
    app->use_esc_key = false;         // Use F12 by default
    app->send_second_enter = false;    // Second ENTER disabled by default
    
    // Initialize HID sender
    TRACE_POINT("hid_alloc");
    app->hid_sender = hid_sender_alloc();
    if(!app->hid_sender) {
        FURI_LOG_E(TAG, "Failed to allocate HID sender");
        free(app);
        return NULL;
    }
    TRACE_POINT("hid_alloc_ok");
    
    // Initialize F12 timer
    TRACE_POINT("timer_alloc");
    app->f12_timer = furi_timer_alloc(f12_timer_callback, FuriTimerTypePeriodic, app);
    if(!app->f12_timer) {
        FURI_LOG_E(TAG, "Failed to allocate F12 timer");
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    app->is_spamming = false;
    TRACE_POINT("timer_alloc_ok");
    
    // Open GUI
    TRACE_POINT("gui_open");
    app->gui = furi_record_open(RECORD_GUI);
    if(!app->gui) {
        FURI_LOG_E(TAG, "Failed to open GUI record");
        furi_timer_free(app->f12_timer);
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    TRACE_POINT("gui_open_ok");
    
    // Initialize view dispatcher
    TRACE_POINT("view_dispatcher_alloc");
    app->view_dispatcher = view_dispatcher_alloc();
    if(!app->view_dispatcher) {
        FURI_LOG_E(TAG, "Failed to allocate view dispatcher");
        furi_record_close(RECORD_GUI);
        furi_timer_free(app->f12_timer);
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    TRACE_POINT("view_dispatcher_ok");
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, bootpicker_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, bootpicker_back_event_callback);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    
    // Initialize scene manager
    TRACE_POINT("scene_manager_alloc");
    app->scene_manager = scene_manager_alloc(&bootpicker_scene_handlers, app);
    if(!app->scene_manager) {
        FURI_LOG_E(TAG, "Failed to allocate scene manager");
        view_dispatcher_free(app->view_dispatcher);
        furi_record_close(RECORD_GUI);
        furi_timer_free(app->f12_timer);
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    TRACE_POINT("scene_manager_ok");
    
    // Initialize views
    TRACE_POINT("main_view_alloc");
    app->main_view = bootpicker_main_view_alloc();
    if(!app->main_view) {
        FURI_LOG_E(TAG, "Failed to allocate main view");
        scene_manager_free(app->scene_manager);
        view_dispatcher_free(app->view_dispatcher);
        furi_record_close(RECORD_GUI);
        furi_timer_free(app->f12_timer);
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    TRACE_POINT("main_view_ok");
    View* main_view = bootpicker_main_view_get_view(app->main_view);
    view_set_input_callback(main_view, bootpicker_main_view_input_callback);
    view_set_context(main_view, app);
    view_dispatcher_add_view(app->view_dispatcher, BootPickerViewMain, main_view);
    
    app->variable_item_list = variable_item_list_alloc();
    if(!app->variable_item_list) {
        FURI_LOG_E(TAG, "Failed to allocate variable item list");
        view_dispatcher_remove_view(app->view_dispatcher, BootPickerViewMain);
        bootpicker_main_view_free(app->main_view);
        scene_manager_free(app->scene_manager);
        view_dispatcher_free(app->view_dispatcher);
        furi_record_close(RECORD_GUI);
        furi_timer_free(app->f12_timer);
        hid_sender_free(app->hid_sender);
        free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, BootPickerViewVariableItemList, 
        variable_item_list_get_view(app->variable_item_list));

    // Load profiles from JSON file
    TRACE_POINT("profiles_load");
    const char* profile_paths[] = {
        APP_DATA_PATH("bootpicker") "/profiles.json",  // User's custom profiles
        APP_ASSETS_PATH("profiles.json"),              // Bundled profiles
    };
    
    bool profiles_loaded = false;
    for(size_t i = 0; i < 2; i++) {
        FURI_LOG_I(TAG, "Trying to load profiles from: %s", profile_paths[i]);
        if(profiles_load_from_file(profile_paths[i])) {
            FURI_LOG_I(TAG, "Successfully loaded %d profiles", profiles_get_count());
            profiles_loaded = true;
            break;
        }
    }
    
    if(!profiles_loaded) {
        FURI_LOG_W(TAG, "Failed to load profiles from any location, using defaults");
        // profiles.c will have already loaded defaults
    }
    TRACE_POINT("profiles_loaded");

    TRACE_POINT("alloc_complete");
    return app;
}

static void bootpicker_app_free(BootPickerApp* app) {
    // Stop and free timer
    furi_timer_stop(app->f12_timer);
    furi_timer_free(app->f12_timer);
    
    // Free HID
    hid_sender_free(app->hid_sender);
    
    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, BootPickerViewMain);
    bootpicker_main_view_free(app->main_view);
    
    view_dispatcher_remove_view(app->view_dispatcher, BootPickerViewVariableItemList);
    variable_item_list_free(app->variable_item_list);
    
    // Free scene manager
    scene_manager_free(app->scene_manager);
    
    // Free view dispatcher
    view_dispatcher_free(app->view_dispatcher);
    
    // Close records
    furi_record_close(RECORD_GUI);
    
    free(app);
}

int32_t bootpicker_app(void* p) {
    UNUSED(p);
    FURI_LOG_I(TAG, "BootPicker starting");
    TRACE_POINT("app_start");

    BootPickerApp* app = bootpicker_app_alloc();
    if(!app) {
        FURI_LOG_E(TAG, "App alloc failed (NULL) - aborting");
        TRACE_POINT("app_alloc_failed");
        return -1;
    }
    TRACE_POINT("app_alloc_ok");

    // Start with main scene
    TRACE_POINT("scene_push_main");
    scene_manager_next_scene(app->scene_manager, BootPickerSceneMain);
    TRACE_POINT("scene_pushed_main");

    // Run view dispatcher
    TRACE_POINT("dispatcher_run");
    view_dispatcher_run(app->view_dispatcher);
    TRACE_POINT("dispatcher_exit");
    
    FURI_LOG_I(TAG, "BootPicker stopping");
    
    bootpicker_app_free(app);
    
    return 0;
}
