//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_ASSERT_HPP_
#define ECORE_ASSERT_HPP_

#define REPORT_ERROR(mesg)              \
    do {                                     \
        _RPTF0(_CRT_ASSERT, mesg);           \
        _CRT_SECURE_INVALID_PARAMETER(mesg); \
    } while (false)

#define VERIFY(cond, mesg)                                          \
    do {                                                            \
        if (cond) { /* contextually convertible to bool paranoia */ \
        } else {                                                    \
            REPORT_ERROR(mesg);                                     \
        }                                                           \
                                                                    \
        __assume(cond);                                             \
    } while (false)

#ifdef _DEBUG
#define ASSERT(cond, mesg) VERIFY(cond, mesg)
#else // ^^^ _DEBUG ^^^ // vvv !_DEBUG vvv
#define ASSERT(cond, mesg) __assume(cond)
#endif // _DEBUG


#endif