# Ghazal Sadeghian-MQTT CLient

import paho.mqtt.subscribe as subscribe
import time
import paho.mqtt.client as paho

broker = "192.168.1.5"
# define callback
sent = ""
recvFlag = False


def on_subscribe(client, userdata, mid, granted_qos):  # create function for callback
    time.sleep(1)
    logging.info("sub acknowledge message id=" + str(mid))
    pass


def on_disconnect(client, userdata, rc=0):
    logging.info("DisConnected result code " + str(rc))


def on_connect(client, userdata, flags, rc):
    logging.info("Connected flags" + str(flags) + "result code " + str(rc))


def on_message(client, userdata, message):
    global sent
    global recvFlag
    msg = str(message.payload.decode("utf-8"))
    print("Random Number Received:  " + msg)
    if (((int(msg)) % 2) == 0):
        print("even")
        sent = "ON"
    else:
        print("odd")
        sent = "OFF"
    recvFlag = True


def on_publish(client, userdata, mid):
    logging.info("message published " + str(mid))


client = paho.Client("PahoClient")
print("connecting to broker ", broker)

client.on_subscribe = on_subscribe  # assign function to callback
client.on_disconnect = on_disconnect  # assign function to callback
client.on_connect = on_connect  # assign function to callback
client.on_message = on_message

client.connect(broker)  # connect
client.loop_start()  # start loop to process received messages
print("subscribing ")
client.subscribe("numGenerator")
# client.subscribe("house/bulb1")#subscribe
time.sleep(2)

while True:
    if (recvFlag == True):
        print("publishing ")
        print(sent)
        client.publish("led", sent)  # publish
        recvFlag = False
client.loop_stop()
