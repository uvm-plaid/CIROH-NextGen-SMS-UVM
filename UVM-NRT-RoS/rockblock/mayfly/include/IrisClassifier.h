#ifndef UUID4376002384
#define UUID4376002384

#include <Arduino.h>

/**
 * RandomForestClassifier(bootstrap=True, ccp_alpha=0.0,
 * class_name=RandomForestClassifier, class_weight=None, criterion=gini,
 * estimator=DecisionTreeClassifier(), estimator_params=('criterion',
 * 'max_depth', 'min_samples_split', 'min_samples_leaf',
 * 'min_weight_fraction_leaf', 'max_features', 'max_leaf_nodes',
 * 'min_impurity_decrease', 'random_state', 'ccp_alpha', 'monotonic_cst'),
 * max_depth=None, max_features=sqrt, max_leaf_nodes=None, max_samples=None,
 * min_impurity_decrease=0.0, min_samples_leaf=1, min_samples_split=2,
 * min_weight_fraction_leaf=0.0, monotonic_cst=None, n_estimators=5,
 * n_jobs=None, num_outputs=3, oob_score=False,
 * package_name=everywhereml.sklearn.ensemble, random_state=None,
 * template_folder=everywhereml/sklearn/ensemble, verbose=0, warm_start=False)
 */
class RandomForestClassifier {
public:
  /**
   * Predict class from features
   */
  int predict(float *x) {
    int predictedValue = 0;
    size_t startedAt = micros();

    uint16_t votes[3] = {0};
    uint8_t classIdx = 0;
    float classScore = 0;

    tree0(x, &classIdx, &classScore);
    votes[classIdx] += classScore;

    tree1(x, &classIdx, &classScore);
    votes[classIdx] += classScore;

    tree2(x, &classIdx, &classScore);
    votes[classIdx] += classScore;

    tree3(x, &classIdx, &classScore);
    votes[classIdx] += classScore;

    tree4(x, &classIdx, &classScore);
    votes[classIdx] += classScore;

    uint8_t maxClassIdx = 0;
    float maxVote = votes[0];

    for (uint8_t i = 1; i < 3; i++) {
      if (votes[i] > maxVote) {
        maxClassIdx = i;
        maxVote = votes[i];
      }
    }

    predictedValue = maxClassIdx;

    latency = micros() - startedAt;

    return (lastPrediction = predictedValue);
  }

  /**
   * Get latency in micros
   */
  uint32_t latencyInMicros() { return latency; }

  /**
   * Get latency in millis
   */
  uint16_t latencyInMillis() { return latency / 1000; }

protected:
  float latency = 0;
  int lastPrediction = 0;

  /**
   * Random forest's tree #0
   */
  void tree0(float *x, uint8_t *classIdx, float *classScore) {

    if (x[2] < 2.599999964237213) {

      *classIdx = 0;
      *classScore = 0.31666666666666665;
      return;

    } else {

      if (x[2] < 4.8500001430511475) {

        if (x[3] < 1.6500000357627869) {

          *classIdx = 1;
          *classScore = 0.36666666666666664;
          return;

        } else {

          if (x[1] < 3.100000023841858) {

            *classIdx = 2;
            *classScore = 0.31666666666666665;
            return;

          } else {

            *classIdx = 1;
            *classScore = 0.36666666666666664;
            return;
          }
        }

      } else {

        if (x[2] < 5.049999952316284) {

          if (x[1] < 2.899999976158142) {

            *classIdx = 2;
            *classScore = 0.31666666666666665;
            return;

          } else {

            *classIdx = 1;
            *classScore = 0.36666666666666664;
            return;
          }

        } else {

          *classIdx = 2;
          *classScore = 0.31666666666666665;
          return;
        }
      }
    }
  }

  /**
   * Random forest's tree #1
   */
  void tree1(float *x, uint8_t *classIdx, float *classScore) {

    if (x[3] < 0.800000011920929) {

      *classIdx = 0;
      *classScore = 0.35;
      return;

    } else {

      if (x[3] < 1.75) {

        if (x[3] < 1.3499999642372131) {

          *classIdx = 1;
          *classScore = 0.225;
          return;

        } else {

          if (x[2] < 4.950000047683716) {

            *classIdx = 1;
            *classScore = 0.225;
            return;

          } else {

            if (x[3] < 1.600000023841858) {

              *classIdx = 2;
              *classScore = 0.425;
              return;

            } else {

              *classIdx = 1;
              *classScore = 0.225;
              return;
            }
          }
        }

      } else {

        if (x[2] < 4.900000095367432) {

          if (x[1] < 3.100000023841858) {

            *classIdx = 2;
            *classScore = 0.425;
            return;

          } else {

            *classIdx = 1;
            *classScore = 0.225;
            return;
          }

        } else {

          *classIdx = 2;
          *classScore = 0.425;
          return;
        }
      }
    }
  }

  /**
   * Random forest's tree #2
   */
  void tree2(float *x, uint8_t *classIdx, float *classScore) {

    if (x[2] < 2.350000023841858) {

      *classIdx = 0;
      *classScore = 0.3416666666666667;
      return;

    } else {

      if (x[0] < 5.950000047683716) {

        if (x[3] < 1.600000023841858) {

          *classIdx = 1;
          *classScore = 0.325;
          return;

        } else {

          if (x[3] < 1.8499999642372131) {

            if (x[3] < 1.75) {

              *classIdx = 2;
              *classScore = 0.3333333333333333;
              return;

            } else {

              *classIdx = 1;
              *classScore = 0.325;
              return;
            }

          } else {

            *classIdx = 2;
            *classScore = 0.3333333333333333;
            return;
          }
        }

      } else {

        if (x[0] < 6.75) {

          if (x[2] < 4.700000047683716) {

            *classIdx = 1;
            *classScore = 0.325;
            return;

          } else {

            if (x[2] < 5.150000095367432) {

              if (x[3] < 1.75) {

                if (x[3] < 1.600000023841858) {

                  *classIdx = 2;
                  *classScore = 0.3333333333333333;
                  return;

                } else {

                  *classIdx = 1;
                  *classScore = 0.325;
                  return;
                }

              } else {

                *classIdx = 2;
                *classScore = 0.3333333333333333;
                return;
              }

            } else {

              *classIdx = 2;
              *classScore = 0.3333333333333333;
              return;
            }
          }

        } else {

          if (x[3] < 1.550000011920929) {

            *classIdx = 1;
            *classScore = 0.325;
            return;

          } else {

            *classIdx = 2;
            *classScore = 0.3333333333333333;
            return;
          }
        }
      }
    }
  }

  /**
   * Random forest's tree #3
   */
  void tree3(float *x, uint8_t *classIdx, float *classScore) {

    if (x[2] < 2.5) {

      *classIdx = 0;
      *classScore = 0.35833333333333334;
      return;

    } else {

      if (x[0] < 7.049999952316284) {

        if (x[2] < 4.75) {

          if (x[0] < 4.950000047683716) {

            if (x[2] < 3.899999976158142) {

              *classIdx = 1;
              *classScore = 0.3333333333333333;
              return;

            } else {

              *classIdx = 2;
              *classScore = 0.30833333333333335;
              return;
            }

          } else {

            *classIdx = 1;
            *classScore = 0.3333333333333333;
            return;
          }

        } else {

          if (x[3] < 1.75) {

            if (x[3] < 1.600000023841858) {

              if (x[2] < 4.950000047683716) {

                *classIdx = 1;
                *classScore = 0.3333333333333333;
                return;

              } else {

                *classIdx = 2;
                *classScore = 0.30833333333333335;
                return;
              }

            } else {

              *classIdx = 1;
              *classScore = 0.3333333333333333;
              return;
            }

          } else {

            if (x[2] < 4.8500001430511475) {

              if (x[1] < 3.100000023841858) {

                *classIdx = 2;
                *classScore = 0.30833333333333335;
                return;

              } else {

                *classIdx = 1;
                *classScore = 0.3333333333333333;
                return;
              }

            } else {

              *classIdx = 2;
              *classScore = 0.30833333333333335;
              return;
            }
          }
        }

      } else {

        *classIdx = 2;
        *classScore = 0.30833333333333335;
        return;
      }
    }
  }

  /**
   * Random forest's tree #4
   */
  void tree4(float *x, uint8_t *classIdx, float *classScore) {

    if (x[2] < 2.449999988079071) {

      *classIdx = 0;
      *classScore = 0.2916666666666667;
      return;

    } else {

      if (x[0] < 6.75) {

        if (x[3] < 1.75) {

          if (x[0] < 4.950000047683716) {

            if (x[2] < 3.899999976158142) {

              *classIdx = 1;
              *classScore = 0.36666666666666664;
              return;

            } else {

              *classIdx = 2;
              *classScore = 0.3416666666666667;
              return;
            }

          } else {

            if (x[2] < 5.049999952316284) {

              *classIdx = 1;
              *classScore = 0.36666666666666664;
              return;

            } else {

              if (x[3] < 1.550000011920929) {

                *classIdx = 2;
                *classScore = 0.3416666666666667;
                return;

              } else {

                *classIdx = 1;
                *classScore = 0.36666666666666664;
                return;
              }
            }
          }

        } else {

          if (x[2] < 4.8500001430511475) {

            if (x[1] < 3.0) {

              *classIdx = 2;
              *classScore = 0.3416666666666667;
              return;

            } else {

              *classIdx = 1;
              *classScore = 0.36666666666666664;
              return;
            }

          } else {

            *classIdx = 2;
            *classScore = 0.3416666666666667;
            return;
          }
        }

      } else {

        *classIdx = 2;
        *classScore = 0.3416666666666667;
        return;
      }
    }
  }
};

static RandomForestClassifier irisClassifier;

#endif