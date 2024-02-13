from urllib import response //internal library for JSON data type
import pyrebase	 //library for firebase
import serial 	 //library for serial communication between Arduino
import time    //time functions
from sim800l import SIM800L	//library function Sim800l
sim800l = SIM800L('/dev/serial0')	//serial port (GPIO pin) for simm800l

//firebase Realtime database configuration
config = {
    "apiKey": "AIzaSyAAawGHw2l1Q6krVGTlAcxUVBaszZyNRlc",
    "authDomain": "health-monitoring-by-salini.firebaseapp.com",
    "databaseURL":"https://health-monitoring-by-salini-default-rtdb.asia southeast1.firebasedatabase.app",
    "projectId": "health-monitoring-by-salini",
    "storageBucket": "health-monitoring-by-salini.appspot.com",
    "messagingSenderId": "1097375343656",
    "appId": "1:1097375343656:web:8a23d9a4f639f4c65f811d"
}

firebase = pyrebase.initialize_app(config)	//creating firebase object using config file
db = firebase.database()	//creating database object using firebase.database function
sensorData = db.child("sensor_data").get()	//accessing sensorDatabse from the firebase database
serialport = serial.Serial("/dev/ttyS0", 9600, timeout=0.5)  //serial port connection for Arduino 

response = serialport.readlines().  //reading sensors data from Arduino through serial port

//calling the loop function
def loop():
    
    pulse = response[0]        //array value 0
    temp = response[1]	     //array value 1

    //updating the values in the db
    db.child("sensor_data").update({"pulse": pulse})
    db.child("sensor_data").update({"temp": temp})

    pulse = db.child("sensor_data").child("pulse").get().val()
    temp = db.child("sensor_data").child("temp").get().val()
    
    //save the alert messages in the db	
    def saveAlert(sms):
        data = {"Alerts": sms}
        db.child("Alerts").push(data)

    sms = ""   //sms variable
    if(temp > 37.8):
        sms = "Patient is having fever of : " + str(temp)
        sim800l.send_sms('+601164271745', sms)  //send the message to the doctor
        saveAlert(sms)	//save the database to the firebase db

    if(pulse > 100):
        sms = "Patient has high pulse rate : " + str(pulse)
        sim800l.send_sms('+601164271745', sms)	
        saveAlert(sms) 		

    if(pulse < 60):
        sms = "Patient has low pulse rate : " + str(pulse)
        sim800l.send_sms('+601164271745', sms)
        saveAlert(sms)


while 1:
    loop()  
    time.sleep(10)  
