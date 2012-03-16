#!/usr/bin/env python

'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

from server import Server


def main():
    server = Server()
    server.FileToTask("../../backend/assets/scene.json")
    server.Listen()


if __name__ == "__main__":
    main()
