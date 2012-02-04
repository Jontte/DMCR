Pakettiformaatti 
================

Jokainen paketti alkaa seuraavasti

    uint32_t header_length
    
    message PacketHeader {
      uint32 length
      uint32 id
    }

jossa `header_length` kertoo sitä seuraavan PacketHeader-paketin pituuden
tavuissa. PacketHeader-viestin length-kenttä kertoo PacketHeaderia seuraavan
paketin pituuden tavuissa ja id-kenttä sen tyypin.

Äskeistä dataa seuraa joku seuraavista

BackendHandshake
----------------

    message BackendHandshake {
      uint32 protocol_version (= 0)
      string key (ei pakollinen)
      string description
    }

ConnectionResult
----------------

    message ConnectionResult {
      uint32 result
    }

NewTask
-------

    message NewTask {
      uint32 id (taskin numero)
      uint32 width
      uint32 height
      uint32 iterations
      string scene
    }

RenderedData
------------

    message RenderedData {
      uint32 id (taskin numero)
      uint32 width
      uint32 height
      uint32 iterations_done
      uint32 data_length
    }
    
    float data[data_length]

Huomaa tässä että itse data ei ole protobuf-pakattua vaan raakana oleva rimpsu
floatteja jotka seuraavat protobuf-pakettia. Tämä siksi että protobuf ei sovellu
hyvin isojen datamäärien serialisointiin.
