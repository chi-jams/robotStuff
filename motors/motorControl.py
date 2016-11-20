import RPi.GPIO as GPIO

GPIO.setmode( GPIO.BOARD )

GPIO.setup( 12, GPIO.OUT )

exitProgram = False

p = GPIO.PWM( 12, 0.5 )
p.start( 90 )
input( 'Press return to stop: ' )

p.stop()

GPIO.cleanup()
