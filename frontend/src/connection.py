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
from datetime import datetime

def ListToPPM(pixels, width, height, filename):
    with open(filename, 'w') as fp:
        #write ppm header
        fp.write("P3\n")
        fp.write("{} {}\n".format(width, height))
        fp.write("255\n")    

        #write pixels
        for i in range(len(pixels)):
            fp.write("%d %d %d\n" % (pixels[i][0],pixels[i][1],pixels[i][2])) #use with P3
            #   pfile.write("%c%c%c" % (red,green,blue))#use with P6
    
        print "File {} was written".format(filename)

VERSION = 0

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
        
        
    def SetScene(self, scene, width, height, sceneid, name, datestring="%Y%m%d-%H%M%S", extension = "ppm"):
        '''
        Sets the scene for the thread to render.
        
        @param scene: the json-scene (string)
        @param width: wanted width of image (int)
        @param height: wanted height of image (int)
        @param sceneid: internal id of scene
        @param name: wanted name of image (W/O extension, eg. "test")
        @param datestring: formation of the date string to be included in filename (default "%Y%m%d-%H%M%S" results in "20120225-184236")
        @param extension: filename extension (default "ppm")  
        
        '''     
        self.scene = scene
        self.width = width
        self.height = height
        self.iterations = 0
        self.sceneid = sceneid
        self.img_name = name
        self.img_extension = extension
        self.datestring = datestring 
        
    def run(self): # overriding threading.Thread.run()
        '''
        Receives packets until someone calls Connection.stop() or catches KeyboardInterrupt
        (which is probably impossible). Closes connection at exit. 
    
    
        @return: nothing
    
        '''
        result = False
        try:
            while not self.stopped() and not result: # try to backend handshake until succeeded
                result = self.socket.Recv_BackendHandshake() # [0] = version, [1]  = description
            
            if result[0] != VERSION: # if backend version doesn't match us 
                self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_CONNECTIONFAILED) # tell BE connection failed
                raise Socket.SocketException    # raise exception so this loop ends
            
            self.be_description = result[1] # store the description backend gives about itself
            
            self.socket.Send_ConnectionResult(Socket.CONNECTIONRESULT_SUCCESS)
            self.socket.Send_NewTask(self.sceneid, self.width, self.height, self.iterations, self.scene)
        
            while not self.stopped():
                image = self.socket.Recv_RenderedData()
                if image:
                    ListToPPM(image, self.width, self.height, self.img_name+str(datetime.now().strftime(self.datestring))+"."+self.img_extension)
            
            
        except KeyboardInterrupt:
            pass
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
        
    def stopped(self):
        '''
        Tells if thread has to stop. 
        
        THIS MUST BE CHECKED IN EVERY OTHERWISE-INFINTE LOOP!
        
        @return: wheter this has to stop (bool)
        
        '''
        
        return self._stop.isSet()
