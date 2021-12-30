import paho.mqtt.client as mqtt
import json
import ssl
import time
import serial,time
import ascii_magic
my_art = ascii_magic.from_image_file("logo.jpeg",char="#")
broker_host = "broker.qubitro.com"
broker_port = 8883
device_id = "35db3e12-43b3-448d-906a-5d6904c34284"
device_token = "05PSq9EoWvWui95X7d4YsYqnclJwNz3qLXmwMvJC"
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        ascii_magic.to_terminal(my_art)
        print("Connected to Qubitro!")
        client.on_publish = on_publish
    else:
        print("Failed to connect, visit: https://docs.qubitro.com/client-guides/connect-device/mqtt\n return code:", rc)

def on_publish(client, obj, publish):
    print("Published: " + str(payload))

client = mqtt.Client(client_id=device_id)
client.tls_set_context(context=None)

client.username_pw_set(username=device_id, password=device_token)
client.connect(broker_host, broker_port, 60)
client.on_connect = on_connect
client.loop_start()

# Example payload format, replace keys and values
#payload = line
#{
 #   "Key1": 30,
  #  "Key2": 30.5
#}
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    ser.flush()

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            payload = line
            client.publish(device_id, payload)
            time.sleep(1)
           # print(line)

#3while True:
 #   if client.is_connected:
  #      client.publish(device_id, payload=json.dumps(payload))
   #     time.sleep(5)