#include "item_state.h"

using namespace Playo3;

bool ItemState::set(enum ItemStateFlag flag) {
    switch(flag) {
        case new_item: return   reset();
        case listened: return   setListened();
        case liked: return      setLiked();
        default: return         setBit(item_state, flag) == flag;
    }
}

void ItemState::setStates(int flags) {
    if (flags < 0) {
        if (bitIsSet(-flags, played))    unset(played);
        if (bitIsSet(-flags, liked))     unsetLiked();
        if (bitIsSet(-flags, not_exist)) unset(not_exist);
    } else {
        if (bitIsSet(flags, listened))  {
            setListened();
            unsetBit(item_state, not_exist);
        }
        if (bitIsSet(flags, liked))     setLiked();
        if (bitIsSet(flags, played))    setBit(item_state, played);
        if (bitIsSet(flags, not_exist)) setBit(item_state, not_exist);
        if (bitIsSet(flags, proceeded)) setBit(item_state, proceeded);
        if (bitIsSet(flags, mark_on_removing)) setBit(item_state, mark_on_removing);
    }
}

bool ItemState::reset() {
    item_state = DEFAULT_MODEL_ITEM_STATE;
    return true;
}

bool ItemState::setListened() {
    if (is(liked)) return false;

    setBit(item_state & (~(visualStateOffset())), listened);
    return true;
}
bool ItemState::setLiked() {
    setBit(item_state & (~(visualStateOffset())), liked);
    return true;
}
bool ItemState::unsetLiked() {
    unset(liked);
    setBit(item_state, listened);
}
