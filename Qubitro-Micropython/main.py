from umqtt.robust import MQTTClient

import machine as m

DeviceID = "3726d909-47e5-42ae-b8da-a479ea7780aa"
DeviceToken ="Hb6zA3E9AvJmWM8fSED1QGrRAX3OXDZevzjv5F9c"

client = MQTTClient(DeviceID, "broker.qubitro.com", 1883, user = DeviceID, password = DeviceToken)

def checkwifi():

    while not sta_if.isconnected():

        time.sleep_ms(500)

        print(".")

        sta_if.connect()


def publish():

    checkwifi()

    client.connect()

    while True:

        msg = b'{"location": 20 ,"value": 27}'

        print(msg)

        client.publish(DeviceID, msg)

        time.sleep(2)


publish()