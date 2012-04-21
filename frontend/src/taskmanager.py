'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Mar 20, 2012

@author: blizzara
'''

import threading # taskmanager is a thread
import Queue    # results are stored and read from queue


class ITask(object):
    '''
    Task interface
    '''
    
    def OnTaskEnd(self, result):
        pass
    
    def SetTaskID(self, task_id):
        self.task_id = task_id
    
    def GetTaskID(self):
        return self.task_id
    
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
            self.taskmanager.tasks[task_id].OnTaskEnd(result)
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
        self.tasks = dict()
        self.uniqueid = 0
        self.results = Queue.Queue()
        self.stopped = False
        
        self.consumer = ResultConsumer(self)
        self.consumer.start()

    def stop(self):
        self.stopped = True 
        self.results.join() # wait for all results to be processed
    
        
    def AddTask(self, task): 
        '''
        
        @param task: task which implements ITask
        
        @return: id assigned to task
         
        '''
        task.SetTaskID(self.uniqueid)
        self.tasks[task.GetTaskID()] = task
        self.uniqueid += 1
        
        return task.GetTaskID()
    
    def RemoveTask(self, task_id):
        '''
        @param task_id: id assigned to task
        
        '''
        
        if self.tasks.has_key(task_id):
            del self.tasks[task_id]
            
    def GetTask(self):
        '''
        '''
        task = self.tasks[self.tasks.keys()[0]] #dirtydirty, return always first task
        return task
    
    def OnTaskEnd(self, task_id, result):
        '''
        '''
        print "TaskManager.OnTaskEnd(): Task ended:", task_id
        if self.tasks.has_key(task_id):
            self.results.put((task_id,result))
        else:
            print "TaskManager.OnTaskEnd(): Task has been removed!"        
