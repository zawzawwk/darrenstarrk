/* Definitions of macros to access `dev_t' values.
   Copyright (C) 1996-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H        1
#include <features.h>
__BEGIN_DECLS
__extension__
extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __THROW __attribute_const__;
__extension__
extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __THROW __attribute_const__;
__extension__
extern unsigned long long int gnu_dev_makedev (unsigned int __major,
                                               unsigned int __minor)
     __THROW __attribute_const__;
#ifdef __USE_EXTERN_INLINES
__extension__ __extern_inline __attribute_const__ unsigned int
__NTH (gnu_dev_major (unsigned long long int __dev))
{
  return ((__dev >> 8) & 0xfff) | ((unsigned int) (__dev >> 32) & ~0xfff);
}
__extension__ __extern_inline __attribute_const__ unsigned int
__NTH (gnu_dev_minor (unsigned long long int __dev))
{
  return (__dev & 0xff) | ((unsigned int) (__dev >> 12) & ~0xff);
}
__extension__ __extern_inline __attribute_const__ unsigned long long int
__NTH (gnu_dev_makedev (unsigned int __major, unsigned int __minor))
{
  return ((__minor & 0xff) | ((__major & 0xfff) << 8)
          | (((unsigned long long int) (__minor & ~0xff)) << 12)
          | (((unsigned long long int) (__major & ~0xfff)) << 32));
}
#endif
__END_DECLS
/* Access the functions with their traditional names.  */
#define major(dev) gnu_dev_major (dev)
#define minor(dev) gnu_dev_minor (dev)
#define makedev(maj, min) gnu_dev_makedev (maj, min)
#endif /* sys/sysmacros.h */
