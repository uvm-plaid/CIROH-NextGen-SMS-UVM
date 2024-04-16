"""
Deep Learning on .wav dataset using MFCC extraction and Neural Networks
"""
import tensorflow as tf
keras = tf.keras
from keras import Sequential, layers
from MfccExtractor import MfccExtractor

NUM_CLASSES = 9

mfcc_extractor = MfccExtractor(num_mfcc=13,one_hot_encode=True,flatten=False)
X_train,X_test,y_train,y_test = mfcc_extractor.get_data()
print(X_train.shape,y_train.shape,X_test.shape,y_test.shape)


model = Sequential()
model.add(layers.Dense(4, activation='relu', input_shape=(X_train[0].shape)))
#model.add(layers.Conv1D(kernel_size=(3),filters=8))
model.add(layers.Dense(activation="softmax",units=NUM_CLASSES))

model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.fit(X_train, y_train, epochs=20,validation_split=0.2)