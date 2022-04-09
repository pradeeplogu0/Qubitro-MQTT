import psutil
import paho.mqtt.client as mqtt
import time
import json
broker_host = "broker.qubitro.com"
broker_port = 8883
device_id = "f11ff84d-34c7-4be9-8662-1ce6e3a20638"
device_token = "eR4gP7H-GwnYVgAJNBcmE0W7wsRbJHNscPA5sCEa"
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
    payload = {'CPU usage': psutil.cpu_percent(interval=0.5) , 'CPU frequency': int(psutil.cpu_freq().current), 
    'RAM Percentage': psutil.virtual_memory().percent,'Swap Percentage': psutil.swap_memory().percent,
    'Memory Percentage':psutil.disk_usage('/').percent}

    client.publish(device_id, payload=json.dumps(payload))
    time.sleep(10)


