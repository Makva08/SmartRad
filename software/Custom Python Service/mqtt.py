#!/usr/bin/python3

import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient,Point
import json

bucket_name = "geiger"

client = InfluxDBClient(url="http://localhost:8086", token="ilzCKysbDrBWCen65Tfbp5xEwlPfLnf9u4ZmdhgDczUDHyeFnjbAB1nUlQaDks3dUUWfa-KgRGa250VjWyjCuw==", org="home")

write_api = client.write_api()

def on_message(client,data,msg):
    try:
        info = json.loads(msg.payload.decode())
        write_api.write(
            bucket = bucket_name,
            record = Point("my-geiger").tag("location",info["location"]).field("measurement",info["measurement"])
        )
        print(info)
    except Exception as ex:
        print(ex,str(msg.payload))

def on_connect(client,userdata,flags,rc):
    client.subscribe("geiger")

client = mqtt.Client()
client.on_message = on_message
client.on_connect =on_connect
client.username_pw_set(username="geiger",password="geiger")
client.connect('localhost',1883)
client.loop_forever()
