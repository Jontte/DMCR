'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Feb 8, 2012

@author: blizzara
'''


import threading # each connection is a thread

from dmcr_socket import Socket

VERSION = 2

class Connection(threading.Thread):
    '''
    classdocs
    '''

    class ThreadStopped(Exception):
        def __init__(self, value):
            self.value = value
        def __str__(self):
            return repr(self.value)


    def __init__(self, conn, addr):
        '''
        Takes care of a new connection. Timeout is set to 4s.
        
        '''
        super(Connection, self).__init__()
        self.socket = Socket(conn, addr)
        self._stop = threading.Event()
        self.taskmanager = None
        self.task = None
    
    
    def SetTaskManager(self, taskmanager):
        self.taskmanager = taskmanager
    
    def FetchTask(self):
        if self.taskmanager:
            return self.taskmanager.GetTask()
        else:
            print "Connection.FetchTask(): No taskmanager, can't fetch task"
            return None


    def Connect(self):
        '''
        Connects with BE.
        
        '''
        
        result = False
        while not self.stopped() and not result: # try to backend handshake until succeeded
            result = self.socket.Recv_BackendHandshake() # [0] = version, [1]  = description
            
        if result[0] != VERSION: # if backend version doesn't match us 
            self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_CONNECTIONFAILED) # tell BE connection failed
            raise Socket.SocketException("Incompatible version")    # raise exception so this loop ends
            
        self.be_description = result[1] # store the description backend gives about itself
            
        self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_SUCCESS)
                          
    
    def run(self): # overriding threading.Thread.run()
        '''
        Receives packets until someone calls Connection.stop() or catches KeyboardInterrupt
        (which is probably impossible). Closes connection at exit. 
    
    
        @return: nothing
    
        '''
        
        task = None

        try:
            self.Connect() # handles connection with BE
            
            while not self.stopped():
                if task and not task.IsAvailable():                
                    self.socket.Send_QuitTask(task.GetTaskID()) # tell backend this is last to render
                    
                if not task or not task.IsAvailable():
                    task = self.FetchTask()
                    if task:
                        self.socket.Send_NewTask(task.task_id, task.width, task.height, task.iterations, task.json)    
                
                if task and task.IsAvailable():
                    rendered_data = self.socket.Recv_RenderedData()
                    if rendered_data:
                        #print rendered_data
                        self.taskmanager.OnTaskEnd(rendered_data['task_id'], rendered_data) #image contains (data, iterations)
                            
        except KeyboardInterrupt:
            pass
        except Socket.SocketException as se:
            print se
        finally:
            self.socket.Close() # socket must be closed no matter what happens
        
    def __str__(self):
        '''
        
        @return: string version of a connection
        
        '''
        return "Socket connected to {0}".format(self.socket.addr)
    
    def stop(self):
        '''
        
        Tells Connection-thread it has to stop.
        
        @return: nothing
        
        '''
        
        self._stop.set()
        self.socket.stopped = True
        self.socket.Close()
        
    def stopped(self):
        '''
        Tells if thread has to stop. 
        
        THIS MUST BE CHECKED IN EVERY OTHERWISE-INFINTE LOOP!
        
        @return: wheter this has to stop (bool)
        
        '''
        
        return self._stop.isSet()
