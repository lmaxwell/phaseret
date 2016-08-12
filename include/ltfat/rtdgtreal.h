#ifndef _RTDGTREAL_H
#define _RTDGTREAL_H

typedef enum
{
    LTFAT_RTDGTPHASE_ZERO,
    LTFAT_RTDGTPHASE_HALFSHIFT
} rtdgt_phasetype;

typedef enum
{
    LTFAT_FORWARD,
    LTFAT_INVERSE
} ltfat_transformdirection;

#define rtdgtreal_processor_execute_compact_max_chan 128

#endif /* _RTDGTREAL_H */


typedef struct LTFAT_NAME(rtdgtreal_plan) LTFAT_NAME(rtdgtreal_plan);
// For now, the inverse plan is the same
typedef LTFAT_NAME(rtdgtreal_plan) LTFAT_NAME(rtidgtreal_plan);

typedef struct LTFAT_NAME(rtdgtreal_fifo_state) LTFAT_NAME(rtdgtreal_fifo_state);

typedef struct LTFAT_NAME(rtidgtreal_fifo_state) LTFAT_NAME(rtidgtreal_fifo_state);

int
LTFAT_NAME(rtdgtreal_commoninit)(const LTFAT_REAL* g, const ltfatInt gl,
                                 const ltfatInt M, const rtdgt_phasetype ptype,
                                 const  ltfat_transformdirection tradir,
                                 LTFAT_NAME(rtdgtreal_plan)** p);

/** Create RTDGTREAL plan
 *
 * The function returns NULL if the FFTW plan cannot be crated or there is not enough
 * memory to allocate internal buffers.
 *
 * \param[in]  g      Window
 * \param[in]  gl     Window length
 * \param[in]  M      Number of FFT channels
 * \param[in]  ptype  Phase convention
 *
 * \returns RTDGTREAL plan
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_init)(const LTFAT_REAL g[], const ltfatInt gl,
                           const ltfatInt M, const rtdgt_phasetype ptype,
                           LTFAT_NAME(rtdgtreal_plan)** p);

/** Execute RTDGTREAL plan
 * \param[in]  p      RTDGTREAL plan
 * \param[in]  f      Input buffer (gl x W)
 * \param[in]  W      Number of channels
 * \param[out] c      Output DGT coefficients (M2 x W)
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_execute)(const LTFAT_NAME(rtdgtreal_plan)* p,
                              const LTFAT_REAL f[], const ltfatInt W,
                              LTFAT_COMPLEX c[]);

/** Destroy RTDGTREAL plan
 * \param[in]  p      RTDGTREAL plan
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_done)(LTFAT_NAME(rtdgtreal_plan)** p);

/** Create RTIDGTREAL plan
 *
 * The function returns NULL if the FFTW plan cannot be crated or there is not enough
 * memory to allocate internal buffers.
 *
 * \param[in]  g      Window
 * \param[in]  gl     Window length
 * \param[in]  M      Number of FFT channels
 * \param[in]  ptype  Phase convention
 *
 * \returns RTIDGTREAL plan
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_init)(const LTFAT_REAL g[], const ltfatInt gl,
                            const ltfatInt M, const rtdgt_phasetype ptype,
                            LTFAT_NAME(rtdgtreal_plan)** p);

/** Execute RTIDGTREAL plan
 * \param[in]  p      RTDGTREAL plan
 * \param[int] c      Input DGT coefficients (M2 x W)
 * \param[in]  W      Number of channels
 * \param[out] f      Output buffer (gl x W)
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_execute)(const LTFAT_NAME(rtidgtreal_plan)* p,
                               const LTFAT_COMPLEX c[], const ltfatInt W,
                               LTFAT_REAL f[]);

/** Destroy RTIDGTREAL plan
 * \param[in]  p      RTIDGTREAL plan
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_done)(LTFAT_NAME(rtidgtreal_plan)** p);

/** Create ring buffer for DGT analysis
 *
 * The ring buffer works as usual when written to, but only constant
 * size (gl) chunks can be read from it and the read pointer is
 * only advanced by a after read.
 *
 * The buffer read and write pointers are initialized such that they
 * reflect the processing delay.
 *
 * \param[in]  fifoLen  Ring buffer size. This should be at least gl + max. expected
 *                      buffer length.
 *                      One more slot is actually allocated for the
 *                      "one slot open" implementation.
 * \param[in]  gl       Window length
 * \param[in]  a        Hop factor
 * \param[in]  Wmax     Maximum number of channels
 *
 * \returns RTDGTREAL_FIFO struct pointer
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_fifo_init)(ltfatInt fifoLen, ltfatInt procDelay, ltfatInt gl, ltfatInt a,
                                ltfatInt Wmax, LTFAT_NAME(rtdgtreal_fifo_state)** p);

/** Write bufLen samples to DGT analysis ring buffer
 *
 * The function returns number of samples written and a negative number if something went
 * wrong.
 * If there is not enough space for all bufLen samples, only available space is used
 * and the number of actually written samples is returned.
 *
 * \param[in]  p        Analysis ring buffer struct
 * \param[in]  buf      Channels to be written.
 * \param[in]  bufLen   Number of samples to be written
 * \param[in]  W        Number of channels
 *
 * \returns Number of samples written
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_fifo_write)(LTFAT_NAME(rtdgtreal_fifo_state)* p, const LTFAT_REAL* buf[],
                                 const ltfatInt bufLen, const ltfatInt W);

/** Read p->gl samples from DGT analysis ring buffer
 *
 * The function attempts to read p->gl samples from the buffer.
 *
 * The function does mothing and returns 0 if there is less than p->gl samples available.
 *
 * \param[in]   p        Analysis ring buffer struct
 * \param[out]  buf      Output array, it is expected to be able to hold p->gl*p->Wmax samples.
 *
 * \returns Number of samples read
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_fifo_read)(LTFAT_NAME(rtdgtreal_fifo_state)* p, LTFAT_REAL buf[]);

/** Destroy DGT analysis ring buffer
 * \param[in]  p      DGT analysis ring buffer
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_fifo_done)(LTFAT_NAME(rtdgtreal_fifo_state)** p);

/** Create ring buffer for DGT synthesis
 *
 * The ring buffer behaves as usual when read from, except it sets the read
 * samples to zero.
 * Only chunks of size gl can be written to it and the write pointer is advanced
 * by a. The samples are added to the existing values instead of the usual
 * overwrite.
 *
 * The buffer read and write pointers are both initialized to the same value.
 *
 * \param[in]  fifoLen  Ring buffer size. This should be at least gl + max. expected
 *                      buffer length. (gl+1) more slots are actually allocated
 *                      to accomodate the overlaps.
 * \param[in]  gl       Window length
 * \param[in]  a        Hop factor
 * \param[in]  Wmax     Maximum number of channels
 *
 * \returns RTIDGTREAL_FIFO struct pointer
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_fifo_init)(ltfatInt fifoLen, ltfatInt gl,
                                 ltfatInt a, ltfatInt Wmax,
                                 LTFAT_NAME(rtidgtreal_fifo_state)** p);

/** Write p->gl samples to DGT synthesis ring buffer
 *
 * The function returns 0 if there is not enough space to write all
 * p->gl samples.
 *
 * \param[in]  p        Synthesis ring buffer struct
 * \param[in]  buf      Samples to be written
 *
 * \returns Number of samples written
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_fifo_write)(LTFAT_NAME(rtidgtreal_fifo_state)* p,
                                  const LTFAT_REAL buf[]);

/** Read bufLen samples from DGT analysis ring buffer
 *
 * The function attempts to read bufLen samples from the buffer.
 *
 * \param[in]   p        Analysis ring buffer struct
 * \param[in]   bufLen   Number of samples to be read
 * \param[in]   W        Number of channels
 * \param[out]  buf      Output channels, each channel is expected to be able to
 *                       hold bufLen samples.
 *
 * \returns Number of samples read
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_fifo_read)(LTFAT_NAME(rtidgtreal_fifo_state)* p,
                                 const ltfatInt bufLen, const ltfatInt W,
                                 LTFAT_REAL* buf[]);

/** Destroy DGT synthesis ring buffer
 * \param[in]  p      DGT synthesis ring buffer
 */
LTFAT_EXTERN int
LTFAT_NAME(rtidgtreal_fifo_done)(LTFAT_NAME(rtidgtreal_fifo_state)** p);


typedef struct LTFAT_NAME(rtdgtreal_processor_state) LTFAT_NAME(rtdgtreal_processor_state);

/** \defgroup rtdgtrealprocessor Real-Time Discrete Gabor Transform Processor
 *  \addtogroup rtdgtrealprocessor
 *  @{
 *  The real-time DGT processor wraps the analysis-modify-synthesis loop for
 *  audio streams. It provides a callback interface which allows user-defined
 *  coefficient manipulation.
 *
 *  Example:
 *  ~~~~~~~~~~~~~~~{.c}
 *  // Simple coefficient modification
 *  void process(void *userdata, const complex float inCoef[], const int M2, const int W, complex float outCoef[])
 *  {
 *      for(int w=0; w<W; w++) // Loop over channels
 *          for(int m=0; m<M2; m++) // Loop over frequencies
 *              out[m+w*M2] = 2.0f*in[m+w*M2];
 *  }
 *
 *  // Initialize
 *  ltfat_rtdgtreal_processor_state_s* procstate = NULL;
 *  ltfat_rtdgtreal_processor_init_win_s( LTFAT_HANN, 1024, 256, 1024, maxChanNo, &process, NULL, &procstate);
 *
 *  // In the audio loop
 *  void audioCallback(float** data, int dataLen, int chanNo)
 *  {
 *      ltfat_rtdgtreal_processor_execute_s(procstate, data, dataLen, chanNo, data);
 *  }
 *
 *  // Teardown
*   ltfat_rtdgtreal_processor_done_s(&procstate);
 *  ~~~~~~~~~~~~~~~
 */

/** Processor callback signature
 *
 * User defined processor callback must comply with this signature.
 *
 * It is safe to assume that out and in are not aliased.
 *
 * \param[in]  userdata   User defined data
 * \param[in]        in   Input coefficients, M2 x W array
 * \param[in]        M2   Length of the arrays; number of unique FFT channels; equals to M/2 + 1
 * \param[in]         W   Number of channels
 * \param[out]      out   Output coefficients, M2 x W array
 *
 *  #### Function versions #
 *  <tt>
 *  typedef void ltfat_rtdgtreal_processor_callback_d(void* userdata, const complex double in[], const int M2,
 *                                                    const int W, complex double out[]);
 *
 *  typedef void ltfat_rtdgtreal_processor_callback_s(void* userdata, const complex float in[], const int M2,
 *                                                    const int W, complex float out[]);
 *  </tt>
 *
 */
typedef void LTFAT_NAME(rtdgtreal_processor_callback)(void* userdata,
        const LTFAT_COMPLEX in[], const int M2, const int W, LTFAT_COMPLEX out[]);

/** Create DGTREAL processor state struct
 *
 * The processor wraps DGTREAL analysis-modify-synthesis loop suitable for
 * stream of data. The -modify- part is user definable via callback.
 * If the callback is NULL, no coefficient modification occurs.
 *
 * \param[in]       ga   Analysis window
 * \param[in]      gal   Length of the analysis window
 * \param[in]       gs   Synthesis window
 * \param[in]      gsl   Length of the synthesis window
 * \param[in]        a   Hop size
 * \param[in]        M   Number of FFT channels
 * \param[in]     Wmax   Maximum number of channels
 * \param[in] callback   Custom function to process the coefficients
 * \param[in] userdata   Custom callback data. Will be passed to the callback.
 *                       Useful for storing state between callback calls.
 * \param[out]    plan   DGTREAL processor state
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_init_d(const double ga[], const ltfatInt gal, const double gs[], const ltfatInt gsl,
 *                                  const ltfatInt a, const ltfatInt M, const ltfatInt Wmax,
 *                                  rtdgtreal_processor_callback_d* callback, void* userdata,
 *                                  rtdgtreal_processor_state_d** plan);
 *
 * ltfat_rtdgtreal_processor_init_s(const float ga[], const ltfatInt gal, const float gs[], const ltfatInt gsl,
 *                                  const ltfatInt a, const ltfatInt M, const ltfatInt Wmax,
 *                                  rtdgtreal_processor_callback_s* callback, void* userdata,
 *                                  rtdgtreal_processor_state_s** plan);
 * </tt>
 *
 * \returns
 * Status code           |  Description
 * ----------------------|----------------------
 * LTFATERR_SUCCESS      |  No error occured
 * LTFATERR_NULLPOINTER  |  One of the following was NULL: \a ga, \a gs, \a plan
 * LTFATERR_BADSIZE      |  \a gla or \a gls was less or equal to 0
 * LTFATERR_NOTPOSARG    |  At least one of the following was less or equal to zero: \a a, \a M, \a Wmax
 * LTFATERR_CANNOTHAPPEN |  \a win was not valid value from the LTFAT_FIRWIN enum.
 * LTFATERR_NOMEM        |  Heap memory allocation failed
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_init)(const LTFAT_REAL ga[], const ltfatInt gal,
                                     const LTFAT_REAL gs[], const ltfatInt gsl,
                                     const ltfatInt a, const ltfatInt M,
                                     const ltfatInt Wmax,
                                     LTFAT_NAME(rtdgtreal_processor_callback)* callback,
                                     void* userdata, LTFAT_NAME(rtdgtreal_processor_state)** plan);

/** Create DGTREAL processor state struct
 *
 * This function provides an alternative way of creating DGTREAL processor
 * state struct. The function accepts only the analysis window and the synthesis
 * window is computed internally.
 *
 * \param[in]      win   Analysis window
 * \param[in]       gl   Length of the windows
 * \param[in]        a   Hop size
 * \param[in]        M   Number of FFT channels
 * \param[in]     Wmax   Maximum number of channels
 * \param[in] callback   Custom function to process the coefficients
 * \param[in] userdata   Custom callback data. Will be passed to the callback.
 *                       Useful for storing state between callback calls.
 * \param[out]    plan   DGTREAL processor state
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_init_win_d(LTFAT_FIRWIN win, const ltfatInt gl, const ltfatInt a, const ltfatInt M,
 *                                      const ltfatInt Wmax, rtdgtreal_processor_callback_d* callback, void* userdata,
 *                                      rtdgtreal_processor_state_d** plan);
 *
 * ltfat_rtdgtreal_processor_init_win_s(LTFAT_FIRWIN win, const ltfatInt gl, const ltfatInt a, const ltfatInt M,
 *                                      const ltfatInt Wmax, rtdgtreal_processor_callback_s* callback, void* userdata,
 *                                      rtdgtreal_processor_state_s** plan);
 * </tt>
 *
 * \returns
 * Status code           |  Description
 * ----------------------|----------------------
 * LTFATERR_SUCCESS      |  No error occured
 * LTFATERR_NULLPOINTER  |  \a plan was NULL.
 * LTFATERR_BADSIZE      |  \a gl was less or equal to 0
 * LTFATERR_NOTPOSARG    |  At least one of the following was less or equal to zero: \a a, \a M, \a Wmax
 * LTFATERR_CANNOTHAPPEN |  \a win was not valid value from the LTFAT_FIRWIN enum.
 * LTFATERR_NOMEM        |  Heap memory allocation failed
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_init_win)(LTFAT_FIRWIN win,
        const ltfatInt gl, const ltfatInt a, const ltfatInt M,
        const ltfatInt Wmax,
        LTFAT_NAME(rtdgtreal_processor_callback)* callback,
        void* userdata,
        LTFAT_NAME(rtdgtreal_processor_state)** plan);

/** Process samples
 *
 * Process multichannel input samples. Channels are stored as an array of
 * pointers to the actual data arrays.
 *
 * This function is mean to be called from the audio loop.
 *
 * Output is lagging behind the input by (gl-1) samples.
 * The function can run inplace i.e. in==out.
 *
 * \param[in]      p  DGTREAL processor
 * \param[in]     in  Input channels
 * \param[in]    len  Length of the channels
 * \param[in] chanNo  Number of channels
 * \param[out]   out  Output frame
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_execute_d(ltfat_rtdgtreal_processor_state_d* p, const double* in[],
 *                                     const ltfatInt L, const ltfatInt W, double* out[]);
 *
 * ltfat_rtdgtreal_processor_execute_s(ltfat_rtdgtreal_processor_state_d* p, const float* in[],
 *                                     const ltfatInt L, const ltfatInt W, float* out[]);
 * </tt>
 *
 * \returns
 * Status code           |  Description
 * ----------------------|----------------------
 * LTFATERR_SUCCESS      |  No error occured
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_execute)(LTFAT_NAME(rtdgtreal_processor_state)* p,
                                        const LTFAT_REAL* in[],
                                        const ltfatInt L, const ltfatInt W,
                                        LTFAT_REAL* out[]);

/** Process samples
 *
 * Works exactly like rtdgtreal_processor_execute except that the multichannel
 * buffers are stored one after the other in the memory.
 *
 * \param[in]      p  DGTREAL processor
 * \param[in]     in  Input channels
 * \param[in]    len  Length of the channels
 * \param[in] chanNo  Number of channels
 * \param[out]   out  Output frame
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_execute_compact_d(ltfat_rtdgtreal_processor_state_d* p, const double in[],
 *                                             const ltfatInt L, const ltfatInt W, double out[]);
 *
 * ltfat_rtdgtreal_processor_execute_compact_s(ltfat_rtdgtreal_processor_state_d* p, const float in[],
 *                                             const ltfatInt L, const ltfatInt W, float out[]);
 * </tt>
 *
 * \returns
 * Status code           |  Description
 * ----------------------|----------------------
 * LTFATERR_SUCCESS      |  No error occured
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_execute_compact)(
    LTFAT_NAME(rtdgtreal_processor_state)* p,
    const LTFAT_REAL in[],
    const ltfatInt len, const ltfatInt chanNo,
    LTFAT_REAL out[]);

/** Destroy DGTREAL processor state
 * \param[in]  p      DGTREAL processor
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_done_d(ltfat_rtdgtreal_processor_state_d** plan);
 *
 * ltfat_rtdgtreal_processor_done_s(ltfat_rtdgtreal_processor_state_s** plan);
 * </tt>
 *
 * \returns
 * Status code              | Description
 * -------------------------|--------------------------------------------
 * LTFATERR_SUCCESS         | Indicates no error
 * LTFATERR_NULLPOINTER     | plan or *plan was NULL.
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_done)(LTFAT_NAME(rtdgtreal_processor_state)** plan);

/** Set DGTREAL processor callback
 *
 * Function replaces the callback in the struct. This is not thread safe.
 * Only call this if there is no chance that the execute function is called
 * simultaneously in a different thread.
 *
 * \param[in]            p   DGTREAL processor state
 * \param[in]     callback   Custom function to process the coefficients
 * \param[in]     userdata   Custom callback data. Will be passed to the callback.
 *                           Useful for storing state between callback calls.
 *
 * #### Function versions #
 * <tt>
 * ltfat_rtdgtreal_processor_setcallback_d(ltfat_rtdgtreal_processor_state_d* p,
 *                                         ltfat_rtdgtreal_processor_callback_d* callback,
 *                                         void* userdata);
 *
 * ltfat_rtdgtreal_processor_setcallback_s(ltfat_rtdgtreal_processor_state_s* p,
 *                                         ltfat_rtdgtreal_processor_callback_s* callback,
 *                                         void* userdata);
 * </tt>
 */
LTFAT_EXTERN int
LTFAT_NAME(rtdgtreal_processor_setcallback)(LTFAT_NAME(rtdgtreal_processor_state)* p,
        LTFAT_NAME(rtdgtreal_processor_callback)* callback,
        void* userdata);

/** Default processor callback
 *
 * The callback just copies data from input to the output.
 *
 * It is used when no other processor callback is registered.
 *
 */
LTFAT_EXTERN void
LTFAT_NAME(default_rtdgtreal_processor_callback)(void* userdata, const LTFAT_COMPLEX in[],
        const int M2, const int W, LTFAT_COMPLEX out[]);

/** @}*/

int
LTFAT_NAME(rtdgtreal_execute_wrapper)(void* p,
                                      const LTFAT_REAL* f, const ltfatInt W,
                                      LTFAT_COMPLEX* c);

int
LTFAT_NAME(rtidgtreal_execute_wrapper)(void* p,
                                       const LTFAT_COMPLEX* c, const ltfatInt W,
                                       LTFAT_REAL* f);
