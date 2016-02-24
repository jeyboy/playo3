#ifndef ITEM_ERRORS
#define ITEM_ERRORS

enum ItemErrors {
    err_none = 0,
    err_not_existed = 1,

    warn_not_supported = 1000,
    warn_not_accessable,
    warn_not_size_permitted
};

#endif // ITEM_ERRORS
