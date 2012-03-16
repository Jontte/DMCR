#
# This file is a part of the DMCR project and is subject to the terms and
# conditions defined in file 'LICENSE.txt', which is a part of this source
# code package.
#

require 'protobuf'
require 'socket'

require_relative 'streamreader'
require_relative 'dmcr_protocol.pb'

class DmcrServer
  def initialize
    @s = TCPServer.new('localhost', 9393)
  end
  
  def run
    cli = @s.accept
    
    loop do
      hdr_len = cli.read_uint32_be
      break if not hdr_len
      hdr = Dmcr::Packet::PacketHeader.new
      hdr.parse_from_string(cli.read(hdr_len))
      p hdr
      pkt_data = cli.read(hdr.length)
      p pkt_data
      
      case hdr.id
      when 1
        pkt = Dmcr::Packet::BackendHandshake.new
        pkt.parse_from_string(pkt_data)
        p pkt
        
        reply = Dmcr::Packet::ConnectionResult.new
        reply.result = 1
        
        send_packet(2, cli, reply)
      else
        raise 'unknown msg'
      end
    end
  end
  
  def send_packet(id, cli, packet)
    pkt_data = packet.serialize_to_string
    
    hdr = Dmcr::Packet::PacketHeader.new
    hdr.length = pkt_data.length
    hdr.seq = 1
    hdr.id = id
    
    hdr_data = hdr.serialize_to_string
    
    cli.write_uint32_be hdr_data.length
    cli << hdr_data
    cli << pkt_data
  end
end

DmcrServer.new.run
