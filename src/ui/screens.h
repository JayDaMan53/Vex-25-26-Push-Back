#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H



#include "liblvgl/core/lv_obj.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *team_color;
    lv_obj_t *feild_side;
    lv_obj_t *auto_type;
    lv_obj_t *waiting;
    lv_obj_t *running;
    lv_obj_t *header;
    lv_obj_t *header_label;
    lv_obj_t *content;
    lv_obj_t *red_team;
    lv_obj_t *skills;
    lv_obj_t *blue_team;
    lv_obj_t *header_1;
    lv_obj_t *header_label_1;
    lv_obj_t *content_1;
    lv_obj_t *left_side;
    lv_obj_t *right_side;
    lv_obj_t *header_3;
    lv_obj_t *header_label_3;
    lv_obj_t *content_3;
    lv_obj_t *option_a;
    lv_obj_t *option_b;
    lv_obj_t *option_d;
    lv_obj_t *option_c;
    lv_obj_t *option_e;
    lv_obj_t *content_2;
    lv_obj_t *obj0;
    lv_obj_t *loading;
    lv_obj_t *loading_text;
    lv_obj_t *cancel;
    lv_obj_t *start;
    lv_obj_t *content_4;
    lv_obj_t *body;
    lv_obj_t *running_label;
    lv_obj_t *robot_battery_label;
    lv_obj_t *robot_battery_value;
    lv_obj_t *imu_calabrated;
    lv_obj_t *robot_imu_value;
    lv_obj_t *auto_photo;
    lv_obj_t *cat;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_TEAM_COLOR = 1,
    SCREEN_ID_FEILD_SIDE = 2,
    SCREEN_ID_AUTO_TYPE = 3,
    SCREEN_ID_WAITING = 4,
    SCREEN_ID_RUNNING = 5,
};

void create_screen_team_color();
void tick_screen_team_color();

void create_screen_feild_side();
void tick_screen_feild_side();

void create_screen_auto_type();
void tick_screen_auto_type();

void create_screen_waiting();
void tick_screen_waiting();

void create_screen_running();
void tick_screen_running();

enum Themes {
    THEME_ID_SKILLS,
    THEME_ID_RED_TEAM,
    THEME_ID_BLUE_TEAM,
};
enum Colors {
    COLOR_ID_BLUE,
    COLOR_ID_RED,
    COLOR_ID_SKILLS,
    COLOR_ID_TEAM_COLOR,
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