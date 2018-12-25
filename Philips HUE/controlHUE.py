import time
from qhue import Bridge

b = Bridge('169.254.6.242', 'JkAQ4hrJDOQC2txQrzfHDfrbEdvsaeyM6BG6XjjC')

green = 30000
yellow = 12750
red = 350

lights = b.lights

# initial state
b.lights[1](1, 'state', bri=128, hue=green) # car
b.lights[2](1, 'state', bri=128, hue=red) # human


def ampel_control():
	# Lampe1 geht von Grün auf Orange (Autos bereiten sich auf Rot Phase vor)
	print("Autos bereiten sich auf Rot Phase vor")
	b.lights[1](bri=128, hue=yellow)
	time.sleep(2)
	
	# Lampe1 geht von Orange auf Rot (Autos müssen halten)
	print("Autos müssen halten")
	b.lights[1](bri=128, hue=red)
	time.sleep(2)
	
	# Lampe2 geht von Rot auf Grün (Menschen dürfen passieren)
	print("Menschen dürfen passieren")
	b.lights[2](bri=128, hue=green)
	time.sleep(5)
	
	# Lampe2 geht von Grün auf Rot (Menschen müssen wieder halten)
	print("Menschen müssen wieder halten")
	b.lights[2](bri=128, hue=red)
	time.sleep(2)
	
	# Lampe1 geht von Rot auf Orange (Autos bereiten sich auf Anfahrt vor)
	print("Autos bereiten sich auf Anfahrt vor")
	b.lights[1](bri=128, hue=yellow)
	time.sleep(2)
	
	# Lampe1 geht von Orange auf Grün (Autos dürfen weiter fahren)
	print("Autos dürfen weiter fahren")
	b.lights[1](bri=128, hue=green)

	
while(True):
    print("Moechten Sie die Strasse ueberqueren? (j|n)")
    i = input()
    if i == 'j':
        ampel_control()
    elif i == 'n':
        break
		