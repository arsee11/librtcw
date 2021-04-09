/*
 *  Copyright 2004 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <vector>

#include "api/crypto/crypto_options.h"
#include "pc/session_description.h"

// Types and classes used in media session descriptions.

#ifndef PC_MEDIA_SESSION_H_
#define PC_MEDIA_SESSION_H_

namespace cricket {

// Convenience functions.
bool IsMediaContent(const ContentInfo* content);
bool IsAudioContent(const ContentInfo* content);
bool IsVideoContent(const ContentInfo* content);
bool IsDataContent(const ContentInfo* content);
const ContentInfo* GetFirstMediaContent(const ContentInfos& contents,
                                        MediaType media_type);
const ContentInfo* GetFirstAudioContent(const ContentInfos& contents);
const ContentInfo* GetFirstVideoContent(const ContentInfos& contents);
const ContentInfo* GetFirstDataContent(const ContentInfos& contents);
const ContentInfo* GetFirstMediaContent(const SessionDescription* sdesc,
                                        MediaType media_type);
const ContentInfo* GetFirstAudioContent(const SessionDescription* sdesc);
const ContentInfo* GetFirstVideoContent(const SessionDescription* sdesc);
const ContentInfo* GetFirstDataContent(const SessionDescription* sdesc);
const AudioContentDescription* GetFirstAudioContentDescription(
    const SessionDescription* sdesc);
const VideoContentDescription* GetFirstVideoContentDescription(
    const SessionDescription* sdesc);
const RtpDataContentDescription* GetFirstRtpDataContentDescription(
    const SessionDescription* sdesc);
const SctpDataContentDescription* GetFirstSctpDataContentDescription(
    const SessionDescription* sdesc);
// Non-const versions of the above functions.
// Useful when modifying an existing description.
ContentInfo* GetFirstMediaContent(ContentInfos* contents, MediaType media_type);
ContentInfo* GetFirstAudioContent(ContentInfos* contents);
ContentInfo* GetFirstVideoContent(ContentInfos* contents);
ContentInfo* GetFirstDataContent(ContentInfos* contents);
ContentInfo* GetFirstMediaContent(SessionDescription* sdesc,
                                  MediaType media_type);
ContentInfo* GetFirstAudioContent(SessionDescription* sdesc);
ContentInfo* GetFirstVideoContent(SessionDescription* sdesc);
ContentInfo* GetFirstDataContent(SessionDescription* sdesc);
AudioContentDescription* GetFirstAudioContentDescription(
    SessionDescription* sdesc);
VideoContentDescription* GetFirstVideoContentDescription(
    SessionDescription* sdesc);
RtpDataContentDescription* GetFirstRtpDataContentDescription(
    SessionDescription* sdesc);
SctpDataContentDescription* GetFirstSctpDataContentDescription(
    SessionDescription* sdesc);

// Helper functions to return crypto suites used for SDES.
void GetSupportedAudioSdesCryptoSuites(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<int>* crypto_suites);
void GetSupportedVideoSdesCryptoSuites(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<int>* crypto_suites);
void GetSupportedDataSdesCryptoSuites(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<int>* crypto_suites);
void GetSupportedAudioSdesCryptoSuiteNames(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<std::string>* crypto_suite_names);
void GetSupportedVideoSdesCryptoSuiteNames(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<std::string>* crypto_suite_names);
void GetSupportedDataSdesCryptoSuiteNames(
    const webrtc::CryptoOptions& crypto_options,
    std::vector<std::string>* crypto_suite_names);

}  // namespace cricket

#endif  // PC_MEDIA_SESSION_H_
