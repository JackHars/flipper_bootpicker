#pragma once

#include <gui/view.h>

typedef struct BootPickerMainView BootPickerMainView;
typedef void (*BootPickerMainViewCallback)(void* context);

BootPickerMainView* bootpicker_main_view_alloc();
void bootpicker_main_view_free(BootPickerMainView* view);
View* bootpicker_main_view_get_view(BootPickerMainView* view);

void bootpicker_main_view_set_profile_name(BootPickerMainView* view, const char* name);
void bootpicker_main_view_set_status(BootPickerMainView* view, const char* status);
void bootpicker_main_view_set_spamming(BootPickerMainView* view, bool spamming);

