'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

'''
Created on Feb 8, 2012

@author: blizzara
'''
import socket
import threading
import connection
import taskmanager
import scenetask

class Server(threading.Thread):
    '''
    Server accepts connections and gives them tasks to do.
    Use Server.FileToTask(filename) to set the scene to be rendered.
    Then call Server.Listen() to begin listening.
    
    
    '''

    def __init__(self, port = 9393):
        '''
        Constructor initializes server. Use Listen() to start accepting connections.
        
        
        @param port: port to listen to (default 9393) 
        
        '''
        
        super(Server, self).__init__()
        
        self.host = ''
        self.port = port
        self.connections = list()
        self.taskmanager = taskmanager.TaskManager()
        self.stopped = False
        self.setDaemon(True) # set daemon so we don't have to wait for accept to quit
    
    def stop(self):
        if not self.stopped:
            self.stopped = True
            self.s.settimeout(1)
            self.s.close()
            print "Server stopped"
        
        
    
    def FileToTask(self, filename, width, height, iterations):
        '''
        Reads a file and stores in self.task, which is given to every connection.
        
        @param filename: the filename of the json-file describing the scene.
        
        '''
        try:
            task = scenetask.SceneTask()
            task.SetSceneFromFile(filename)
            task.SetSceneOpts(width, height, iterations)
            self.taskmanager.AddTask(task)
        except IOError as e:
            print "Server.FileToTask: IOError with file", filename, ":", e
            
         

    
    def run(self):
        '''
        Starts the server. Server accepts connections and creates connection.Connections take care of each backend.
        For each new backend self.scene is given to be rendered.
          
        Only way to stop nicely is KeyboardInterrupt. 
        
        @return: nothing
        
        '''

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind((self.host, self.port)) # takes tuple containing host and port
        self.s.listen(1)
        #self.s.settimeout(5)
        try:
            while not self.stopped:
                self.connections.append(connection.Connection(*self.s.accept()))
                self.connections[-1].SetTaskManager(self.taskmanager)
                self.connections[-1].start()
        except KeyboardInterrupt as e:
            print "Excepted, quitting", e
        finally:
            for i,a in enumerate(self.connections):
                print "Stopping thread ", a
                self.connections[i].stop()
                self.connections[i].join()
                print "Thread stopped"
        