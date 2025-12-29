#include <LVGL_CYD.h>
#include "lvgl_ui.h"
#include "lvgl_objects.h"
#include "lvgl_defs.h"

void setup_soc_arc(lv_obj_t * parent) {
  soc_arc = lv_arc_create(parent);
  lv_arc_set_rotation(soc_arc, 135);
  lv_arc_set_bg_angles(soc_arc, 0, 270);
  lv_obj_align(soc_arc, LV_ALIGN_CENTER, 0, 10);
  lv_arc_set_range(soc_arc, LDR_ARC_MIN, LDR_ARC_MAX);
  lv_arc_set_value(soc_arc, 0);
  lv_obj_set_style_bg_color(soc_arc, SOC_HIGH_COLOR, 0);
  
  soc_label = lv_label_create(parent);
  lv_label_set_text(soc_label, "0%");
  lv_obj_align(soc_label, LV_ALIGN_CENTER, 0, 0);
}

void update_soc_arc(uint8_t value) {
    lv_arc_set_value(soc_arc, value);
    lv_label_set_text_fmt(soc_label, "%d%%", value);
}

// creates a new obj to use as base screen, and set some properties, such as flex
// vertical arrangement, centered horizontally, transparent background, etc.
lv_obj_t * new_screen(lv_obj_t * parent) {

  lv_obj_t * obj = lv_obj_create(parent);
  lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_border_width(obj, 0, 0);
  lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN); // no space at the top
  lv_obj_set_style_pad_row(obj, 10, LV_PART_MAIN);

  return obj;
}

void main_screen() {
  if(!scr_main) {
    scr_main = new_screen(NULL);
    lv_obj_set_size(scr_main, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));

    lv_obj_t* main_tabview = lv_tabview_create(scr_main);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);
    lv_obj_set_size(main_tabview, lv_obj_get_width(scr_main), lv_obj_get_height(scr_main));
    lv_obj_align(main_tabview, LV_ALIGN_TOP_LEFT, 0, 0);

    // Make the tab buttons shorter (not as tall)
    lv_obj_t* tab_btns = lv_tabview_get_tab_btns(main_tabview);
    lv_obj_set_height(tab_btns, 30);

    lv_obj_t* batt_1_tab = lv_tabview_add_tab(main_tabview, "Batt 1");
    lv_obj_t* batt_2_tab = lv_tabview_add_tab(main_tabview, "Batt 2");
    lv_obj_t* batt_3_tab = lv_tabview_add_tab(main_tabview, "Batt 3");
    lv_obj_t* batt_4_tab = lv_tabview_add_tab(main_tabview, "Batt 4");

    setup_soc_arc(batt_1_tab);
    //soc_label = lv_label_create(scr_main);
    //lv_obj_align(soc_label, LV_ALIGN_CENTER, 0, 0);
    //lv_label_set_text(soc_label, "--");
  }
  lv_screen_load(scr_main);                         // Tell LVGL to load as active screen
}

// Set up bottom and top LVGL layers
void setup_layers() {
  // LVGL has multiple layers, one below and two above the 'active screen'.
  // I use the 'bottom' layer for the background color and keep all further
  // objects on transparent background.

  // bottom layer opaque white
  lv_obj_set_style_bg_opa(lv_layer_bottom(), LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_bg_color(lv_layer_bottom(), lv_color_white(), LV_PART_MAIN);
}