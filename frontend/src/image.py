'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Mar 17, 2012

@author: blizzara
'''

#Requires PyPNG
import assets.png as png

#we want this cooler division here
from __future__ import division

class InvalidImage(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class Image(object):
    '''
    classdocs
    '''


    def __init__(self, width, height):
        '''
        Constructor
        '''
        self.iterations = 0
        self.width = width
        self.height = height
        self.image = [[[]*3]*height]*width
        self.writer = png.Writer(width = self.width, height = self.height, bitdepth = 8)
    
    def AddFromString(self, data, iterations):
        '''
        
        @param data: png as string
        @param iterations: how many iterations the new image contains
          
        '''
        WIDTH = 0
        HEIGHT = 1
        PIXELS = 2
        META = 3
        
        new_img = png.Reader(data=data).read() # (width, height, pixels, meta)
        if new_img[WIDTH] != self.width or new_img[HEIGHT] != self.height:
            raise InvalidImage
        
        for w in xrange(self.width):
            for h in xrange(self.height):
                for i in range(3):
                    self.image[w][h][i] += new_img[PIXELS][w][h][i]
        
        self.iterations += iterations
        
        
    def Write(self,filename):
        # should work, even thoug Aptana complains
        self.writer.write(filename, [[[channel / self.iterations for channel in pixel] for pixel in row] for row in self.image])
        
        