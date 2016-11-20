import RPi.GPIO as GPIO

GPIO.setmode( GPIO.BOARD )

GPIO.setup( 3, GPIO.OUT )

exitProgram = False

while exitProgram == False:
	keyIn = raw_input( "Exit Program?" )
	if keyIn == 'y':
		exitProgram = True
	elif keyIn == 'on':
		GPIO.output( 3, GPIO.HIGH )
	elif keyIn == 'off':
		GPIO.output( 3, GPIO.LOW )

GPIO.cleanup()
