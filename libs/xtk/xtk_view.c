#include "xtk_view.h"
#include <stdlib.h>

LIST_HEAD(xtk_view_list_head);

int xtk_view_init()
{
    list_init(&xtk_view_list_head);
    return 0;
}

int xtk_view_exit()
{
    xtk_view_t *pview, *pview_next;
    list_for_each_owner_safe (pview, pview_next, &xtk_view_list_head, list) {
        xtk_view_remove(pview);
        xtk_view_destroy(pview);
    }
    return 0;
}

xtk_view_t *xtk_view_create()
{
    xtk_view_t *view = (xtk_view_t *) malloc(sizeof(xtk_view_t));
    view->view = -1;
    list_init(&view->list);
    list_init(&view->spirit_list_head);
    view->spirit = NULL;
    view->nowait = 0;
    return view;
}

int xtk_view_destroy(xtk_view_t *view)
{
    if (!view)
        return -1;
    view->view = -1;
    if (list_find(&view->list, &xtk_view_list_head))
        list_del(&view->list);
    free(view);
    return 0;
}

void xtk_view_add(xtk_view_t *view)
{
    list_add_tail(&view->list, &xtk_view_list_head);
}

void xtk_view_remove(xtk_view_t *view)
{
    list_del(&view->list);
}

xtk_view_t *xtk_view_find(int view)
{
    xtk_view_t *pview;
    list_for_each_owner (pview, &xtk_view_list_head, list) {
        if (pview->view == view)
            return pview;
    }
    return pview;
}
