#include "xtk.h"
#include <uview.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <dotfont.h>

extern dotfont_library_t __xtk_dotflib;

void xtk_spirit_init(xtk_spirit_t *spirit, int x, int y, int width, int height)
{
    list_init(&spirit->list);
    list_init(&spirit->signal_list);
    spirit->type = XTK_SPIRIT_TYPE_UNKNOWN;
    spirit->x = x;
    spirit->y = y;
    spirit->width = width;
    spirit->height = height;
    spirit->width_min = 0;
    spirit->height_min = 0;
    spirit->visible = 0;
    spirit->max_text_len = -1;  /* no limit */
    spirit->invisible_char = 0; /* no invisible char */
    memset(&spirit->style, 0, sizeof(xtk_style_t));

    spirit->style.border_color = XTK_NONE_COLOR;
    spirit->style.color = XTK_BLACK;
    spirit->style.align = XTK_ALIGN_LEFT;
    spirit->text = NULL;
    spirit->image = NULL;
    spirit->surface = NULL;

    spirit->style.background_color = XTK_NONE_COLOR;
    spirit->style.background_align = XTK_ALIGN_LEFT;
    spirit->background_image = NULL;

    spirit->style.cursor = XTK_CURSOR_NORMAL;   // 默认光标

    spirit->container = NULL;
    spirit->attached_container = NULL;
    spirit->view = -1;

    spirit->show_bottom = NULL;
    spirit->show_middle = NULL;
    
}

xtk_spirit_t *xtk_spirit_create(int x, int y, int width, int height)
{
    xtk_spirit_t *spirit = malloc(sizeof(xtk_spirit_t));
    if (!spirit)
        return NULL;
    xtk_spirit_init(spirit, x, y, width, height);
    return spirit;
}

int xtk_spirit_cleanup(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    if (spirit->background_image) {
        xtk_image_destroy(spirit->background_image);
        spirit->background_image = NULL;
    }
    if (spirit->image) {
        xtk_image_destroy(spirit->image);
        spirit->image = NULL;
    }
    if (spirit->surface) {
        xtk_surface_destroy(spirit->surface);
        spirit->surface = NULL;
    }
    if (spirit->text) {
        free(spirit->text);
        spirit->text = NULL;
    }
    xtk_signal_destroy_all(spirit);
    return 0;
}

int xtk_spirit_destroy(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    
    /* 根据不同的类型进行销毁 */
    switch (spirit->type) {
    case XTK_SPIRIT_TYPE_WINDOW:
        {
            if (xtk_check_main_loop()) { // 处于main loop中才触发事件
                // 触发删除事件
                if (xtk_signal_emit_by_name(spirit, "delete_event"))
                    return 0;
                // 再触发销毁事件
                xtk_signal_emit_by_name(spirit, "destroy_event");
            }
            return xtk_window_destroy(XTK_WINDOW(spirit));        
        }
    default:
        break;
    }
    if (xtk_spirit_cleanup(spirit) < 0)
        return -1;
    free(spirit);
    return 0;
}

int xtk_spirit_destroy_all(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp, *next;
    list_for_each_owner_safe (tmp, next, &container->children_list, list) {    
        xtk_spirit_destroy(tmp);
    }
    xtk_spirit_destroy(spirit);
    return 0;
}

int xtk_spirit_set_pos(xtk_spirit_t *spirit, int x, int y)
{
    if (!spirit)
        return -1;
    spirit->x = x;
    spirit->y = y;
    return 0;
}

int xtk_spirit_set_type(xtk_spirit_t *spirit, xtk_spirit_type_t type)
{
    if (!spirit)
        return -1;
    spirit->type = type;
    return 0;
}

int xtk_spirit_set_view(xtk_spirit_t *spirit, int view)
{
    if (!spirit)
        return -1;
    spirit->view = view;
    return 0;
}

int xtk_spirit_set_size(xtk_spirit_t *spirit, int width, int height)
{
    if (!spirit)
        return -1;
    spirit->width = width;
    spirit->height = height;
    return 0;
}

int xtk_spirit_set_size_request(xtk_spirit_t *spirit, int width, int height)
{
    if (!spirit)
        return -1;
    spirit->width_min = width;
    spirit->height_min = height;
    // 根据不同的精灵类型做出不同的行为操作
    switch (spirit->type)
    {
    case XTK_SPIRIT_TYPE_WINDOW:
        xtk_window_set_size_minim(XTK_WINDOW(spirit), width, height);
        break;    
    default:
        break;
    }
    return 0;
}

int xtk_spirit_set_text(xtk_spirit_t *spirit, char *text)
{
    if (!spirit)
        return -1;
    int new_len = strlen(text);
    /* 设置文本长度 */
    if (spirit->max_text_len > 0 && spirit->max_text_len < new_len)
        new_len = spirit->max_text_len;

    if (!spirit->text) {
        spirit->text = malloc(new_len + 1);
        memset(spirit->text, 0, new_len + 1);
    } else {
        int old_len = strlen(spirit->text);
        if (old_len < new_len) {
            spirit->text = realloc(spirit->text, new_len + 1);
            assert(spirit->text);
            memset(spirit->text, 0, new_len + 1);
        } else {
            memset(spirit->text, 0, old_len);
        }
    }
    assert(spirit->text);
    strcpy(spirit->text, text);
    spirit->text[strlen(spirit->text)] = '\0';
    return 0;
}

int xtk_spirit_set_text_max_len(xtk_spirit_t *spirit, int maxlen)
{
    if (!spirit || maxlen < 1)
        return -1;
    spirit->max_text_len = maxlen;
    if (spirit->text) {
        int new_len = strlen(spirit->text);
        /* 截断文本 */
        if (spirit->max_text_len > 0 && spirit->max_text_len < new_len) {
            new_len = spirit->max_text_len;
            spirit->text[new_len] = '\0';
        }
    }
    return 0;
}

int xtk_spirit_auto_size(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    if (spirit->text && spirit->style.color != XTK_NONE_COLOR) {
        int len = strlen(spirit->text);
        dotfont_t *dotfont = dotfont_find(&__xtk_dotflib, DOTF_STANDARD_NAME);
        assert(dotfont);
        int w = dotfont_get_char_width(dotfont) * len;
        int h = dotfont_get_char_height(dotfont);
        spirit->width = w + 4;
        if (h > spirit->height)
            spirit->height = h + 4;
    }
    return 0;
}

int xtk_spirit_set_background_image(xtk_spirit_t *spirit, char *filename)
{
    if (!spirit)
        return -1;
    if (filename == NULL) {
        if (spirit->background_image)
            xtk_image_destroy(spirit->background_image);
        spirit->background_image = NULL;
        return 0;
    }
    xtk_image_t *img = xtk_image_load(filename);
    if (!img)
        return -1;
    if (spirit->background_image) {
        xtk_image_destroy(spirit->background_image);
        spirit->background_image = NULL;
    }
    spirit->background_image = img;
    return 0;
}

int xtk_spirit_set_image(xtk_spirit_t *spirit, char *filename)
{
    if (!spirit)
        return -1;
    if (filename == NULL) {
        if (spirit->image)
            xtk_image_destroy(spirit->image);
        spirit->image = NULL;
        return 0;
    }
    xtk_image_t *img = xtk_image_load(filename);
    if (!img)
        return -1;
    if (spirit->image) {
        xtk_image_destroy(spirit->image);
        spirit->image = NULL;
    }
    spirit->image = img;
    return 0;
}

int xtk_spirit_set_image2(xtk_spirit_t *spirit, char *filename, int width, int height)
{
    if (!spirit)
        return -1;
    if (filename == NULL) {
        if (spirit->image)
            xtk_image_destroy(spirit->image);
        spirit->image = NULL;
        return 0;
    }
    if (width > spirit->width || width <= 0)
        width = spirit->width;
    if (height > spirit->height || height <= 0)
        height = spirit->height;
    xtk_image_t *img = xtk_image_load2(filename, width, height);
    if (!img)
        return -1;
    if (spirit->image) {
        xtk_image_destroy(spirit->image);
        spirit->image = NULL;
    }
    spirit->image = img;
    return 0;
}

int xtk_spirit_set_surface(xtk_spirit_t *spirit, xtk_surface_t *surface)
{
    if (!spirit)
        return -1;
    if (surface == NULL) {
        if (spirit->surface)
            xtk_surface_destroy(spirit->surface);
        spirit->surface = NULL;
        return 0;
    }
    if (spirit->surface) {
        xtk_surface_destroy(spirit->surface);
        spirit->surface = NULL;
    }
    spirit->surface = surface;
    return 0;
}

int xtk_spirit_reset_size(xtk_spirit_t *spirit, int width, int height)
{
    if (!spirit)
        return -1;
    // resize surface
    if (spirit->surface) {
        if (xtk_surface_resize(spirit->surface, width, height) < 0)
            return -1;
    }
    spirit->width = width;
    spirit->height = height;
    return 0;
}

int xtk_spirit_set_container(xtk_spirit_t *spirit, xtk_container_t *container)
{
    if (!spirit)
        return -1;
    if (container == NULL) {
        if (spirit->container)
            xtk_container_destroy(spirit->container);
        spirit->container = NULL;
        return 0;
    }
    if (spirit->container) {
        xtk_container_destroy(spirit->container);
        spirit->container = NULL;
    }
    spirit->container = container;
    return 0;
}

void xtk_aligin_calc_pos(xtk_style_t *style, xtk_align_t align, int box_width, int box_height, 
        int width, int height, int *out_x, int *out_y)
{
    int x, y;
    switch (align) {
    case XTK_ALIGN_LEFT:
        x = style->padding;
        y = box_height / 2 - height / 2;
        break;
    case XTK_ALIGN_RIGHT:
        x = box_width - width - style->padding;
        y = box_height / 2 - height / 2;
        break;
    case XTK_ALIGN_TOP:
        x = box_width / 2 - width / 2;
        y = style->padding;
        break;
    case XTK_ALIGN_BOTTOM:
        x = box_width / 2 - width / 2;
        y = box_height - height - style->padding;
        break;
    default:
        x = box_width / 2 - width / 2;
        y = box_height / 2 - height / 2;
        break;
    }
    *out_x = x;
    *out_y = y;
}

int xtk_spirit_calc_aligin_pos(xtk_spirit_t *spirit, int width, int height, int *out_x, int *out_y)
{
    if (!spirit)
        return -1;
    xtk_aligin_calc_pos(&spirit->style, spirit->style.align, spirit->width, spirit->height,
        width, height, out_x, out_y);
    return 0;
}

/* 将精灵渲染到bmp位图中 */
int xtk_spirit_to_surface(xtk_spirit_t *spirit, xtk_surface_t *surface)
{
    if (!spirit || !surface)
        return -1;

    int start_x = spirit->x;
    int start_y = spirit->y;

    int off_x = 0, off_y = 0;
    /* 背景 */
    if (spirit->style.background_color != XTK_NONE_COLOR) {
        xtk_surface_rectfill(surface, start_x, start_y, spirit->width, spirit->height,
            spirit->style.background_color);
    }
    if (spirit->background_image) {
        // 根据对齐方式设置显示位置
        xtk_aligin_calc_pos(&spirit->style, spirit->style.background_align, spirit->width, spirit->height, spirit->background_image->w,
            spirit->background_image->h, &off_x, &off_y);
        xtk_surface_t src_surface;
        xtk_surface_init(&src_surface, 
            (unsigned int) spirit->background_image->w,
            (unsigned int) spirit->background_image->h,
            (uview_color_t *) spirit->background_image->buf);

        xtk_rect_t srcrect = {0, 0, src_surface.w, src_surface.h};
        xtk_rect_t dstrect = {start_x + off_x, start_y + off_y, surface->w, surface->h};
        xtk_surface_blit(&src_surface, &srcrect, surface, &dstrect);
    }
    
    /* 前景 */
    if (spirit->surface) {
        // 根据对齐方式设置显示位置
        xtk_aligin_calc_pos(&spirit->style, spirit->style.align, spirit->width, spirit->height, 
            spirit->surface->w, spirit->surface->h, &off_x, &off_y);
        
        xtk_rect_t srcrect = {0, 0, spirit->surface->w, spirit->surface->h};
        xtk_rect_t dstrect = {start_x + off_x, start_y + off_y, surface->w, surface->h};
        xtk_surface_blit(spirit->surface, &srcrect, surface, &dstrect);
    }
    if (spirit->image) {
        // 根据对齐方式设置显示位置
        xtk_aligin_calc_pos(&spirit->style, spirit->style.align, spirit->width, spirit->height, spirit->image->w,
            spirit->image->h, &off_x, &off_y);
        
        xtk_surface_t src_surface;
        xtk_surface_init(&src_surface, 
            (unsigned int) spirit->image->w,
            (unsigned int) spirit->image->h,
            (uview_color_t *) spirit->image->buf);
        
        xtk_rect_t srcrect = {0, 0, src_surface.w, src_surface.h};
        xtk_rect_t dstrect = {start_x + off_x, start_y + off_y, surface->w, surface->h};
        xtk_surface_blit(&src_surface, &srcrect, surface, &dstrect);
    }
    
    if (spirit->show_middle)
        spirit->show_middle(spirit);

    if (spirit->text && spirit->style.color != XTK_NONE_COLOR) {
        dotfont_t *dotfont = dotfont_find(&__xtk_dotflib, DOTF_STANDARD_NAME);
        assert(dotfont);

        xtk_aligin_calc_pos(&spirit->style, spirit->style.align, spirit->width, spirit->height, 
            dotfont_get_char_width(dotfont) * strlen(spirit->text),
            dotfont_get_char_height(dotfont), &off_x, &off_y);
        
        /* 把文字渲染到surface，然后再拷贝到目的surface */
        if (!spirit->text_surface) {
            spirit->text_surface = xtk_surface_create(spirit->width, spirit->height);
        } else {
            xtk_surface_resize(spirit->text_surface, spirit->width, spirit->height);
        }
        if (spirit->text_surface) {
            uview_bitmap_t bmp;
            uview_bitmap_init(&bmp, surface->w, surface->h, (uview_color_t *) surface->pixels);
            if (spirit->invisible_char) {
                xtk_text_to_bitmap_ex(spirit->text, spirit->invisible_char, spirit->style.color,
                    DOTF_STANDARD_NAME, &bmp, start_x + off_x, start_y + off_y);
            } else {
                xtk_text_to_bitmap(spirit->text, spirit->style.color, DOTF_STANDARD_NAME,
                    &bmp, start_x + off_x, start_y + off_y);
            }
        }
    }

    /* 边框 */
    if (spirit->style.border_color != XTK_NONE_COLOR) {
        xtk_surface_rect(surface, start_x, start_y, spirit->width, spirit->height,
            spirit->style.border_color);
    }

    if (spirit->show_bottom)
        spirit->show_bottom(spirit);

    return 0;
}

void xtk_spirit_adjust_pos_by_type_all(xtk_spirit_t *spirit)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return;
    
    xtk_spirit_t *tmp;
    switch (spirit->type) {
    case XTK_SPIRIT_TYPE_WINDOW:
        {
            // 相对位置
            int x = 0;
            int y = 0;
            // 根据不容的容器规则，进行位置布局
            list_for_each_owner (tmp, &container->children_list, list) {
                xtk_spirit_set_pos(tmp, x, y);
                x += tmp->width;
            }
        }
        break;
    default:
        break;
    }
}

/**
 * 显示精灵
 */
int xtk_spirit_show(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    spirit->visible = 1;
    // 处理特殊精灵
    switch (spirit->type) {
    case XTK_SPIRIT_TYPE_WINDOW:
        uview_show(spirit->view);
        return 0;
    default:
        break;
    }
    // 默认情况
    if (!spirit->attached_container)
        return -1;
    xtk_spirit_t *attached_spirit = (xtk_spirit_t *)spirit->attached_container->spirit;
    if (!attached_spirit->surface)
        return -1;
    
    xtk_spirit_to_surface(spirit, attached_spirit->surface);
    
    if (UVIEW_BAD_ID(attached_spirit->view))
        return -1;
    
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, attached_spirit->surface->w, attached_spirit->surface->h, (uview_color_t *) attached_spirit->surface->pixels);
    if (uview_bitblt_update_ex(attached_spirit->view, attached_spirit->x + spirit->x,
        attached_spirit->y + spirit->y, &bmp, spirit->x, spirit->y, 
        spirit->width, spirit->height))
        return -1;
    return 0;
}

/**
 * 显示精灵下面的所有精灵
 */
int xtk_spirit_show_children(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {    
        xtk_spirit_show(tmp);
    }
    return 0;
}
/**
 * 显示精灵下面的所有精灵
 */
int xtk_spirit_show_all(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    if (xtk_spirit_show_children(spirit) < 0)
        return -1;
    if (xtk_spirit_show(spirit) < 0)
        return -1;
    return 0;
}

/**
 * 隐藏精灵
 */
int xtk_spirit_hide(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;

    spirit->visible = 0;
    // 处理特殊精灵
    switch (spirit->type) {
    case XTK_SPIRIT_TYPE_WINDOW:
        uview_hide(spirit->view);
        return 0;
    default:
        break;
    }
    
    // 默认情况
    if (!spirit->attached_container)
        return -1;
    xtk_spirit_t *attached_spirit = (xtk_spirit_t *)spirit->attached_container->spirit;
    if (!attached_spirit->surface)
        return -1;
    int start_x = spirit->x;
    int start_y = spirit->y;
    xtk_surface_rectfill(attached_spirit->surface, start_x, start_y, spirit->width, spirit->height, 
        attached_spirit->style.background_color);
    if (UVIEW_BAD_ID(attached_spirit->view))
        return -1;
    uview_bitmap_t bmp;
    uview_bitmap_init(&bmp, attached_spirit->surface->w, attached_spirit->surface->h, (uview_color_t *) attached_spirit->surface->pixels);
    uview_bitblt_update_ex(attached_spirit->view, attached_spirit->x + start_x,
        attached_spirit->y + start_y, &bmp, start_x, start_y, 
        spirit->width, spirit->height);
    return 0;
}

/**
 * 隐藏精灵下面的所有精灵
 */
int xtk_spirit_hide_all(xtk_spirit_t *spirit)
{
    if (!spirit)
        return -1;
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {    
        xtk_spirit_hide(tmp);
    }
    xtk_spirit_hide(spirit);
    return 0;
}
