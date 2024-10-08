# Environmental Machine Learning Audio Features and Models Follow-Up Literature Review
### Summary/Important Points: 
- New features to possibly test (citations of where to find more info below in notes)
    - Spectral Features
      - Band energy ratio (https://github.com/musikalkemist/AudioSignalProcessingForML/blob/master/22%20-%20Implementing%20Band%20Energy%20Ratio%20from%20Scratch%20with%20Python/Implementing%20band%20energy%20ratio%20from%20scratch.ipynb)
      - Spectral Flux
      - Statistical Moments
        - Spectral Centroid
        - Signal Bandwidth
        - Spectral flatness
        - Spectral roll-off
      - Short time energy
      - Linear prediction Cepstral Coefficients (LPCC)
      - Matching Pursuit: method of feature extraction
        - [title] (https://scikit-learn.org/stable/modules/generated/sklearn.linear_model.OrthogonalMatchingPursuit.html)
        - Still trying to fully understand how this extracts the features/if this creates different features than other methods? Is this being used for feature extraction, feature selection, or both?
        - Question: most articles I’m seeing that use this are from 2000s-2010s. The latest I’ve seen using MP is 2021. Is this okay, or would this indicate that it’s outdated in the field? The articles I did see claim that using MP allows them to get higher accuracy than with just MFCCs.
      - "A Deep Learning Approach to Speech, Music and Environmental Noise Classification"
        - Lists other features on pages 75-76 along with feature importance. Just found this article; will look more into the features listed.
      - Possible models to try:
        - Gaussian Mixture Models: I saw this mentioned in a few papers and there is a Python function for it. However, it does not look to be supported by micromlgen.
        - Models we have not yet tried that are supported by micromlgen:
          - Gaussian NB
          - Relevance Vector Machines (like SVM but much smaller and gets the probabilities of the classification results)
            - (https://eloquentarduino.github.io/2020/02/even-smaller-machine-learning-models-for-your-mcu/)
            - Gaussian NB (generally seems to not perform as well as SVM)-is it worth trying anyway?
            - SEFR
          - K Nearest Neighbors
            - I’ve seen this mentioned in multiple ML environmental classification papers, but am not sure if it is supported by micromlgen
  - General question/still trying to get the hang of: how to know when to try something vs when to research more if it’s worth trying?

### Notes on Papers Reviewed:
  - “Environmental sound monitoring using machine learning on mobile devices” by Marc Green and Damian Murphy (https://www.sciencedirect.com/science/article/pii/S0003682X19303007)
    - *Biphony vs geophony “which split natural sounds into those produced by animals and those produced by geological forces” (could this be something we consider later on, to rule out weather sounds vs animal sounds?)*
    - Uses MFCCs
    - “Mel-Frequency Cepstral Coefficient (MFCC) features were extracted from the zeroth-order (mono-omni) channel in a manner similar to the baseline models in [19], [20]… The library was configured to extract 20 MFCC coefficients, covering the frequency range up to approximately 11 kHz.”
    - References a paper using the Gaussian Mixture Model (Looks like this  separates data into clusters, similar to k means clustering-trying to understand if we can use this to perform classification?) (https://scikit-learn.org/stable/modules/generated/sklearn.mixture.GaussianMixture.html)
  - "Acoustic scene classification: A comprehensive survey” (https://www.sciencedirect.com/science/article/abs/pii/S0957417423024041#:~:text=Acoustic%20scene%20classification%20(ASC)%20is%20to%20assign%20a)
    - Not fully applicable, but did a quick skim to see if anything relates. Does involve analyzing audio signals
  - “Environmental Sound Recognition With Time–Frequency Audio Features" (https://ieeexplore.ieee.org/abstract/document/5109766)
    - Uses time domain features, this paper uses in conjunction with MFCCs to improve classification accuracy
    - Matching Pursuit (MP) for signal classification (!!)
      - {https://scikit-learn.org/stable/modules/generated/sklearn.linear_model.OrthogonalMatchingPursuit.html)
    - Short-time energy
    - Spectral features
      - MFCCs
      - Band energy ratio
        - https://github.com/musikalkemist/AudioSignalProcessingForML/blob/master/22%20-%20Implementing%20Band%20Energy%20Ratio%20from%20Scratch%20with%20Python/Implementing%20band%20energy%20ratio%20from%20scratch.
      - Spectral Flux
      - Statistical Moments
        - Spectral Centroid
        - Signal Bandwidth
        - Spectral flatness
        - Spectral roll-off
    - Linear prediction cepstral coefficients (LPCCs)
      - “Current sample can be predicted… as a linear combination of previous samples… calculated using autocorrelation method in this work [L. Rabiner and B.-H. Juang, Fundamentals of Speech Recognition, NJ, Englewood Cliffs:Prentice-Hall, 1993.
    - Feature selection discussion in paper:
      - Adding more features not always helpful
      - As feature dimension increases, “data points become sparser and there are potentially irrelevant features that could negatively impact the classification result”
      - This article reviews a paper the authors wrote (5. S. Chu, S. Narayanan, C.-C. J. Kuo and M. J. Matari, "Where am I? Scene recognition for mobilerobots using audio features", Proc. ICME, 2006.) with the important outcome “Although the results showed improvements, the features found after the feature selection process were found to be specific to each classifier and environment type.” *****
        - Similar phenomenon observed in this paper 
        - 13. V. Peltonen, Computational auditory scene recognition, 2001.
        - Authors of this paper created Matching Pursuit to try to improve feature selection
        - (Question, I wonder if our proposed grid search method would work, or generate the same problem? Or wait, that’s feature importance… so they’re still doing feature selection? Or are they meshing them together into one with MP?)
    - Signal Representation with Matching Pursuit (MP)
      - Obtaining minimum num of basis vectors to represent a signal (sparse approximation) – NP-complete problem
      - “Dictionary that capacitates the decomposition of a signal by selecting basis vectors from a given dictionary to find the best subset”
      - Decomposition of an overcomplete function
    - Feature Extraction w/ MP
      - Advantages:
        - Captures inherent structure within each type of signal and maps from a large, complex signal to a small, simple feature space
        - More invariant to background noise and may capture characteristics in the signal that MFCCs miss!
        - (Question: Still trying to understand, is MP being used as a method for extracting features? So what we’d get back, we’d call MP features? So I don’t think this is feature importance? OH-yes-because it’s feature extraction. But where does feature selection come in?)
      - MP can be used to recover/approximate the original signal, however, here it is being used as a tool for feature extraction for classification
      - Reduces the residual energy w/ as few atoms as possible
      - Gets us an interpretation of the signal structures
      - Based on assumption that most important info of a signal lies in leading synthesizing atoms w/ the highest energy. So, MP selects atoms in order by eliminating the largest residual energy
    - https://ieeexplore.ieee.org/abstract/document/6469861
      - (Another article, showing the use of MP in environmental audio)
      - Question: most articles I’m seeing are from early 2000s-2010s. The latest I’ve seen using MP is 2021. Is this okay, or would this indicate that it’s outdated in the field? Trying to understand how much to research vs just try it out.
      - Also mentions k-nearest neighbors. Should we try this model?
     - "A Deep Learning Approach to Speech, Music and Environmental Noise Classification"
       - (Paper can be downloaded as a PDF, but I am not seeing a link)
       - Performs both non-neural network models and neural network models, including a CNN. Shows highest accuracy w/ CNN, and SVM slightly behind.
       - Pages 75-76 have a list of features! Most of which look different from what we've tried. Also shows ranking of features (like feature importance).








