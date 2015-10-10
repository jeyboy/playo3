#ifndef SINGLETON
#define SINGLETON

namespace Core {
    class Singleton;

    class SingletonDestroyer {
        Singleton * p_instance;
    public:
        ~SingletonDestroyer();
        void initialize(Singleton * p);
    };

    class Singleton {
        static Singleton * p_instance;
        static SingletonDestroyer destroyer;
    protected:
        Singleton() { }
        Singleton(const Singleton &);
        Singleton & operator=(Singleton &);
       ~Singleton() { }
        friend class SingletonDestroyer;
    public:
        static Singleton & getInstance();
    };
}

#endif // SINGLETON
