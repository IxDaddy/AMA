import requests
import sys

def send(argv):
	url = "https://api.telegram.org/bot1945580972:AAHD7xB-aN6Y-Jz0vSNqoZjtQwM9kevrBwM/sendMessage?chat_id=@AMA_cryptobot&text="
	url+= argv
	requests.post(url)

def main(argv):
    send(argv)

if __name__ == "__main__":
    main(sys.argv[1])