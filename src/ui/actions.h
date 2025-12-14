#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include "liblvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void action_change_team_color(lv_event_t * e);
extern void action_change_feild_side(lv_event_t * e);
extern void action_change_auto_type(lv_event_t * e);
extern void action_start_auton(lv_event_t * e);
extern void action_cancel_auton(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/