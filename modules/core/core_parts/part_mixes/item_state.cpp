#include "item_state.h"

using namespace Core;

void ItemState::set(const enum ItemStateFlag & flag) {
    switch(flag) {
        case flag_new_item:
        case flag_listened:
        case flag_liked:    { SET_VISUAL_BIT(item_state, flag); }
        default:            SET_BITS(item_state, flag);
    }
}

void ItemState::unset(const enum ItemStateFlag & flag) {
    switch(flag) {
        case flag_listened:
        case flag_liked:    { UNSET_VISUAL_BIT(item_state, flag); break; }
        case flag_new_item: return;
        default:            UNSET_BITS(item_state, flag);
    }
}

void ItemState::setStates(const int & flags) {
    if (BITS_IS_SET(flags, flag_proccessing))               set(flag_proccessing);
    if (BITS_IS_SET(flags, flag_played))                    set(flag_played);
    if (BITS_IS_SET(flags, flag_checked))                   set(flag_checked);
    if (BITS_IS_SET(flags, flag_expanded))                  set(flag_expanded);
    if (BITS_IS_SET(flags, flag_mark_on_removing))          set(flag_mark_on_removing);

    if (BITS_IS_SET(flags, flag_not_proccessing))           unset(flag_proccessing);
    if (BITS_IS_SET(flags, flag_not_played))                unset(flag_played);
    if (BITS_IS_SET(flags, flag_not_checked))               unset(flag_checked);
    if (BITS_IS_SET(flags, flag_not_expanded))              unset(flag_expanded);
    if (BITS_IS_SET(flags, flag_not_mark_on_removing))      unset(flag_mark_on_removing);

    if (BITS_IS_SET(flags, flag_new_item))                  set(flag_new_item);
    if (BITS_IS_SET(flags, flag_liked))                     set(flag_liked);
    if (BITS_IS_SET(flags, flag_listened))                  set(flag_listened);
    if (BITS_IS_SET(flags, flag_proceeded))                 set(flag_proceeded);

    if (BITS_IS_SET(flags, flag_not_liked))                 unset(flag_liked);
    if (BITS_IS_SET(flags, flag_not_listened))              unset(flag_listened);
    if (BITS_IS_SET(flags, flag_not_proceeded))             unset(flag_proceeded);
}
