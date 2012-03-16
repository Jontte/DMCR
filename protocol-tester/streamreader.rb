#
# This file is a part of the DMCR project and is subject to the terms and
# conditions defined in file 'LICENSE.txt', which is a part of this source
# code package.
#

module StreamReader
  def __read_count(bytes_per_obj, count, type)
    data = read(count*bytes_per_obj)
    return nil if not data
    objs = data.unpack("#{type}*")
    if (count == 1)
      objs.first
    else
      objs
    end
  end
  
  def read_uint8(n=1)
    __read_count(1, n, 'C')
  end
  
  def read_uint16(n=1)
    __read_count(2, n, 'S')
  end
  
  def read_uint16_le(n=1)
    __read_count(2, n, 'v')
  end
  
  def read_int16(n=1)
    __read_count(2, n, 's')
  end
  
  def read_uint16_be(n=1)
    __read_count(2, n, 'n')
  end
  
  def read_uint32(n=1)
    __read_count(4, n, 'L')
  end
  
  def read_uint32_le(n=1)
    __read_count(4, n, 'V')
  end
  
  def read_uint32_be(n=1)
    __read_count(4, n, 'N')
  end
  
  def read_uint64_be(n=1)
    read(n*8).reverse.unpack('Q*').first
  end
  
  def read_float_be(n=1)
    __read_count(4, n, 'g')
  end
  
  def read_double_be(n=1)
    __read_count(8, n, 'G')
  end
end

module StreamWriter
  def cache
    @__sw_out = ''
    yield
    self << @__sw_out
    @__sw_out = nil
  end
  
  def __write(objs, type)
    data = objs.pack(type*objs.length)
    (@__sw_out || self) << data
  end
  
  def write_uint8(*a)
    __write(a, 'C')
  end
  
  def write_uint16(*a)
    __write(a, 'S')
  end
  
  def write_int16(*a)
    __write(a, 's')
  end
  
  def write_uint16_le(*a)
    __write(a, 'v')
  end
  
  def write_uint16_be(*a)
    __write(a, 'n')
  end
  
  def write_uint32(*a)
    __write(a, 'L')
  end
  
  def write_uint32_le(*a)
    __write(a, 'V')
  end
  
  def write_uint32_be(*a)
    __write(a, 'N')
  end
  
  def write_float_be(*a)
    __write(a, 'g')
  end
  
  def write_double_be(*a)
    __write(a, 'G')
  end
  
  def write_data(data)
    (@__sw_out || self) << data
  end
end

class IO
  include StreamReader
  include StreamWriter
end

require 'stringio'

class StringIO
  include StreamReader
  include StreamWriter
end

