#ifndef QXTGLOBAL_H
#define QXTGLOBAL_H

#include <qglobal.h>

#define QXT_DECLARE_PRIVATE(PUB) friend class PUB##Private; QxtPrivateInterface<PUB, PUB##Private> qxt_d;
#define QXT_DECLARE_PUBLIC(PUB) friend class PUB;
#define QXT_INIT_PRIVATE(PUB) qxt_d.setPublic(this);
#define QXT_D(PUB) PUB##Private& d = qxt_d()
#define QXT_P(PUB) PUB& p = qxt_p()

template <typename PUB>
class QxtPrivate {
public:
    virtual ~QxtPrivate() {}
    inline void QXT_setPublic(PUB * pub) { qxt_p_ptr = pub; }

protected:
    inline PUB & qxt_p() { return *qxt_p_ptr; }
    inline const PUB & qxt_p() const { return *qxt_p_ptr; }

private:
    PUB * qxt_p_ptr;
};

template <typename PUB, typename PVT>
class QxtPrivateInterface {
    friend class QxtPrivate<PUB>;
public:
    QxtPrivateInterface() { pvt = new PVT; }
    ~QxtPrivateInterface() { delete pvt; }

    inline void setPublic(PUB * pub) { pvt -> QXT_setPublic(pub); }
    inline PVT & operator()() { return *static_cast<PVT *>(pvt); }
    inline const PVT & operator()() const { return *static_cast<PVT *>(pvt); }
private:
    QxtPrivateInterface(const QxtPrivateInterface &) { }
    QxtPrivateInterface & operator=(const QxtPrivateInterface &) { }
    QxtPrivate<PUB> * pvt;
};

#endif // QXT_GLOBAL
