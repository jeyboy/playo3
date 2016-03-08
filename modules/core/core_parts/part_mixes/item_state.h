#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace Core {
    #define DEFAULT_ITEM_STATE (flag_checked /*| flag_new_item*/ | flag_expanded)
    #define fieldStatePart(x) (x & 15)

    enum ItemStateFlag {
        flag_not_played = 262144,
        flag_not_proccessing = 131072,
        flag_not_mark_on_removing = 65536,
        flag_not_checked = 32768,
        flag_not_expanded = 16384,
        flag_not_proceeded = 8192,
        flag_not_listened = 4096,
        flag_not_liked = 2048,

        flag_mark_on_removing = 1024,
        flag_played = 512,
        flag_proccessing = 256,

        flag_checked = 128,
        flag_expanded = 64,
        //32
        //16
        // per source
        flag_proceeded = 8,
        flag_listened = 4,
        flag_liked = 2,
        flag_new_item = 1
    };

    class ItemState {
        public:
            inline ItemState(int state = DEFAULT_ITEM_STATE) { item_state = state; }
            inline virtual ~ItemState() {}

            inline bool has(enum ItemStateFlag flag) const { return bitIsSet(item_state, flag); }
            bool set(enum ItemStateFlag flag); // use only for flag_expanded and highter
            inline int states() const { return item_state; }

            inline int saveStates() const { return (unsigned char)item_state; }
            inline int visualStates() const { return item_state & 7; }  // get 3 first bits

            void copyBits(ItemState * other) { // need carefully test this
                item_state = item_state - fieldStatePart(item_state) + fieldStatePart(other -> item_state);
            }
        protected:
            inline void unset(enum ItemStateFlag flag) { unsetBit(item_state, flag); }
            void setStates(int flags);



            bool reset();
            bool setListened();
            bool setLiked();
            bool unsetLiked();

            inline bool bitIsSet(int val, int pos) const { return (val & pos) == pos; }
            inline int setBit(int val, int pos) { return (item_state = val | pos) & pos; }
            int unsetBit(int val, int pos) { return (item_state = val & (~(pos))); }

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
