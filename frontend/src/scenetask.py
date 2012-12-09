'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Apr 21, 2012

@author: blizzara
'''

import datetime

import taskmanager
import image

import preview

class SceneTask(taskmanager.ITask):
        
    def __init__(self):
        '''
        Initializes scene with default width and height. Use SetSceneFromFile and SetSceneOpts
        
        @param id: a unique id for this scene 
        '''
        self.json = ""
        self.image = None
        self.width = 800
        self.height = 600
        self.iterations = 2
        self.img_name = ""
        self.filename = ""
        self.datestring = "%Y%m%d-%H%M%S"
        self.img_extension = "png"
        self.preview = preview.Preview()
        
    def __str__(self):
        string = super(SceneTask, self).__str__() 
        string += "  :::  Rendering '{}' as '{}': {}x{}, {} iterations.".format(self.filename, self.img_extension, self.width, self.height, self.iterations)
        string += "  :::  {}".format(str(self.image))
        return string

    
    def SetSceneFromFile(self, filename):
        '''
        Reads a file and stores in self.scene, which is given to every connection.
        
        @param filename: the filename of the json-file describing the scene.
        
        ''' 
        with open(filename,'r') as f:
            self.filename = filename
            self.json = '\n'.join(f.readlines())
            self.img_name = filename.split("/")[-1].replace(".json", "")
            
    
    def SetSceneOpts(self, width = None, height = None, iterations = None):
        if width:
            self.width = width
        if height:
            self.height = height
        if iterations:
            self.iterations = iterations
        
    def BlendResult(self, data, iterations, fmt):
        if not self.image:
            self.image = image.Image(self.width, self.height)
        self.image.AddFromString(data, iterations, fmt)
        
    def OnTaskEnd(self, rendered_data):
        #print "SceneTask.OnTaskEnd(): Task finished, blending and writing."
        self.BlendResult(rendered_data['data'], rendered_data['iterations_done'], rendered_data['data_format'])
        self.last_filename = self.img_name+str(datetime.datetime.now().strftime(self.datestring))+"."+self.img_extension
        self.image.Write(self.last_filename)
        self.preview.update(self.last_filename)
