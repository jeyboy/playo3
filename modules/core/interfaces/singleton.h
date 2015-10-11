#ifndef SINGLETON
#define SINGLETON

//int(*fooCheck)(int)     = T::foo; // check static method aviability in class while inheriting

namespace Core {
    template<class T> class SingletonPtr;

    template<class T> class SingletonDestroyer {
        T * p_instance;
    public:
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
       ~SingletonPtr() { }
        friend class SingletonDestroyer<T>;
    public:
        static T & obj() {
            if(!p_instance)
                destroyer.initialize((p_instance = new T()));
            return *p_instance;
        }
    };

    template<class T> T * SingletonPtr<T>::p_instance = 0;
    template<class T> SingletonDestroyer<T> SingletonPtr<T>::destroyer;


/////////////////////////////////////////////////////////////////////////////
/// Meyers singleton
/////////////////////////////////////////////////////////////////////////////


    template<class T> class Singleton {
    public:
        static T & obj() {
            static T obj;
            return obj;
        }

        virtual ~Singleton() {}
    protected:
        Singleton() {}
        Singleton(const Singleton &);
        Singleton & operator=(const Singleton &);
    };
}

#endif // SINGLETON
