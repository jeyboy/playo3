#ifndef BASE_ITEM_INTERFACE
#define BASE_ITEM_INTERFACE

namespace Core {
    enum ItemBehaviour {
        behaviour_obj,
        behaviour_rel
    };

    class IItem;

    class IBaseItem {
    protected:
        ItemBehaviour behaviour;
    public:       
        IBaseItem(ItemBehaviour item_behaviour = behaviour_obj) : behaviour(item_behaviour) {}
        inline virtual ~IBaseItem() {}
        virtual IItem * obj() { return this; }
    };
}

#endif // ITEM_INTERFACE

