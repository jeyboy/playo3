#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace Core {
    #define DEFAULT_ITEM_STATE (ItemState::flag_checked | ItemState::flag_new_item | ItemState::flag_expanded)

    class ItemState {
        public:
            enum ItemStateFlag {
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
//                if_proceeded = 256,
                flag_checked = 128,
                flag_expanded = 64,
//                not_exist = 32,
//                not_supported = 16,
                flag_proceeded = 8,
                flag_new_item = 4,
                flag_listened = 2,
                flag_liked = 1
            };

            inline ItemState(int state = DEFAULT_ITEM_STATE) { item_state = state; }
            inline virtual ~ItemState() {}

            inline bool is(enum ItemStateFlag flag) const { return bitIsSet(item_state, flag); }
            inline void unset(enum ItemStateFlag flag) { unsetBit(item_state, flag); }
            bool set(enum ItemStateFlag flag);

            void setStates(int flags);

            inline int states() const { return item_state; }
            inline int saveStates() const { return (unsigned char)item_state; }
//            inline int innerStates() { return item_state & ((0 << 6) - 1) << 3; }
            inline int visualStates() const { return item_state & 7; }

        protected:
            bool reset();
            bool setNewItem();
            bool setListened();
            bool setLiked();
            bool unsetLiked();

            inline bool bitIsSet(int val, int pos) const { return (val & pos) == pos; }
            inline bool setBit(int val, int pos) { return (item_state = val | pos) & pos; }
            int unsetBit(int val, int pos) { return (item_state = val & (~(pos))); }

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
