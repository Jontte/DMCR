'''
Created on Feb 8, 2012

@author: blizzara
'''

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
        
    
    def Listen(self):
        self.connection = connection.Connection(self.host, self.port)
        print self.connection # just for debugging
        
        self.connection.ReceivePacket()
        self.connection.Close()
