#we want this cooler division here
from __future__ import division

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
import pypng as png
from dmcr_socket import Socket

# the exception to throw when something bad happens here
class InvalidImage(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class Image(object):
    '''
    Image-class takes care of turning incoming binary data into images. 
    Averages multiple pass images to one and writes png-files.
    
    
    '''


    def __init__(self, width, height):
        '''
        Initializes Image-instance with given width and height.
        
        @param width: image width in pixels
        @param height: image height in pixels  
        
        '''
        self.iterations = 0 # iterations initially 0
        
        self.width = width  # store image width and height
        self.height = height
        
        # initialize boxed-row flat-pixel -list for storing image in memory, see pypng for details
        self.image = [[0 for i in range(width*3)] for j in range(height)]
        
        # initialize png.Writer, see pypng for details 
        self.writer = png.Writer(width = self.width, height = self.height, bitdepth = 8)
    
    def AddFromString(self, data, iterations, fmt):
        '''
        Average new image and the instance image. 
        Actually sums the iteration-weighted values of new image into existing one.
        Raises InvalidImage if data's images dimensions don't match instances.
        
        @param data: png as string
        @param iterations: how many iterations the new image contains
        
        @return: nothing
          
        '''
        # enum return tuple of png.Reader()
        WIDTH = 0
        HEIGHT = 1
        PIXELS = 2
        META = 3
        
        # read incoming image data into boxed-row flat-pixel list
        new_img = png.Reader(bytes=data).read() # (width, height, pixels, meta)
        
        #test that image dimensions match
        if new_img[WIDTH] != self.width or new_img[HEIGHT] != self.height:
            raise InvalidImage # raises exception if they don't

        print "Image format", fmt

        for h, row in enumerate(new_img[PIXELS]): # boxed-row flat-pixel list
            for i, value in enumerate(row):
                if (fmt == Socket.PNG_8BIT):
                    value = value / 255 * 1023
                self.image[h][i] += value*iterations # weight pixel values by iterations
                          
        self.iterations += iterations # increment instances iteration count
        
    def Clamp16(self, v):
        if v > 1023:
            v = 1023
        return v / 1023 * 255

    def Write(self,filename):
        '''
        Writes the image contained in this instance to file "filename"
        
        
        @param filename: the file image will be written into
        @return: nothing
        
        ''' 
        print "Writing image to file", filename
        
        iterations = self.iterations if self.iterations > 0 else 1 #prevent division by zero
        print "So far we have done", self.iterations, "iterations, jea!"
        # values are weighted sums, so divide by iteration count
        scaled = [[self.Clamp16(value / iterations) for value in row] for row in self.image] 
 
        
        #write to file (wb = write binary)
        with open(filename, 'wb') as fp:
            self.writer.write(fp, scaled)
        print "Image writing completed!"
        
        
