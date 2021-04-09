/*
 *  Copyright 2004 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "pc/media_session.h"

namespace cricket {

// RTP Profile names
// http://www.iana.org/assignments/rtp-parameters/rtp-parameters.xml
// RFC4585
const char kMediaProtocolAvpf[] = "RTP/AVPF";
// RFC5124
const char kMediaProtocolDtlsSavpf[] = "UDP/TLS/RTP/SAVPF";

// We always generate offers with "UDP/TLS/RTP/SAVPF" when using DTLS-SRTP,
// but we tolerate "RTP/SAVPF" in offers we receive, for compatibility.
const char kMediaProtocolSavpf[] = "RTP/SAVPF";

// Note that the below functions support some protocol strings purely for
// legacy compatibility, as required by JSEP in Section 5.1.2, Profile Names
// and Interoperability.

static bool IsMediaContentOfType(const ContentInfo* content,
                              MediaType media_type) {
  if (!content || !content->media_description()) {
    return false;
  }
  return content->media_description()->type() == media_type;
}


bool IsMediaContent(const ContentInfo* content) {
  return (content && (content->type == MediaProtocolType::kRtp ||
                      content->type == MediaProtocolType::kSctp));
}

bool IsAudioContent(const ContentInfo* content) {
  return IsMediaContentOfType(content, MEDIA_TYPE_AUDIO);
}

bool IsVideoContent(const ContentInfo* content) {
  return IsMediaContentOfType(content, MEDIA_TYPE_VIDEO);
}

bool IsDataContent(const ContentInfo* content) {
  return IsMediaContentOfType(content, MEDIA_TYPE_DATA);
}

const ContentInfo* GetFirstMediaContent(const ContentInfos& contents,
                                        MediaType media_type) {
  for (const ContentInfo& content : contents) {
    if (IsMediaContentOfType(&content, media_type)) {
      return &content;
    }
  }
  return nullptr;
}

const ContentInfo* GetFirstAudioContent(const ContentInfos& contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_AUDIO);
}

const ContentInfo* GetFirstVideoContent(const ContentInfos& contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_VIDEO);
}

const ContentInfo* GetFirstDataContent(const ContentInfos& contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_DATA);
}

const ContentInfo* GetFirstMediaContent(const SessionDescription* sdesc,
                                        MediaType media_type) {
  if (sdesc == nullptr) {
    return nullptr;
  }

  return GetFirstMediaContent(sdesc->contents(), media_type);
}

const ContentInfo* GetFirstAudioContent(const SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_AUDIO);
}

const ContentInfo* GetFirstVideoContent(const SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_VIDEO);
}

const ContentInfo* GetFirstDataContent(const SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_DATA);
}

const MediaContentDescription* GetFirstMediaContentDescription(
    const SessionDescription* sdesc,
    MediaType media_type) {
  const ContentInfo* content = GetFirstMediaContent(sdesc, media_type);
  return (content ? content->media_description() : nullptr);
}

const AudioContentDescription* GetFirstAudioContentDescription(
    const SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_AUDIO);
  return desc ? desc->as_audio() : nullptr;
}

const VideoContentDescription* GetFirstVideoContentDescription(
    const SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_VIDEO);
  return desc ? desc->as_video() : nullptr;
}

const RtpDataContentDescription* GetFirstRtpDataContentDescription(
    const SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_DATA);
  return desc ? desc->as_rtp_data() : nullptr;
}

const SctpDataContentDescription* GetFirstSctpDataContentDescription(
    const SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_DATA);
  return desc ? desc->as_sctp() : nullptr;
}

//
// Non-const versions of the above functions.
//

ContentInfo* GetFirstMediaContent(ContentInfos* contents,
                                  MediaType media_type) {
  for (ContentInfo& content : *contents) {
    if (IsMediaContentOfType(&content, media_type)) {
      return &content;
    }
  }
  return nullptr;
}

ContentInfo* GetFirstAudioContent(ContentInfos* contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_AUDIO);
}

ContentInfo* GetFirstVideoContent(ContentInfos* contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_VIDEO);
}

ContentInfo* GetFirstDataContent(ContentInfos* contents) {
  return GetFirstMediaContent(contents, MEDIA_TYPE_DATA);
}

ContentInfo* GetFirstMediaContent(SessionDescription* sdesc,
                                  MediaType media_type) {
  if (sdesc == nullptr) {
    return nullptr;
  }

  return GetFirstMediaContent(&sdesc->contents(), media_type);
}

ContentInfo* GetFirstAudioContent(SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_AUDIO);
}

ContentInfo* GetFirstVideoContent(SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_VIDEO);
}

ContentInfo* GetFirstDataContent(SessionDescription* sdesc) {
  return GetFirstMediaContent(sdesc, MEDIA_TYPE_DATA);
}

MediaContentDescription* GetFirstMediaContentDescription(
    SessionDescription* sdesc,
    MediaType media_type) {
  ContentInfo* content = GetFirstMediaContent(sdesc, media_type);
  return (content ? content->media_description() : nullptr);
}

AudioContentDescription* GetFirstAudioContentDescription(
    SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_AUDIO);
  return desc ? desc->as_audio() : nullptr;
}

VideoContentDescription* GetFirstVideoContentDescription(
    SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_VIDEO);
  return desc ? desc->as_video() : nullptr;
}

RtpDataContentDescription* GetFirstRtpDataContentDescription(
    SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_DATA);
  return desc ? desc->as_rtp_data() : nullptr;
}

SctpDataContentDescription* GetFirstSctpDataContentDescription(
    SessionDescription* sdesc) {
  auto desc = GetFirstMediaContentDescription(sdesc, MEDIA_TYPE_DATA);
  return desc ? desc->as_sctp() : nullptr;
}

}  // namespace cricket
