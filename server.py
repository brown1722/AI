from firebase import firebase
firebase = firebase.FirebaseApplication('https://database-d07ce-default-rtdb.firebaseio.com/', None)


import cv2
import time
import os
from flask import Flask, request
import numpy as np
from multiprocessing import Value


app = Flask(__name__)


@app.route('/')
@app.route('/index', methods=['GET'])
def index():
	return "ESP32-CAM Flask Server", 200


@app.route('/upload', methods=['POST','GET'])
def upload():
	received = request
	img = None	
	if received.files:
		print(received.files['imageFile'])
		# convert string of image data to uint8
		file  = received.files['imageFile']
		nparr = np.fromstring(file.read(), np.uint8)
		# decode image
		img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
		cv2.imwrite('C:/Users/trong/PBL5/Result/input.jpg', img)		
		
		with open('C:/Users/trong/PBL5/Result/input.txt', 'w') as ff:
			ff.write('')
		os.system('python C:/Users/trong/PBL5/yolov5-master/detect.py --weights C:/Users/trong/PBL5/yolov5-master/runs/train/exp2/weights/best.pt --source C:/Users/trong/PBL5/Result/input.jpg --save-txt --project C:/Users/trong/PBL5/Result ')
		with open('C:/Users/trong/PBL5/Result/input.txt', 'r') as f:
			lines =  f.readlines()
			arr = [] 
			for line in lines:
				words = line.split()
				arr.append(words[0])
			if '1' in arr and '0' in arr:
				firebase.put('/', 'Fan', 1)
				firebase.put('/', 'DEN1', 1)
				firebase.put('/', 'DEN2', 1)
				firebase.put('/', 'DEN3', 1)
				firebase.put('/', 'DEN4', 1)
			if '0' in arr:
				firebase.put('/','Fan',1)	
				firebase.put('/','DEN1',1)
				firebase.put('/','DEN2',1)
				firebase.put('/','DEN3',1)
				firebase.put('/','DEN4',1)
			elif '1' in arr:
				firebase.put('/','Fan',0)
				firebase.put('/','DEN1',0)
				firebase.put('/','DEN2',0)
				firebase.put('/','DEN3',0)
				firebase.put('/','DEN	4',0)
			else: 
				firebase.put('/', 'Fan', 0)
				firebase.put('/', 'DEN1', 0)
				firebase.put('/', 'DEN2', 0)
				firebase.put('/', 'DEN3', 0)
				firebase.put('/', 'DEN4', 0)
		time.sleep(3)
		return "[SUCCESS] Image Received", 201
	else:
		return "[FAILED] Image Not Received", 204
	
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

