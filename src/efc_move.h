#ifndef EFC_MOVE_H_
#define EFC_MOVE_H_

#include <platform/platform.h>

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
#include <utility>

#define RVALUE_REF(Class) Class &&

/* This is needed for constructions like
 * "RVALUE_REF2(ScopedPointer<_Tp1, _Dp1>)" */
#define RVALUE_REF2(Class1, Class2) const ::std::__mover<(Class1 Class2)> &

#define RVALUE_VAL(Var) (Var)
#else

/* FIXME: We do not have move constructor for ::EFC::String
 * So we have to define specialization of std::__mover for ::EFC::String */
#include <efc/String>

namespace std {
    template <typename T>
    class __mover
    {
    public:
        __mover(T &ref) :
            ref(ref)
        {}

        template <typename R>
        inline operator __mover<R> () const { return ref; }

        T &ref;
    };

    template <>
    class __mover< ::EFC::String >
    {
    public:
        __mover(::EFC::String &ref) :
            ref(ref)
        {}

        template <typename R>
        inline operator __mover<R> () const { return ref; }

        /* FIXME: We do not have move constructor for ::EFC::String
         * So we have to call a copy constructor instead */
        inline operator ::EFC::String &() const { return ref; }

        ::EFC::String &ref;
    };

    template <typename T>
    inline __mover<T> move(T &val) { return val; }

    template <typename T>
    inline __mover<T> move(const T &val) { return const_cast<T &>(val); }

    template <typename T>
    inline __mover<T> move(const __mover<T> &val) { return val.ref; }
}

#define RVALUE_REF(Class) const ::std::__mover< Class > &

/* This is needed for constructions like
 * "RVALUE_REF2(ScopedPointer<_Tp1, _Dp1>)" */
#define RVALUE_REF2(Class1, Class2) const ::std::__mover<(Class1 Class2)> &

#define RVALUE_VAL(Var) (Var.ref)
#endif

#endif /* EFC_MOVE_H_ */
