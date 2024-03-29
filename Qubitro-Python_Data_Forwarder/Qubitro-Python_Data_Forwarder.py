import paho.mqtt.client as mqtt
import json,time

broker_host = "broker.qubitro.com"
broker_port = 8883
device_id = "78a1a09e-f0c0-437d-b3f7-a2401acb41aa"
device_token = "DasuQGCu0Ff85-bxl$MSlC0BsqZsQG$9aZtRdq$D"
def on_connect(client, userdata, flags, rc):
    if rc == 0:
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

while True:
    payload = {'Apple': 10 , 'Banana': 20, 'Cow': 30}
    client.publish(device_id, payload=json.dumps(payload))
    time.sleep(5)
        