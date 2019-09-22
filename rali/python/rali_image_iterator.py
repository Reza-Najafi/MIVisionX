
from  rali_common import *
import numpy as np

class ImageIterator:
    def __init__(self, pipeline,tensor_layout = TensorLayout.NCHW, multiplier = 1.0, offset = 0.0):
        self.pipe = pipeline
        self.tensor_format =tensor_layout
        self.multiplier = multiplier
        self.offset = offset
        if pipeline.build() != 0:
            raise Exception('Failed to build the augmentation graph')
        self.w = pipeline.getOutputWidth()
        self.h = pipeline.getOutputHeight()
        self.b = pipeline.getBatchSize()
        self.n = pipeline.getOutputImageCount()
        color_format = self.pipe.getOutputColorFormat()
        self.p = (1 if color_format is ColorFormat.IMAGE_U8 else 3)
        height = self.h*self.b*self.n
        #print ('h = ', h, 'w = ', self.pipe.w)
        self.out_image = np.zeros((height, self.w, self.p), dtype = "uint8")
        self.out_tensor = np.zeros(( self.b*self.n, self.p, self.h, self.w,), dtype = "float32")

    def next(self):
        return self.__next__()

    def __next__(self):

        if self.pipe.getReaminingImageCount() <= 0:
            raise StopIteration

        if self.pipe.run() != 0:
            raise StopIteration

        self.pipe.copyToNPArray(self.out_image)
        if(TensorLayout.NCHW == self.tensor_format):
            self.pipe.copyToTensorNCHW(self.out_tensor, self.multiplier, self.offset)
        else:
            self.pipe.copyToTensorNHWC(self.out_tensor, self.multiplier, self.offset)

        return self.out_image , self.out_tensor

    def reset(self):
        self.pipe.reset()

    def __iter__(self):
        self.pipe.reset()
        return self

    def imageCount(self):
        return self.pipe.getReaminingImageCount()