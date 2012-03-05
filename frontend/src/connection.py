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

class Connection(threading.Thread):
    '''
    classdocs
    '''


    def __init__(self, conn, addr):
        '''
        Initialize and connect
        '''
        super(Connection, self).__init__()
        self.conn = conn
        self.addr = addr
        
    def run(self): # overriding threading.Thread.run()
    
        while True:
            self.ReceivePacket()
    
        
    def __str__(self):
        '''
        
        @return: string version of a connection
        
        '''
        return "Socket connected to {0}".format(self.addr)
    
    def Close(self):
        '''
        Close connection
        
        '''
        self.conn.close()
    
    def ReceiveHeader(self):
        '''
        Receives info about coming packet (id) and it's length.
        
        @return: a tuple containing (id, length)
        
        '''
        
        
        # receive first header length
        data = '' # empty string to store what we get
        while len(data) < 4: # first coming is 4 bytes
            data += self.conn.recv(4-len(data)) # recv may not give everything at once, so wait until we have 4 bytes
        
        data_tuple = struct.unpack("!L", data) # unpack binary data to long int (! means network)
        header_length  = int(data_tuple[0]) # int conversion maybe not needed, but first element of tuple anyways
        
        # if header length is usable, receive whole header
        if header_length < 1:
            return False
        
        header_data = self.ReceiveData(header_length)
        
        header = proto.PacketHeader()
        header.ParseFromString(header_data)
        
        return (header.id, header.length)
        
    def ReceiveData(self, length):
        data = ''
        data_length = 0
        while data_length < length:
            data += self.conn.recv(length-data_length)
            data_length = len(data)
        return data
        
    def ReceivePacket(self):
        packet_id, length = self.ReceiveHeader()
        
        data = self.ReceiveData(length)
        
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
        self.Send_NewTask()
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
    
        print rendered_data # nothing clever to do yet, just print
        return rendered_data


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
        
        self.SendData(head) # send the header
                
    def SendData(self, data):
        ''' Just sends given data to client.
        '''
        self.conn.sendall(data)
        
    def SendPacket(self, type_id, data):

        self.SendHeader(type_id, data.ByteSize())
        self.SendData(data)
        
        
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
