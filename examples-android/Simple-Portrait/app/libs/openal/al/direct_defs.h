#ifndef AL_DIRECT_DEFS_H
#define AL_DIRECT_DEFS_H

namespace detail_ {

template<typename T>
constexpr T DefaultVal() noexcept { return T{}; }

template<>
constexpr void DefaultVal() noexcept { }

} // namespace detail_

#define DECL_FUNC(R, Name)                                                    \
auto AL_APIENTRY Name() noexcept -> R                                         \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get());                                       \
}





#ifdef __ANDROID__
// Provide an implementation that does not use Direct functions for Android
#define DECL_FUNC1(R, Name, T1, n1)                                           \
auto AL_APIENTRY Name(T1 n1) noexcept -> R                                    \
{                                                                             \
    auto context = GetContextRef();                                           \
    if (!context) UNLIKELY return detail_::DefaultVal<R>();                   \
    return Default##Name(n1);                                                 \
}

// Provide a default implementation if needed
ALboolean DefaultalIsExtensionPresent(const ALchar* extName) {
    // Replace with a suitable implementation or return a default value
    return AL_FALSE; // Example: no extensions are available by default
}

#else
// The original macro that uses Direct functions
#define DECL_FUNC1(R, Name, T1, n1)                                           \
auto AL_APIENTRY Name(T1 n1) noexcept -> R                                    \
{                                                                             \
    auto context = GetContextRef();                                           \
    if (!context) UNLIKELY return detail_::DefaultVal<R>();                   \
    return Name##Direct(context.get(), n1);                                   \
}
#endif



//#define DECL_FUNC2(R, Name, T1,n1, T2,n2)                                     \
//auto AL_APIENTRY Name(T1 n1, T2 n2) noexcept -> R                             \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct(context.get(), n1, n2);                               \
//}

//#define DECL_FUNC3(R, Name, T1,n1, T2,n2, T3,n3)                              \
//auto AL_APIENTRY Name(T1 n1, T2 n2, T3 n3) noexcept -> R                      \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct(context.get(), n1, n2, n3);                           \
//}
//
//#define DECL_FUNC4(R, Name, T1,n1, T2,n2, T3,n3, T4,n4)                       \
//auto AL_APIENTRY Name(T1 n1, T2 n2, T3 n3, T4 n4) noexcept -> R               \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct(context.get(), n1, n2, n3, n4);                       \
//}
//
//#define DECL_FUNC5(R, Name, T1,n1, T2,n2, T3,n3, T4,n4, T5,n5)                \
//auto AL_APIENTRY Name(T1 n1, T2 n2, T3 n3, T4 n4, T5 n5) noexcept -> R        \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct(context.get(), n1, n2, n3, n4, n5);                   \
//}


#define DECL_FUNCEXT(R, Name,Ext)                                             \
auto AL_APIENTRY Name##Ext() noexcept -> R                                    \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get());                                  \
}

#define DECL_FUNCEXT1(R, Name,Ext, T1,n1)                                     \
auto AL_APIENTRY Name##Ext(T1 n1) noexcept -> R                               \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), n1);                              \
}
//
//#define DECL_FUNCEXT2(R, Name,Ext, T1,n1, T2,n2)                              \
//auto AL_APIENTRY Name##Ext(T1 n1, T2 n2) noexcept -> R                        \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct##Ext(context.get(), n1, n2);                          \
//}

//#define DECL_FUNCEXT3(R, Name,Ext, T1,n1, T2,n2, T3,n3)                       \
//auto AL_APIENTRY Name##Ext(T1 n1, T2 n2, T3 n3) noexcept -> R                 \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct##Ext(context.get(), n1, n2, n3);                      \
//}

#define DECL_FUNCEXT4(R, Name,Ext, T1,n1, T2,n2, T3,n3, T4,n4)                \
auto AL_APIENTRY Name##Ext(T1 n1, T2 n2, T3 n3, T4 n4) noexcept -> R          \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), n1, n2, n3, n4);                  \
}

//#define DECL_FUNCEXT5(R, Name,Ext, T1,n1, T2,n2, T3,n3, T4,n4, T5,n5)         \
//auto AL_APIENTRY Name##Ext(T1 n1, T2 n2, T3 n3, T4 n4, T5 n5) noexcept -> R   \
//{                                                                             \
//    auto context = GetContextRef();                                           \
//    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
//    return Name##Direct##Ext(context.get(), n1, n2, n3, n4, n5);              \
//}

#define DECL_FUNCEXT6(R, Name,Ext, T1,n1, T2,n2, T3,n3, T4,n4, T5,n5, T6,n6)  \
auto AL_APIENTRY Name##Ext(T1 n1, T2 n2, T3 n3, T4 n4, T5 n5, T6 n6) noexcept -> R \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), n1, n2, n3, n4, n5, n6);          \
}

#define DECL_FUNCEXT8(R, Name,Ext, T1,n1, T2,n2, T3,n3, T4,n4, T5,n5, T6,n6, T7,n7, T8,n8) \
auto AL_APIENTRY Name##Ext(T1 n1, T2 n2, T3 n3, T4 n4, T5 n5, T6 n6, T7 n7, T8 n8) noexcept -> R \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), n1, n2, n3, n4, n5, n6, n7, n8);  \
}

#endif /* AL_DIRECT_DEFS_H */
