#include "bootpicker_main_view.h"
#include <gui/elements.h>
#include <furi.h>

struct BootPickerMainView {
    View* view;
};

typedef struct {
    char profile_name[32];
    char status[32];
} BootPickerMainViewModel;

static void bootpicker_main_view_draw_callback(Canvas* canvas, void* _model) {
    BootPickerMainViewModel* model = _model;
    
    canvas_clear(canvas);
    canvas_set_font(canvas, FontSecondary);
    
    // Profile names at corners/edges
    // Top: Ubuntu (UP arrow)
    canvas_draw_str_aligned(canvas, 64, 2, AlignCenter, AlignTop, "Ubuntu");
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignTop, "↑");
    
    // Left: Windows (LEFT arrow)
    canvas_draw_str_aligned(canvas, 2, 32, AlignLeft, AlignCenter, "Win");
    canvas_draw_str_aligned(canvas, 10, 32, AlignLeft, AlignCenter, "←");
    
    // Right: Profile 3 (RIGHT arrow)
    canvas_draw_str_aligned(canvas, 126, 32, AlignRight, AlignCenter, "P3");
    canvas_draw_str_aligned(canvas, 118, 32, AlignRight, AlignCenter, "→");
    
    // Bottom: P3 (DOWN arrow)
    canvas_draw_str_aligned(canvas, 64, 54, AlignCenter, AlignBottom, "↓");
    canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignBottom, "P3");
    
    // Center: Status
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, model->status);
}

static void bootpicker_main_view_enter_callback(void* context) {
    UNUSED(context);
}

static void bootpicker_main_view_exit_callback(void* context) {
    UNUSED(context);
}

BootPickerMainView* bootpicker_main_view_alloc() {
    BootPickerMainView* view = malloc(sizeof(BootPickerMainView));
    if(!view) {
        return NULL;
    }
    
    view->view = view_alloc();
    if(!view->view) {
        free(view);
        return NULL;
    }
    
    view_allocate_model(view->view, ViewModelTypeLocking, sizeof(BootPickerMainViewModel));
    view_set_draw_callback(view->view, bootpicker_main_view_draw_callback);
    view_set_enter_callback(view->view, bootpicker_main_view_enter_callback);
    view_set_exit_callback(view->view, bootpicker_main_view_exit_callback);
    
    // Initialize model
    with_view_model(
        view->view,
        BootPickerMainViewModel* model,
        {
            strcpy(model->profile_name, "Windows");
            strcpy(model->status, "Ready");
        },
        false);
    
    return view;
}

void bootpicker_main_view_free(BootPickerMainView* view) {
    view_free(view->view);
    free(view);
}

View* bootpicker_main_view_get_view(BootPickerMainView* view) {
    return view->view;
}

void bootpicker_main_view_set_profile_name(BootPickerMainView* view, const char* name) {
    if(!view || !view->view || !name) {
        return;
    }
    with_view_model(
        view->view,
        BootPickerMainViewModel* model,
        { strncpy(model->profile_name, name, sizeof(model->profile_name) - 1); },
        true);
}

void bootpicker_main_view_set_status(BootPickerMainView* view, const char* status) {
    if(!view || !view->view || !status) {
        return;
    }
    with_view_model(
        view->view,
        BootPickerMainViewModel* model,
        { strncpy(model->status, status, sizeof(model->status) - 1); },
        true);
}

void bootpicker_main_view_set_spamming(BootPickerMainView* view, bool spamming) {
    UNUSED(view);
    UNUSED(spamming);
    // No longer needed, but keep for compatibility
}
