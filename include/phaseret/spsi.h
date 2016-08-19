#ifndef _spsi_h
#define _spsi_h
#include "config.h"

/** \addtogroup spsi
 *  \{
 */

/** SPSI algorithm implementation
 *
 *  M2 = M/2 + 1, N = L/a
 *
 *  \param[in]             s   Target magnitude, size M2 x N x W
 *  \param[in]             L   Transform length
 *  \param[in]             W   Number of signal channels
 *  \param[in]             a   Hop factor
 *  \param[in]             M   Number of frequency channels
 *  \param[in,out] initphase   [in]  phase of -1 frame,
 *                             [out] phase of [N-1] frame, size M2 x W or NULL,
 *                             but then a memory allocation will occur.
 *  \param[out]            c   Coefficients with reconstructed phase, size M2 x N x W
 *  \returns
 *  Status code          |  Description
 *  ---------------------|---------------------
 *  LTFATERR_SUCCESS     | No error occurred
 *  LTFATERR_NULLPOINTER | \a s or \a c was NULL
 *  LTFATERR_NOTPOSARG   | At least one of the following was not positive: \a L, \a W, \a a, \a M
 *  LTFATERR_NOMEM       | Heap allocation failed
 */
int
spsi(const double s[], int L, int W, int a, int M, double initphase[], complex double c[]);

/** Masked SPSI algorithm implementation
 *
 *  Works as spsi() except c[ii]=cinit[ii] if mask[ii] evaluates to true.
 *
 *  M2 = M/2 + 1, N = L/a
 *
 *  \param[in]         cinit   Initial coefficients, size M2 x N x W
 *  \param[in]          mask   Mask of known coefficients
 *  \param[in]             L   Transform length
 *  \param[in]             W   Number of signal channels
 *  \param[in]             a   Hop factor
 *  \param[in]             M   Number of frequency channels
 *  \param[in,out] initphase   [in]  phase of -1 frame,
 *                             [out] phase of [N-1] frame, size M2 x W or NULL,
 *                             but then a memory allocation will occur.
 *  \param[out]            c   Coefficients with reconstructed phase, size M2 x N x W
 *  \returns
 *  Status code          |  Description
 *  ---------------------|---------------------
 *  LTFATERR_SUCCESS     | No error occurred
 *  LTFATERR_NULLPOINTER | \a cinit, \a c or \a mask was NULL
 *  LTFATERR_NOTPOSARG   | At least one of the following was not positive: \a L, \a W, \a a, \a M
 *  LTFATERR_NOMEM       | Heap allocation failed
 */
int
spsi_withmask(const complex double cinit[], const int mask[], int L, int W, int a, int M,
              double initphase[], complex double c[]);


/** \} */
void
spsiupdate(const double* scol, int stride, int a, int M, double* tmpphase);

#endif /* _spsi_h */
