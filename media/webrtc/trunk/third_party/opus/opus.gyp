# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'targets': [
    {
      'target_name': 'opus',
      'type': '<(library)',
      'defines': [
        'OPUS_BUILD',
        'FLOATING_POINT',
        'VAR_ARRAYS',
      ],
      'conditions': [
        ['OS=="linux"', {
          'cflags': [
            '-std=c99',
          ],
          'cflags_mozilla': [
            '-std=c99',
          ],
          'defines': [
            'HAVE_LRINTF',
          ],
        }]
      ],

      'include_dirs': [
        'source/include',
        'source/src',
        'source/celt',
        'source/silk',
        'source/silk/float',
      ],
      'sources': [
        # opus wrapper/glue
        'source/src/opus.c',
        'source/src/opus_decoder.c',
        'source/src/opus_encoder.c',
        'source/src/repacketizer.c',

        # celt sub-codec
        'source/celt/bands.c',
        'source/celt/celt.c',
        'source/celt/cwrs.c',
        'source/celt/entcode.c',
        'source/celt/entdec.c',
        'source/celt/entenc.c',
        'source/celt/kiss_fft.c',
        'source/celt/laplace.c',
        'source/celt/mathops.c',
        'source/celt/mdct.c',
        'source/celt/modes.c',
        'source/celt/pitch.c',
        'source/celt/celt_lpc.c',
        'source/celt/quant_bands.c',
        'source/celt/rate.c',
        'source/celt/vq.c',

        # silk sub-codec
        'source/silk/CNG.c',
        'source/silk/code_signs.c',
        'source/silk/init_decoder.c',
        'source/silk/decode_core.c',
        'source/silk/decode_frame.c',
        'source/silk/decode_parameters.c',
        'source/silk/decode_indices.c',
        'source/silk/decode_pulses.c',
        'source/silk/decoder_set_fs.c',
        'source/silk/dec_API.c',
        'source/silk/enc_API.c',
        'source/silk/encode_indices.c',
        'source/silk/encode_pulses.c',
        'source/silk/gain_quant.c',
        'source/silk/interpolate.c',
        'source/silk/LP_variable_cutoff.c',
        'source/silk/NLSF_decode.c',
        'source/silk/NSQ.c',
        'source/silk/NSQ_del_dec.c',
        'source/silk/PLC.c',
        'source/silk/shell_coder.c',
        'source/silk/tables_gain.c',
        'source/silk/tables_LTP.c',
        'source/silk/tables_NLSF_CB_NB_MB.c',
        'source/silk/tables_NLSF_CB_WB.c',
        'source/silk/tables_other.c',
        'source/silk/tables_pitch_lag.c',
        'source/silk/tables_pulses_per_block.c',
        'source/silk/VAD.c',
        'source/silk/control_audio_bandwidth.c',
        'source/silk/quant_LTP_gains.c',
        'source/silk/VQ_WMat_EC.c',
        'source/silk/HP_variable_cutoff.c',
        'source/silk/NLSF_encode.c',
        'source/silk/NLSF_VQ.c',
        'source/silk/NLSF_unpack.c',
        'source/silk/NLSF_del_dec_quant.c',
        'source/silk/process_NLSFs.c',
        'source/silk/stereo_LR_to_MS.c',
        'source/silk/stereo_MS_to_LR.c',
        'source/silk/check_control_input.c',
        'source/silk/control_SNR.c',
        'source/silk/init_encoder.c',
        'source/silk/control_codec.c',
        'source/silk/A2NLSF.c',
        'source/silk/ana_filt_bank_1.c',
        'source/silk/biquad_alt.c',
        'source/silk/bwexpander_32.c',
        'source/silk/bwexpander.c',
        'source/silk/debug.c',
        'source/silk/decode_pitch.c',
        'source/silk/inner_prod_aligned.c',
        'source/silk/lin2log.c',
        'source/silk/log2lin.c',
        'source/silk/LPC_analysis_filter.c',
        'source/silk/LPC_inv_pred_gain.c',
        'source/silk/table_LSF_cos.c',
        'source/silk/NLSF2A.c',
        'source/silk/NLSF_stabilize.c',
        'source/silk/NLSF_VQ_weights_laroia.c',
        'source/silk/pitch_est_tables.c',
        'source/silk/resampler.c',
        'source/silk/resampler_down2_3.c',
        'source/silk/resampler_down2.c',
        'source/silk/resampler_private_AR2.c',
        'source/silk/resampler_private_down_FIR.c',
        'source/silk/resampler_private_IIR_FIR.c',
        'source/silk/resampler_private_up2_HQ.c',
        'source/silk/resampler_rom.c',
        'source/silk/sigm_Q15.c',
        'source/silk/sort.c',
        'source/silk/sum_sqr_shift.c',
        'source/silk/stereo_decode_pred.c',
        'source/silk/stereo_encode_pred.c',
        'source/silk/stereo_find_predictor.c',
        'source/silk/stereo_quant_pred.c',

        # silk floating point engine
        'source/silk/float/apply_sine_window_FLP.c',
        'source/silk/float/corrMatrix_FLP.c',
        'source/silk/float/encode_frame_FLP.c',
        'source/silk/float/find_LPC_FLP.c',
        'source/silk/float/find_LTP_FLP.c',
        'source/silk/float/find_pitch_lags_FLP.c',
        'source/silk/float/find_pred_coefs_FLP.c',
        'source/silk/float/LPC_analysis_filter_FLP.c',
        'source/silk/float/LTP_analysis_filter_FLP.c',
        'source/silk/float/LTP_scale_ctrl_FLP.c',
        'source/silk/float/noise_shape_analysis_FLP.c',
        'source/silk/float/prefilter_FLP.c',
        'source/silk/float/process_gains_FLP.c',
        'source/silk/float/regularize_correlations_FLP.c',
        'source/silk/float/residual_energy_FLP.c',
        'source/silk/float/solve_LS_FLP.c',
        'source/silk/float/warped_autocorrelation_FLP.c',
        'source/silk/float/wrappers_FLP.c',
        'source/silk/float/autocorrelation_FLP.c',
        'source/silk/float/burg_modified_FLP.c',
        'source/silk/float/bwexpander_FLP.c',
        'source/silk/float/energy_FLP.c',
        'source/silk/float/inner_product_FLP.c',
        'source/silk/float/k2a_FLP.c',
        'source/silk/float/levinsondurbin_FLP.c',
        'source/silk/float/LPC_inv_pred_gain_FLP.c',
        'source/silk/float/pitch_analysis_core_FLP.c',
        'source/silk/float/scale_copy_vector_FLP.c',
        'source/silk/float/scale_vector_FLP.c',
        'source/silk/float/schur_FLP.c',
        'source/silk/float/sort_FLP.c',
      ]
    }
  ]
}
