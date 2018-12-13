import keras
import numpy as np

def load_model(path):
    f = open(path,'r')
    jsonstr=f.read()
    f.close()
    return keras.models.model_from_json(jsonstr)

def save_model(model,path):
    f = open(path, 'w')
    f.write(model.to_json())
    f.close()