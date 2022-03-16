import speedtest
import paho.mqtt.client as mqtt
import json

broker_host = "broker.qubitro.com"
broker_port = 8883
device_id = "bae59a13-822d-4e20-8b18-e4cbd534dc00"
device_token = "ShVwmHXpVv9Idvywv6QPbs4UzJzWAq8inveKOzqj"
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
    st = speedtest.Speedtest()
    st.get_best_server()
    payload = {'Download': round(st.download() / 1000000, 2) , 'Upload': round(st.upload() / 1000000, 2), 'Ping': round(st.results.ping, 2)}
    client.publish(device_id, payload=json.dumps(payload))
        