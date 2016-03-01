#ifndef DATA_ITEM_STATE
#define DATA_ITEM_STATE

namespace Core {
    #define DEFAULT_FILE_STATE (ItemState::new_item)

    class DataItemState {
        public:
            inline DataItemState(int state = DEFAULT_FILE_STATE) { item_state = state; }

            inline bool is(enum DataItemState::Flag flag) const { return bitIsSet(item_state, flag); }
            inline void unset(enum DataItemState::Flag flag) { unsetBit(item_state, flag); }
            bool set(enum DataItemState::Flag flag) {
                switch(flag) {
                    case new_item: return   reset();
                    case listened: return   setListened();
                    case liked: return      setLiked();
                    default: return         setBit(item_state, flag) == (int)flag;
                }
            }
            void setStates(int flags) {
                if (bitIsSet(flags, new_item))                  set(new_item);
                if (bitIsSet(flags, liked))                     set(liked);
                if (bitIsSet(flags, listened))                  set(listened);
                if (bitIsSet(flags, proceeded))                 set(proceeded);

                if (bitIsSet(flags, not_liked))                 unsetLiked();
                if (bitIsSet(flags, not_listened)) {
                    unset(listened);
                    set(new_item);
                }
                if (bitIsSet(flags, not_proceeded))             unset(proceeded);
            }

            inline int states() const { return item_state; }
            inline int visualStates() const { return item_state & 3; } // get six first bits

        protected:
            bool reset() {
                item_state = DEFAULT_FILE_STATE;
                return true;
            }

            bool setListened() {
                if (is(liked)) return false;

                setBit(item_state & (~(visualStates())), listened);
                return true;
            }
            bool setLiked() {
                setBit(item_state & (~(visualStates())), liked);
                return true;
            }
            bool unsetLiked() {
                unset(liked);
                return setBit(item_state, listened) == listened;
            }

            inline bool bitIsSet(int val, int pos) const { return (val & pos) == pos; }
            inline unsigned char setBit(unsigned char val, unsigned char pos) { return (item_state = val | pos) & pos; }
            unsigned char unsetBit(unsigned char val, unsigned char pos) { return (item_state = val & (~(pos))); }

            unsigned char item_state;
    };
}

#endif // DATA_ITEM_STATE
