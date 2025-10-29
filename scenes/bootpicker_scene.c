#include "bootpicker_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const bootpicker_scene_on_enter_handlers[])(void*) = {
#include "bootpicker_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const bootpicker_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
#include "bootpicker_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const bootpicker_scene_on_exit_handlers[])(void*) = {
#include "bootpicker_scene_config.h"
};
#undef ADD_SCENE

// Scene manager configuration
const SceneManagerHandlers bootpicker_scene_handlers = {
    .on_enter_handlers = bootpicker_scene_on_enter_handlers,
    .on_event_handlers = bootpicker_scene_on_event_handlers,
    .on_exit_handlers = bootpicker_scene_on_exit_handlers,
    .scene_num = BootPickerSceneNum,
};

