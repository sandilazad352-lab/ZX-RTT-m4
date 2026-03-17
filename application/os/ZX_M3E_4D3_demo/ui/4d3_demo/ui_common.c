#include "ui_common.h"


static E_SCREEN_ID cur_scr = E_MAX_SCR;
static SCR_FUNC_TYPE *act_scr_func = NULL;
static SCR_FUNC_TYPE *top_layer_func = NULL;
static SCR_LIST_TYPE scr_parm_list[E_MAX_SCR] = {0};

void scr_add_user_data(lv_obj_t *scr, E_SCREEN_ID id, SCR_FUNC_TYPE *desc)
{
    scr_parm_list[id].scr = scr;
    scr_parm_list[id].src_ops = desc;

    if(id == E_TOP_LAYER)
        top_layer_func = desc;
}

//刷新当前屏幕
void scr_refr_func(Event_Data_t *args)
{
    if(act_scr_func)
        act_scr_func->refr_func(args);
    if(top_layer_func)
        top_layer_func->refr_func(args);
}

//退出当前屏幕
void scr_quit_func(void *args)
{
    if(act_scr_func != NULL)
        act_scr_func->quit_func(args);
}

//载入屏幕
void scr_load_func(E_SCREEN_ID id, void *args)
{
    if(id >= E_MAX_SCR)
    {
        APP_DEBUG("%s value overflow", __func__);
        return;
    }
    cur_scr = id;
    if((scr_parm_list[id].scr == NULL) || (scr_parm_list[id].src_ops == NULL))
        return;

    if(act_scr_func != NULL)
        act_scr_func->quit_func(args);

    scr_parm_list[id].src_ops->load_func(args);

    if(top_layer_func != NULL)
        top_layer_func->load_func(&id);
    printf("id:%d\n",id);

    lv_scr_load(scr_parm_list[id].scr);
    act_scr_func = scr_parm_list[id].src_ops;
}

//获取当前屏幕
E_SCREEN_ID scr_act_get(void)
{
    return cur_scr;
}


