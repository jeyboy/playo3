#include "item_state.h"

using namespace Core;

bool ItemState::set(enum ItemStateFlag flag) {
    switch(flag) {
        case flag_new_item: return   setNewItem();
        case flag_listened: return   setListened();
        case flag_liked: return      setLiked();
        default: return         setBit(item_state, flag) == (int)flag;
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

//void ItemState::setStates(int flags) {
//    if (flags < 0) {
//        if (bitIsSet(-flags, proccessing))    unset(proccessing);
//        if (bitIsSet(-flags, played))    unset(played);
//        if (bitIsSet(-flags, liked))     unsetLiked();
////        if (bitIsSet(-flags, not_exist)) unset(not_exist);
////        if (bitIsSet(-flags, not_supported)) unset(not_supported);
//    } else {
//        if (bitIsSet(flags, listened))  {
//            setListened();
////            unsetBit(item_state, not_exist);
//        }
//        if (bitIsSet(flags, liked))     setLiked();
//        if (bitIsSet(flags, played))    setBit(item_state, played);
////        if (bitIsSet(flags, not_exist)) setBit(item_state, not_exist);
//        if (bitIsSet(flags, proceeded)) setBit(item_state, proceeded);
//        if (bitIsSet(flags, mark_on_removing)) setBit(item_state, mark_on_removing);
//        if (bitIsSet(flags, proccessing)) setBit(item_state, proccessing);
////        if (bitIsSet(flags, not_supported)) setBit(item_state, not_supported);

//        // negative variants

//        if (bitIsSet(flags, not_proccessing))     unset(proccessing);
//        if (bitIsSet(flags, not_played))     unset(played);
//        if (bitIsSet(flags, not_liked))     unsetLiked();
////        if (bitIsSet(flags, exist))     unset(not_exist);
////        if (bitIsSet(flags, supported))     unset(not_supported);
//    }
//}

bool ItemState::setNewItem() {
    setBit(item_state & (~(visualStates())), flag_new_item);
    return true;
}

bool ItemState::reset() {
    item_state = DEFAULT_ITEM_STATE;
    return true;
}

bool ItemState::setListened() {
    if (is(flag_liked)) return false;

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
