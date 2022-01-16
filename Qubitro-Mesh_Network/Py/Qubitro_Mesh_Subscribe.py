import requests
import time
from colorama import Fore, Back, Style

while (True):
    url = "https://api.qubitro.com/v1/projects/6b876774-016c-4a54-aba3-26e702839d43/devices/996c4667-a2d2-4cec-8488-86c20c5214c9/data?keys=temp,humi,pres,Alti&period=1&limit=1"

    headers = {
     "Accept": "application/json",
     "Authorization": "Bearer XXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    }

    response = requests.request("GET", url, headers=headers)

    print(response.text)
    time.sleep(1)

