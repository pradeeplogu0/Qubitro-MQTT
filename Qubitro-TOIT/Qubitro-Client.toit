import qubitro
import gpio
main:
  led_pin1 := gpio.Pin 12 --output
  led_pin2 := gpio.Pin 2 --output
    
  led_pin1.set 1
  led_pin2.set 0
  client ::= qubitro.connect
    --id="996c4667-a2d2-4cec-8488-86c20c5214c9"
    --token="3$QQpSGvsxFLJ7a06TMY4$zgu97pIEkDvDoOSmPf"
  client.publish { "Data1": random 1000,"Data2": random 1000, "Data3":random 1000}
  print "Yeah im uploading!! "
  led_pin1.set 0
  led_pin2.set 1
  sleep --ms=10000
  led_pin1.close
  led_pin2.close
    
    