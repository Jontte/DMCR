#!/usr/bin/env python

'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

from server import Server
import sys

def help():
    print """This is DMCR frontend version {}.
For licensing see LICENSE.txt.

Usage:
    --width= / -w=: set width
    --height= / -h=: set height
    --file= / -f=: set file to render (json SDL)
    --iterations= / -i=: set iterations
    --help / -h: for this help
"""

def main():
    filename = "../../scenes/scene.json"
    width = 800
    height = 600
    iterations = 5
    if len(sys.argv) > 1: #if we have a filename
        for opt in sys.argv[1:]:
            if len(opt.split("=")) > 1:
                cmd, opt = opt.split("=")
                if cmd == "--file" or cmd == '-f':
                    filename = opt
                elif cmd == '--width' or cmd == '-w':
                    width = int(opt)
                elif cmd == '--height' or cmd=='-h':
                    height = int(opt)
                elif cmd == '--iterations' or cmd=='-i':
                    iterations = int(opt)
                else:  
                    print "Unknown option ({} = {}), use -f, -w, -h, -i.".format(cmd, opt)
             else:
                 help()


    print "Starting FE server with:\n{}: {}x{}, {} iterations".format(filename, width, height, iterations)

    server = Server()
    server.FileToTask(filename, width, height, iterations)
    server.Listen()


if __name__ == "__main__":
    main()
