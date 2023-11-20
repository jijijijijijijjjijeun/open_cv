from getchar import Getchar
import cv2
import serial    
import timeit  
import mediapipe as mp 
import numpy as np
import time     
import os
   
#clear = lambda : os.system('cls')
   
#sp  = serial.Serial('com7', 9600, timeout=1)
sp  = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
#가중치 파일 경로
cascade_filename = 'haarcascade_frontalface_alt.xml'
#모델 불러오기
cascade = cv2.CascadeClassifier(cascade_filename)

#cam = cv2.VideoCapture(0)
cam = cv2.VideoCapture(2)

#인식할 수 있는 최대 손 개수
max_num_hands = 1

#손 제스쳐 매핑 딕셔너리
gesture = {
    0:'A',1:'B',2:'C',3:'D',4:'E',5:'F',6:'G',7:'H',
    8:'I',9:'J',10:'K',11:'L',12:'M',13:'N',14:'O', 
    15:'P',16:'Q',17:'R',18:'S',19:'T',20:'U',21:'V', 
    22:'W',23:'X',24:'Y',25:'Z',26:'spacing',27:'clear'
    }
    
#MediaPipe 손 모듈을 초기화
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

#손 인식을 위한 데이터를 로드
hands = mp_hands.Hands(
    max_num_hands = max_num_hands,
    min_detection_confidence = 0.5,
    min_tracking_confidence = 0.5)

file = np.genfromtxt('data.txt', delimiter=',') 
angleFile = file[:,:-1]
labelFile = file[:,-1]
angle = angleFile.astype(np.float32)
label = labelFile.astype(np.float32)

knn = cv2.ml.KNearest_create()
knn.train(angle, cv2.ml.ROW_SAMPLE, label)
 
#pan, tilt 초기값 설정
pan = 90
tilt = 80
_pan = 90
_tilt = 80

#마진 설정
margin_x = 50
margin_y = 50

def send_pan(pan):  
    print("                       ",end ="")    #cmd 화면
    tx_dat = "pan" + str(pan) + "\n"            #아두이노로 전송기 위한 값  
    sp.write(tx_dat.encode())
    print(tx_dat)

def send_tilt(tilt):
    print("                       ",end ="")    #cmd 화면
    tx_dat = "tilt" + str(tilt) + "\n"          #아두이노로 전송기 위한 값 
    sp.write(tx_dat.encode())
    print(tx_dat)

if not cam.isOpened():
    print("Could not open webcam")
    exit()
    
#영상 검출기
def videoDetector(cam,cascade):
       
    global pan; 
    global _pan; 
    global tilt; 
    global _tilt;
    
    send_pan(90)
    send_tilt(80)
    kb = Getchar()
    key = ''                    #문자열 누적
    startTime = time.time()     #현재 시간 저장
    prev_index = 0              #이전 제스처의 인덱스(이전과 다르면 새로운 제스처로 인식)
    recognizeDelay = 1          #1초 동안 같은 제스처 인식하지 않음
        
    while cam.isOpened():                                                   
       
        #캡처 이미지 불러오기
        ret,img1 = cam.read()
        #좌우 반전
        img = cv2.flip(img1, 1)
        #영상 압축
        img = cv2.resize(img,dsize=None,fx=1.0,fy=1.0)
        #그레이 스케일 변환
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) 
        #cascade 얼굴 탐지 알고리즘 
        results = cascade.detectMultiScale(gray,            #입력 이미지
                                           scaleFactor= 1.1,#이미지 피라미드 스케일 factor
                                           minNeighbors=5,  #인접 객체 최소 거리 픽셀
                                           minSize=(20,20)  #탐지 객체 최소 크기
                                           )
                                                                        
        for box in results:
            x, y, w, h = box
            center_x = x + w//2
            center_y = y + h//2
            #clear()
            print("center = (%s, %s)" %(center_x, center_y))
            if center_x < 320 - margin_x:
                print("                                  ",end ="")
                print("pan left")
                if pan - 1 >= 0:
                    pan = pan - 1
                    _pan = pan
                else:
                    pan = 0
                    _pan = pan
            elif center_x > 320 + margin_x:
                print("                                  ",end ="")
                print("pan right")
                if pan + 1 <= 180:
                    pan = pan + 1
                    _pan = pan
                else:
                    pan = 180
                    _pan = pan
            else:
                print("                                  ",end ="")
                print("pan stop")
                pan = _pan
            
            send_pan(pan) 
                
            if center_y < 240 - margin_y:
                print("                                  ",end ="")
                print("tilt down")
                if tilt - 1 >= 0:
                    tilt = tilt - 1
                    _tilt = tilt
                else:
                    tilt = 0
                    _tilt = tilt
            elif center_y > 240 + margin_y:
                print("                                  ",end ="")
                print("tilt up")
                if tilt + 1 <= 180:
                    tilt = tilt + 1
                    _tilt = tilt
                else:
                    tilt = 180
                    _tilt = tilt
            else:   
                print("                                  ",end ="")
                print("tilt stop")
                tilt = _tilt
                    
            send_tilt(tilt)
            
            cv2.rectangle(img, (x,y), (x+w, y+h), (255,255,255), thickness=2)
        
        if not ret:
            continue
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) 
        #MediaPipe Hands 모듈을 사용하여 손을 검출, 추적
        result = hands.process(imgRGB)
        
        #if 손이 검출되면
        if result.multi_hand_landmarks is not None: 
            for res in result.multi_hand_landmarks: 
                joint = np.zeros((21,3))
                for j,lm in enumerate(res. landmark):   #각 손의 랜드마크 좌표를 가져와 joint 배열에 저장
                    joint[j] = [lm.x, lm.y, lm.z]
                
                #손 랜드마크를 이용하여 손가락 관절 각도를 계산               
                v1 = joint[[0, 1, 2, 3, 0, 5, 6, 7, 0, 9,10,11, 0,13,14,15, 0,17,18,19], :] 
                v2 = joint[[1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20], :]
                
                 #각 관절 각도 벡터를 계산하고 단위 벡터로 정규화
                v = v2 - v1
                v = v / np. linalg.norm(v,axis=1)[:, np.newaxis]
                
                #벡터 간의 각도를 계산하고 각도를 도 단위로 변환
                compareV1 = v[[0, 1, 2, 4, 5, 6, 7, 8, 9,10,12,13,14,16,17], :] 
                compareV2 = v[[1, 2, 3, 5, 6, 7, 9,10,11,13,14,15,17,18,19], :] 
                angle = np.arccos (np.einsum('nt,nt->n', compareV1, compareV2))
                
                #각도를 도 단위로 변환합니다
                angle = np. degrees (angle)
               
                #KNN 분류기를 사용하여 손 제스처를 인식합니다.
                data= np.array([angle], dtype=np.float32)
                ret, results, neighbours, dist = knn.findNearest(data,3)
                index= int(results[0][0])
                
                if index in gesture.keys():                 #인식된 손 제스처를 처리
                    if index != prev_index:
                        startTime = time.time()
                        prev_index = index
                    else:
                        if time.time() - startTime > recognizeDelay:
                            if index == 26:
                                key += ' '
                            elif index == 27:
                                key = ''
                            else:
                                key += gesture[index]
                            startTime = time.time()
                    
                    #화면에 인식된 손 제스처를 표시
                    cv2.putText(img, gesture[index].upper(), (int(res.landmark[0].x * img.shape[1] - 10),
                                int(res.landmark[0].y * img.shape[0] + 40)), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 204, 204), 3)
                #손 관절을 이미지에 그림
                mp_drawing.draw_landmarks (img, res, mp_hands. HAND_CONNECTIONS)
         # 화면에 현재 key를 표시합니다.        
        cv2.putText(img, key, (20,440), cv2.FONT_HERSHEY_SIMPLEX, 2, (200,20,200), 3)
        

         # 영상 출력        
        cv2.imshow('VideoFrame',img)
        
        k = cv2.waitKey(5) & 0xFF
            
        if k == 27:
            break
       
            
    camture.release()
    cv2.destroyAllWindows()                                    
# 영상 탐지기
videoDetector(cam,cascade)                                                                                                                                                                                           
