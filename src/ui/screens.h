#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include "liblvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *running;
    lv_obj_t *tabs;
    lv_obj_t *bar;
    lv_obj_t *auton;
    lv_obj_t *cooking;
    lv_obj_t *team_color_container;
    lv_obj_t *team_color_label;
    lv_obj_t *red_team_button;
    lv_obj_t *blue_team_button;
    lv_obj_t *skills_button;
    lv_obj_t *skills_label;
    lv_obj_t *field_side_container;
    lv_obj_t *field_side_label;
    lv_obj_t *left_side;
    lv_obj_t *right_side;
    lv_obj_t *auto_type_container;
    lv_obj_t *auto_type_label;
    lv_obj_t *normal;
    lv_obj_t *no_interference;
    lv_obj_t *do_nothing;
    lv_obj_t *skills_label_2;
    lv_obj_t *start_auto;
    lv_obj_t *header;
    lv_obj_t *header_label;
    lv_obj_t *content;
    lv_obj_t *timer_visual;
    lv_obj_t *timer_label;
    lv_obj_t *stop_auto;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_RUNNING = 2,
};

void create_screen_main();
void tick_screen_main();

void create_screen_running();
void tick_screen_running();

enum Themes {
    THEME_ID_RED_TEAM,
    THEME_ID_BLUE_TEAM,
    THEME_ID_SKILLS,
};
enum Colors {
    COLOR_ID_RED_TEAM,
    COLOR_ID_BLUE_TEAM,
    COLOR_ID_SKILLS,
    COLOR_ID_UI_COLOR_A,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[3][4];
extern uint32_t active_theme_index;

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/