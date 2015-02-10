#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace Playo3 {
    #define DEFAULT_MODEL_ITEM_STATE (ItemState::checked | ItemState::new_item)
    #define DEFAULT_MODEL_CONTAINER_STATE (ItemState::checked | ItemState::expanded)

    class ItemState {
        public:
            enum ItemStateFlag {
                played = 512,
                proceeded = 256, // once list proceeding
                checked = 128,
                expanded = 64,
                info_required = 32,
                not_exist = 16,
                not_supported = 8,
                new_item = 4,
                listened = 2,
                liked = 1
            };

            inline ItemState(int state = DEFAULT_MODEL_ITEM_STATE) { item_state = state; }

            inline bool is(enum ItemStateFlag flag) const { return bitIsSet(item_state, flag); }
            inline void unset(enum ItemStateFlag flag) { unsetBit(item_state, flag); }
            bool set(enum ItemStateFlag flag);

            void setStates(int flags);

            inline int states() const { return item_state; }
            inline int saveStates() const { return (unsigned char)item_state; }
//            inline int innerStates() { return item_state & ((0 << 6) - 1) << 3; }
            inline int visualStates() const { return is(played) ? played : visualStateOffset(); }

        protected:
            inline int visualStateOffset() const { return item_state & 7; }  // get six first bits

            bool reset();
            bool setListened();
            bool setLiked();

            inline bool bitIsSet(int val, int pos) const { return (val & pos) == pos; }
            inline int setBit(int val, int pos) { return (item_state = val | pos) & pos; }
            int unsetBit(int val, int pos) { return (item_state = val & (~(pos))); }

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
