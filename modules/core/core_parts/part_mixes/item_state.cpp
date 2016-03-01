#include "item_state.h"

using namespace Core;

bool ItemState::set(enum ItemStateFlag flag) {
    if (flag >= flag_not_liked) setStates(flag);

    switch(flag) {
        case flag_new_item: return reset();
        case flag_listened: return setListened();
        case flag_liked:    return setLiked();
        default:            return setBit(item_state, flag) == (int)flag;
    }
}

void ItemState::setStates(int flags) {
    if (bitIsSet(flags, flag_proccessing))               set(flag_proccessing);
    if (bitIsSet(flags, flag_played))                    set(flag_played);
    if (bitIsSet(flags, flag_checked))                   set(flag_checked);
    if (bitIsSet(flags, flag_expanded))                  set(flag_expanded);
    if (bitIsSet(flags, flag_mark_on_removing))          set(flag_mark_on_removing);

    if (bitIsSet(flags, flag_not_proccessing))           unset(flag_proccessing);
    if (bitIsSet(flags, flag_not_played))                unset(flag_played);
    if (bitIsSet(flags, flag_not_checked))               unset(flag_checked);
    if (bitIsSet(flags, flag_not_expanded))              unset(flag_expanded);
    if (bitIsSet(flags, flag_not_mark_on_removing))      unset(flag_mark_on_removing);

    if (bitIsSet(flags, flag_new_item))                  set(flag_new_item);
    if (bitIsSet(flags, flag_liked))                     set(flag_liked);
    if (bitIsSet(flags, flag_listened))                  set(flag_listened);
    if (bitIsSet(flags, flag_proceeded))                 set(flag_proceeded);

    if (bitIsSet(flags, flag_not_liked))                 unsetLiked();
    if (bitIsSet(flags, flag_not_listened)) {
        unset(flag_listened);
        set(flag_new_item);
    }
    if (bitIsSet(flags, flag_not_proceeded))             unset(flag_proceeded);
}

bool ItemState::reset() {
    item_state = DEFAULT_ITEM_STATE;
    return true;
}

bool ItemState::setListened() {
    if (has(flag_liked)) return false;

    setBit(item_state & (~(visualStates())), flag_listened);
    return true;
}
bool ItemState::setLiked() {
    setBit(item_state & (~(visualStates())), flag_liked);
    return true;
}
bool ItemState::unsetLiked() {
    unset(flag_liked);
    return setBit(item_state, flag_listened) == flag_listened;
}
