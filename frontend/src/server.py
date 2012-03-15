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
import connection

class Server(object):
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
        self.host = ''
        self.port = port
        self.connections = list()
    
    def FileToTask(self, filename):
        '''
        Reads a file and stores in self.task, which is given to every connection.
        
        @param filename: the filename of the json-file describing the scene.
        
        ''' 
        with open(filename,'r') as f:
            self.scene = '\n'.join(f.readlines())     
    
    def Listen(self):
        '''
        Starts the server. Server accepts connections and creates connection.Connections take care of each backend.
        For each new backend self.scene is given to be rendered.
          
        Only way to stop nicely is KeyboardInterrupt. 
        
        @return: nothing
        
        '''

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.host, self.port)) # takes tuple containing host and port
        s.listen(1)
        #s.settimeout(10)
        try:
            while True:
                self.connections.append(connection.Connection(*s.accept()))
                self.connections[-1].SetScene(scene=self.scene, width=400, height=300, sceneid=1, name="test")
                self.connections[-1].start()
        except KeyboardInterrupt as e:
            print "Excepted, quitting", e
            for i,a in enumerate(self.connections):
                print "Stopping thread ", a
                self.connections[i].stop()
                self.connections[i].join()
                print "Thread stopped"
        