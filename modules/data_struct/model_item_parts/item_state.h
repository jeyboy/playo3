#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace Playo3 {
    #define DEFAULT_MODEL_ITEM_STATE (ItemState::checked | ItemState::new_item | ItemState::unprocessed)
    #define DEFAULT_MODEL_CONTAINER_STATE (ItemState::checked | ItemState::expanded)

//    #define STATE_UNPROCESSED 256
//    #define STATE_DEFAULT 128
//    #define STATE_LISTENED 64
//    #define STATE_LIKED 32

//    #define STATE_CHECKED 16

//    #define STATE_EXPANDED 8
//    #define STATE_PLAYED 4

//    #define STATE_NOT_EXIST 2
//    #define STATE_LIST_PROCEED 1

    class ItemState {
        public:
            enum ItemStateFlag {
                new_item = 128,
                listened = 64,
                liked = 32,
                checked = 16,
                expanded = 8,
                played = 4,
                not_exist = 2,
                proceeded = 1,
                unprocessed = 0
            };

            inline ItemState(int state = DEFAULT_MODEL_ITEM_STATE) { item_state = state; }

            inline bool is(enum ItemStateFlag flag) const { return bitIsSet(item_state, flag); }
            inline void unset(enum ItemStateFlag flag) { unsetBit(item_state, flag); }
            bool set(enum ItemStateFlag flag);

            void setStates(int flags);

            inline int stateVal() const { return item_state; }
            inline int saveStateVal() { return item_state & ((0 << 5) - 1) << 3; }
            inline int itemStateVal() const { return is(played) ? played : item_state & ((0 << 3) - 1) << 5; }

        protected:
            bool reset();
            bool setListened();
            bool setLiked();
            bool setExpanded();
            bool setPlayed();
            bool setNotExist();

            inline bool bitIsSet(int val, int pos) const { return (val & pos) == pos; }
            inline int setBit(int val, int pos) { return (item_state = val | pos) & pos; }
            int unsetBit(int val, int pos) { return (item_state = val & (~(pos))); }

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
