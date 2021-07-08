#include "xtk.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/vmm.h>

static xtk_timer_t *xtk_window_find_timer(xtk_window_t *window, uint32_t timer_id);
static int xtk_window_destroy_timer_all(xtk_window_t *window);

static xtk_window_style_t __xtk_window_style_defult = {
    1, 
    24,
    XTK_RGB(245, 245, 245), // background
    XTK_RGB(225, 225, 225),
    XTK_RGB(230, 230, 230), // front
    XTK_RGB(200, 200, 200),
    XTK_RGB(200, 200, 200), // border
    XTK_RGB(180, 180, 180),
    XTK_RGB(25, 25, 25),      // text
    XTK_RGB(118, 118, 118),
};

void xtk_window_calc_content_size(xtk_window_t *window, int vwidth, int vheight, int *cwidth, int *cheight)
{
    int content_width, content_height;
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        content_width = vwidth - window->style->border_thick * 2;
        content_height = vheight - window->style->border_thick * 2 - window->style->navigation_height;
    } else {
        content_width = vwidth;
        content_height = vheight;
    }
    *cwidth = content_width;
    *cheight = content_height;
}

void xtk_window_calc_view_size(xtk_window_t *window, int cwidth, int cheight, int *vwidth, int *vheight)
{
    int view_width, view_height;
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        view_width = cwidth + window->style->border_thick * 2;
        view_height = cheight + window->style->border_thick * 2 + window->style->navigation_height;
    } else {
        view_width = cwidth;
        view_height = cheight;
    }
    *vwidth = view_width;
    *vheight = view_height;
}

/**
 * width和height是整个视图的大小，所以，求创窗口大小时需要进行计算。
 */
static int xtk_window_change_size(xtk_window_t *window, int width, int height)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->spirit;

    // 计算新内容大小
    int content_width = 0, content_height = 0;
    xtk_window_calc_content_size(window, width, height, &content_width, &content_height);

    // 内容一样大，那么就不调整
    /*
    printf("xtk window change size: from %d %d to %d %d\n", window->content_width,
        window->content_height, content_width, content_height);*/
    if (window->content_width == content_width && window->content_height == content_height) {
        /*printf("xtk window change size: from %d %d to %d %d same, just return.\n", 
        window->content_width, window->content_height, content_width, content_height);*/
        return 0;
    }
    
    window->content_width = content_width;
    window->content_height = content_height;

    if (xtk_window_mmap_resize(window, width, height) < 0) /* 尝试调整映射窗口大小 */
        return -1;

    // 调整窗口精灵大小
    xtk_spirit_reset_size(spirit, content_width, content_height);
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        xtk_spirit_reset_size(&window->window_spirit, width, height);
        // 重绘窗口
        xtk_window_draw_border(window, 1, 1);
    } else if (window->type == XTK_WINDOW_POPUP) {
        xtk_window_draw_no_border(window);
    }
    // 设置可移动区域大小
    xtk_window_reset_mobile_area(window);
    
    // TODO: 调整子精灵的位置和大小

    // 显示精灵子控件
    xtk_spirit_show_children(spirit);

    // 触发窗口改变信号
    xtk_event_t event;
    event.type = XTK_WINRESIZE_EVENT;
    event.winresize.w = content_width;
    event.winresize.h = content_height;
    xtk_signal_emit_arg(spirit, "resize_event", &event);
    return 0;
}

/**
 * width和height是内容窗口的大小
 */
int xtk_window_resize(xtk_window_t *window, int width, int height)
{
    int win_width = 0, win_height = 0;
    xtk_window_calc_view_size(window, width, height, &win_width, &win_height);
    uview_repair_size(&win_width, &win_height);
    return uview_resize(window->spirit.view, win_width, win_height);
}

/**
 * 过滤鼠标消息，过滤掉返回1，没有返回0
 */
int xtk_mouse_fileter_msg(xtk_spirit_t *spirit, uview_msg_t *msg)
{
    uview_msg_get_mouse_x(msg) = uview_msg_get_mouse_x(msg) - spirit->x;
    uview_msg_get_mouse_y(msg) = uview_msg_get_mouse_y(msg) - spirit->y;
    int x = uview_msg_get_mouse_x(msg);
    int y = uview_msg_get_mouse_y(msg);
    if (x < 0 || y < 0 || x >= spirit->width || y >= spirit->height) {
        return 1;
    }
    int msg_type = uview_msg_get_type(msg);
    xtk_event_t event;
    int tmpval = -1;
    switch (msg_type) {
    case UVIEW_MSG_MOUSE_MOTION:
        event.type = XTK_MOUSE_MOTION;                
        event.motion.x = x;
        event.motion.y = y;
        if (xtk_signal_emit_arg(spirit, "motion_notify", &event))
            return 1;
        break;
    case UVIEW_MSG_MOUSE_LBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_LBTN_DOWN:
    case UVIEW_MSG_MOUSE_MBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_MBTN_DOWN:
    case UVIEW_MSG_MOUSE_RBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_RBTN_DOWN:
        if (msg_type == UVIEW_MSG_MOUSE_LBTN_DOWN || msg_type == UVIEW_MSG_MOUSE_LBTN_DBLCLK) {
            tmpval = UVIEW_BTN_LEFT;
        } else if (msg_type == UVIEW_MSG_MOUSE_MBTN_DOWN || msg_type == UVIEW_MSG_MOUSE_MBTN_DBLCLK) {
            tmpval = UVIEW_BTN_MIDDLE;
        } else if (msg_type == UVIEW_MSG_MOUSE_RBTN_DOWN || msg_type == UVIEW_MSG_MOUSE_RBTN_DBLCLK) {
            tmpval = UVIEW_BTN_RIGHT;
        }
        event.type = XTK_MOUSE_BUTTON_DOWN;
        event.button.state = XTK_PRESSED;
        event.button.button = tmpval;
        event.button.x = x;
        event.button.y = y;
        if (xtk_signal_emit_arg(spirit, "button_press", &event))
            return 1;
        break;
    case UVIEW_MSG_MOUSE_LBTN_UP:
    case UVIEW_MSG_MOUSE_MBTN_UP:
    case UVIEW_MSG_MOUSE_RBTN_UP:
        if (msg_type == UVIEW_MSG_MOUSE_LBTN_UP) {
            tmpval = UVIEW_BTN_LEFT;
        } else if (msg_type == UVIEW_MSG_MOUSE_MBTN_UP) {
            tmpval = UVIEW_BTN_MIDDLE;
        } else if (msg_type == UVIEW_MSG_MOUSE_RBTN_UP) {
            tmpval = UVIEW_BTN_RIGHT;
        }
        event.type = XTK_MOUSE_BUTTON_UP;
        event.button.state = XTK_RELEASED;
        event.button.button = tmpval;
        event.button.x = x;
        event.button.y = y;
        if (xtk_signal_emit_arg(spirit, "button_release", &event))
            return 1;
        break;
    case UVIEW_MSG_MOUSE_WHEEL_UP:
    case UVIEW_MSG_MOUSE_WHEEL_DOWN:
    case UVIEW_MSG_MOUSE_WHEEL_LEFT:
    case UVIEW_MSG_MOUSE_WHEEL_RIGHT:
        if (msg_type == UVIEW_MSG_MOUSE_WHEEL_UP) {
            tmpval = XTK_WHEEL_UP;
        } else if (msg_type == UVIEW_MSG_MOUSE_WHEEL_DOWN) {
            tmpval = XTK_WHEEL_DOWN;
        } else if (msg_type == UVIEW_MSG_MOUSE_WHEEL_LEFT) {
            tmpval = XTK_WHEEL_LEFT;
        } else if (msg_type == UVIEW_MSG_MOUSE_WHEEL_RIGHT) {
            tmpval = XTK_WHEEL_RIGHT;
        }
        event.type = XTK_MOUSE_WHEEL;
        event.wheel.wheel = tmpval;
        event.wheel.x = x;
        event.wheel.y = y;
        if (xtk_signal_emit_arg(spirit, "button_scroll", &event))
            return 1;
        break;
    default :
        break;
    }
    return 0;
}

int xtk_keyboard_fileter_msg(xtk_spirit_t *spirit, uview_msg_t *msg)
{
    xtk_event_t event;
    int msg_type = uview_msg_get_type(msg);
    if (msg_type == UVIEW_MSG_KEY_DOWN) {
        event.type = XTK_KEY_DOWN;
        event.key.state = XTK_PRESSED;
        event.key.keycode.code = uview_msg_get_key_code(msg);
        event.key.keycode.modify = uview_msg_get_key_modify(msg);
        if (xtk_signal_emit_arg(spirit, "key_press", &event))
            return 1;
    } else if (msg_type == UVIEW_MSG_KEY_UP) {
        event.type = XTK_KEY_UP;
        event.key.state = XTK_RELEASED;
        event.key.keycode.code = uview_msg_get_key_code(msg);
        event.key.keycode.modify = uview_msg_get_key_modify(msg);
        if (xtk_signal_emit_arg(spirit, "key_release", &event))
            return 1;
    }
    return 0;
}


void xtk_window_filter_msg(xtk_window_t *window, uview_msg_t *msg)
{
    xtk_spirit_t *spirit = &window->spirit;
    int msg_type = uview_msg_get_type(msg);
    // 转换鼠标坐标位置为窗口内容的坐标
    switch (msg_type) {
    case UVIEW_MSG_MOUSE_MOTION:
    case UVIEW_MSG_MOUSE_LBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_LBTN_DOWN:
    case UVIEW_MSG_MOUSE_MBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_MBTN_DOWN:
    case UVIEW_MSG_MOUSE_RBTN_DBLCLK:
    case UVIEW_MSG_MOUSE_RBTN_DOWN:
    case UVIEW_MSG_MOUSE_LBTN_UP:
    case UVIEW_MSG_MOUSE_MBTN_UP:
    case UVIEW_MSG_MOUSE_RBTN_UP:
    case UVIEW_MSG_MOUSE_WHEEL_UP:
    case UVIEW_MSG_MOUSE_WHEEL_DOWN:
    case UVIEW_MSG_MOUSE_WHEEL_LEFT:
    case UVIEW_MSG_MOUSE_WHEEL_RIGHT:
        if (xtk_mouse_fileter_msg(spirit, msg))
            return;
        break;
    case UVIEW_MSG_KEY_DOWN:
    case UVIEW_MSG_KEY_UP:
        if (xtk_keyboard_fileter_msg(spirit, msg))
            return;
        break;
    case UVIEW_MSG_HIDE:
        xtk_mouse_motion(spirit, -1, -1);
        xtk_spirit_hide(spirit);
        break;
    case UVIEW_MSG_SHOW:
        xtk_mouse_motion(spirit, -1, -1);
        xtk_spirit_show(spirit);
        break;
    case UVIEW_MSG_ACTIVATE:
        if (window->type == XTK_WINDOW_TOPLEVEL) {
            xtk_window_set_active(window, true);        
            xtk_signal_emit_by_name(spirit, "active");    
            return;    
        }
        break;
    case UVIEW_MSG_INACTIVATE:
        if (window->type == XTK_WINDOW_TOPLEVEL) {
            xtk_window_set_active(window, false);        
            xtk_signal_emit_by_name(spirit, "inactive");    
            return;
        }
        break;
    case UVIEW_MSG_LEAVE:
        xtk_signal_emit_by_name(spirit, "leave_notify");
        break;
    case UVIEW_MSG_ENTER:
        xtk_signal_emit_by_name(spirit, "enter_notify");
        break;
    case UVIEW_MSG_RESIZE:
        /* 响应大小调整, width和height是视图的大小 */
        xtk_window_change_size(window, uview_msg_get_resize_width(msg),
            uview_msg_get_resize_height(msg));
        /* 调整窗口后，鼠标位置发生了改变，需要做一次位置检测 */
        xtk_mouse_motion(spirit, -1, -1);
        break;
    case UVIEW_MSG_PAINT:
        if (window->paint_callback) {
            xtk_rect_t rect;
            if (!xtk_window_get_invalid(window, &rect)) {
                if (xtk_rect_valid(&rect)) {
                    window->paint_callback(spirit, &rect);
                    return;
                }
            }
        }
        break;
    case UVIEW_MSG_TIMER:
        {
            // 寻找定时器
            unsigned int timer_id = uview_msg_get_timer_id(msg);
            xtk_timer_t *timer = xtk_window_find_timer(window, timer_id);
            if (timer) {
                if (timer->callback) {
                    bool timer_restart = timer->callback(spirit, timer_id, timer->calldata);
                    // 返回值为true就继续执行触发定时器
                    if (timer_restart) {
                        xtk_window_restart_timer(window, timer_id, -1);
                    } 
                }
                return;
            }
            break;
        }
    case UVIEW_MSG_MOVE:
        {
            xtk_event_t event;
            event.type = XTK_WINMOVE_EVENT;
            event.winmove.x = uview_msg_get_move_x(msg);
            event.winmove.y = uview_msg_get_move_y(msg);
            xtk_signal_emit_arg(spirit, "move_event", &event);
            printf("window move to %d, %d\n", event.winmove.x, event.winmove.y);
        }
    default:
        break;
    }
    // 调用用户处理函数
    if (window->routine)
        window->routine(spirit, msg);
}

/**
 * 窗口逻辑
 * 返回0表示消息被截断，返回-1表示消息还可以继续传递
 */
int xtk_window_main(xtk_spirit_t *spirit, uview_msg_t *msg)
{
    // 每个窗口精灵都需要进行这些消息检测
    int x = uview_msg_get_mouse_x(msg) - spirit->x;
    int y = uview_msg_get_mouse_y(msg) - spirit->y;
    switch (uview_msg_get_type(msg)) {
    case UVIEW_MSG_LEAVE:
    case UVIEW_MSG_ENTER:
        xtk_mouse_motion(spirit, x, y);
        break;
    case UVIEW_MSG_MOUSE_MOTION:
        if (!xtk_mouse_motion(spirit, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_LBTN_DOWN:
        if (!xtk_mouse_btn_down(spirit, UVIEW_BTN_LEFT, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_MBTN_DOWN:
        if (!xtk_mouse_btn_down(spirit, UVIEW_BTN_MIDDLE, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_RBTN_DOWN:
        if (!xtk_mouse_btn_down(spirit, UVIEW_BTN_RIGHT, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_LBTN_UP:
        if (!xtk_mouse_btn_up(spirit, UVIEW_BTN_LEFT, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_MBTN_UP:
        if (!xtk_mouse_btn_up(spirit, UVIEW_BTN_MIDDLE, x, y))
            return 0;
        break;
    case UVIEW_MSG_MOUSE_RBTN_UP:
        if (!xtk_mouse_btn_up(spirit, UVIEW_BTN_RIGHT, x, y))
            return 0;
        break;
    case UVIEW_MSG_KEY_DOWN:
        if (!xtk_keyboard_key_down(spirit, uview_msg_get_key_code(msg), 
            uview_msg_get_key_modify(msg)))
            return 0;
        break;
    case UVIEW_MSG_KEY_UP:
        if (!xtk_keyboard_key_up(spirit, uview_msg_get_key_code(msg), 
            uview_msg_get_key_modify(msg)))
            return 0;
        break;        
    default:
        break;
    }
    return -1;  // 需要进一步处理
}

int xtk_window_quit(xtk_spirit_t *spirit)
{
    return xtk_spirit_destroy_all(spirit);
}

/**
 * 绘制窗口边框
 * @is_active: 是否为激活状态，1为激活，0为不激活
 * @win_back: 是否重绘窗体背景：1为重绘，0为不重绘
 */
int xtk_window_draw_border(xtk_window_t *window, 
        int is_active, int win_back)
{
    if (!window)
        return -1;
    // printf("draw border %d %d\n", is_active, win_back);
    uview_color_t back, border, text_c;
    if (is_active) {
        back = window->style->background_color_active;
        border = window->style->border_color_active;
        text_c = window->style->text_color_active;
        window->winflgs |= XTK_WINDOW_ACTIVE;
    } else {
        back = window->style->background_color_inactive;
        border = window->style->border_color_inactive;
        text_c = window->style->text_color_inactive;
        window->winflgs |= XTK_WINDOW_ACTIVE;
    }
    xtk_spirit_t *win_spirit = &window->window_spirit;
    xtk_spirit_t *spirit = &window->spirit;
    assert(win_spirit->surface);
    assert(spirit);

    win_spirit->style.background_color = back;
    spirit->style.background_color = window->style->background_color_active;
    int border_thick = window->style->border_thick;
    
    // 重绘内容区域
    if (win_back) {
        xtk_surface_rectfill(win_spirit->surface, border_thick, border_thick, 
            win_spirit->width - border_thick * 2, win_spirit->height - border_thick * 2, 
            spirit->style.background_color);
    }

    int navigation_bottom = border_thick + window->style->navigation_height;

    // 绘制导航栏
    xtk_surface_rectfill(win_spirit->surface, border_thick, border_thick, 
        win_spirit->width - border_thick * 2, window->style->navigation_height, back);
    xtk_surface_rectfill(win_spirit->surface, border_thick, 
        navigation_bottom - border_thick,
        win_spirit->width - border_thick * 2, border_thick, border);
    
    /* 基础边框 */
    xtk_surface_rectfill(win_spirit->surface, 0, 0, win_spirit->width, border_thick, border);
    xtk_surface_rectfill(win_spirit->surface, 0, win_spirit->height - border_thick, win_spirit->width,
        border_thick, border);
    xtk_surface_rectfill(win_spirit->surface, 0, 0, border_thick, win_spirit->height, border);
    xtk_surface_rectfill(win_spirit->surface, win_spirit->width - border_thick, 0, 
        border_thick, win_spirit->height, border);

    /* TODO: 修改按钮，标题，图标颜色 */
    xtk_window_navigation_t *navigation = &window->navigation;
    // 有才设置颜色
    if (navigation->title)
        navigation->title->style.color = text_c;
    
    xtk_container_t *container = win_spirit->container;
    assert(container);
    xtk_spirit_t *navigation_spirit;
    xtk_button_t *btn;
    list_for_each_owner (navigation_spirit, &container->children_list, list) {
        if (navigation_spirit->type == XTK_SPIRIT_TYPE_BUTTON) {
            btn = XTK_BUTTON(navigation_spirit);
            btn->color_idle = back;
            btn->color_touch = XTK_RGB_SUB(back, 0x40, 0x40, 0x40);
            btn->color_click = XTK_RGB_SUB(back, 0x20, 0x20, 0x20);
            navigation_spirit->style.background_color = btn->color_idle;
        } else if (navigation_spirit->type == XTK_SPIRIT_TYPE_LABEL) {
            navigation_spirit->style.background_color = back;
        }
    }

    // 刷新边框到屏幕上
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, win_spirit->surface->w, win_spirit->surface->h,
        (uview_color_t *) win_spirit->surface->pixels);
    uview_bitblt(win_spirit->view, 0, 0, &bmp);
    uview_update(win_spirit->view, 0, 0, win_spirit->width, win_spirit->height);

    // TODO: 刷新所有精灵
    xtk_spirit_show_children(win_spirit);

    //if (win_back) {
    // 刷新content surface内容
    uview_bitmap_init(&bmp, spirit->surface->w, spirit->surface->h,
        (uview_color_t *) spirit->surface->pixels);
    uview_bitblt_update(spirit->view, spirit->x,
        spirit->y, &bmp);
    xtk_spirit_show_children(spirit);
    //}
    return 0;
}

/**
 * 简单绘制没有边框的窗口
 */
int xtk_window_draw_no_border(xtk_window_t *window)
{
    if (!window)
        return -1;
    uview_color_t back = window->style->background_color_active;
    xtk_spirit_t *spirit = &window->spirit;
    assert(spirit->surface);

    // 绘制内容
    xtk_surface_clear(spirit->surface);
    xtk_surface_rectfill(spirit->surface, 0, 0, spirit->width, spirit->height, back);

    // 刷新到屏幕上
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, spirit->surface->w, spirit->surface->h, (uview_color_t *) spirit->surface->pixels);
    uview_bitblt(spirit->view, 0, 0, &bmp);
    uview_update(spirit->view, 0, 0, spirit->width, spirit->height);
    return 0;
}

static bool xtk_window_close_button_event(xtk_spirit_t *spirit, void *data)
{
    xtk_window_t *window = (xtk_window_t *) data;
    xtk_spirit_destroy(&window->spirit);
    return true;
}

static bool xtk_window_minim_button_event(xtk_spirit_t *spirit, void *data)
{
    xtk_window_t *window = (xtk_window_t *) data;
    xtk_window_minim(window);
    return true;
}

static bool xtk_window_maxim_button_event(xtk_spirit_t *spirit, void *data)
{
    xtk_window_t *window = (xtk_window_t *) data;
    /* 根据窗口当前状态进行状态变换，如果没有处于窗口最大化就进行窗口最大化
        如果已经是最大化就恢复原来的窗口大小
    */
    xtk_window_maxim2(window);
    return true;
}

static int xtk_window_create_navigation(xtk_window_t *window)
{
    xtk_window_navigation_t *navigation = &window->navigation;
    navigation->title = NULL;
    xtk_spirit_t *window_spirit = &window->window_spirit;

    xtk_spirit_t *spirit_minim = xtk_button_create_with_label("-");
    assert(spirit_minim);
    xtk_spirit_t *spirit_maxim = xtk_button_create_with_label("O");
    assert(spirit_maxim);
    xtk_spirit_t *spirit_close = xtk_button_create_with_label("X");
    assert(spirit_close);

    int x = window->style->border_thick;
    int y = window->style->border_thick + window->style->navigation_height / 2;
    xtk_spirit_set_pos(spirit_close, x, y - spirit_close->height / 2);
    
    x += spirit_close->width;
    xtk_spirit_set_pos(spirit_minim, x, y - spirit_minim->height / 2);
    
    x += spirit_minim->width;
    xtk_spirit_set_pos(spirit_maxim, x, y - spirit_maxim->height / 2);
    
    xtk_container_add(XTK_CONTAINER(window_spirit), spirit_close);
    xtk_container_add(XTK_CONTAINER(window_spirit), spirit_minim);
    xtk_container_add(XTK_CONTAINER(window_spirit), spirit_maxim);
    
    // 设置按钮信号事件
    xtk_signal_connect(spirit_close, "button_release", XTK_CALLBACK(xtk_window_close_button_event), window);
    xtk_signal_connect(spirit_minim, "button_release", XTK_CALLBACK(xtk_window_minim_button_event), window);
    xtk_signal_connect(spirit_maxim, "button_release", XTK_CALLBACK(xtk_window_maxim_button_event), window);
    return 0;
}

static int xtk_window_navigation_button_width(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_container_t *container = window->window_spirit.container;
    if (!container)
        return -1;
    xtk_spirit_t *spirit;
    int width = 0;
    list_for_each_owner (spirit, &container->children_list, list) {
        if (spirit->type == XTK_SPIRIT_TYPE_BUTTON) {
            width += spirit->width;
        }
    }
    return width;
}

static int xtk_window_destroy_navigation(xtk_window_t *window)
{
    return xtk_container_remove_and_destroy_all(window->window_spirit.container);    
} 

int xtk_window_show(xtk_window_t *window)
{
    if (!window)
        return -1;
    uview_show(window->spirit.view);
    return 0;
}

int xtk_window_spirit_setup(xtk_window_t *window, xtk_spirit_t *spirit, int x, int y, int width, int height)
{
    xtk_spirit_init(spirit, x, y, width, height);
    xtk_spirit_set_type(spirit, XTK_SPIRIT_TYPE_WINDOW);
    spirit->style.align = XTK_ALIGN_CENTER;
    spirit->style.background_color = window->style->background_color_inactive;
    
    xtk_container_t *container = xtk_container_create(spirit);
    if (!container) {
        return -1;
    }
    xtk_spirit_set_container(spirit, container);
    
    xtk_surface_t *surface = xtk_surface_create(width, height);
    if (!surface) {
        xtk_spirit_set_container(spirit, NULL);
        return -1;
    }
    xtk_surface_clear(surface);
    xtk_spirit_set_surface(spirit, surface);
    return 0;
}

int xtk_window_spirit_setdown(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    xtk_spirit_set_surface(spirit, NULL);
    xtk_spirit_set_container(spirit, NULL);
    xtk_spirit_cleanup(spirit);
    return 0;
}

int xtk_window_view_setup(xtk_window_t *window, int x, int y, int width, int height)
{
    // 创建视图
    int view = uview_open(width, height);
    if (view < 0) {
        return -1;
    }
    uview_set_type(view, UVIEW_TYPE_WINDOW);
    uview_set_pos(view, x, y);
    // 绑定视图
    xtk_spirit_set_view(&window->spirit, view);
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        xtk_spirit_set_view(&window->window_spirit, view);
    }
    
    xtk_view_t *pview = xtk_view_create();
    assert(pview);
    pview->view = view;
    list_add(&window->spirit.list, &pview->spirit_list_head);    
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        list_add(&window->window_spirit.list, &pview->spirit_list_head);
    }
    xtk_view_add(pview);
    pview->spirit = &window->spirit;
    return 0;
}

int xtk_window_view_setdown(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_view_t *pview = xtk_view_find(window->spirit.view);
    if (!pview)
        return -1;
    list_del(&window->spirit.list);
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        list_del(&window->window_spirit.list);
    }
    pview->spirit = NULL;
    xtk_view_remove(pview);
    uview_close(pview->view);
    xtk_view_destroy(pview);
    return 0;
}

static xtk_spirit_t *xtk_window_create_toplevel(xtk_window_t *window)
{
    int width = window->content_width;
    int height = window->content_height;
    
    // 初始化精灵
    int new_width = 0, new_height = 0;
    xtk_window_calc_view_size(window, width, height, &new_width, &new_height);

    xtk_spirit_t *window_spirit = &window->window_spirit;
    xtk_window_spirit_setup(window, window_spirit, 0, 0, new_width, new_height);
    
    xtk_spirit_t *spirit = &window->spirit;
    if (xtk_window_spirit_setup(window, spirit, window->style->border_thick, 
        window->style->border_thick + window->style->navigation_height,
        width, height) < 0)
    {
        xtk_window_spirit_setdown(window_spirit);
        return NULL;
    }
    // 创建导航栏
    if (xtk_window_create_navigation(window) < 0) {
        xtk_window_spirit_setdown(window_spirit);
        xtk_window_spirit_setdown(spirit);
        return NULL;
    }

    if (xtk_window_view_setup(window, 0, 0, new_width, new_height) < 0) {
        xtk_window_destroy_navigation(window);
        xtk_window_spirit_setdown(window_spirit);
        xtk_window_spirit_setdown(spirit);
        return NULL;
    }
    xtk_window_reset_mobile_area(window);
    // 窗口需要绘制
    xtk_window_draw_border(window, 1, 1);
    return spirit;
}

static xtk_spirit_t *xtk_window_create_popup(xtk_window_t *window)
{
    int width = window->content_width;
    int height = window->content_height;

    xtk_spirit_t *spirit = &window->spirit;
    if (xtk_window_spirit_setup(window, spirit, 0, 0, width, height) < 0) {
        return NULL;
    }
    if (xtk_window_view_setup(window, 0, 0, width, height) < 0) {
        xtk_window_view_setdown(window);
        return NULL;
    }
    xtk_window_reset_mobile_area(window);
    return spirit;
}

xtk_spirit_t *xtk_window_create(xtk_window_type_t type)
{
    xtk_window_t *window = malloc(sizeof(xtk_window_t));
    if (!window)
        return NULL;
    memset(window, 0, sizeof(xtk_window_t));
    window->type = type;
    window->routine = NULL;
    window->paint_callback = NULL;
    window->style = &__xtk_window_style_defult;
    window->winflgs = XTK_WINDOW_RESIZABLE;
    window->content_width = XTK_WINDOW_WIDTH_DEFAULT;
    window->content_height = XTK_WINDOW_HEIGHT_DEFAULT;
    window->extension = NULL;
    xtk_surface_init(&window->mmap_surface, 0, 0, NULL);
    xtk_rect_init(&window->invalid_rect, 0, 0, 0, 0);
    xtk_rect_init(&window->backup_win_info, 0, 0, 0, 0);
    list_init(&window->timer_list_head);
    xtk_spirit_t *spirit = NULL;
    if (type == XTK_WINDOW_TOPLEVEL) {
        spirit = xtk_window_create_toplevel(window);
    } else if (type == XTK_WINDOW_POPUP) {
        spirit = xtk_window_create_popup(window);
    }
    if (!spirit) {
        free(window);
    } else {
        // window
        assert(!xtk_signal_create(spirit, "delete_event"));
        assert(!xtk_signal_create(spirit, "destroy_event"));
        assert(!xtk_signal_create(spirit, "active"));
        assert(!xtk_signal_create(spirit, "inactive"));
        assert(!xtk_signal_create(spirit, "move_event"));
        assert(!xtk_signal_create(spirit, "maxim_notify"));
        assert(!xtk_signal_create(spirit, "minim_notify"));
        assert(!xtk_signal_create(spirit, "restore_notify"));
        assert(!xtk_signal_create(spirit, "resize_event"));
        
        // mouse
        assert(!xtk_signal_create(spirit, "button_press"));
        assert(!xtk_signal_create(spirit, "button_release"));
        assert(!xtk_signal_create(spirit, "motion_notify"));        
        assert(!xtk_signal_create(spirit, "button_scroll"));
        assert(!xtk_signal_create(spirit, "enter_notify"));
        assert(!xtk_signal_create(spirit, "leave_notify"));

        // keyboard
        assert(!xtk_signal_create(spirit, "key_press"));
        assert(!xtk_signal_create(spirit, "key_release"));
        
    }
    return spirit;
}

xtk_spirit_t *xtk_window_create_simple(char *title, int x, int y, int width, int height)
{
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_TOPLEVEL);
    if (!spirit)
        return NULL;
    assert(!xtk_window_set_title(XTK_WINDOW(spirit), title));
    assert(!xtk_window_set_default_size(XTK_WINDOW(spirit), width, height));
    assert(!xtk_window_set_position_absolute(XTK_WINDOW(spirit), x, y));
    return spirit;
}

int xtk_window_invalid_rect(xtk_window_t *window, xtk_rect_t *rect)
{
    if (!window)
        return -1;
    if (xtk_rect_valid(&window->invalid_rect)) {
        xtk_rect_merge(&window->invalid_rect, rect);
    } else {
        xtk_rect_init(&window->invalid_rect, rect->x, rect->y, rect->w, rect->h);
    }
    return 0;
}

int xtk_window_invalid_window(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->spirit;
    xtk_rect_init(&window->invalid_rect, 0, 0, spirit->width, spirit->height);
    return 0;
}

int xtk_window_get_invalid(xtk_window_t *window, xtk_rect_t *rect)
{
    if (!window || !rect)
        return -1;
    if (xtk_rect_valid(&window->invalid_rect)) {
        *rect = window->invalid_rect;
        xtk_rect_init(&window->invalid_rect, 0, 0, 0, 0);
        return 0;
    }
    return -1;
}

int xtk_window_paint(xtk_window_t *window)
{
    if (!window)
        return -1;
    if (xtk_rect_valid(&window->invalid_rect)) {
        int vid = -1;
        if (!uview_get_vid(window->spirit.view, &vid)) {
            uview_msg_t msg;
            uview_msg_header(&msg, UVIEW_MSG_PAINT, vid);
            uview_send_msg(window->spirit.view, &msg);
        }
    }
    return 0;
}

int xtk_window_paint_callback(xtk_window_t *window, xtk_win_paint_callback_t callback)
{
    if (!window)
        return -1;
    window->paint_callback = callback;
    return 0;
}

extern int __xtk_has_window_close;

int xtk_window_destroy(xtk_window_t *window)
{
    if (!window)
        return -1;
    
    /* 销毁资源 */
    xtk_window_destroy_timer_all(window);
    xtk_window_munmap(window);

    if (xtk_window_view_setdown(window) < 0)
        return -1;
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        xtk_window_destroy_navigation(window);
        xtk_window_spirit_setdown(&window->window_spirit);
    }
    xtk_signal_destroy_all(&window->spirit);
    xtk_window_spirit_setdown(&window->spirit);
    free(window);
    // 检查到有窗口销毁
    __xtk_has_window_close = 1;
    return 0;
}

int xtk_window_set_title(xtk_window_t *window, char *title)
{
    if (!window || !title)
        return -1;
    
    if (window->type != XTK_WINDOW_TOPLEVEL)
        return -1;

    xtk_window_navigation_t *navigation = &window->navigation;
    if (!navigation->title) {
        navigation->title = xtk_label_create(title);
        if (!navigation->title)
            return -1;
        navigation->title->style.background_color = XTK_NONE_COLOR;
        navigation->title->style.align = XTK_ALIGN_LEFT;
        // 第一次创建需要添加到容器中
        xtk_container_add(XTK_CONTAINER(&window->window_spirit), navigation->title);
    } else {
        /* 背景颜色和容器颜色一致 */
        navigation->title->style.background_color = \
            (window->winflgs & XTK_WINDOW_ACTIVE) ? window->style->background_color_active : \
            window->style->background_color_inactive;

        // 隐藏原来的数据
        xtk_spirit_hide(navigation->title);
        xtk_label_set_text(navigation->title, title);
    }
    // 调整位置
    int x = window->style->border_thick * 2 + xtk_window_navigation_button_width(window);
    int y = window->style->border_thick + window->style->navigation_height / 2  - \
        navigation->title->height / 2;
    xtk_spirit_set_pos(navigation->title, x, y);
    // 显示精灵
    xtk_spirit_show(navigation->title);
    return 0;
}

int xtk_window_set_resizable(xtk_window_t *window, bool resizable)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->window_spirit;
    if (resizable) {
        window->winflgs |= XTK_WINDOW_RESIZABLE;
        uview_set_resizable(spirit->view);
    } else {
        window->winflgs &= ~XTK_WINDOW_RESIZABLE;
        uview_set_unresizable(spirit->view);
    }
    return 0;
}

int xtk_window_reset_mobile_area(xtk_window_t *window)
{
    if (!window)
        return -1;
    int left, top, right, bottom; 
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        left = window->style->border_thick;
        left += xtk_window_navigation_button_width(window); // 加上按钮占用的长度

        top = window->style->border_thick;
        right = window->window_spirit.width - window->style->border_thick;
        bottom = window->style->border_thick + window->style->navigation_height;
    } else {
        left = window->spirit.x;
        top = window->spirit.y;
        right = window->spirit.width;
        bottom = window->spirit.height;
    }
    uview_set_drag_region(window->spirit.view, left, top, right, bottom);
    return 0;
}

xtk_surface_t *xtk_window_get_surface(xtk_window_t *window)
{
    if (!window)
        return NULL;
    return window->spirit.surface;
}

xtk_surface_t *xtk_window_get_mmap_surface(xtk_window_t *window)
{
    if (!window)
        return NULL;
    if (!window->mmap_surface.pixels || !window->mmap_surface.w || !window->mmap_surface.h)
        return NULL;
    assert(window->mmap_surface.pixels);
    assert(window->mmap_surface.w);
    assert(window->mmap_surface.h);
    return &window->mmap_surface;
}

int xtk_window_flip(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->spirit;
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, spirit->surface->w, spirit->surface->h,
        spirit->surface->pixels);
    return uview_bitblt_update(spirit->view, spirit->x, spirit->y,
            &bmp);
}

int xtk_window_update(xtk_window_t *window, int x, int y, int w, int h)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->spirit;
    
    if (x >= spirit->width)
        return 0;
    if (y >= spirit->height)
        return 0;
    
    int ex = x + w, ey = y + h;
    if (ex > spirit->width)
        ex = spirit->width;
    if (ey > spirit->height)
        ey = spirit->height;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, spirit->surface->w, spirit->surface->h,
        spirit->surface->pixels);
    return uview_bitblt_update_ex(spirit->view, spirit->x + x, spirit->y + y,
            &bmp, x, y, ex - x, ey - y);
}

int xtk_window_refresh(xtk_window_t *window, int x, int y, int w, int h)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->spirit;
    
    if (x >= spirit->width)
        return 0;
    if (y >= spirit->height)
        return 0;
    int left = spirit->x + x;
    int top = spirit->y + y;
    int right = left + w;
    int buttom = top + h;
    return uview_update(spirit->view, left, top, right, buttom);
}

int xtk_window_set_position(xtk_window_t *window, xtk_window_position_t pos)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit = &window->window_spirit;
    if (spirit->view < 0)
        return -1;
    switch (pos) {
    case XTK_WIN_POS_NONE:
        {
            int vx = 0, vy = 0;
            uview_get_lastpos(spirit->view, &vx, &vy);
            uview_set_pos(spirit->view, vx, vy);
        }
        break;
    case XTK_WIN_POS_CENTER_ALWAYS:
        uview_set_unmoveable(spirit->view);
    case XTK_WIN_POS_CENTER:
        {
            int w = 0, h = 0;
            uview_get_screensize(spirit->view, &w, &h);
            uview_set_pos(spirit->view, w / 2 - spirit->width / 2,
                h / 2 - spirit->height / 2);
        }
        break;    
    case XTK_WIN_POS_MOUSE:
        {
            int mx = 0, my = 0;
            uview_get_mousepos(spirit->view, &mx, &my);
            uview_set_pos(spirit->view, mx, my);
        }
        break;  
    default:
        break;
    }
    return 0;
}

int xtk_window_set_default_size(xtk_window_t *window, int width, int height)
{
    if (!window)
        return -1;
    int vwidth, vheight; // 视图大小
    xtk_window_calc_view_size(window, width, height, &vwidth, &vheight);
    uview_repair_size(&vwidth, &vheight);
    xtk_window_calc_content_size(window, vwidth, vheight, &width, &height);
    
    // 调整精灵的大小
    if (xtk_window_resize(window, width, height) < 0)
        return -1;
    if (xtk_window_change_size(window, vwidth, vheight) < 0)
        return -1;
    return 0;
}

/**
 * 设置消息例程，可以让用户自己处理消息。
 * 消息处理函数如下：
 * typedef void (*xtk_window_routine_t) (xtk_spirit_t *, uview_msg_t *);
 * @parma:      window：要设置消息例程的窗口
 * @routine:    消息例程函数指针
 * @return:     成功返回0，失败返回-1
 */
int xtk_window_set_routine(xtk_window_t *window, xtk_window_routine_t routine)
{
    if (!window)
        return -1;
    window->routine = routine;
    return 0;
}

int xtk_window_set_active(xtk_window_t *window, bool is_active)
{
    if (!window)
        return -1;
    xtk_window_draw_border(window, is_active, 0);
    return 0;
}

int xtk_window_set_fixed(xtk_window_t *window, bool fiexed)
{
    if (!window)
        return -1;
    if (fiexed) {
        if (window->type == XTK_WINDOW_POPUP) { // popup类型才可以设置成固定
            uview_set_type(window->spirit.view, UVIEW_TYPE_FIXED);
        }
        uview_set_unmoveable(window->spirit.view);
        // uview_set_unresizable(window->spirit.view);
    } else {
        if (window->type == XTK_WINDOW_POPUP) { // popup类型才需要还原
            uview_set_type(window->spirit.view, UVIEW_TYPE_WINDOW);
        }
        uview_set_moveable(window->spirit.view);
    }
    return 0;
}

int xtk_window_set_size_minim(xtk_window_t *window, int width, int height)
{
    if (!window)
        return -1;
    int vwidth = 0, vheight = 0;
    xtk_window_calc_view_size(window, width, height, &vwidth, &vheight);
    // size是视图大小，需要计算
    uview_set_size_min(window->spirit.view, vwidth, vheight);
    return 0;
}

int xtk_window_get_screen(xtk_window_t *window, int *width, int *height)
{
    if (!window)
        return -1;
    return uview_get_screensize(window->spirit.view, width, height);
}

int xtk_window_resize_to_screen(xtk_window_t *window)
{
    if (!window)
        return -1;
    int width = 0;
    int height = 0;
    if (xtk_window_get_screen(window, &width, &height) < 0) {
        printf("%s: get screen size failed!\n", __func__);
        return -1;
    }
    if (!width || !height) {
        printf("%s: get screen size error!\n", __func__);
        return -1;
    }
    return xtk_window_set_default_size(window, width, height);
}

int xtk_window_load_mouse_cursors(xtk_window_t *window, char *pathname)
{
    if (!window)
        return -1;
    if (window->spirit.view < 0)
        return -1;
    xtk_mouse_load_cursors(window->spirit.view, pathname);    
    return 0;
}

int xtk_window_set_position_absolute(xtk_window_t *window, int x, int y)
{
    if (!window)
        return -1;
    if (window->spirit.type == XTK_SPIRIT_TYPE_WINDOW) {
        uview_set_pos(window->spirit.view, x, y);
        return 0;
    }
    return -1;
}

int xtk_window_get_position(xtk_window_t *window, int *x, int *y)
{
    if (!window)
        return -1;
    if (window->spirit.view < 0)
        return -1;
    return uview_get_pos(window->spirit.view, x, y);
}

/**
 * maxim window
 */
int xtk_window_maxim(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_rect_t info_rect;
    if (window->winflgs & XTK_WINDOW_MAXIM) {
        return -1;
    }
    int wx = 0, wy = 0;
    xtk_window_get_position(window, &wx, &wy);
    // 备份原来的大小和位置
    xtk_rect_init(&window->backup_win_info, wx, wy,
        window->window_spirit.width, window->window_spirit.height);

    // 设置要调整成的大小和位置
    xtk_window_get_screen(window, (int *) &info_rect.w, (int *) &info_rect.h);
    xtk_rect_init(&info_rect, 0, 0, info_rect.w, info_rect.h);
    
    window->winflgs |= XTK_WINDOW_MAXIM;
    if (window->winflgs & XTK_WINDOW_RESIZABLE) {
        // 小窗口是可以调整大小的，就要禁用大小调整
        xtk_window_set_resizable(window, false);
        window->winflgs |= XTK_WINDOW_DISABLERESIZE; // 记录禁止调整大小标志
    }
    // 需要将屏幕（视图）大小转换成窗口内容大小    
    xtk_window_calc_content_size(window, info_rect.w, info_rect.h, 
        (int *) &info_rect.w, (int *) &info_rect.h);
    // 重新设置和大小
    xtk_window_resize(window, info_rect.w, info_rect.h);
    xtk_window_set_position_absolute(window, info_rect.x, info_rect.y);
    xtk_signal_emit_by_name(&window->spirit, "maxim_notify");    
    return 0;
}

/**
 * maxim window when no maxim
 * restore window when maxim
 */
int xtk_window_maxim2(xtk_window_t *window)
{
    if (!window)
        return -1;
    if ((window->winflgs & XTK_WINDOW_MAXIM)) {    /* 不是最大化状态就返回错误 */
        return xtk_window_restore(window);
    } else {
        return xtk_window_maxim(window);
    }
}

/**
 * minim window
 */
int xtk_window_minim(xtk_window_t *window)
{
    if (!window)
        return -1;
    if (xtk_spirit_hide(&window->spirit) < 0)
        return -1;
    xtk_signal_emit_by_name(&window->spirit, "minim_notify");    
    return 0;
}

/**
 * restore window size after maxim
 */
int xtk_window_restore(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_rect_t info_rect;
    if (!(window->winflgs & XTK_WINDOW_MAXIM)) {    /* 不是最大化状态就返回错误 */
        return -1;
    }
    // 从备份的信息恢复原来的大小和位置
    info_rect = window->backup_win_info;
    window->winflgs &= ~XTK_WINDOW_MAXIM;

    if (window->winflgs & XTK_WINDOW_DISABLERESIZE) {
        xtk_window_set_resizable(window, true);
        window->winflgs &= ~XTK_WINDOW_DISABLERESIZE; // 清除禁止调整大小标志
    }
    
    // 需要将屏幕（视图）大小转换成窗口内容大小    
    xtk_window_calc_content_size(window, info_rect.w, info_rect.h, 
        (int *) &info_rect.w, (int *) &info_rect.h);
    // 重新设置和大小
    xtk_window_resize(window, info_rect.w, info_rect.h);
    xtk_window_set_position_absolute(window, info_rect.x, info_rect.y);    
    xtk_signal_emit_by_name(&window->spirit, "restore_notify");    
    return 0;
}

uint32_t xtk_window_add_timer(xtk_window_t *window, uint32_t interval,
        xtk_timer_callback_t function, void *data)
{
    if (!window)
        return 0;
    xtk_timer_t *timer = xtk_timer_create(interval, function, data);
    if (!timer)
        return 0;
    int timer_id = uview_add_timer(window->spirit.view, interval);
    if (timer_id < 0) {
        xtk_timer_destroy(timer);
        return 0;
    }
    timer->timer_id = timer_id;
    list_add(&timer->list, &window->timer_list_head);
    return timer->timer_id;
}

int xtk_window_remove_timer(xtk_window_t *window, uint32_t timer_id)
{
    if (!window)
        return -1;
    xtk_timer_t *timer = xtk_window_find_timer(window, timer_id);
    if (!timer)
        return -1;
    list_del_init(&timer->list);
    uview_del_timer(window->spirit.view, timer->timer_id);
    xtk_timer_destroy(timer);
    return 0;
}

int xtk_window_restart_timer(xtk_window_t *window, uint32_t timer_id, int interval)
{
    if (!window)
        return -1;
    xtk_timer_t *timer = xtk_window_find_timer(window, timer_id);
    if (!timer)
        return -1;
    if (interval <= 0)
        interval = timer->interval;
    return uview_restart_timer(window->spirit.view, timer_id, interval);
}

static int xtk_window_destroy_timer_all(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_timer_t *timer, *timer_next;
    list_for_each_owner_safe (timer, timer_next, &window->timer_list_head, list) {
        list_del_init(&timer->list);
        uview_del_timer(window->spirit.view, timer->timer_id);
        xtk_timer_destroy(timer);
    }
    return 0;
}

static xtk_timer_t *xtk_window_find_timer(xtk_window_t *window, uint32_t timer_id)
{
    if (!window)
        return NULL;
    xtk_timer_t *timer;
    list_for_each_owner (timer, &window->timer_list_head, list) {
        if (timer->timer_id == timer_id) {
            return timer; 
        }
    }
    return NULL;
}

int xtk_window_mmap(xtk_window_t *window)
{
    if (!window)
        return -1;
    xtk_spirit_t *spirit;
    if (window->type == XTK_WINDOW_TOPLEVEL) {
        spirit = &window->window_spirit;
    } else {
        spirit = &window->spirit;
    }
    size_t view_size = spirit->width * spirit->height * sizeof(xtk_color_t);
    void *addr = xmmap(window->spirit.view, view_size, 0);
    if (addr == (void *) -1)
        return -1;
    xtk_surface_init(&window->mmap_surface, spirit->width, spirit->height, addr);
    return 0;
}

int xtk_window_munmap(xtk_window_t *window)
{
    if (!window)
        return -1;
    
    if (!window->mmap_surface.pixels)
        return -1;
    if (!xmunmap(window->mmap_surface.pixels, window->mmap_surface.w * 
        window->mmap_surface.h * sizeof(xtk_color_t)))
        xtk_surface_init(&window->mmap_surface, 0, 0, NULL);
    return 0;
}

int xtk_window_mmap_resize(xtk_window_t *window, int width, int height)
{
    if (!window)
        return -1;
    if (!window->mmap_surface.pixels) //没有映射过就返回0，不算做失败，只是不做后面的处理而已
        return 0;
    // 重新映射
    size_t view_size = width * height * sizeof(xtk_color_t);
    void *addr = xmmap(window->spirit.view, view_size, 0);
    if (addr == (void *) -1)
        return -1;
    // 取消之前的映射
    if (xtk_window_munmap(window) < 0) {
        xmunmap(addr, view_size);
        return -1;
    }
    xtk_surface_init(&window->mmap_surface, width, height, addr);
    return 0;
}
