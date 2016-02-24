#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace Core {
    #define DEFAULT_FILE_STATE (ItemState::checked | ItemState::new_item)
    #define DEFAULT_PLAYLIST_STATE (ItemState::checked | ItemState::expanded)

    class ItemState {
        public:
            enum ItemStateFlag {
                not_proccessing = 8388608,
//                not_undefined_status = 4194304,
                not_played = 2097152,
//                not_proceeded = 1048576,
//                not_checked = 524288,
//                not_expanded = 262144,
//                info_not_required = 131072,
//                exist = 65536,
//                supported = 32768,
//                not_new_item = 16384,
//                not_listened = 8192,
                not_liked = 4096,

                proccessing = 2048,
                mark_on_removing = 1024,
                played = 512,
                proceeded = 256, // once list proceeding
                checked = 128,
                expanded = 64,
                info_required = 32,
//                not_exist = 16,
//                not_supported = 8,
                new_item = 4,
                listened = 2,
                liked = 1
            };

            inline ItemState(int state = DEFAULT_FILE_STATE) { item_state = state; }
            inline virtual ~ItemState() {}

            inline bool is(enum ItemStateFlag flag) const { return bitIsSet(item_state, flag); }
            inline void unset(enum ItemStateFlag flag) { unsetBit(item_state, flag); }
            bool set(enum ItemStateFlag flag);

            void setStates(int flags);

            inline int states() const { return item_state; }
            inline int saveStates() const { return (unsigned char)item_state; }
//            inline int innerStates() { return item_state & ((0 << 6) - 1) << 3; }
            inline int visualStates() const { return visualStateOffset(); }

        protected:
            inline int visualStateOffset() const { return item_state & 7; }  // get six first bits

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
