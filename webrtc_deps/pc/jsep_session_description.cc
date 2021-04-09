/*
 *  Copyright 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/jsep_session_description.h"

#include <memory>

#include "p2p/base/port.h"
#include "pc/session_description.h"
#include "pc/webrtc_sdp.h"
#include "rtc_base/arraysize.h"

using cricket::SessionDescription;

namespace webrtc {

const int JsepSessionDescription::kDefaultVideoCodecId = 100;
const char JsepSessionDescription::kDefaultVideoCodecName[] = "VP8";

// TODO(steveanton): Remove this default implementation once Chromium has been
// updated.
SdpType SessionDescriptionInterface::GetType() const {
  absl::optional<SdpType> maybe_type = SdpTypeFromString(type());
  if (maybe_type) {
    return *maybe_type;
  } else {
    RTC_LOG(LS_WARNING) << "Default implementation of "
                           "SessionDescriptionInterface::GetType does not "
                           "recognize the result from type(), returning "
                           "kOffer.";
    return SdpType::kOffer;
  }
}

SessionDescriptionInterface* CreateSessionDescription(const std::string& type,
                                                      const std::string& sdp,
                                                      SdpParseError* error) {
  absl::optional<SdpType> maybe_type = SdpTypeFromString(type);
  if (!maybe_type) {
    return nullptr;
  }

  return CreateSessionDescription(*maybe_type, sdp, error).release();
}

std::unique_ptr<SessionDescriptionInterface> CreateSessionDescription(
    SdpType type,
    const std::string& sdp) {
  return CreateSessionDescription(type, sdp, nullptr);
}

std::unique_ptr<SessionDescriptionInterface> CreateSessionDescription(
    SdpType type,
    const std::string& sdp,
    SdpParseError* error_out) {
  auto jsep_desc = std::make_unique<JsepSessionDescription>(type);
  if (type != SdpType::kRollback) {
    if (!SdpDeserialize(sdp, jsep_desc.get(), error_out)) {
      return nullptr;
    }
  }
  return std::move(jsep_desc);
}

std::unique_ptr<SessionDescriptionInterface> CreateSessionDescription(
    SdpType type,
    const std::string& session_id,
    const std::string& session_version,
    std::unique_ptr<cricket::SessionDescription> description) {
  auto jsep_description = std::make_unique<JsepSessionDescription>(type);
  bool initialize_success = jsep_description->Initialize(
      std::move(description), session_id, session_version);
  RTC_DCHECK(initialize_success);
  return std::move(jsep_description);
}

JsepSessionDescription::JsepSessionDescription(SdpType type) : type_(type) {}

JsepSessionDescription::JsepSessionDescription(const std::string& type) {
  absl::optional<SdpType> maybe_type = SdpTypeFromString(type);
  if (maybe_type) {
    type_ = *maybe_type;
  } else {
    RTC_LOG(LS_WARNING)
        << "JsepSessionDescription constructed with invalid type string: "
        << type << ". Assuming it is an offer.";
    type_ = SdpType::kOffer;
  }
}

JsepSessionDescription::JsepSessionDescription(
    SdpType type,
    std::unique_ptr<cricket::SessionDescription> description,
    absl::string_view session_id,
    absl::string_view session_version)
    : description_(std::move(description)),
      session_id_(session_id),
      session_version_(session_version),
      type_(type) {
  RTC_DCHECK(description_);
  candidate_collection_.resize(number_of_mediasections());
}

JsepSessionDescription::~JsepSessionDescription() {}

bool JsepSessionDescription::Initialize(
    std::unique_ptr<cricket::SessionDescription> description,
    const std::string& session_id,
    const std::string& session_version) {
  if (!description)
    return false;

  session_id_ = session_id;
  session_version_ = session_version;
  description_ = std::move(description);
  candidate_collection_.resize(number_of_mediasections());
  return true;
}

bool JsepSessionDescription::AddCandidate(const IceCandidateInterface *candidate)
{
    return false;
}

size_t JsepSessionDescription::RemoveCandidates(const std::vector<cricket::Candidate> &candidates)
{
    return -1;
}

size_t JsepSessionDescription::number_of_mediasections() const
{
  if (!description_)
    return 0;
  return description_->contents().size();
}


const IceCandidateCollection* JsepSessionDescription::candidates(
    size_t mediasection_index) const {
  if (mediasection_index >= candidate_collection_.size())
    return NULL;
  return &candidate_collection_[mediasection_index];
}

bool JsepSessionDescription::ToString(std::string* out) const {
  if (!description_ || !out) {
    return false;
  }
  *out = SdpSerialize(*this);
  return !out->empty();

}

int JsepSessionDescription::GetMediasectionIndex(
    const cricket::Candidate& candidate) {
  return -1;
}

}  // namespace webrtc
