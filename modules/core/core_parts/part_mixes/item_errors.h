#ifndef ITEM_ERRORS
#define ITEM_ERRORS

enum ItemErrors {
    err_none = 0,
    err_not_existed,
    err_not_finded,

    warn_not_supported = 1000,
    warn_not_accessable,
    warn_not_permitted
};

#endif // ITEM_ERRORS
