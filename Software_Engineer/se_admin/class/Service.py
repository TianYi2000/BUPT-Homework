from datetime import datetime
from Config import Config


class Service:

    # 初始化对象，传入室温 indoor_temp
    def __init__(self, indoor_temp):
        self.start_time = datetime.now().date()
        self.current_time = datetime.now().date()
        self.end_time = datetime.now().date()
        self.service_time = 0       # 服务时长
        self.wait_time = 0          # 等待时长
        self.indoor_temp = indoor_temp
        self.mode = Config.mode
        self.temperature = Config.default_temp
        self.fan_speed = Config.default_speed

    # 判断是否达到目标温度，若达到返回TRUE
    def is_finished(self):
        if self.indoor_temp > self.temperature and self.mode == 2:
                return True
        elif self.indoor_temp < self.temperature and self.mode == 1:
                return True
        return False

    # 更新服务时长
    def set_service_time(self, service_time):
        self.service_time = service_time

    # 增加服务时长
    def add_service_time(self, time_interval):
        self.service_time += time_interval

    # 更新等待时长
    def set_wait_time(self, wait_time):
        self.wait_time = wait_time

    # 减少等待时长
    def reduce_wait_time(self, time_interval):
        self.wait_time -= time_interval
       
    # 更新室内温度
    def set_indoor_temp(self, indoor_temp):
        self.indoor_temp = indoor_temp

    # 设置模式(暂定为制冷、制热)用1、2表示
    def set_mode(self, mode):
        if 1 <= mode <= 2:
            self.mode = mode
            return True
        else:
            return False

    # 设置温度
    def set_temperature(self, temp):
        if Config.min_temp <= temp <= Config.max_temp:
            self.temperature = temp
            return True
        else:
            return False

    # 设置风速
    def set_fan_speed(self, speed):
        if Config.min_speed <= speed <= Config.max_speed:
            self.fan_speed = speed
            return True
        else:
            return False

    # 返回开始时间、当前时间、温度、风速
    def get_info(self):
        self.current_time = datetime.now().date()
        info = [self.start_time, self.current_time, self.temperature, self.fan_speed]
        return info

    # 销毁对象，返回开始时间、结束时间
    def __del__(self):
        self.end_time = datetime.now().date()
