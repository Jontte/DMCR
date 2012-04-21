#!/usr/bin/env python

'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

from server import Server
import sys
import argparse

def main():
	parser = argparse.ArgumentParser(
		description='This is the DMCR Frontend. For licensing see LICENSE.txt.',
		add_help=False)
		
	parser.add_argument('--file', '-f', 		default='../../scenes/scene.json')
	parser.add_argument('--width', '-w', 		default=800)
	parser.add_argument('--height', '-h', 		default=600)
	parser.add_argument('--iterations', '-i', 	default=5)
	parser.print_help()
	params = vars(parser.parse_args())
	
	print "Starting FE server with:\n{file}: {width}x{height}, {iterations} iterations".format(**params)

	server = Server()
	server.FileToTask(params['file'], params['width'], params['height'], params['iterations'])
	server.Listen()


if __name__ == "__main__":
    main()
