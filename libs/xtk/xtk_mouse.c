#include "xtk.h"
#include <stddef.h>
#include <stdio.h>

int xtk_mouse_motion(xtk_spirit_t *spirit, int x, int y)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {
        if (!tmp->visible)
            continue;
        switch (tmp->type)
        {
        case XTK_SPIRIT_TYPE_LABEL:
            {
                
            }
            break;
        case XTK_SPIRIT_TYPE_BUTTON:
            {
                xtk_button_t *button = XTK_BUTTON(tmp);
                if (button->disabled)
                    break;
                if (XTK_IN_SPIRIT(tmp, x, y)) {
                    uview_set_mouse_state(spirit->view, tmp->style.cursor);                        
                    if (button->state == XTK_BUTTON_IDLE) {
                        xtk_signal_emit_by_name(tmp, "enter_notify");
                        xtk_button_change_state(button, XTK_BUTTON_TOUCH);
                        return 0;
                    }
                } else {
                    if (button->state != XTK_BUTTON_IDLE) {
                        xtk_button_change_state(button, XTK_BUTTON_IDLE);
                        uview_set_mouse_state(spirit->view, XTK_CURSOR_NORMAL);
                        xtk_signal_emit_by_name(tmp, "leave_notify");
                    }
                }
            }
            break;
        case XTK_SPIRIT_TYPE_ENTRY:
            {
                xtk_entry_t *entry = XTK_ENTRY(tmp);
                //if (XTK_IN_SPIRIT(tmp, x, y)) {
                if (xtk_entry_get_focus(entry)) {
                    if (entry->start_selecting) {
                        int pos = xtk_entry_get_position(entry);
                        int new_pos = xtk_entry_locate_position(entry, x - tmp->x);
                        if (pos != new_pos) {
                            entry->select_end_pos = new_pos;
                            printf("move pos from %d to %d\n", pos, new_pos);
                            xtk_spirit_show(&entry->spirit);
                        }

                        //xtk_entry_set_position(entry, pos);
                        //}
                    }
                }
                //}
            }
            break;
        default:
            break;
        }
    }
    return -1;
}

int xtk_mouse_btn_down(xtk_spirit_t *spirit, int btn, int x, int y)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {
        if (!tmp->visible)
            continue;
        switch (tmp->type)
        {
        case XTK_SPIRIT_TYPE_LABEL:
            {

            }
            break;
        case XTK_SPIRIT_TYPE_BUTTON:
            {
                if (btn != UVIEW_BTN_LEFT)
                    break;
                xtk_button_t *button = XTK_BUTTON(tmp);
                if (button->disabled)
                    break;
                
                if (XTK_IN_SPIRIT(tmp, x, y)) {
                    uview_set_mouse_state(spirit->view, tmp->style.cursor);                        
                    if (button->state == XTK_BUTTON_TOUCH) {
                        xtk_signal_emit_by_name(tmp, "button_press");
                        xtk_button_change_state(button, XTK_BUTTON_CLICK);
                        return 0;  
                    }
                } else {
                    if (button->state == XTK_BUTTON_TOUCH) {
                        xtk_signal_emit_by_name(tmp, "button_press");
                        xtk_button_change_state(button, XTK_BUTTON_CLICK);
                        return 0;  
                    }
                }
            }
            break;
        case XTK_SPIRIT_TYPE_ENTRY:
            {
                if (btn != UVIEW_BTN_LEFT)
                    break;
                xtk_entry_t *entry = XTK_ENTRY(tmp);
                if (XTK_IN_SPIRIT(tmp, x, y)) {
                    /* 定位 */
                    
                    int pos = xtk_entry_locate_position(entry, x - tmp->x);
                    printf("start pos at %d\n", pos);
                    if (!entry->start_selecting) { // 第一次点击,需要重置选区
                        xtk_entry_select_region(entry, pos, pos);
                    } else {
                        entry->select_start_pos = pos;
                    }

                    xtk_entry_set_position(entry, pos);
                    xtk_entry_set_focus(entry, true);
                    /* 激活后才选择，就不会刷新 */
                    entry->start_selecting = 1;
                } else {
                    entry->start_selecting = 0;
                    xtk_entry_set_focus(entry, false);
                }
            }
            break;
        default:
            break;
        }
    }
    return -1;
}

int xtk_mouse_btn_up(xtk_spirit_t *spirit, int btn, int x, int y)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {
        if (!tmp->visible)
            continue;
        switch (tmp->type)
        {
        case XTK_SPIRIT_TYPE_LABEL:
            {

            }
            break;
        case XTK_SPIRIT_TYPE_BUTTON:
            {
                if (btn != UVIEW_BTN_LEFT)
                    break;
                    
                xtk_button_t *button = XTK_BUTTON(tmp);
                if (button->disabled)
                    break;
                
                if (XTK_IN_SPIRIT(tmp, x, y)) {
                    uview_set_mouse_state(spirit->view, tmp->style.cursor);                        
                    if (button->state == XTK_BUTTON_CLICK) {
                        // printf("mouse call signal: %d, %d\n", x, y);
                        xtk_button_change_state(button, XTK_BUTTON_TOUCH);
                        xtk_signal_emit_by_name(tmp, "button_release");
                        return 0;
                    }
                }
            }
            break;
        case XTK_SPIRIT_TYPE_ENTRY:
            {
                if (btn != UVIEW_BTN_LEFT)
                    break;
                xtk_entry_t *entry = XTK_ENTRY(tmp);
                //if (XTK_IN_SPIRIT(tmp, x, y)) {
                if (xtk_entry_get_focus(entry)) {
                    if (entry->start_selecting) {
                        entry->select_end_pos = xtk_entry_get_position(entry);
                        printf("end pos at %d\n", entry->select_end_pos);
                        /* 有可能end比start小，于是调整位置 */
                        if (entry->select_end_pos < entry->select_start_pos) {
                            int tmp_pos = entry->select_end_pos;
                            entry->select_end_pos = entry->select_start_pos;
                            entry->select_start_pos = tmp_pos;
                        }
                        printf("selection [%d, %d)\n", entry->select_start_pos, entry->select_end_pos);
                        entry->start_selecting = 0;
                        xtk_spirit_show(&entry->spirit);
                    }
                }
                //}
            }
            break;
        default:
            break;
        }
    }
    return -1;
}

int xtk_mouse_load_cursor(int view, char *path, uview_mouse_state_t state, int offx, int offy)
{
    xtk_image_t *img = xtk_image_load2(path, 32, 32);
    if (!img)
        return -1;
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, img->w, img->h, (uview_color_t *) img->buf);
    uview_mouse_state_info_t info;
    info.bmp = &bmp;
    info.off_x = offx;
    info.off_y = offy;
    info.state = state;
    if (uview_set_mouse_state_info(view, &info) < 0) {
        free(img);
        return -1;
    }
    free(img);
    return 0;
}

typedef struct {
    char *pathname;
    int off_x;
    int off_y;
} xtk_cursor_info_t;


static xtk_cursor_info_t cursor_info[] = {
    {"normal", -7, -7},
    {"hold", -14, -14},
    {"help", -7, -7},
    {"pen", -14, -14},
    {"busy", -7, -7},
    {"accurate", -14, -14},
    {"text", 0, 0},
    {"unusable", -14, -14},
    {"vresize", -14, -14},
    {"hresize", -14, -14},
    {"dresize1", -14, -14},
    {"dresize2", -14, -14},
    {"resizeall", -14, -14},
    {"hand", -11, -8},
};

void xtk_mouse_load_cursors(int view, char *pathname)
{
    uview_mouse_state_t i;
    char filename[MAX_PATH];
    for (i = 0; i < UVIEW_MOUSE_STATE_NR; i++) {
        memset(filename, 0, MAX_PATH);
        strcpy(filename, pathname);
        strcat(filename, "/");
        strcat(filename, cursor_info[i].pathname);
        strcat(filename, ".png");
        printf("mouse cursor: %s.\n", filename);
        if (xtk_mouse_load_cursor(view, filename, i, cursor_info[i].off_x, cursor_info[i].off_y) < 0)
            printf("mouse load cursor %s failed!\n", filename);
    }
}