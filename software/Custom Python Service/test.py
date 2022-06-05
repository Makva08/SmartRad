from influxdb_client import InfluxDBClient,Point
import time
import random

bucket_name = "geiger"

client = InfluxDBClient(url="http://localhost:8086", token="ilzCKysbDrBWCen65Tfbp5xEwlPfLnf9u4ZmdhgDczUDHyeFnjbAB1nUlQaDks3dUUWfa-KgRGa250VjWyjCuw==", org="home")

write_api = client.write_api()

while(True):
    time.sleep(1)
    num = random.randint(0,100)

    write_api.write(
        bucket = bucket_name,
        record = Point("my-geiger").tag("location","12.3;23.5").field("measurement",num)
    )
    print(num)
