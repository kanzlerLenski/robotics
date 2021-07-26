import vrep                  
import sys
import time   
import numpy as np

# на всякий случай закрываем установленные подключения, если таковые есть.

vrep.simxFinish(-1) 

# устанавливаем своё.

clientID = vrep.simxStart('127.0.0.1',19999,True,True,5000,5)

# проверяем подключение.

if clientID != -1:  
    print('Connected to remote API server.')
    
else:
    print('Connection not successful.')
    sys.exit('Could not connect.')

# вектор H для МНК.

H = np.ones((2,1))

# список звеньев, выступающих в качестве объектов управления.
# поскольку управление роботом будет осуществляться посредством 
# поворота колёс, которых 4, влево или вправо в зависимости от местонахождения
# препятствия, разделим их на левые и правые. 

left_joint_h = []
right_joint_h = []

for i in range(1, 5):
    
    # левые колёса.
    
    if i in (2,3):
        errorCode, left_joint_handle = vrep.simxGetObjectHandle(clientID,
                                    'Revolute_joint{}'.format(str(i)), 
                                        vrep.simx_opmode_oneshot_wait)
        left_joint_h.append(left_joint_handle)
    
    # правые.
    
    else:
        errorCode, right_joint_handle = vrep.simxGetObjectHandle(clientID,
                                        'Revolute_joint{}'.format(str(i)), 
                                        vrep.simx_opmode_oneshot_wait)
        right_joint_h.append(right_joint_handle)

# список для сенсоров (2 ультразвуковых в виде конусов).

sensor_h = []

# массив для измерений.

sensor_val = np.array([])

for i in range(1,3):
    
        errorCode, sensor_handle = vrep.simxGetObjectHandle(clientID,
                                'Proximity_sensor{}'.format(str(i)), 
                                vrep.simx_opmode_oneshot_wait)      
        
        sensor_h.append(sensor_handle) 
        
        errorCode, detectionState, detectedPoint, detectedObjectHandle, \
        detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID,
                                    sensor_handle, vrep.simx_opmode_streaming)  
        
        sensor_val=np.append(sensor_val,np.linalg.norm(detectedPoint)) 
        
t = time.time()

# запускаем симуляцию на 2 минуты.

while (time.time() - t) < 120:
    
    sensor_val = np.array([])
    
    # считываем данные с сенсоров.
    
    for i in range(1,3):
    
        errorCode, detectionState, detectedPoint, detectedObjectHandle, \
        detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID,
                                    sensor_h[i-1], vrep.simx_opmode_buffer)   
        
        # нормализуем полученные данные и добавляем в список измерений. 
        
        sensor_val = np.append(sensor_val, np.linalg.norm(detectedPoint))
        print(sensor_val)
    
    # прлучаем индекс сенсора с минимальным показанием, т.е. находтим сенсор, 
    # который ближе всего находится к препятствию.
    
    min_idx = np.where(sensor_val == np.min(sensor_val))[0][0]
    
    # посредством метода МНК находим истинное местоположение препятствия.
    
    sensor_true_val = float(np.linalg.inv(H.T.dot(H)).dot(H.T).dot(sensor_val))
    print(sensor_true_val)
    
    # если робот находится достаточно близко к препятствию,
    # преобразовываем истинное значение, чтобы оно не было большим.
    
    if sensor_true_val > 0.2:
        steer = -1/sensor_true_val
    else:
        steer = 0
    
    # постоянная скорость.
    
    v = 1.0
    
    # коэффициент для ещё большего уменьшения полученного значения,
    # чтобы робот не набирал скорость слишком быстро. 
    
    kp = 0.2
    
    # если препятствие находится слева, увеличиваем скорость на правых колёсах.
    
    if min_idx == 0:
        
        vl = v - kp*steer
        vr = v + kp*steer

    # иначе – поворачиваем влево.
    
    else:
        
        vl = v + kp*steer
        vr = v - kp*steer
        
    print("v_left =", vl)
    print("v_right =", vr)

    # передаём полученные значение скорости колёсам. 
    
    errorCode = vrep.simxSetJointTargetVelocity(clientID, left_joint_h[0],
                                                vl, vrep.simx_opmode_streaming)
    
    errorCode = vrep.simxSetJointTargetVelocity(clientID, left_joint_h[1],
                                                vl, vrep.simx_opmode_streaming)
    
    errorCode = vrep.simxSetJointTargetVelocity(clientID, right_joint_h[0],
                                                vr, vrep.simx_opmode_streaming)
    
    errorCode = vrep.simxSetJointTargetVelocity(clientID, right_joint_h[1],
                                                vr, vrep.simx_opmode_streaming)
    
    # повторяем вышеописанную процедуру каждые 2 секунды.
    
    time.sleep(0.2) 

# по истечении времени симуляции останавливаем робота, передав нулевые скорости
# на колёса.

errorCode = vrep.simxSetJointTargetVelocity(clientID, left_joint_h[0], 0,
                                            vrep.simx_opmode_streaming)
errorCode = vrep.simxSetJointTargetVelocity(clientID, left_joint_h[1], 0,
                                            vrep.simx_opmode_streaming)
errorCode = vrep.simxSetJointTargetVelocity(clientID, right_joint_h[0], 0,
                                            vrep.simx_opmode_streaming)
errorCode = vrep.simxSetJointTargetVelocity(clientID, right_joint_h[1], 0,
                                            vrep.simx_opmode_streaming)
