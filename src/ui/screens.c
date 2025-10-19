#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Tabs
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 50);
            objects.tabs = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffbcbcbc), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0f0f0f), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Bar
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.bar = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4a4a4a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffe9e9e9), LV_PART_MAIN | LV_STATE_DEFAULT);
                    // lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    lv_obj_t *obj = lv_tabview_get_content(parent_obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // Auton
                            lv_obj_t *obj = lv_tabview_add_tab(lv_obj_get_parent(parent_obj), "Auton");
                            objects.auton = obj;
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4d4d4d), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffb7b7b7), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // TeamColorContainer
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.team_color_container = obj;
                                    lv_obj_set_pos(obj, 6, -3);
                                    lv_obj_set_size(obj, 371, 173);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // TeamColorLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.team_color_label = obj;
                                            lv_obj_set_pos(obj, 7, 0);
                                            lv_obj_set_size(obj, 346, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            // lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "Team Color: Red Team");
                                        }
                                        {
                                            // RedTeamButton
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.red_team_button = obj;
                                            lv_obj_set_pos(obj, 30, 42);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][0]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Red Team");
                                                }
                                            }
                                        }
                                        {
                                            // BlueTeamButton
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.blue_team_button = obj;
                                            lv_obj_set_pos(obj, 30, 82);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Blue Team");
                                                }
                                            }
                                        }
                                        {
                                            // SkillsButton
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.skills_button = obj;
                                            lv_obj_set_pos(obj, 30, 122);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // SkillsLabel
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.skills_label = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Skills");
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    // FieldSideContainer
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.field_side_container = obj;
                                    lv_obj_set_pos(obj, 381, -3);
                                    lv_obj_set_size(obj, 371, 173);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // FieldSideLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.field_side_label = obj;
                                            lv_obj_set_pos(obj, 7, 0);
                                            lv_obj_set_size(obj, 346, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            // lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "Field Side: Left Side");
                                        }
                                        {
                                            // LeftSide
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.left_side = obj;
                                            lv_obj_set_pos(obj, 30, 42);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Left Side");
                                                }
                                            }
                                        }
                                        {
                                            // RightSide
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.right_side = obj;
                                            lv_obj_set_pos(obj, 30, 82);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Right Side");
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    // AutoTypeContainer
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.auto_type_container = obj;
                                    lv_obj_set_pos(obj, 6, 170);
                                    lv_obj_set_size(obj, 371, 173);
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // AutoTypeLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.auto_type_label = obj;
                                            lv_obj_set_pos(obj, 7, 0);
                                            lv_obj_set_size(obj, 346, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            // lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "Auto Type: A");
                                        }
                                        {
                                            // Normal
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.normal = obj;
                                            lv_obj_set_pos(obj, 30, 42);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Normal");
                                                }
                                            }
                                        }
                                        {
                                            // NoInterference
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.no_interference = obj;
                                            lv_obj_set_pos(obj, 30, 82);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "No Interference");
                                                }
                                            }
                                        }
                                        {
                                            // DoNothing
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.do_nothing = obj;
                                            lv_obj_set_pos(obj, 30, 122);
                                            lv_obj_set_size(obj, 310, 31);
                                            lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // SkillsLabel_2
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.skills_label_2 = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                    lv_label_set_text(obj, "Do Nothing");
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    // Start Auto
                                    lv_obj_t *obj = lv_btn_create(parent_obj);
                                    objects.start_auto = obj;
                                    lv_obj_set_pos(obj, 13, 358);
                                    lv_obj_set_size(obj, 735, 40);
                                    lv_obj_add_event_cb(obj, action_run_auto, LV_EVENT_PRESSED, (void *)0);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                    lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "Start Auto");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_running() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.running = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Header
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff252525), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe9e9e9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff4d4d4d), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff00ae3e), LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff2e), LV_PART_ITEMS | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // HeaderLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.header_label = obj;
                    lv_obj_set_pos(obj, 8, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    // lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Text");
                }
            }
        }
        {
            // Content
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.content = obj;
            lv_obj_set_pos(obj, 0, 50);
            lv_obj_set_size(obj, 800, 430);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0f0f0f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe9e9e9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff00ae3e), LV_PART_ITEMS | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00ff2e), LV_PART_ITEMS | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // TimerVisual
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.timer_visual = obj;
                    lv_obj_set_pos(obj, 484, 79);
                    lv_obj_set_size(obj, 274, 272);
                    lv_arc_set_range(obj, 0, 15);
                    lv_arc_set_value(obj, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // Timer Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.timer_label = obj;
                            lv_obj_set_pos(obj, 82, 110);
                            lv_obj_set_size(obj, 110, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "0:00");
                        }
                    }
                }
                {
                    // Stop Auto
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.stop_auto = obj;
                    lv_obj_set_pos(obj, 33, 379);
                    lv_obj_set_size(obj, 735, 40);
                    lv_obj_add_event_cb(obj, action_stop_auto, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(theme_colors[active_theme_index][3]), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_width(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_transform_height(obj, -1, LV_PART_MAIN | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Stop Auto");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_running();
}

void tick_screen_running() {
}

void change_color_theme(uint32_t theme_index) {
    active_theme_index = theme_index;
    
    lv_obj_set_style_border_color(objects.bar, lv_color_hex(theme_colors[theme_index][3]), LV_PART_ITEMS | LV_STATE_CHECKED);
    
    lv_obj_set_style_bg_color(objects.bar, lv_color_hex(theme_colors[theme_index][3]), LV_PART_ITEMS | LV_STATE_CHECKED);
    
    lv_obj_set_style_bg_color(objects.red_team_button, lv_color_hex(theme_colors[theme_index][0]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.blue_team_button, lv_color_hex(theme_colors[theme_index][1]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.skills_button, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.left_side, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.right_side, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.normal, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.no_interference, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.do_nothing, lv_color_hex(theme_colors[theme_index][2]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.start_auto, lv_color_hex(theme_colors[theme_index][3]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.timer_visual, lv_color_hex(theme_colors[theme_index][3]), LV_PART_KNOB | LV_STATE_DEFAULT);
    
    lv_obj_set_style_arc_color(objects.timer_visual, lv_color_hex(theme_colors[theme_index][3]), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(objects.stop_auto, lv_color_hex(theme_colors[theme_index][3]), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_invalidate(objects.main);
    lv_obj_invalidate(objects.running);
}

uint32_t theme_colors[3][4] = {
    { 0xffe00000, 0xff007bff, 0xff212121, 0xffa30000 },
    { 0xffe00000, 0xff007bff, 0xff212121, 0xff007bff },
    { 0xffe00000, 0xff007bff, 0xff212121, 0xff212121 },
};


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_running,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_running();
}
