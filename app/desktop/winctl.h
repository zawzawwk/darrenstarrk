#ifndef _BDE_WINCTL_H   /* bookos desktop environment (BDE) */
#define _BDE_WINCTL_H

#include <xtk.h>
#include <sys/list.h>

#define WINCTL_ICON_PATH_DEFAULT    "/system/icons/default.png"

#define WINCTL_ICON_SIZE        36
#define WINCTL_SIZE_DEFAULT    (WINCTL_ICON_SIZE + 4)

#define WINCTL_BACK_COLOR       XTK_RGB(40, 40, 40)
#define WINCTL_ACTIVE_COLOR     XTK_RGB(80, 80, 80)

typedef struct {
    bool ishidden;          /* 窗口隐藏 */
    bool isfocus;           /* 窗口聚焦 */
    xtk_spirit_t *button;   /* 按钮精灵 */
    int winid;              /* 窗口控制对应的窗口 */
    list_t list;            /* 窗口控制链表 */
} winctl_t;

extern winctl_t *winctl_last;
winctl_t *winctl_create(int winid);
int winctl_destroy(winctl_t *winctl);
int winctl_destroy_all();
winctl_t *winctl_find(int winid);

void winctl_get_focus(winctl_t *winctl);
void winctl_lost_focus(winctl_t *winctl);
void winctl_paint(winctl_t *winctl);

int winctl_set_icon(winctl_t *winctl, char *pathname);

#endif  /* _BDE_WINCTL_H */