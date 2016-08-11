#ifndef SINGLETON
#define SINGLETON

//int(*fooCheck)(int)     = T::foo; // check static method aviability in class while inheriting

#include <qjsonobject.h>
#include <qwidget.h>

namespace Core {
    template<class T> class SingletonPtr;

    template<class T> class SingletonDestroyer {
        T * p_instance;
    public:
        SingletonDestroyer() : p_instance(0) {}
        ~SingletonDestroyer() { delete p_instance; }
        void initialize(T * p) { p_instance = p; }
    };

    template<class T> class SingletonPtr {
        static T * p_instance;
        static SingletonDestroyer<T> destroyer;
    protected:
        SingletonPtr() { }
        SingletonPtr(const SingletonPtr &);
        SingletonPtr & operator=(SingletonPtr &);
        virtual ~SingletonPtr() { }
        friend class SingletonDestroyer<T>;
    public:
        static T & obj() {
            if(!p_instance)
                destroyer.initialize((p_instance = new T()));
            return *p_instance;
        }

        static T & linked_obj_with_init(QJsonObject * obj, QWidget * parent) {
            if(!p_instance)
                p_instance = new T(obj, parent);
            return *p_instance;
        }
    };

    template<class T> T * SingletonPtr<T>::p_instance = 0;
    template<class T> SingletonDestroyer<T> SingletonPtr<T>::destroyer;

/////////////////////////////////////////////////////////////////////////////
/// Meyers singleton
/////////////////////////////////////////////////////////////////////////////


    template<class T> class Singleton {
        Singleton(const Singleton &) = delete;
        Singleton & operator=(const Singleton &) = delete;
    public:
        static T & obj() {
            static T obj;
            return obj;
        }
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    };
/////////////////////////////////////////////////////////////////////////////
/// singleton without removing control
/////////////////////////////////////////////////////////////////////////////

    template<class T> class RelSingleton {
    public:
        static T & obj() {
            if (!target) target = new T();

            return *target;
        }

        static void destruct() { delete target; }

        virtual ~RelSingleton() {}
    protected:
        static T * target;
        RelSingleton() {}
        RelSingleton(const RelSingleton &);
        RelSingleton & operator=(const RelSingleton &);
    };

    template<class T> T * RelSingleton<T>::target = 0;
}

#endif // SINGLETON
