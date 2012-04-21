#!/usr/bin/env python

'''
 This file is a part of the DMCR project and is subject to the terms and
 conditions defined in file 'LICENSE.txt', which is a part of this source
 code package.
'''

from server import Server
import sys
import argparse
import readline # for up-arrow and such

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
	server.FileToTask(params['file'], int(params['width']), int(params['height']), int(params['iterations']))

	try:
		server.start()
		while True:
			choice = raw_input("> ")
			if choice == 'l':
				for task in server.taskmanager.ListTasks(): 
					print str(task) 
			elif choice == 's':
				task_id = raw_input("Task ID: ")
				server.taskmanager.StopTask(int(task_id))
			elif choice == 'a':
				filename = raw_input("Filename: ")
				width = 800
				height = 600
				iterations = 5
				
				width_i = raw_input("Width (800): ")
				if width_i != '':
					width = int(width_i)
				height_i = raw_input("Height (600): ")
				if height_i != '':
					height = int(height_i)
				
				iterations_i = raw_input("Iterations (5): ")
				if iterations_i != '':
					iterations = int(iterations_i)
				
				server.FileToTask(filename, width, height, iterations)
				
			elif choice == 'q':
				break
				
			else:
				print "l: list tasks\na: add task\ns: stop task\nq: quit"
	except KeyboardInterrupt as e:
		print "DMCR.main(): Excepted, quitting", e
	finally:
		print "Stopping server"
		server.stop()
		server.taskmanager.stop() # wait for tasks to finish


if __name__ == "__main__":
    main()
