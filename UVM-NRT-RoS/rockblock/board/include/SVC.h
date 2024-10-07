#pragma once
#include <Arduino.h>
#include <stdarg.h>

namespace Eloquent {
namespace ML {
namespace Port {
class SVM {
public:
  /**
   * Predict class for features vector
   */
  int predict(float *x) {
    float kernels[83] = {0};
    float decisions[3] = {0};
    int votes[3] = {0};
    kernels[0] = compute_kernel(x, 5.1, 3.8, 1.9, 0.4);
    kernels[1] = compute_kernel(x, 4.8, 3.4, 1.9, 0.2);
    kernels[2] = compute_kernel(x, 5.0, 3.0, 1.6, 0.2);
    kernels[3] = compute_kernel(x, 5.1, 3.3, 1.7, 0.5);
    kernels[4] = compute_kernel(x, 5.1, 3.4, 1.5, 0.2);
    kernels[5] = compute_kernel(x, 5.4, 3.9, 1.7, 0.4);
    kernels[6] = compute_kernel(x, 4.9, 3.1, 1.5, 0.2);
    kernels[7] = compute_kernel(x, 5.0, 3.4, 1.5, 0.2);
    kernels[8] = compute_kernel(x, 5.0, 3.5, 1.6, 0.6);
    kernels[9] = compute_kernel(x, 4.6, 3.1, 1.5, 0.2);
    kernels[10] = compute_kernel(x, 5.1, 3.8, 1.6, 0.2);
    kernels[11] = compute_kernel(x, 4.5, 2.3, 1.3, 0.3);
    kernels[12] = compute_kernel(x, 5.1, 3.7, 1.5, 0.4);
    kernels[13] = compute_kernel(x, 4.9, 3.0, 1.4, 0.2);
    kernels[14] = compute_kernel(x, 5.4, 3.4, 1.7, 0.2);
    kernels[15] = compute_kernel(x, 5.5, 2.4, 3.7, 1.0);
    kernels[16] = compute_kernel(x, 6.6, 3.0, 4.4, 1.4);
    kernels[17] = compute_kernel(x, 5.7, 2.9, 4.2, 1.3);
    kernels[18] = compute_kernel(x, 5.6, 3.0, 4.5, 1.5);
    kernels[19] = compute_kernel(x, 5.8, 2.7, 4.1, 1.0);
    kernels[20] = compute_kernel(x, 5.0, 2.0, 3.5, 1.0);
    kernels[21] = compute_kernel(x, 5.6, 2.7, 4.2, 1.3);
    kernels[22] = compute_kernel(x, 5.7, 3.0, 4.2, 1.2);
    kernels[23] = compute_kernel(x, 6.2, 2.9, 4.3, 1.3);
    kernels[24] = compute_kernel(x, 6.0, 2.2, 4.0, 1.0);
    kernels[25] = compute_kernel(x, 5.4, 3.0, 4.5, 1.5);
    kernels[26] = compute_kernel(x, 5.5, 2.3, 4.0, 1.3);
    kernels[27] = compute_kernel(x, 5.0, 2.3, 3.3, 1.0);
    kernels[28] = compute_kernel(x, 5.5, 2.4, 3.8, 1.1);
    kernels[29] = compute_kernel(x, 6.7, 3.0, 5.0, 1.7);
    kernels[30] = compute_kernel(x, 5.9, 3.2, 4.8, 1.8);
    kernels[31] = compute_kernel(x, 5.1, 2.5, 3.0, 1.1);
    kernels[32] = compute_kernel(x, 6.0, 2.7, 5.1, 1.6);
    kernels[33] = compute_kernel(x, 5.5, 2.5, 4.0, 1.3);
    kernels[34] = compute_kernel(x, 6.3, 2.5, 4.9, 1.5);
    kernels[35] = compute_kernel(x, 6.1, 2.9, 4.7, 1.4);
    kernels[36] = compute_kernel(x, 6.5, 2.8, 4.6, 1.5);
    kernels[37] = compute_kernel(x, 7.0, 3.2, 4.7, 1.4);
    kernels[38] = compute_kernel(x, 5.9, 3.0, 4.2, 1.5);
    kernels[39] = compute_kernel(x, 5.7, 2.6, 3.5, 1.0);
    kernels[40] = compute_kernel(x, 5.2, 2.7, 3.9, 1.4);
    kernels[41] = compute_kernel(x, 6.1, 3.0, 4.6, 1.4);
    kernels[42] = compute_kernel(x, 6.6, 2.9, 4.6, 1.3);
    kernels[43] = compute_kernel(x, 5.5, 2.6, 4.4, 1.2);
    kernels[44] = compute_kernel(x, 5.6, 3.0, 4.1, 1.3);
    kernels[45] = compute_kernel(x, 4.9, 2.4, 3.3, 1.0);
    kernels[46] = compute_kernel(x, 6.9, 3.1, 4.9, 1.5);
    kernels[47] = compute_kernel(x, 5.7, 2.8, 4.1, 1.3);
    kernels[48] = compute_kernel(x, 6.3, 2.3, 4.4, 1.3);
    kernels[49] = compute_kernel(x, 6.4, 2.9, 4.3, 1.3);
    kernels[50] = compute_kernel(x, 6.1, 2.8, 4.0, 1.3);
    kernels[51] = compute_kernel(x, 5.8, 2.6, 4.0, 1.2);
    kernels[52] = compute_kernel(x, 6.3, 2.8, 5.1, 1.5);
    kernels[53] = compute_kernel(x, 6.4, 3.1, 5.5, 1.8);
    kernels[54] = compute_kernel(x, 6.3, 2.7, 4.9, 1.8);
    kernels[55] = compute_kernel(x, 5.7, 2.5, 5.0, 2.0);
    kernels[56] = compute_kernel(x, 6.0, 3.0, 4.8, 1.8);
    kernels[57] = compute_kernel(x, 5.8, 2.7, 5.1, 1.9);
    kernels[58] = compute_kernel(x, 6.2, 3.4, 5.4, 2.3);
    kernels[59] = compute_kernel(x, 6.4, 2.7, 5.3, 1.9);
    kernels[60] = compute_kernel(x, 5.8, 2.8, 5.1, 2.4);
    kernels[61] = compute_kernel(x, 6.9, 3.2, 5.7, 2.3);
    kernels[62] = compute_kernel(x, 6.1, 2.6, 5.6, 1.4);
    kernels[63] = compute_kernel(x, 6.0, 2.2, 5.0, 1.5);
    kernels[64] = compute_kernel(x, 7.2, 3.2, 6.0, 1.8);
    kernels[65] = compute_kernel(x, 6.3, 3.4, 5.6, 2.4);
    kernels[66] = compute_kernel(x, 6.8, 3.0, 5.5, 2.1);
    kernels[67] = compute_kernel(x, 6.3, 3.3, 6.0, 2.5);
    kernels[68] = compute_kernel(x, 6.2, 2.8, 4.8, 1.8);
    kernels[69] = compute_kernel(x, 6.4, 3.2, 5.3, 2.3);
    kernels[70] = compute_kernel(x, 6.9, 3.1, 5.4, 2.1);
    kernels[71] = compute_kernel(x, 6.5, 3.0, 5.2, 2.0);
    kernels[72] = compute_kernel(x, 6.7, 3.3, 5.7, 2.1);
    kernels[73] = compute_kernel(x, 6.7, 3.3, 5.7, 2.5);
    kernels[74] = compute_kernel(x, 7.2, 3.0, 5.8, 1.6);
    kernels[75] = compute_kernel(x, 6.7, 3.1, 5.6, 2.4);
    kernels[76] = compute_kernel(x, 7.4, 2.8, 6.1, 1.9);
    kernels[77] = compute_kernel(x, 6.3, 2.9, 5.6, 1.8);
    kernels[78] = compute_kernel(x, 6.5, 3.0, 5.5, 1.8);
    kernels[79] = compute_kernel(x, 5.6, 2.8, 4.9, 2.0);
    kernels[80] = compute_kernel(x, 5.9, 3.0, 5.1, 1.8);
    kernels[81] = compute_kernel(x, 4.9, 2.5, 4.5, 1.7);
    kernels[82] = compute_kernel(x, 7.1, 3.0, 5.9, 2.1);
    decisions[0] =
        0.048796921968 + kernels[0] + kernels[1] + kernels[2] + kernels[3] +
        kernels[4] + kernels[5] + kernels[6] + kernels[7] + kernels[8] +
        kernels[9] + kernels[10] * 0.221208088282 + kernels[11] + kernels[12] +
        kernels[13] + kernels[14] - kernels[15] - kernels[19] - kernels[20] -
        kernels[22] - kernels[27] - kernels[28] - kernels[31] - kernels[33] -
        kernels[39] - kernels[40] - kernels[44] - kernels[45] +
        kernels[47] * -0.221208088282 - kernels[50] - kernels[51];
    decisions[1] = -0.020209729671 + kernels[0] + kernels[1] + kernels[2] +
                   kernels[3] + kernels[5] + kernels[6] * 0.452899266841 +
                   kernels[8] + kernels[14] + kernels[52] * -0.452899266841 -
                   kernels[54] - kernels[56] - kernels[63] - kernels[68] -
                   kernels[79] - kernels[80] - kernels[81];
    decisions[2] =
        -0.034882992506 + kernels[16] + kernels[17] + kernels[18] +
        kernels[19] + kernels[21] + kernels[22] + kernels[23] + kernels[24] +
        kernels[25] + kernels[26] + kernels[28] * 0.224177328391 + kernels[29] +
        kernels[30] + kernels[32] + kernels[33] + kernels[34] + kernels[35] +
        kernels[36] + kernels[37] + kernels[38] + kernels[40] + kernels[41] +
        kernels[42] + kernels[43] + kernels[44] + kernels[46] + kernels[47] +
        kernels[48] + kernels[49] + kernels[50] + kernels[51] - kernels[52] -
        kernels[53] - kernels[54] - kernels[55] - kernels[56] - kernels[57] -
        kernels[58] - kernels[59] - kernels[60] - kernels[61] - kernels[62] -
        kernels[63] - kernels[64] - kernels[65] - kernels[66] - kernels[67] -
        kernels[68] - kernels[69] - kernels[70] - kernels[71] - kernels[72] -
        kernels[73] - kernels[74] - kernels[75] +
        kernels[76] * -0.224177328391 - kernels[77] - kernels[78] -
        kernels[79] - kernels[80] - kernels[81] - kernels[82];
    votes[decisions[0] > 0 ? 0 : 1] += 1;
    votes[decisions[1] > 0 ? 0 : 2] += 1;
    votes[decisions[2] > 0 ? 1 : 2] += 1;
    int val = votes[0];
    int idx = 0;

    for (int i = 1; i < 3; i++) {
      if (votes[i] > val) {
        val = votes[i];
        idx = i;
      }
    }

    return idx;
  }

protected:
  /**
   * Compute kernel between feature vector and support vector.
   * Kernel type: rbf
   */
  float compute_kernel(float *x, ...) {
    va_list w;
    va_start(w, 4);
    float kernel = 0.0;

    for (uint16_t i = 0; i < 4; i++) {
      kernel += pow(x[i] - va_arg(w, double), 2);
    }

    return exp(-0.01 * kernel);
  }
};
} // namespace Port
} // namespace ML
} // namespace Eloquent
