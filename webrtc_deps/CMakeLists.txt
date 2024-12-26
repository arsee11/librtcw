cmake_minimum_required(VERSION 3.5)

####################################################################
project(webrtc_deps)

add_subdirectory(third_party/libsrtp)
add_subdirectory(third_party/libyuv)

set(SRCS
  #pc/jsep_session_description.cc
  #pc/webrtc_sdp.cc
  #pc/media_session.cc
  absl_inl/absl/absl/strings/ascii.cc
)

add_library(${PROJECT_NAME} STATIC ${SRCS})


####################################################################
project(webrtc_p2p)

set(P2P_SRCS 
  webrtc/p2p/base/async_stun_tcp_socket.cc
  webrtc/p2p/base/basic_async_resolver_factory.cc
  webrtc/p2p/base/basic_ice_controller.cc
  webrtc/p2p/base/basic_packet_socket_factory.cc
  webrtc/p2p/base/connection.cc
  webrtc/p2p/base/connection_info.cc
  webrtc/p2p/base/default_ice_transport_factory.cc
  webrtc/p2p/dtls/dtls_transport.cc
  webrtc/p2p/dtls/dtls_transport_internal.cc
  webrtc/p2p/base/ice_controller_interface.cc
  webrtc/p2p/base/ice_credentials_iterator.cc
  webrtc/p2p/base/ice_switch_reason.cc
  webrtc/p2p/base/ice_transport_internal.cc
  webrtc/p2p/base/p2p_constants.cc
  webrtc/p2p/base/p2p_transport_channel.cc
  webrtc/p2p/base/packet_transport_internal.cc
  webrtc/p2p/base/port_allocator.cc
  webrtc/p2p/base/port.cc
  webrtc/p2p/base/port_interface.cc
  webrtc/p2p/base/pseudo_tcp.cc
  webrtc/p2p/base/regathering_controller.cc
  webrtc/p2p/base/stun_port.cc
  webrtc/p2p/base/stun_request.cc
  webrtc/p2p/base/stun_server.cc
  webrtc/p2p/base/stun_dictionary.cc
  webrtc/p2p/base/tcp_port.cc
  webrtc/p2p/base/transport_description.cc
  webrtc/p2p/base/transport_description_factory.cc
  webrtc/p2p/base/turn_port.cc
  webrtc/p2p/base/turn_server.cc
  webrtc/p2p/base/wrapping_active_ice_controller.cc
  webrtc/p2p/client/basic_port_allocator.cc
  webrtc/p2p/client/turn_port_factory.cc
  webrtc/p2p/stunprober/stun_prober.cc
)

add_library(${PROJECT_NAME} STATIC ${P2P_SRCS})


####################################################################
project(webrtc_api)

set(API_SRCS 
  webrtc/api/audio_options.cc
  webrtc/api/candidate.cc
  webrtc/api/dtls_transport_interface.cc
  webrtc/api/field_trials_registry.cc
  webrtc/api/frame_transformer_interface.cc
  webrtc/api/jsep.cc
  webrtc/api/jsep_ice_candidate.cc
  webrtc/api/rtc_error.cc
  webrtc/api/rtp_parameters.cc

  webrtc/api/video/color_space.cc
  webrtc/api/video/encoded_image.cc
  webrtc/api/video/encoded_frame.cc
  webrtc/api/video/hdr_metadata.cc
  webrtc/api/video/i444_buffer.cc
  webrtc/api/video/i420_buffer.cc
  webrtc/api/video/i422_buffer.cc
  webrtc/api/video/nv12_buffer.cc
  webrtc/api/video/video_frame.cc
  webrtc/api/video/video_frame_buffer.cc
  webrtc/api/video/video_frame.cc
  webrtc/api/video/video_source_interface.cc
  webrtc/api/video/video_bitrate_allocation.cc
  webrtc/api/video/video_content_type.cc
  webrtc/api/video/video_timing.cc
  webrtc/api/video/video_bitrate_allocator.cc
  webrtc/api/video/video_frame_metadata.cc

  webrtc/api/video_codecs/av1_profile.cc
  webrtc/api/video_codecs/h264_profile_level_id.cc
  webrtc/api/video_codecs/scalability_mode.cc
  webrtc/api/video_codecs/sdp_video_format.cc
  webrtc/api/video_codecs/simulcast_stream.cc
  webrtc/api/video_codecs/video_decoder.cc
  webrtc/api/video_codecs/video_encoder.cc
  webrtc/api/video_codecs/video_codec.cc
  webrtc/api/video_codecs/video_codec.cc
  webrtc/api/video_codecs/vp9_profile.cc
  
  webrtc/api/rtp_parameters.cc
  webrtc/api/media_types.cc
  webrtc/api/units/data_rate.cc
  webrtc/api/units/data_size.cc
  webrtc/api/units/frequency.cc
  webrtc/api/rtc_error.cc
  webrtc/api/rtp_transceiver_interface.cc
  webrtc/api/rtp_receiver_interface.cc
  webrtc/api/rtp_sender_interface.cc
  webrtc/api/media_stream_interface.cc
  webrtc/api/rtp_packet_info.cc
  webrtc/api/audio_options.cc
  webrtc/api/rtc_event_log/rtc_event.cc
  webrtc/api/rtp_headers.cc
  webrtc/api/units/timestamp.cc
  webrtc/api/units/time_delta.cc
  webrtc/api/task_queue/task_queue_base.cc
  webrtc/api/task_queue/pending_task_safety_flag.cc
  webrtc/api/transport/network_types.cc
  webrtc/api/transport/stun.cc
  webrtc/api/transport/field_trial_based_config.cc
  webrtc/api/transport/rtp/dependency_descriptor.cc
  webrtc/api/crypto/crypto_options.cc
  webrtc/api/dtls_transport_interface.cc
  webrtc/api/candidate.cc
  webrtc/api/jsep.cc
  webrtc/api/jsep_ice_candidate.cc
  webrtc/api/audio_codecs/audio_format.cc
  webrtc/api/call/transport.cc
)
add_library(${PROJECT_NAME} STATIC ${API_SRCS})


####################################################################
project(webrtc_common_media)

set(COMMON_SRCS
  webrtc/call/rtp_demuxer.cc

  webrtc/common_video//generic_frame_descriptor/generic_frame_info.cc
  webrtc/common_video//h264/h264_bitstream_parser.cc
  webrtc/common_video//h264/h264_common.cc
  webrtc/common_video//h264/pps_parser.cc
  webrtc/common_video//h264/sps_parser.cc
  webrtc/common_video//h264/sps_vui_rewriter.cc

  webrtc/media/base/codec.cc
  webrtc/media/base/media_constants.cc
  webrtc/media/base/rid_description.cc
  webrtc/media/base/rtp_utils.cc
  webrtc/media/base/stream_params.cc
  webrtc/media/base/sdp_video_format_utils.cc
  webrtc/media/base/turn_utils.cc

  webrtc/common_video/generic_frame_descriptor/generic_frame_info.cc
  webrtc/common_video/h264/h264_bitstream_parser.cc
  webrtc/common_video/h264/h264_common.cc
  webrtc/common_video/h264/pps_parser.cc
  webrtc/common_video/h264/sps_parser.cc
  webrtc/common_video/h264/sps_vui_rewriter.cc
  webrtc/common_video/generic_frame_descriptor/generic_frame_info.cc
  webrtc/common_video/h264/h264_bitstream_parser.cc
  webrtc/common_video/h264/h264_common.cc
  webrtc/common_video/h264/pps_parser.cc
  webrtc/common_video/h264/sps_parser.cc
  webrtc/common_video/h264/sps_vui_rewriter.cc
)
add_library(${PROJECT_NAME} STATIC ${COMMON_SRCS})


####################################################################
project(webrtc_modules)

set(MODULES_SRCS
  webrtc/modules/video_coding/encoded_frame.cc
  webrtc/modules/video_coding/include/video_codec_interface.cc

  webrtc/modules/rtp_rtcp/include/report_block_data.cc
  webrtc/modules/rtp_rtcp/include/rtp_rtcp_defines.cc
  webrtc/modules/rtp_rtcp/source/absolute_capture_time_interpolator.cc
  webrtc/modules/rtp_rtcp/source/absolute_capture_time_sender.cc
  webrtc/modules/rtp_rtcp/source/create_video_rtp_depacketizer.cc
  webrtc/modules/rtp_rtcp/source/active_decode_targets_helper.cc
  webrtc/modules/rtp_rtcp/source/dtmf_queue.cc
  webrtc/modules/rtp_rtcp/source/fec_private_tables_bursty.cc
  webrtc/modules/rtp_rtcp/source/fec_private_tables_random.cc
  webrtc/modules/rtp_rtcp/source/flexfec_header_reader_writer.cc
  webrtc/modules/rtp_rtcp/source/flexfec_receiver.cc
  webrtc/modules/rtp_rtcp/source/flexfec_sender.cc
  webrtc/modules/rtp_rtcp/source/flexfec_03_header_reader_writer.cc
  webrtc/modules/rtp_rtcp/source/forward_error_correction.cc
  webrtc/modules/rtp_rtcp/source/forward_error_correction_internal.cc
  webrtc/modules/rtp_rtcp/source/leb128.cc
  webrtc/modules/rtp_rtcp/source/ntp_time_util.cc
  webrtc/modules/rtp_rtcp/source/packet_loss_stats.cc
  webrtc/modules/rtp_rtcp/source/receive_statistics_impl.cc
  webrtc/modules/rtp_rtcp/source/remote_ntp_time_estimator.cc
  webrtc/modules/rtp_rtcp/source/rtcp_nack_stats.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/app.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/bye.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/common_header.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/compound_packet.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/congestion_control_feedback.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/dlrr.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/extended_reports.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/fir.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/loss_notification.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/nack.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/pli.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/psfb.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/rapid_resync_request.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/receiver_report.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/remb.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/remote_estimate.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/report_block.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/rrtr.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/rtpfb.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/sdes.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/sender_report.cc
  webrtc/modules/rtp_rtcp/source/packet_sequencer.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/target_bitrate.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmb_item.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmbn.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/tmmbr.cc
  webrtc/modules/rtp_rtcp/source/rtcp_packet/transport_feedback.cc
  webrtc/modules/rtp_rtcp/source/rtcp_receiver.cc
  webrtc/modules/rtp_rtcp/source/rtcp_sender.cc
  webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_extension.cc
  webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_reader.cc
  webrtc/modules/rtp_rtcp/source/rtp_dependency_descriptor_writer.cc
  webrtc/modules/rtp_rtcp/source/rtp_descriptor_authentication.cc
  webrtc/modules/rtp_rtcp/source/rtp_format.cc
  webrtc/modules/rtp_rtcp/source/rtp_format_h264.cc
  webrtc/modules/rtp_rtcp/source/rtp_format_video_generic.cc
  webrtc/modules/rtp_rtcp/source/rtp_format_vp8.cc
  webrtc/modules/rtp_rtcp/source/rtp_format_vp9.cc
  webrtc/modules/rtp_rtcp/source/rtp_generic_frame_descriptor.cc
  webrtc/modules/rtp_rtcp/source/rtp_generic_frame_descriptor_extension.cc
  webrtc/modules/rtp_rtcp/source/rtp_header_extension_map.cc
  webrtc/modules/rtp_rtcp/source/rtp_header_extension_size.cc
  webrtc/modules/rtp_rtcp/source/rtp_header_extensions.cc
  webrtc/modules/rtp_rtcp/source/rtp_packet.cc
  webrtc/modules/rtp_rtcp/source/rtp_packet_history.cc
  webrtc/modules/rtp_rtcp/source/rtp_packet_received.cc
  webrtc/modules/rtp_rtcp/source/rtp_packet_to_send.cc
  webrtc/modules/rtp_rtcp/source/rtp_packetizer_av1.cc
  webrtc/modules/rtp_rtcp/source/rtp_rtcp_impl.cc
  webrtc/modules/rtp_rtcp/source/rtp_sender.cc
  webrtc/modules/rtp_rtcp/source/rtp_sender_audio.cc
  webrtc/modules/rtp_rtcp/source/rtp_sender_egress.cc
  webrtc/modules/rtp_rtcp/source/rtp_sender_video.cc
  webrtc/modules/rtp_rtcp/source/rtp_sender_video_frame_transformer_delegate.cc
  webrtc/modules/rtp_rtcp/source/rtp_sequence_number_map.cc
  webrtc/modules/rtp_rtcp/source/rtp_util.cc
  webrtc/modules/rtp_rtcp/source/rtp_video_header.cc
  webrtc/modules/rtp_rtcp/source/rtp_video_layers_allocation_extension.cc
  webrtc/modules/rtp_rtcp/source/source_tracker.cc
  webrtc/modules/rtp_rtcp/source/tmmbr_help.cc
  webrtc/modules/rtp_rtcp/source/ulpfec_generator.cc
  webrtc/modules/rtp_rtcp/source/ulpfec_header_reader_writer.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_av1.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_generic.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_h264.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_raw.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_vp8.cc
  webrtc/modules/rtp_rtcp/source/video_rtp_depacketizer_vp9.cc

  webrtc/modules/rtp_rtcp/source/deprecated/deprecated_rtp_sender_egress.cc
)
add_library(${PROJECT_NAME} STATIC ${MODULES_SRCS})


####################################################################
project(webrtc_pc)

set(PC_SRCS
  webrtc/pc/dtls_srtp_transport.cc
  webrtc/pc/dtls_transport.cc
  webrtc/pc/external_hmac.cc
  webrtc/pc/ice_transport.cc
  webrtc/pc/jsep_ice_candidate.cc
  webrtc/pc/jsep_session_description.cc
  webrtc/pc/media_protocol_names.cc
  webrtc/pc/media_session.cc
  webrtc/pc/rtp_media_utils.cc
  webrtc/pc/rtp_transport.cc
  webrtc/pc/session_description.cc
  webrtc/pc/simulcast_description.cc
  webrtc/pc/simulcast_sdp_serializer.cc
  webrtc/pc/srtp_session.cc
  webrtc/pc/srtp_transport.cc
  webrtc/pc/webrtc_sdp.cc
  )
add_library(${PROJECT_NAME} STATIC ${PC_SRCS})


####################################################################
project(webrtc_logging)

set(LOGGING_SRCS
  webrtc/logging/rtc_event_log/ice_logger.cc
  webrtc/logging/rtc_event_log/events/rtc_event_ice_candidate_pair.cc
  webrtc/logging/rtc_event_log/events/rtc_event_ice_candidate_pair_config.cc
  webrtc/logging/rtc_event_log/events/rtc_event_dtls_transport_state.cc
  webrtc/logging/rtc_event_log/events/rtc_event_dtls_writable_state.cc
  webrtc/logging/rtc_event_log/events/rtc_event_rtcp_packet_incoming.cc
  webrtc/logging/rtc_event_log/events/rtc_event_rtcp_packet_outgoing.cc
  webrtc/logging/rtc_event_log/events/rtc_event_rtp_packet_incoming.cc
  webrtc/logging/rtc_event_log/events/rtc_event_rtp_packet_outgoing.cc
)
add_library(${PROJECT_NAME} STATIC ${LOGGING_SRCS})


####################################################################
project(webrtc_system_wrappers)

set(SYSTEM_WRAPPERS_SRCS
  webrtc/system_wrappers/source/clock.cc
  webrtc/system_wrappers/source/metrics.cc
  webrtc/system_wrappers/source/rtp_to_ntp_estimator.cc
  webrtc/system_wrappers/source/field_trial.cc

)
add_library(${PROJECT_NAME} STATIC ${SYSTEM_WRAPPERS_SRCS})


####################################################################
project(webrtc_base)

set(BASE_SRCS
  webrtc/rtc_base/async_dns_resolver.cc
  webrtc/rtc_base/bitstream_reader.cc
  webrtc/rtc_base/bitrate_tracker.cc
  webrtc/rtc_base/callback_list.cc
  webrtc/rtc_base/copy_on_write_buffer.cc
  webrtc/rtc_base/checks.cc
  webrtc/rtc_base/crypto_random.cc
  webrtc/rtc_base/frequency_tracker.cc
  webrtc/rtc_base/openssl_key_pair.cc
  webrtc/rtc_base/openssl_digest.cc
  webrtc/rtc_base/openssl_stream_adapter.cc
  webrtc/rtc_base/openssl_identity.cc
  webrtc/rtc_base/openssl_certificate.cc
  webrtc/rtc_base/openssl_utility.cc
  webrtc/rtc_base/openssl_adapter.cc
  webrtc/rtc_base/openssl_session_cache.cc
  # webrtc/rtc_base/socket_server.cc
  webrtc/rtc_base/system_time.cc
  webrtc/rtc_base/synchronization/yield_policy.cc
  webrtc/rtc_base/synchronization/sequence_checker_internal.cc
  webrtc/rtc_base/rate_statistics.cc
  webrtc/rtc_base/socket_address.cc
  webrtc/rtc_base/ip_address.cc
  webrtc/rtc_base/logging.cc
  webrtc/rtc_base/strings/string_builder.cc
  webrtc/rtc_base/string_encode.cc
  webrtc/rtc_base/string_to_number.cc
  webrtc/rtc_base/thread.cc
  webrtc/rtc_base/platform_thread_types.cc
  webrtc/rtc_base/physical_socket_server.cc
  webrtc/rtc_base/async_socket.cc
  webrtc/rtc_base/async_tcp_socket.cc
  webrtc/rtc_base/async_udp_socket.cc
  webrtc/rtc_base/socket.cc
  webrtc/rtc_base/socket_adapters.cc
  webrtc/rtc_base/async_packet_socket.cc
  webrtc/rtc_base/network/sent_packet.cc
  webrtc/rtc_base/time_utils.cc
  webrtc/rtc_base/buffer_queue.cc
  webrtc/rtc_base/ssl_stream_adapter.cc
  webrtc/rtc_base/stream.cc
  webrtc/rtc_base/ssl_certificate.cc
  webrtc/rtc_base/ssl_fingerprint.cc
  webrtc/rtc_base/network_constants.cc
  webrtc/rtc_base/rtc_certificate.cc
  webrtc/rtc_base/network.cc
  webrtc/rtc_base/numerics/event_based_exponential_moving_average.cc
  webrtc/rtc_base/rate_tracker.cc
  webrtc/rtc_base/byte_buffer.cc
  webrtc/rtc_base/network_monitor.cc
  webrtc/rtc_base/net_helper.cc
  webrtc/rtc_base/weak_ptr.cc
  webrtc/rtc_base/network_route.cc
  webrtc/rtc_base/network/received_packet.cc
  webrtc/rtc_base/ssl_identity.cc
  webrtc/rtc_base/event.cc
  webrtc/rtc_base/unique_id_generator.cc
  webrtc/rtc_base/bit_buffer.cc
  webrtc/rtc_base/message_digest.cc
  webrtc/rtc_base/race_checker.cc
  webrtc/rtc_base/random.cc
  webrtc/rtc_base/event_tracer.cc
  webrtc/rtc_base/rate_limiter.cc
  webrtc/rtc_base/third_party/base64/base64.cc
  webrtc/rtc_base/string_utils.cc
  webrtc/rtc_base/rtc_certificate_generator.cc
  webrtc/rtc_base/platform_thread.cc
  webrtc/rtc_base/null_socket_server.cc
  webrtc/rtc_base/net_helpers.cc
  webrtc/rtc_base/ssl_adapter.cc
  webrtc/rtc_base/ifaddrs_converter.cc
  webrtc/rtc_base/crc32.cc
  webrtc/rtc_base/zero_memory.cc

  webrtc/rtc_base/experiments/struct_parameters_parser.cc
  webrtc/rtc_base/experiments/field_trial_parser.cc
  webrtc/rtc_base/experiments/field_trial_units.cc

  webrtc/rtc_base/memory/fifo_buffer.cc
  webrtc/rtc_base/memory/aligned_malloc.cc

  webrtc/rtc_base/task_utils/repeating_task.cc

  webrtc/rtc_base/internal/default_socket_server.cc

  webrtc/rtc_base/deprecated/recursive_critical_section.cc
)

add_library(${PROJECT_NAME} STATIC ${BASE_SRCS})

####################################################################

