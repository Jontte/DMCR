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
            self.task = self.taskmanager.GetTask()
        else:
            print "Connection.FetchTask(): No taskmanager, can't fetch task"

        
    def run(self): # overriding threading.Thread.run()
        '''
        Receives packets until someone calls Connection.stop() or catches KeyboardInterrupt
        (which is probably impossible). Closes connection at exit. 
    
    
        @return: nothing
    
        '''
        
        self.FetchTask()
        
        result = False
        try:
            while not self.stopped() and not result: # try to backend handshake until succeeded
                result = self.socket.Recv_BackendHandshake() # [0] = version, [1]  = description
            
            if result[0] != VERSION: # if backend version doesn't match us 
                self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_CONNECTIONFAILED) # tell BE connection failed
                raise Socket.SocketException("Incompatible version")    # raise exception so this loop ends
            
            self.be_description = result[1] # store the description backend gives about itself
            
            self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_SUCCESS)
            self.socket.Send_NewTask(self.task.task_id, self.task.width, self.task.height, self.task.iterations, self.task.json)
            counter = 0
            
            while not self.stopped():
                image = self.socket.Recv_RenderedData()
                if image:
                    self.taskmanager.OnTaskEnd(self.task.GetTaskID(), image) #image contains (data, iterations)
                counter += 1
                if counter == 2:
                    print "Connection.run(): Got two images out of this backend, stopping it now."
                    self.socket.Send_QuitTask(self.task.GetTaskID()) # tell backend this is last to render
                elif counter == 3:
                    print "Connection.run(): This backend has finished it's work."
                    self.stop()
            
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
