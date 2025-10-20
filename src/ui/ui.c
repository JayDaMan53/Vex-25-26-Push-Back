#include <stdio.h>
#if defined(EEZ_FOR_LVGL)
#include <eez/core/vars.h>
#endif

#include "ui.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "vars.h"







#if defined(EEZ_FOR_LVGL)

void init_AutoSelect() {
    eez_flow_init(assets, sizeof(assets), (lv_obj_t **)&objects, sizeof(objects), images, sizeof(images), actions);
}

void AutoSelect_tick() {
    eez_flow_tick();
    tick_screen(g_currentScreen);
}

#else

#include <string.h>

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

void AutoSelect_loadScreen(enum ScreensEnum screenId) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
    // printf("Object status: %s\n", (screen == NULL) ? "NULL" : "Not NULL");
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

void init_AutoSelect() {
    create_screens();
    change_color_theme(1);
    AutoSelect_loadScreen(SCREEN_ID_MAIN);

}

// void AutoSelect_tick() {
//     tick_screen(currentScreen);
// }

#endif
