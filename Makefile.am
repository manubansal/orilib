
include $(top_srcdir)/Makefile.am.common

#--------------------------------------
# OpenRadio implementation-only library 
#--------------------------------------
noinst_LIBRARIES = lib.orilib.a	#GNU naming convention
lib_orilib_a_SOURCES = \
  WIFILIB_util.c \
  ORILIB_printDebug.c \
  ORILIB_GPIO_i.c \
  ORILIB_GPIO_input_i.c \
  ORILIB_Scrambler_Init_i.c \
  ORILIB_Scrambler24_i.c \
  ORILIB_TailZeroer24_i.c \
  ORILIB_AckPacker_i.c \
  ORILIB_ConvolutionalEncoder24_Init_i.c \
  ORILIB_ConvolutionalEncoder24_i.c \
  ORILIB_ByteReader_i.c \
  ORILIB_ByteReader_Init_i.c \
  ORILIB_EthInterfaceInit_i.c \
  ORILIB_EthReaderTimedBlocking_i.c \
  ORILIB_EthReaderBuffered_i.c \
  ORILIB_EthReaderBuffered_example_pkt_filter.c \
  ORILIB_EthReaderBuffered_Init_i.c \
  ORILIB_EthReaderSimple_i.c \
  ORILIB_EthReader_i.c \
  ORILIB_EthReader_Init_i.c \
  ORILIB_EthReader_Reinit_i.c \
  ORILIB_EthWriter_i.c \
  ORILIB_EthWriter_Init_i.c \
  ORILIB_EthWriterStatic_i.c \
  ORILIB_TraceReader_i.c \
  ORILIB_TraceReader_Init_i.c \
  ORILIB_TraceReader80_i.c \
  ORILIB_TraceReader80_Init_i.c \
  ORILIB_TraceReader80_ThrottleReinit_i.c \
  ORILIB_FileTraceReader80_i.c \
  IO_MemWriter_i.c \
  IO_MemWriter_Init_i.c \
  ORILIB_CFOEstimatorLTF_i.c \
  ORILIB_BufferJoin2To1_i.c \
  ORILIB_CFOCorrectorG_i.c \
  ORILIB_CFOCorrectorGM_i.c \
  ORILIB_CFOCorrector_algos.c \
  ORILIB_CFOCorrectorGOffset_i.c \
  ORILIB_CFOCorrectorGMOffset_i.c \
  ORILIB_ChEstimatorLTF_Float_i.c \
  ORILIB_ChEstimatorLTF_i.c \
  ORILIB_ChEstimatorLTF_algorithms.c \
  ORILIB_OFDMModulator_i.c \
  ORILIB_OFDMModulator_InitPerPacket_i.c \
  ORILIB_OFDMDemodulator_i.c \
  ORILIB_OFDMDemodulator_InitPerPacket_i.c \
  ORILIB_OFDMEqualizer_Float_i.c \
  ORILIB_OFDMEqualizer_i.c \
  ORILIB_OFDMEqualizer_algorithms.c  \
  ORILIB_SoftDemapperBPSKyhstar_i.c \
  ORILIB_SoftSlicer64QAMyhatLUT_i.c \
  ORILIB_SoftDeinterleaver48_i.c \
  ORILIB_SoftDeinterleaver288_i.c \
  ORILIB_SoftBranchMetricsR1by2F24_i.c \
  ORILIB_SoftBranchMetricsR1by2F216_i.c \
  ORILIB_ViterbiDecoder_InitOnce_i.c \
  ORILIB_ViterbiDecoder_GenConf_i.c \
  ORILIB_ViterbiDecoder_GenConfSL_i.c \
  ORILIB_ViterbiDecoder_GenConfSLip_i.c \
  ORILIB_ViterbiDecoderVCP_algorithms.c \
  ORILIB_ViterbiDecoderVCP_i.c \
  ORILIB_ViterbiDecoderVCP_Dynamic_i.c \
  ORILIB_ViterbiDecoderVCP_WaitForCompletion_i.c \
  ORILIB_ViterbiDecoderVCP_InitTrueWFC_i.c \
  ORILIB_ViterbiDecoder_CacheBranchMetricsR1by2F24_i.c \
  ORILIB_BlockTouchInOut_i.c \
  ORILIB_PLCPParser_i.c  \
  ORILIB_ByteCacheInit_C120_i.c \
  ORILIB_ByteCacheInit_C200_i.c \
  ORILIB_ByteCacheInit_C432_i.c \
  ORILIB_ByteCacheInit_C2048_i.c \
  ORILIB_ByteCacheCappedInitFPlcpState_C432_i.c \
  ORILIB_ByteCacheCappedInitFPlcpState_C2048_i.c \
  ORILIB_ByteCachePut_a8_C120_i.c \
  ORILIB_ByteCachePut_a8_C120B48_i.c \
  ORILIB_ByteCachePut_a8_C200B48_i.c \
  ORILIB_ByteCachePut_a8_C432B432_i.c \
  ORILIB_ByteCachePut_a8_C2048B432_i.c \
  ORILIB_ByteCachePut_a8_C2048_i.c \
  ORILIB_ByteCacheCappedPut_a8_C432B432_i.c \
  ORILIB_ByteCacheCappedPut_a8_C2048B432_i.c \
  ORILIB_ByteCacheGetFrame_a8_C120_i.c \
  ORILIB_ByteCacheGetFrame_a8_C200_i.c \
  ORILIB_ByteCacheGetFrame_a8_C432_i.c \
  ORILIB_ByteCacheGetFrame_a8_C2048_i.c \
  ORILIB_ByteCacheCappedGetFrame_a8_C432_i.c \
  ORILIB_ByteCacheCappedGetFrame_a8_C2048_i.c \
  WIFILIB_parameters.c \
  ORILIB_DataDecodeDecisionInit_i.c \
  ORILIB_DataDecodeDecision_i.c \
  ORILIB_DebugPrintBitBucket_i.c \
  ORILIB_BitCacheSkipPut_a8_C1024b_i.c \
  ORILIB_BitCacheGet_a8_C1024_i.c \
  ORILIB_BitCacheInit_C1024_i.c \
  ORILIB_BitCacheSkipPut_circular_a8_C1024b_i.c \
  ORILIB_BitCacheGet_circular_a8_C1024_i.c \
  ORILIB_BitCacheInit_circular_C1024_i.c \
  ORILIB_ViterbiDecodingMap_i.c \
  ORILIB_SoftDepuncturerR3by4F216_i.c \
  ORILIB_Descrambler_i.c \
  ORILIB_Descrambler288_i.c \
  ORILIB_DescramblerInit_i.c \
  ORILIB_CRC32_i.c \
  ORILIB_CRC32_VarBytes_i.c \
  ORILIB_CRC32_VarBytes_Init_i.c \
  ORILIB_CRC32_Decision_i.c \
  ORILIB_EnergyTracker_i.c \
  ORILIB_EnergyTracker_Init_i.c \
  ORILIB_SimpleEnergyTracker_i.c \
  ORILIB_SimpleEnergyTracker_Init_i.c \
  ORILIB_AutoGainController_i.c \
  ORILIB_AutoGainController_Init_i.c \
  ORILIB_WindowCorrelator_i.c \
  ORILIB_SchmidlCoxMetricComputer_i.c \
  ORILIB_PeakDetector_i.c \
  ORILIB_PreambleDetector_Init_i.c \
  ORILIB_PacketSearchDecision_i.c \
  ORILIB_PayloadParser_i.c \
  ORILIB_CNRTracker_i.c \
  ORILIB_CNRTrackerIN_i.c \
  ORILIB_BPSKEVMEstimator_i.c \
  ORILIB_ChannelImpulseResponse_i.c \
  ORILIB_BufferAlign_inner.c \
  ORILIB_BufferAlign_i.c \
  ORILIB_BufferAlign_Init_i.c \
  ORILIB_BufferAlignOffset_i.c \
  ORILIB_beta_BufferAlign_inner.c \
  ORILIB_beta_BufferAlign_INP96_OUT80_i.c \
  ORILIB_beta_BufferAlign_Init_i.c \
  ORILIB_beta_BufferAlignOffset_INP96_OUT80_i.c \
  ORILIB_TraceReaderN_i.c \
  ORILIB_TraceReaderN_Init_i.c \
  ORILIB_TraceReaderN_ThrottleReinit_i.c \
  ORILIB_Wait_i.c \
  ORILIB_Interleaver48_i.c \
  ORILIB_PuncturerOneHalf_i.c \
  ORILIB_MapperBPSK_i.c \
  ORILIB_Interleaver48.c \
  WIFILIB_lookupTables.c \
  ORILIB_SoftDeinterleaver_algorithms.c \
  ORILIB_SoftBranchMetricsR1by2_algorithms.c \
  ORILIB_SoftDepuncturer_algos.c \
  ORILIB_Descrambler_algorithms.c \
  ORILIB_CRC32_algos.c \
  dsplib_twiddles/gen_twiddle_fft16x16.c \
  dsplib_twiddles/gen_twiddle_ifft16x16.c  \
  iqmath/IQmathTables.c
#
#the two dsplib_twiddles files are added in the build because dsplib 3_2_0_1 build
#does not include those objects. 

#  ORILIB_ViterbiDecoderVCP_InitIdx_i.c \
#

dist_doc_DATA = README.md

