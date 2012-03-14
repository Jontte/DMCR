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
        Takes care of a new connection. Timeout is set to 4s.
        
        '''
        super(Connection, self).__init__()
        self.conn = conn
        self.conn.settimeout(4)
        self.addr = addr
        self._stop = threading.Event()
        
    def run(self): # overriding threading.Thread.run()
        '''
        Receives packets until someone calls Connection.stop() or catches KeyboardInterrupt
        (which is probably impossible). Closes connection at exit. 
    
    
        @return: nothing
    
        '''
    
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
        '''
        
        Tells Connection-thread it has to stop.
        
        @return: nothing
        
        '''
        
        self._stop.set()
        
    def stopped(self):
        '''
        Tells if thread has to stop. 
        
        THIS MUST BE CHECKED IN EVERY OTHERWISE-INFINTE LOOP!
        
        @return: wheter this has to stop (bool)
        
        '''
        
        return self._stop.isSet()
    
    def Close(self):
        '''
        Closes connection
        
        @return: nothing
        
        '''
        self.conn.close()
    
    def ReceiveData(self, length):
        '''
        Receives data from own socket. 
        Returns when has received asked number of bytes.
        Raises Connection.ThreadStopped if someone calls Connection.stop() 
        while this is running.
        
        @param length: how many bytes are wanted (int)
        @return: received data (as it was received)
        
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
        '''
        Receives a complete packet: first header and then the packet-data itself. 
        Packets are given to separate functions to handle. 
        
        
        @return: nothing 
                
        '''
        packet_id, length = self.ReceiveHeader()
        if packet_id == False:
            return
        try:
            data = self.ReceiveData(length)
        except Connection.ThreadStopped:
            return
        
        if packet_id == 1:
            self.Recv_BackendHandshake(data)
#        elif packet_id == 2:                    # these packets are
#            self.Recv_ConnectionResult(data)    # sent by frontend
#        elif packet_id == 3:                    # so no need to 
#            self.Recv_NewTask(data)            # receive them
        elif packet_id == 4:
            self.Recv_RenderedData(data)
        else:
            pass
        
    def Recv_BackendHandshake(self, data):
        '''
        Turns binary string into BackendHandshake-protobuf packet.
        
        Stores protocol version and description, prints handshake and sends 
        a ConnectionResult and a NewTask to backend. 
        
        
        @param data: binary data containing BackendHandshake-protobuf packet
        @return: received BackendHandshake-protobuf packet 
        
        ''' 
        
        handshake = proto.BackendHandshake()
        handshake.ParseFromString(data)
        
        self.protocol_version = handshake.protocol_version
        self.description = handshake.description
        print handshake # nothing clever to do yet, just print
        
        self.Send_ConnectionResult(CONNECTIONRESULT_SUCCESS)
        self.Send_NewTask(1,400,300,10,self.scene)
        return handshake

# these following packets are sent by frontend, no need to know how to receive them

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
        '''
        Turns binary string into RenderedData-protobuf packet.
        
        Reads incoming image size from packet, then receives the image,
        converts it to list of pixels and writes to "test.ppm".
        
        @param data: binary data containing BackendHandshake-protobuf packet
        @return: nothing
        
        ''' 
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
            pixels[-1][0] = int(struct.unpack("!L", data[i:i+4])[0] / 16843009)
            pixels[-1][1] = int(struct.unpack("!L", data[i+4:i+8])[0] / 16843009)
            pixels[-1][2] = int(struct.unpack("!L", data[i+8:i+12])[0] / 16843009)
            
            #pixels.append(struct.unpack("!L"*3, data[i,i+12])) #doesn't work
            i = i + 12
        if len(pixels) != rendered_data.width * rendered_data.height:
            print "Dimension mismatch: got {} pixels, but image size should be {} x {}".format(len(pixels),rendered_data.width, rendered_data.height)
            return
        ListToPPM(pixels, rendered_data.width, rendered_data.height, "test.ppm")
        

        #return rendered_data


    def SendHeader(self, id, length):
        '''
        Sends info about coming packet, first the length of header and then
        the header (Protobuf-packet) itself: packet id and packet length.
        
        @return: nothing
        
        '''
        
        head = proto.PacketHeader()
        head.length = length
        head.id = id
        
        
        
        head_size = head.ByteSize()
        head_size_bin = struct.pack("!L", head_size) 
        
        
        self.SendData(head_size_bin) # send size of header as !L
        
        self.SendData(head.SerializeToString()) # send the header
                
    def SendData(self, data):
        ''' 
        Just sends given data to client.
        
        @param data: binary data to be sent (string)
        
        @return: nothing 
        
        '''
        self.conn.sendall(data)
        
    def SendPacket(self, type_id, protobuf_packet):
        '''
        Sends packet (data) of type (type_id). Handles sending of header and packet itself.
        
        @param type_id: type of packet (enum)
        @param protobuf_packet: packet to be sent as a protobuf-packet
        
        @return: nothing
        
        '''

        self.SendHeader(type_id, protobuf_packet.ByteSize())
        self.SendData(protobuf_packet.SerializeToString())
        
    # No need to send this packet from frontend
    '''
    def Send_BackendHandshake(self, data):
        
        handshake = proto.BackendHandshake()
        handshake.ParseFromString(data)
        
        self.protocol_version = handshake.protocol_version
        self.description = handshake.description
        print handshake # nothing clever to do yet, just print
        return handshake
    '''
         
    def Send_ConnectionResult(self, result):
        '''
        Creates ConnectionResult -protobuf packet and sends it.
        
        @param result: result to be sent (eg. CONNECTIONRESULT_SUCCESS)
        
        @return: nothing
        
        ''' 
        conn_result = proto.ConnectionResult()
        conn_result.result = result
        
        self.SendPacket(CONNECTIONRESULT, conn_result)
        
    def Send_NewTask(self, task_id, width, height, iterations, scene):
        '''
        Creates NewTask -protobuf packet and sends it.
        
        @param task_id: id of task to be sent (int)
        @param width: wanted width of image (int)
        @param height: wanted height of image (int)
        @param iterations: how many iterations we want to be done (int)   
        @param scene: the json scene-file to be rendered (string) 
        
        @return: nothing
        '''
        newtask = proto.NewTask()
        newtask.id = task_id
        newtask.width = width
        newtask.height = height
        newtask.iterations = iterations
        newtask.scene = scene
        
        self.SendPacket(NEWTASK, newtask)
        
        
        #No need to send this packet from frontend
'''    def Send_RenderedData(self, data):
        rendered_data = proto.RenderedData()
        rendered_data.ParseFromString(data)
    
        print rendered_data # nothing clever to do yet, just print
        return rendered_data
        '''
