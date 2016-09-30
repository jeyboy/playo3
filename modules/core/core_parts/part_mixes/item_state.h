#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

#include "modules/core/flag_ops.h"

namespace Core {
    #define DEFAULT_STATE (ItemState::flag_checked)
    #define DEFAULT_ITEM_STATE (DEFAULT_STATE | ItemState::flag_new_item)
    #define DEFAULT_CONTAINER_STATE (DEFAULT_STATE | ItemState::flag_proceeded | ItemState::flag_expanded)

    #define BITS_IS_SET(res, cbits) ((res & cbits) == cbits)
    #define SET_BITS(res, cbits) (res |= cbits, cbits)
    #define UNSET_BITS(res, cbits) res &= (~(cbits))

    #define VISUAL_BITS(res) (res & 7)
    #define SET_VISUAL_BIT(res, cbit) \
        int visual_part = VISUAL_BITS(res); \
        if (visual_part < cbit) \
            res = (res - visual_part) + cbit
    #define UNSET_VISUAL_BIT(res, cbit) res = (res - VISUAL_BITS(res)) + (cbit == 1 ? cbit : cbit >> 1)

    class ItemState {
        public:
            enum ItemStateFlag {
                flag_not_in_proc = 2097152,
                flag_not_proccessing = 1048576,
                flag_not_mark_on_removing = 524288,
                flag_not_played = 262144,
                flag_not_checked = 131072,
                flag_not_expanded = 65536,
                flag_not_proceeded = 32768,
//              flag_not_new_item = 16384,
                flag_not_listened = 8192,
                flag_not_liked = 4096,

                flag_proccessing = 2048,
                flag_mark_on_removing = 1024,
                flag_played = 512,
                flag_in_proc = 256,
                flag_checked = 128,
                flag_expanded = 64,
//                not_exist = 32,
//                not_supported = 16,
                flag_proceeded = 8,
                flag_liked = 4,
                flag_listened = 2,
                flag_new_item = 1
            };

            inline ItemState(int state = DEFAULT_ITEM_STATE) { item_state = state; }
            inline virtual ~ItemState() {}

            inline bool is(const enum ItemStateFlag & flag) const { return BITS_IS_SET(item_state, flag); }
            void set(const enum ItemStateFlag & flag);
            void unset(const enum ItemStateFlag & flag);

            void setStates(const int & flags);

            inline int states() const { return item_state; }
            inline int saveStates() const { return (unsigned char)item_state; }
            inline int visualStates() const { return VISUAL_BITS(item_state); }

        protected:
            //bool reset() {
            //    item_state = DEFAULT_ITEM_STATE;
            //    return true;
            //}

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
