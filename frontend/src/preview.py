'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on December 8, 2012

@author: blizzara
'''

import Image
import ImageTk
import mtTkinter.mtTkinter as Tkinter

class Preview():
        
    def __init__(self):
        '''
        '''
        self.main_window = Tkinter.Tk()

        self.tkimg = None;

    def update(self,filename):
        img = Image.open(filename)
        if not self.tkimg:
            self.tkimg = ImageTk.PhotoImage(img)
            self.image_label = Tkinter.Label(self.main_window, image=self.tkimg)
            self.image_label.pack()
        else:
            self.tkimg.paste(img)

    def __str__(self):
        return "Preview window"

    
