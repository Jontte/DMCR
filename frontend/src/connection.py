'''
Created on Feb 8, 2012

@author: blizzara
'''

import socket   # use BSD sockets
import struct   # used to convert to and fro binary data
import threading # each connection is a thread
import dmcr_protocol_pb2 as proto # import protobuf classes

# from socket.cpp
#  enum PacketId { Packet_BackendHandshake = 1, Packet_ConnectionResult = 2,
#                  Packet_NewTask = 3, Packet_RenderedData = 4 };

BACKENDHANDSHAKE = 1
CONNECTIONRESULT = 2
NEWTASK = 3
RENDEREDDATA = 4


# enum ConnectionResult { ConnectionResult_Success, ConnectionResult_InvalidKey,
#                       ConnectionResult_ConnectionFailed };
CONNECTIONRESULT_SUCCESS = 0
CONNECTIONRESULT_INVALIDKEY = 1
CONNECTIONRESULT_CONNECTIONFAILED = 2

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
        Initialize and connect
        '''
        super(Connection, self).__init__()
        self.conn = conn
        self.conn.settimeout(4)
        self.addr = addr
        self._stop = threading.Event()
        
    def run(self): # overriding threading.Thread.run()
    
        try:
            while not self.stopped():
                self.ReceivePacket()
        except KeyboardInterrupt:
            pass
        finally:
            self.Close()
        
    def __str__(self):
        '''
        
        @return: string version of a connection
        
        '''
        return "Socket connected to {0}".format(self.addr)
    
    def stop(self):
        self._stop.set()
        
    def stopped(self):
        return self._stop.isSet()
    
    def Close(self):
        '''
        Close connection
        
        '''
        self.conn.close()
    
    def ReceiveData(self, length):
        '''
        Receives data from own socket. 
        Returns when has length bytes or self.stop() is called.
        
        
        @param length: how many bytes are wanted
        @return: received data as it was received
        
        '''
        data = ''
        data_length = 0
        while data_length < length:
            if self.stopped():
                raise Connection.ThreadStopped("Stopped while receiveng data")
            try: 
                data += self.conn.recv(length-data_length)
            except socket.timeout:
                pass
            data_length = len(data)
        return data
    
    def ReceiveHeader(self):
        '''
        Receives info about coming packet (id) and it's length.
        
        @return: a tuple containing (id, length)
        
        '''
        
        # receive first header length
        try:
            data = self.ReceiveData(4) # first comes 4 bytes indicating headers length
        except Connection.ThreadStopped:
            return False, False
        
        data_tuple = struct.unpack("!L", data) # unpack binary data to long int (! means network)
        header_length  = int(data_tuple[0]) # int conversion maybe not needed, but first element of tuple anyways
        
        # if header length is usable, receive whole header
        if header_length < 1:
            return False, False
        
        try:
            header_data = self.ReceiveData(header_length)
        except Connection.ThreadStopped:
            return False, False
        
        header = proto.PacketHeader()
        header.ParseFromString(header_data)
        
        return header.id, header.length
        
    def ReceivePacket(self):
        packet_id, length = self.ReceiveHeader()
        if packet_id == False:
            return
        try:
            data = self.ReceiveData(length)
        except Connection.ThreadStopped:
            return
        
        if packet_id == 1:
            self.Recv_BackendHandshake(data)
        elif packet_id == 2:
            self.Recv_ConnectionResult(data)
        elif packet_id == 3:
            self.Recv_NewTask(data)
        elif packet_id == 4:
            self.Recv_RenderedData(data)
        else:
            pass
        
    def Recv_BackendHandshake(self, data):
        
        handshake = proto.BackendHandshake()
        handshake.ParseFromString(data)
        
        self.protocol_version = handshake.protocol_version
        self.description = handshake.description
        print handshake # nothing clever to do yet, just print
        
        self.Send_ConnectionResult(CONNECTIONRESULT_SUCCESS)
        self.Send_NewTask(1,400,300,10,self.scene)
        return handshake
        
    '''    def Recv_ConnectionResult(self, data):
        conn_result = proto.ConnectionResult()
        conn_result.ParseFromString(data)
        result = conn_result.result
        print conn_result # nothing clever to do yet, just print
        return conn_result
    '''
    
    '''    def Recv_NewTask(self, data):
        newtask = proto.NewTask()
        newtask.ParseFromString(data)
        
        print newtask # nothing clever to do yet, just print
        return newtask
    '''      
    def Recv_RenderedData(self, data):
        rendered_data = proto.RenderedData()
        rendered_data.ParseFromString(data)
        data_len = rendered_data.data_length
        try:
            data = self.ReceiveData(data_len)
        except Connection.ThreadStopped:
            print "Thread closed while receiveng picture."
            return
        i = 0
        pixels = list()
        print rendered_data
        print "unpacking image data"
        print 
        print str(i)+ "/" + str(data_len)
        while i + 11 < data_len:
            pixels.append([0,0,0,0])
            pixels[-1][0] = struct.unpack("!L", data[i:i+4])[0]
            pixels[-1][1] = struct.unpack("!L", data[i+4:i+8])[0]
            pixels[-1][2] = struct.unpack("!L", data[i+8:i+12])[0]
            
            #pixels.append(struct.unpack("!L"*3, data[i,i+12])) #doesn't work
            i = i + 12
        if len(pixels) != rendered_data.width * rendered_data.height:
            print "Dimension mismatch: got {} pixels, but image size should be {} x {}".format(len(pixels),rendered_data.width, rendered_data.height)
            return
        ListToPPM(pixels, rendered_data.width, rendered_data.height, "test.ppm")
        

        #return rendered_data


    def SendHeader(self, id, length):
        '''
        Sends info about coming packet (id) and it's length.
        
        @return: a tuple containing (id, length)
        
        '''
        
        head = proto.PacketHeader()
        head.length = length
        head.id = id
        
        
        
        head_size = head.ByteSize()
        head_size_bin = struct.pack("!L", head_size) 
        
        
        self.SendData(head_size_bin) # send size of header as !L
        
        self.SendData(head.SerializeToString()) # send the header
                
    def SendData(self, data):
        ''' Just sends given data to client.
        '''
        self.conn.sendall(data)
        
    def SendPacket(self, type_id, data):

        self.SendHeader(type_id, data.ByteSize())
        self.SendData(data.SerializeToString())
        
        
    def Send_BackendHandshake(self, data):
        
        handshake = proto.BackendHandshake()
        handshake.ParseFromString(data)
        
        self.protocol_version = handshake.protocol_version
        self.description = handshake.description
        print handshake # nothing clever to do yet, just print
        return handshake
        
    def Send_ConnectionResult(self, result):
        conn_result = proto.ConnectionResult()
        conn_result.result = result
        
        self.SendPacket(CONNECTIONRESULT, conn_result)
        
    def Send_NewTask(self, task_id, width, height, iterations, scene):
        newtask = proto.NewTask()
        newtask.id = task_id
        newtask.width = width
        newtask.height = height
        newtask.iterations = iterations
        newtask.scene = scene
        
        self.SendPacket(NEWTASK, newtask)
        
'''    def Send_RenderedData(self, data):
        rendered_data = proto.RenderedData()
        rendered_data.ParseFromString(data)
    
        print rendered_data # nothing clever to do yet, just print
        return rendered_data
        '''
