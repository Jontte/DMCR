'''
Created on Feb 8, 2012

@author: blizzara
'''
import socket
import connection

class Server(object):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor
        '''
        self.host = ''
        self.port = 9393
        self.connections = list()
    
    def FileToTask(self, filename):
        with open(filename,'r') as f:
            self.scene = '\n'.join(f.readlines())     
    
    def Listen(self):

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.host, self.port)) # takes tuple containing host and port
        s.listen(1)

        while True:
            self.connections.append(connection.Connection(*s.accept()))
            self.connection.scene = self.scene
            self.connections[-1].start()
        
        