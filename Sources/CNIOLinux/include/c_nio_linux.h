//===----------------------------------------------------------------------===//
//
// This source file is part of the SwiftNIO open source project
//
// Copyright (c) 2017-2018 Apple Inc. and the SwiftNIO project authors
// Licensed under Apache License v2.0
//
// See LICENSE.txt for license information
// See CONTRIBUTORS.txt for the list of SwiftNIO project authors
//
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#ifndef C_NIO_LINUX_H
#define C_NIO_LINUX_H

#ifdef __linux__
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>

// Some explanation is required here.
//
// Due to SR-6772, we cannot get Swift code to directly see any of the mmsg structures or
// functions. However, we *can* get C code built by SwiftPM to see them. For this reason we
// elect to provide a selection of shims to enable Swift code to use recv_mmsg and send_mmsg.
// Mostly this is fine, but to minimise the overhead we want the Swift code to be able to
// create the msgvec directly without requiring further memory fussiness in our C shim.
// That requires us to also construct a C structure that has the same layout as struct mmsghdr.
//
// Conveniently glibc has pretty strict ABI stability rules, and this structure is part of the
// glibc ABI, so we can just reproduce the structure definition here and feel confident that it
// will be sufficient.
//
// If SR-6772 ever gets resolved we can remove this shim.
//
// https://bugs.swift.org/browse/SR-6772

typedef struct {
    struct msghdr msg_hdr;
    unsigned int msg_len;
} CNIOLinux_mmsghdr;

int CNIOLinux_sendmmsg(int sockfd, CNIOLinux_mmsghdr *msgvec, unsigned int vlen, int flags);
int CNIOLinux_recvmmsg(int sockfd, CNIOLinux_mmsghdr *msgvec, unsigned int vlen, int flags, struct timespec *timeout);

#endif
#endif
