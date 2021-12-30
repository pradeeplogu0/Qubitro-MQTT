import qubitro
import gpio
main:
  led_pin1 := gpio.Pin 12 --output
  led_pin2 := gpio.Pin 2 --output
    
  led_pin1.set 1
  led_pin2.set 0
  client ::= qubitro.connect
    --id="253c34c6-af14-45fd-9866-c6add0117147"
    --token="VpVrlbUfn6jjrTTdddNgMfsjCI6ko8-iki63so9t"
  client.publish { "Data1": random 1000,"Data2": random 1000, "Data3":random 1000}
  print "Yeah im uploading!! "
  led_pin1.set 0
  led_pin2.set 1
  sleep --ms=10000
  led_pin1.close
  led_pin2.close
    
    