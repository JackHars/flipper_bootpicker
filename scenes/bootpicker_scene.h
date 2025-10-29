#pragma once

#include <gui/scene_manager.h>

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) BootPickerScene##id,
typedef enum {
#include "bootpicker_scene_config.h"
    BootPickerSceneNum,
} BootPickerScene;
#undef ADD_SCENE

extern const SceneManagerHandlers bootpicker_scene_handlers;

// Scene entry points
void bootpicker_scene_main_on_enter(void* context);
bool bootpicker_scene_main_on_event(void* context, SceneManagerEvent event);
void bootpicker_scene_main_on_exit(void* context);

void bootpicker_scene_settings_on_enter(void* context);
bool bootpicker_scene_settings_on_event(void* context, SceneManagerEvent event);
void bootpicker_scene_settings_on_exit(void* context);

