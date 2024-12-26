cmake_minimum_required(VERSION 3.5)

####################################################################
project(webrtc_deps)

set(SRCS
  #pc/jsep_session_description.cc
  #pc/webrtc_sdp.cc
  #pc/media_session.cc
  ${WEBRTC_INCLUDE_DIRS}/absl_inl/absl/absl/strings/ascii.cc
)

add_library(${PROJECT_NAME} STATIC ${SRCS})


####################################################################
project(webrtc_p2p)

set(P2P_SRCS 
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/async_stun_tcp_socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/basic_async_resolver_factory.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/basic_ice_controller.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/basic_packet_socket_factory.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/connection.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/connection_info.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/default_ice_transport_factory.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/dtls/dtls_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/dtls/dtls_transport_internal.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/ice_controller_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/ice_credentials_iterator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/ice_switch_reason.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/ice_transport_internal.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/p2p_constants.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/p2p_transport_channel.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/packet_transport_internal.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/port_allocator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/port.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/port_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/pseudo_tcp.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/regathering_controller.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/stun_port.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/stun_request.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/stun_server.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/stun_dictionary.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/tcp_port.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/transport_description.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/transport_description_factory.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/turn_port.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/turn_server.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/base/wrapping_active_ice_controller.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/client/basic_port_allocator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/client/turn_port_factory.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/p2p/stunprober/stun_prober.cc
)

add_library(${PROJECT_NAME} STATIC ${P2P_SRCS})


####################################################################
project(webrtc_api)

set(API_SRCS 
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/audio_options.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/candidate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/dtls_transport_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/field_trials_registry.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/frame_transformer_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/jsep.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/jsep_ice_candidate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtc_error.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_parameters.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/color_space.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/encoded_image.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/encoded_frame.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/hdr_metadata.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/i444_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/i420_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/i422_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/nv12_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_frame.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_frame_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_frame.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_source_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_bitrate_allocation.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_content_type.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_timing.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_bitrate_allocator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video/video_frame_metadata.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/av1_profile.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/h264_profile_level_id.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/scalability_mode.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/sdp_video_format.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/simulcast_stream.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/video_decoder.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/video_encoder.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/video_codec.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/video_codec.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/video_codecs/vp9_profile.cc
  
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_parameters.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/media_types.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/units/data_rate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/units/data_size.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/units/frequency.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtc_error.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_transceiver_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_receiver_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_sender_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/media_stream_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_packet_info.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/audio_options.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtc_event_log/rtc_event.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/rtp_headers.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/units/timestamp.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/units/time_delta.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/task_queue/task_queue_base.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/task_queue/pending_task_safety_flag.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/transport/network_types.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/transport/stun.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/transport/field_trial_based_config.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/transport/rtp/dependency_descriptor.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/crypto/crypto_options.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/dtls_transport_interface.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/candidate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/jsep.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/jsep_ice_candidate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/audio_codecs/audio_format.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/api/call/transport.cc
)
add_library(${PROJECT_NAME} STATIC ${API_SRCS})


####################################################################
project(webrtc_common_media)

set(COMMON_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/call/rtp_demuxer.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//generic_frame_descriptor/generic_frame_info.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//h264/h264_bitstream_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//h264/h264_common.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//h264/pps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//h264/sps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video//h264/sps_vui_rewriter.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/codec.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/media_constants.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/rid_description.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/rtp_utils.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/stream_params.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/sdp_video_format_utils.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/media/base/turn_utils.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/generic_frame_descriptor/generic_frame_info.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/h264_bitstream_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/h264_common.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/pps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/sps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/sps_vui_rewriter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/generic_frame_descriptor/generic_frame_info.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/h264_bitstream_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/h264_common.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/pps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/sps_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/common_video/h264/sps_vui_rewriter.cc
)
add_library(${PROJECT_NAME} STATIC ${COMMON_SRCS})


####################################################################
project(webrtc_modules)

set(MODULES_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/video_coding/encoded_frame.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/video_coding/include/video_codec_interface.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/include/report_block_data.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/include/rtp_rtcp_defines.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/absolute_capture_time_interpolator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/absolute_capture_time_sender.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/create_video_rtp_depacketizer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/active_decode_targets_helper.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/dtmf_queue.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/fec_private_tables_bursty.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/fec_private_tables_random.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/flexfec_header_reader_writer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/flexfec_receiver.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/flexfec_sender.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/flexfec_03_header_reader_writer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/forward_error_correction.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/forward_error_correction_internal.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/leb128.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/ntp_time_util.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/packet_loss_stats.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/receive_statistics_impl.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/remote_ntp_time_estimator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_nack_stats.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/app.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/bye.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/common_header.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/compound_packet.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/congestion_control_feedback.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/dlrr.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/extended_reports.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/fir.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/loss_notification.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/nack.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/pli.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/psfb.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/rapid_resync_request.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/receiver_report.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/remb.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/remote_estimate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/report_block.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/rrtr.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/rtpfb.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/sdes.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/sender_report.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/packet_sequencer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/target_bitrate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmb_item.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmbn.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmbr.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_packet/transport_feedback.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_receiver.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtcp_sender.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_extension.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_reader.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_writer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_descriptor_authentication.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_format.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_format_h264.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_format_video_generic.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_format_vp8.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_format_vp9.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_generic_frame_descriptor.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_generic_frame_descriptor_extension.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_header_extension_map.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_header_extension_size.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_header_extensions.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_packet.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_packet_history.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_packet_received.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_packet_to_send.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_packetizer_av1.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_rtcp_impl.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sender.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sender_audio.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sender_egress.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sender_video.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sender_video_frame_transformer_delegate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_sequence_number_map.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_util.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_video_header.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/rtp_video_layers_allocation_extension.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/source_tracker.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/tmmbr_help.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/ulpfec_generator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/ulpfec_header_reader_writer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_av1.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_generic.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_h264.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_raw.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_vp8.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_vp9.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/modules/rtp_rtcp/source/deprecated/deprecated_rtp_sender_egress.cc
)
add_library(${PROJECT_NAME} STATIC ${MODULES_SRCS})


####################################################################
project(webrtc_pc)

set(PC_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/dtls_srtp_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/dtls_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/external_hmac.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/ice_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/jsep_ice_candidate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/jsep_session_description.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/media_protocol_names.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/media_session.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/rtp_media_utils.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/rtp_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/session_description.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/simulcast_description.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/simulcast_sdp_serializer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/srtp_session.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/srtp_transport.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/pc/webrtc_sdp.cc
  )
add_library(${PROJECT_NAME} STATIC ${PC_SRCS})


####################################################################
project(webrtc_logging)

set(LOGGING_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/ice_logger.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_ice_candidate_pair.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_ice_candidate_pair_config.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_dtls_transport_state.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_dtls_writable_state.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_rtcp_packet_incoming.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_rtcp_packet_outgoing.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_rtp_packet_incoming.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/logging/rtc_event_log/events/rtc_event_rtp_packet_outgoing.cc
)
add_library(${PROJECT_NAME} STATIC ${LOGGING_SRCS})


####################################################################
project(webrtc_system_wrappers)

set(SYSTEM_WRAPPERS_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/system_wrappers/source/clock.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/system_wrappers/source/metrics.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/system_wrappers/source/rtp_to_ntp_estimator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/system_wrappers/source/field_trial.cc

)
add_library(${PROJECT_NAME} STATIC ${SYSTEM_WRAPPERS_SRCS})


####################################################################
project(webrtc_base)

set(BASE_SRCS
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/async_dns_resolver.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/bitstream_reader.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/bitrate_tracker.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/callback_list.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/copy_on_write_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/checks.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/crypto_random.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/frequency_tracker.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_key_pair.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_digest.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_stream_adapter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_identity.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_certificate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_utility.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_adapter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/openssl_session_cache.cc
  # ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/socket_server.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/system_time.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/synchronization/yield_policy.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/synchronization/sequence_checker_internal.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/rate_statistics.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/socket_address.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ip_address.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/logging.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/strings/string_builder.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/string_encode.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/string_to_number.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/thread.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/platform_thread_types.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/physical_socket_server.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/async_socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/async_tcp_socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/async_udp_socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/socket_adapters.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/async_packet_socket.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network/sent_packet.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/time_utils.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/buffer_queue.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ssl_stream_adapter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/stream.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ssl_certificate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ssl_fingerprint.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network_constants.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/rtc_certificate.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/numerics/event_based_exponential_moving_average.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/rate_tracker.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/byte_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network_monitor.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/net_helper.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/weak_ptr.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network_route.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/network/received_packet.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ssl_identity.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/event.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/unique_id_generator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/bit_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/message_digest.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/race_checker.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/random.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/event_tracer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/rate_limiter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/third_party/base64/base64.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/string_utils.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/rtc_certificate_generator.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/platform_thread.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/null_socket_server.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/net_helpers.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ssl_adapter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/ifaddrs_converter.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/crc32.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/zero_memory.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/experiments/struct_parameters_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/experiments/field_trial_parser.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/experiments/field_trial_units.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/memory/fifo_buffer.cc
  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/memory/aligned_malloc.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/task_utils/repeating_task.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/internal/default_socket_server.cc

  ${WEBRTC_INCLUDE_DIRS}/webrtc/rtc_base/deprecated/recursive_critical_section.cc
)

add_library(${PROJECT_NAME} STATIC ${BASE_SRCS})

####################################################################

