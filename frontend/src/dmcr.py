#!/usr/bin/env python

from server import Server


def main():
    server = Server()
    server.FileToTask("../../backend/assets/scene.json")
    server.Listen()


if __name__ == "__main__":
    main()
