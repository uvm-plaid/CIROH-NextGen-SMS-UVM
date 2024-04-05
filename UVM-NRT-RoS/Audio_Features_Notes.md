# Audio Features Notes

* Very helpful page to get code for many audio features: https://scikit-maad.github.io/features.html

## Entropy (this is helpful to understand for temporal and spectoral entropy indices)
 * Helpful StatQuest Video about Entropy: https://www.youtube.com/watch?v=YtebGVx-Fxw
    * Surprise is inversely proportional to probability (high probability of X occurring, low surprise that X occurred)
    * Surprise = log(1/Probability)
    * Total surprise = all of the surprise values added up
    * Entropy is the average of all of the surprise values
    * Entropy is the expected value of the surprise
    * E(Surprise) = Probability a surprise will occur * Surprise

Note: I believe temporal entropy and spectral entropy can be used in place of temporal entropy index and spectral entropy index. Meriem Ferroudj's paper, "Detection of Rain in Acoustic Recordings of the Environment Using Machine Learning Techniques" mentions temporal entropy index and spectral entropy index, but then refers to these as temporal entropy and spectral entropy throughout the paper. When I look up these terms further, I am only finding information about temporal/spectral entropy (not including index).

## Temporal Entropy


## Spectral Entropy



## Shannon Entropy