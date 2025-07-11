/**
 *        d8888 888                .d8888b.
 *       d88888 888               d88P  Y88b
 *      d88P888 888               888    888
 *     d88P 888 88888b.   .d88b.  888        88888b.d88b.  88888b.
 *    d88P  888 888 "88b d8P  Y8b 888        888 "888 "88b 888 "88b
 *   d88P   888 888  888 88888888 888    888 888  888  888 888  888
 *  d8888888888 888 d88P Y8b.     Y88b  d88P 888  888  888 888 d88P
 * d88P     888 88888P"   "Y8888   "Y8888P"  888  888  888 88888P"
 *                                                         888
 * A simple file comparison tool                           888
 *                                                         888
 * Copyright (c) 2025, Abe Mishler
 * Licensed under the Universal Permissive License v 1.0
 * as shown at https://oss.oracle.com/licenses/upl/.
 */

#pragma once

namespace AbeCmp {

// @brief Get the architecture of the host system.
//        Returns a string representing the architecture type.
//        This function is useful for determining the architecture of the system on which the application is running.
//        It can be used to optimize performance or compatibility based on the architecture.
const char*
getArch()
{
    static const char* x86_32 = "32-bit x86";
    static const char* x86_64 = "64-bit x86";
    static const char* amd64 = "amd64";
    static const char* unknown = "unknown";

#if defined(i386) || defined(i686) // 32-bit x86 architectures
    return x86_32;
#elif defined(__x86_64__) // 64-bit x86 architectures
    return x86_64;
#elif defined(_M_IX86)    // Microsoft compiler for 32-bit x86
    return x86_32;
#elif defined(_M_X64)     // Microsoft compiler for 64-bit x86
    return x86_64;
#elif defined(__amd64__)  // 64-bit AMD architectures
    return amd64;
#elif defined(__amd64)    // 64-bit AMD architectures (older compilers)
    return amd64;
#else
    return unknown;
#endif
}

// @brief Get the type of host system.
//        Returns a string representing the host type.
//        This function is useful for determining the environment in which the application is running.
const char*
getHostType()
{
    static const char* env_mac = "Mac";
    static const char* env_cygwin = "Cygwin";
    static const char* env_gnu_linux = "Gnu/Linux";
    static const char* env_linux = "Linux";
    static const char* env_msys2 = "MSYS2";
    static const char* env_unknown = "Unknown";
    static const char* env_windows = "Windows";

#if defined(__APPLE__)
    return env_mac;
#elif defined(__CYGWIN__)
    return env_cygwin;
#elif defined(__gnu_linux__)
    return env_gnu_linux;
#elif defined(__linux__)
    return env_linux;
#elif defined(__MSYS__)
    return env_msys2;
#elif defined(_WIN32)
    return env_windows;
#endif
    return env_unknown;
}

} // namespace AbeCmp
