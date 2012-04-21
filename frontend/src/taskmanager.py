'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Mar 20, 2012

@author: blizzara
'''

import datetime
import threading # taskmanager is a thread
import Queue    # results are stored and read from queue

import image


class Scene(object):
    '''
     
    '''
    
    def __init__(self, id):
        '''
        Initializes scene with default width and height. Use SetSceneFromFile and SetSceneOpts
        
        @param id: a unique id for this scene 
        '''
        self.json = ""
        self.id = id
        self.image = None
        self.width = 800
        self.height = 600
        self.iterations = 2
        self.img_name = ""
        self.datestring = "%Y%m%d-%H%M%S"
        self.img_extension = "png"
        
    
    def SetSceneFromFile(self, filename):
        '''
        Reads a file and stores in self.scene, which is given to every connection.
        
        @param filename: the filename of the json-file describing the scene.
        
        ''' 
        with open(filename,'r') as f:
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
        
    def OnTaskEnd(self, image):
        print "Task finished, blending and writing."
        self.BlendResult(*image)
        self.image.Write(self.img_name+str(datetime.datetime.now().strftime(self.datestring))+"."+self.img_extension)
    

class ResultConsumer(threading.Thread):
    
    def __init__(self, taskmanager):
        super(ResultConsumer, self).__init__()
        self.taskmanager = taskmanager
        self.setDaemon(True) # daemon thread, doesn't have to be stopped
    
    def run(self):
        while True:
            task_id,result = self.taskmanager.results.get(block = True, timeout = None)
            #print self.taskmanager.results.get(block = True, timeout = None)
            #print "task_id:",task_id
            #print "Result: ",result
            self.taskmanager.scenes[task_id].OnTaskEnd(result)
            self.taskmanager.results.task_done()
            

class TaskManager(object):
    '''
    TaskManager creates tasks, delivers them to backend through Connections 
    and composes images.
    '''


    def __init__(self):
        '''
        Constructor
        '''        
        self.scenes = dict()
        self.uniqueid = 0
        self.results = Queue.Queue()
        self.stopped = False
        
        self.consumer = ResultConsumer(self)
        self.consumer.start()

    def stop(self):
        self.stopped = True 
        self.results.join() # wait for all results to be processed
    
        
    def AddScene(self, filename, width, height, iterations): 
        '''        
        '''
        newscene = Scene(self.uniqueid)
        self.uniqueid += 1
        
        newscene.SetSceneFromFile(filename)
        newscene.SetSceneOpts(width, height, iterations)
        
        self.scenes[newscene.id] = newscene
    
    def GetTask(self):
        '''
        '''
        scene =self.scenes[self.scenes.keys()[0]] #dirtydirty 
        return (scene.id, scene.width, scene.height, scene.iterations, scene.json)
    
    def OnTaskEnd(self, task_id, result):
        '''
        '''
        print "Task ended:", task_id
        self.results.put((task_id,result))        
