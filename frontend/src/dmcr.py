
from server import Server


def main():
    server = Server()
    server.SceneToTask("../../backend/assets/scene.json")
    server.Listen()


if __name__ == "__main__":
    main()
