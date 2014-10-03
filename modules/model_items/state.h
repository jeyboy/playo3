#ifndef MODEL_ITEM_STATE_H
#define MODEL_ITEM_STATE_H

namespace ModelItems {
    enum StateFlag {
        unprocessed = 256,
        new_item = 128,
        listened = 64,
        liked = 32,
        checked = 16,
        expanded = 8,
        played = 4,
        not_exist = 2,
        proceeded = 1
    };

#define DEFAULT_MODEL_ITEM_STATE (checked | new_item)

//    #define STATE_UNPROCESSED 256
//    #define STATE_DEFAULT 128
//    #define STATE_LISTENED 64
//    #define STATE_LIKED 32

//    #define STATE_CHECKED 16

//    #define STATE_EXPANDED 8
//    #define STATE_PLAYED 4

//    #define STATE_NOT_EXIST 2
//    #define STATE_LIST_PROCEED 1

    class State {
        public:
            State();
            State(int state);

            bool isNone();
            bool isListened();
            bool isLiked();

            bool isPlayed();
            bool isProceed();
            bool isNotExist();
            bool isUnprocessed();
            bool isExpanded();

            bool isChecked();

            bool setNone();
            bool setListened();
            bool setLiked();

            bool setExpanded();
            bool unsetExpanded();

            bool setPlayed();
            bool unsetPlayed();

            bool setNotExist();
            bool unsetNotExist();

            bool setProceed();
            bool unsetProceed();

            bool setUnprocessed();

            bool setChecked();
            bool unsetChecked();

            bool setBit(int val);

            int getValue() const;
            int getFuncValue();
            int currStateValue();

        private:
            bool bitIsSet(int val, int pos);
            int setBit(int val, int pos);
            int unsetBit(int val, int pos);

            int item_state;
    };

}

#endif // MODEL_ITEM_STATE_H
