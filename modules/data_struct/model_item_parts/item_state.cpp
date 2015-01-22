#include "item_state.h"

using namespace Playo3;

bool ItemState::set(enum ItemStateFlag flag) {
    switch(flag) {
        case new_item: return reset();
        case listened: return setListened();
        case liked: return setLiked();
        case expanded: return setExpanded();
        case played: return setPlayed();
        case not_exist: return setNotExist();
        default: return setBit(item_state, flag) == flag;
    }
}

void ItemState::setStates(int flags) {
    if (flags < 0) {
        if (bitIsSet(flags, played)) unset(played);
        if (bitIsSet(flags, not_exist)) unset(not_exist);
    } else {
        if (bitIsSet(flags, listened)) setListened();
        if (bitIsSet(flags, liked)) setLiked();
        if (bitIsSet(flags, played)) setPlayed();
        if (bitIsSet(flags, not_exist)) setNotExist();
        if (bitIsSet(flags, proceeded)) setBit(item_state, proceeded);
    }
}

bool ItemState::reset() {
    item_state = DEFAULT_MODEL_ITEM_STATE;
    return true;
}

bool ItemState::setListened() {
    if (!is(liked) && !is(unprocessed)) {
        setBit(clearState(), listened);
        return true;
    }
    return false;
}
bool ItemState::setLiked() {
    if (is(unprocessed)) return false;
    setBit(clearState(), liked);
    return true;
}


bool ItemState::setPlayed() {
    if (is(unprocessed)) return false;
    setBit(item_state, played);
    return true;
}

bool ItemState::setExpanded() {
    if (!is(unprocessed)) return false;
    setBit(item_state, expanded);
    return true;
}

bool ItemState::setNotExist() {
    if (is(unprocessed)) return false;
    setBit(item_state, not_exist);
    return true;
}
