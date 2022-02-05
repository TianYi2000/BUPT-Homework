from Service import Service
from ServiceQueue import ServiceQueue
from WaitQueue import WaitQueue
from Config import Config
import time
import xlwt     # 用于输出xls文件

import sys

sys.path.append("..\code\server")
from sqldb import *

class Dispatcher:

    '''
    初始化
    service_num:服务对象数量上限

    处于等待队列返回1 服务队列返回2
    '''

    request_num = 0
    def __init__(self):
        # room_id 和 service_id 的映射
        self.lists = []
        self.sq = ServiceQueue()
        self.wq = WaitQueue()
        self.max_service_id=0 #当前最大的service_id
        self.max_service=3 #允许同时服务最大数量
        self.wait_time=120 #默认等待服务时长 120s
        self.max_wait_time = 999999999999  # 最大等待服务时长，若等待时长为此值，等待时长不减少，服务队列空的时候才加入服务队列
        self.unit=60 #时间片长度60s
        
        self.day_in = {}                # 入住时间
        self.day_out = {}               # 退房时间
        self.TimesOfOnoff = {}          # 开关次数
        self.TimesOfChangetemp = {}     # 调温次数
        self.TimesOfChangespeed = {}    # 调风速次数
        self.TimesOfDispatch = {}       # 调度次数


    # 创建一个服务并添加队列信息 返回1：busy 返回2：ok 返回3：error
    def create_service(self, room_id,indoor_temp):
        
        curtime = time.strftime('%Y-%m-%d')
        self.day_in.setdefault(room_id, curtime)
        self.TimesOfOnoff.setdefault(room_id, 0)
        self.TimesOfChangetemp.setdefault(room_id, 0)
        self.TimesOfChangespeed.setdefault(room_id, 0)
        self.TimesOfDispatch.setdefault(room_id, 0)
        
        
        service = Service(indoor_temp)
        # service_id记录每一次服务
        service_id = self.max_service_id+1
        self.max_service_id += 1
        self.lists.append([int(room_id), service_id])

        flag=-1
        # 执行策略 ......
        if(self.sq.get_service_num()<self.max_service):
            self.sq.append_service(service_id, service)
            flag=2
        else:
            flag=1
            lowest_id,lowest_speed=self.sq.get_lowest_speed_service()
            #print(lowest_id)
            if service.fan_speed>lowest_speed :
                if(len(lowest_id)==1):
                    move_id,move_service=self.sq.move_service(lowest_id[0])
                    #分配等待服务时间
                    move_service.set_wait_time(self.wait_time)

                    #移入等待队列
                    self.wq.append_service(move_id,move_service)
                    #移入服务队列
                    self.sq.append_service(service_id, service)
                else:
                    #print(lowest_id)
                    longest_id=self.sq.get_longest_service_in_list(lowest_id)
                    #print(longest_id)
                    move_id, move_service = self.sq.move_service(longest_id[0])
                    # 分配等待服务时间
                    move_service.set_wait_time(self.wait_time)

                    # 移入等待队列
                    self.wq.append_service(move_id, move_service)
                    # 移入服务队列
                    self.sq.append_service(service_id, service)
            elif service.fan_speed==lowest_speed:
                # 分配等待服务时间
                service.set_wait_time(self.wait_time)
                # 移入等待队列
                self.wq.append_service(service_id, service)

            elif service.fan_speed<lowest_speed:
                # 分配等待服务时间
                service.set_wait_time(self.max_wait_time)
                # 移入等待队列
                self.wq.append_service(service_id, service)



        return flag


    '''
    每次更新完温度，调用一次。
    '''

    def dispatch(self):

        finish_id=[] #记录达到目标温度的room_id

        #遍历服务队列。若服务完成，则移出队列
        for service_map in self.sq.service_queue:
            service_map[1].add_service_time(self.unit)
            is_finished=service_map[1].is_finished()
            if is_finished:
                move_id, move_service = self.sq.move_service(service_map[0])
                finish_id.append(self.service_id2room_id(move_id))
                self.TimesOfDispatch[self.service_id2room_id(move_id)] += 1

        #遍历等待队列。若等待时间降为0，移入服务队列。
        for service_map in self.wq.wait_queue:
            service_map[1].reduce_wait_time(self.unit)

            if service_map[1].wait_time<=0:
                longest=self.sq.get_longest_service()

                #从服务队列移除服务时间最长的服务
                move_id, move_service = self.sq.move_service(longest[0])
                self.TimesOfDispatch[self.service_id2room_id(move_id)] += 1
                # 分配等待服务时间
                move_service.set_wait_time(self.wait_time)
                # 移入等待队列
                self.wq.append_service(move_id, move_service)

                # 从等待队列移除等待时间=0的服务
                service_id, service = self.wq.move_service(service_map[0])
                # 移入服务队列
                self.sq.append_service(service_id, service)


        #若服务队列空闲，移入等待时间最小的。
        while (self.sq.get_service_num()<self.max_service and self.wq.get_wait_num()>0):
            lowest_id=self.wq.get_lowest_wait_service()
            # 从等待队列移除等待时间最短的服务
            service_id, service = self.wq.move_service(lowest_id[0])
            # 移入服务队列
            self.sq.append_service(service_id, service)

        return finish_id #返回结束服务的room_id

    # 更新室内温度，如果达到目标温度，返回TRUE，否则返回False
    def set_indoor_temp(self, room_id, temp):
        service_id, service = self.find_service(room_id)
        service.set_indoor_temp(temp)
        is_finished = service.is_finished()
        if is_finished:
            move_id, move_service = self.sq.move_service(service_id)
            self.TimesOfDispatch[room_id] += 1
            return True
        return False

    '''
    返回每个房间的信息。list
    list[i][0]为room_id，list[i][1]为状态，1：处于等待队列，2：处于服务队列。
    '''

    def show_state(self):
        state={}
        for service_map in self.wq.wait_queue:
            state.setdefault(self.service_id2room_id(service_map[0]), 1)
        for service_map in self.sq.service_queue:
            state.setdefault(self.service_id2room_id(service_map[0]), 2)

        return state


    '''
    通过service_id 转成对应的room_id
    '''

    def service_id2room_id(self,service_id):
        room_id = -1
        # 根据room_id找到对应的service_id
        for room_service in self.lists:
            if room_service[1] == service_id:
                room_id = room_service[0]

        return room_id



    # 删除某一服务对象并删除队列信息
    def delete_service(self, room_id):
        service_id, service = self.find_service(room_id)
        # 删除服务对象
        if service is not None:
            del service
            self.TimesOfOnoff[room_id] += 1
        else:
            return False

        i = 0
        exist = 0
        for room_service in self.lists:
            if room_service[0] == service_id:
                exist = 1
                break
            i += 1
        # 删除队列信息
        if exist:
            del self.lists[i]

        # 删除服务队列或等待队列信息
        service = self.sq.get_service(service_id)
        if service is not None:
            self.sq.move_service(service_id)
        else:
            service = self.wq.get_service(service_id)
            if service is not None:
                self.wq.move_service(service_id)
        return True

    # 根据room_id找到ServiceQueue或WaitQueue里边的Service对象
    def find_service(self, room_id):
        service_id = -1
        # 根据room_id找到对应的service_id
        for room_service in self.lists:

            if room_service[0] == room_id:
                service_id = room_service[1]
                #print("1---", service_id)
        # 查找ServiceQueue和WaitQueue

        service = self.sq.get_service(service_id)
        if service is None:
            #print("2", service_id)
            service = self.wq.get_service(service_id)
            if service is None:
                #print("3", service_id)
                return service_id, None

        return service_id, service

    # 调节模式
    def change_mode(self, room_id, mode):
        service_id, service = self.find_service(room_id)
        return service.set_mode(mode)

    # 调节温度
    def change_temperature(self, room_id, temp):
        service_id, service = self.find_service(int(room_id))
        self.TimesOfChangetemp[room_id] += 1
        return service.set_temperature(temp)

    # 调节风速
    def change_fan_speed(self, room_id, speed):
        
        self.TimesOfChangespeed[room_id] += 1
        service_id, service = self.find_service(int(room_id))    
        print(service.fan_speed)
        judge=service.set_fan_speed(speed)
        print(service.fan_speed)
         #查看修改风速请求是否被允许
        SerQue=self.sq.get_service_queue()#获取服务队列
        WaiQue=self.wq.get_wait_queue()#获取等待队列
        WaiNum=self.wq.get_wait_num()
        print("SQ")
        for i in range (len(SerQue)):
            print(SerQue[i][0],SerQue[i][1].fan_speed)
        print("WQ")
        for i in range (len(WaiQue)):
            print(WaiQue[i][0],WaiQue[i][1].fan_speed)
        if judge==False:
            return False
        if WaiNum==0:
            return True
        else:
            LowService=SerQue[0][1]#存储服务队列中优先级最低的服务
            HighWait=WaiQue[0][1]#存储等待队列中优先级最高的服务
            LowServiceId=SerQue[0][0]#分别存储二者id
            HighWaitId=WaiQue[0][0]
            for i in range (len(SerQue)):#遍历服务队列，寻找等级最低的服务
                if LowService.fan_speed>SerQue[i][1].fan_speed:
                    LowService=SerQue[i][1]
                    LowServiceId=SerQue[i][0]
                if LowService.fan_speed==SerQue[i][1].fan_speed:
                    if LowService.service_time<SerQue[i][1].service_time:
                        LowService=SerQue[i][1]
                        LowServiceId=SerQue[i][0]
            for i in range(0, len(WaiQue)):#遍历等待队列，寻找等级最高的服务
                if HighWait.fan_speed<WaiQue[i][1].fan_speed:
                    HighWait=WaiQue[i][1]
                    HighWaitId=WaiQue[i][0]
                if HighWait.fan_speed==WaiQue[i][1].fan_speed:
                    if HighWait.wait_time>WaiQue[i][1].wait_time:
                        HighWait=WaiQue[i][1]
                        HighWaitId=WaiQue[i][0]
            if HighWait.fan_speed > LowService.fan_speed :#判断是否需要进行调度
                self.sq.move_service(LowServiceId)#进行调度具体过程
                self.wq.move_service(HighWaitId)
                self.sq.append_service(HighWaitId,HighWait)
                self.wq.append_service(LowServiceId,LowService)
            print("SQ")
            for i in range (len(SerQue)):
                print(SerQue[i][0],SerQue[i][1].fan_speed)
            print("WQ")
            for i in range (len(WaiQue)):
                print(WaiQue[i][0],WaiQue[i][1].fan_speed)
            return True
    '''            
    def GetServiceFee(self,service_id, day_in):
        room_id=0
        for i in range(len(self.lists)):
            if service_id == self.lists[i][1]:
                room_id = self.lists[i][0]
        rdr = get_rdr(int (room_id), day_in)
        if len(rdr)==0:
            return 0
         TotalSerFee=0
        for i in range(len(rdr)):
            if service_id == rdr["service_id"]:
                TotalSerFee=TotalSerFee+rdr["fee"]
        return TotalSerFee    
    '''
    def GetRoomFee(self,room_id,day_in):
        rdr = get_rdr(room_id, day_in)
        print(rdr)
        if len(rdr)==0:
            return 0
        TotalRoomFee=0
        for i in range(len(rdr)):
            if room_id == rdr[i][0]:
                TotalRoomFee=TotalRoomFee+rdr[i][7]
        set_invoice(room_id, day_in,TotalRoomFee)
        return TotalRoomFee


    # 初始化
    def PowerON(self):
        self.SettingMode = True
        return True

    # 设置config

    def SetPara(self, Temp_lowLimit, Temp_highLimit, min_speed, max_speed,
            FeeRate_H, FeeRate_M, FeeRate_L):
        if self.SettingMode:
            Config.set_para(Temp_lowLimit, Temp_highLimit, min_speed, max_speed,
                            FeeRate_H, FeeRate_M, FeeRate_L)
            return True
        else:
            return False

    def SetDefaultPara(self, Mode, default_TargetTemp, default_speed):
        if self.SettingMode:
            Config.set_default_para(Mode, default_TargetTemp, default_speed)
            return True
        else:
            return False

    # 返回各房间的状态信息：状态信息：（是否开机，是否正在服务，是否被挂起），当前室温，目标温度，风速，费率，费用，服务时长
    def GetRoomState(self, room_id):
        for room_service in self.lists:
            if room_service[0] == room_id:
                service_id = room_service[1]
                service = self.sq.get_service(service_id)
                return service.get_info()


    # 展示每个房间的开关次数，使用空调的时长，总费用，被调度的次数、详单数、调温次数、调风次数
    def QueryReport(self, list_room_id, report_type, date):
        report = get_report(list_room_id, report_type, date)
        return report


    # 打印报表
    def PrintReport(self, list_room_id, report_type, date):
    try:
        report = get_report(list_room_id, report_type, date)
        print("========================PrintReport========================")
        workbook = xlwt.Workbook()
        worksheet = workbook.add_sheet('Report')

        borders = xlwt.Borders()  # Create Borders
        borders.left = xlwt.Borders.THICK
        borders.right = xlwt.Borders.THICK
        borders.top = xlwt.Borders.THICK
        borders.bottom = xlwt.Borders.THICK
        borders.left_colour = 0x40
        borders.right_colour = 0x40
        borders.top_colour = 0x40
        borders.bottom_colour = 0x40

        pattern = xlwt.Pattern()  # Create the Pattern
        pattern.pattern = xlwt.Pattern.SOLID_PATTERN  # May be: NO_PATTERN, SOLID_PATTERN, or 0x00 through 0x12
        pattern.pattern_fore_colour = 5  # May be: 8 through 63. 0 = Black, 1 = White, 2 = Red, 3 = Green, 4 = Blue, 5 = Yellow, 6 = Magenta, 7 = Cyan, 16 = Maroon, 17 = Dark Green, 18 = Dark Blue, 19 = Dark Yellow , almost brown), 20 = Dark Magenta, 21 = Teal, 22 = Light Gray, 23 = Dark Gray, the list goes on...
        style = xlwt.XFStyle()  # Create the Pattern
        style.pattern = pattern  # Add Pattern to Style
        style.borders = borders  # Add Borders to Style
        titles = ['Date', 'Room_ID', 'Service_ID', 'TimesOfOnOff', 'Duration', 'TotalFee', 'TimesOfDispatch', 'NumberOfRdr',
                'TimesOfChangeTemp', 'TimesOfChangeSpeed']

        for i in range(0, 10):
            worksheet.col(i).width = 5000
            worksheet.write(0, i, titles[i], style)

        j = 1
        for reporter in report:
            for reporterr in reporter:
                for i in range(0, 10):
                    worksheet.write(j, i, reporterr[i])
                j += 1
        file_name = time.strftime('%Y-%m-%d_%H-%M-%S')
        file_name = str(file_name) + '_Report.xls'
        workbook.save(file_name)
        return True
    except:
        return False
    
    '''SetRdr设置详单
        room_id int,                    //房间编号
        day_in timestamp,               //入住时间
        fanspeed int,                   //风速
        feerate float,                  //费率
        fee float,                      //费用'''
    def SetRdr(self, room_id, day_in, fanspeed, feerate, fee):
        set_rdr(room_id,0, day_in, fanspeed, feerate, fee)
    
    '''SetInvoice设置账单
            room_id int,                    //房间编号
            day_in timestamp,               //入住时间
            total_fee float                //总费用'''
    def SetInvoice(self, room_id, day_in, total_fee):
        set_invoice(room_id, day_in, total_fee)
    
    '''SetReport设置报表
            date timestamp,                 //入住日期
            room_id int,                    //房间编号
            times_of_onoff int,             //开关次数
            duration int,                   //经过时间
            total_fee float,                //总费用
            times_of_dispatch int,          //调度次数
            number_of_rdr int,              //详单数
            times_of_changetemp int,        //调温次数
            times_of_changespeed int        //变速次数'''
    def SetReport(self, date, room_id, service_id, times_of_onoff, duration, total_fee, times_of_dispatch, number_of_rdr,
                   times_of_changetemp, times_of_changespeed):
        set_report(date, room_id, service_id, times_of_onoff, duration, total_fee, times_of_dispatch, number_of_rdr,
                   times_of_changetemp, times_of_changespeed)

    #退房间 里面设置报表
    def CheckOut(self, room_id):
        for room_service in self.lists:
            if room_service[0] == room_id:
                service_id = room_service[1]
                service = self.sq.get_service(service_id)
        NumberOfRdr = 12
        self.SetReport(self.day_in[room_id], room_id, service_id, self.TimesOfOnoff[room_id], service.service_time,
                  self.GetRoomFee(room_id, self.day_in[room_id]), self.TimesOfDispatch[room_id], NumberOfRdr,
                  self.TimesOfChangetemp[room_id], self.TimesOfChangespeed[room_id])
        curtime = time.strftime('%Y-%m-%d')
        self.day_out.setdefault(room_id, curtime)

    def print_queue(self):

        print("=========================================")

        print("service_queue")
        for i in range(0, len(self.sq.service_queue)):

            print("service_id: ",self.sq.service_queue[i][0],"||", end=" ")
            print("start_time: ",self.sq.service_queue[i][1].start_time,"|",
                  "current_time: ",self.sq.service_queue[i][1].current_time,"|",
                  "service_time: ",self.sq.service_queue[i][1].service_time,"|",
                  "wait_time: ",self.sq.service_queue[i][1].wait_time,"|",
                  "indoor_temp: ", self.sq.service_queue[i][1].indoor_temp,"|",
                  "temperature: ", self.sq.service_queue[i][1].temperature,"|",
                  "fan_speed: ", self.sq.service_queue[i][1].fan_speed)

        print("---------------------------------------")
        print("wait_queue")
        for i in range(0, len(self.wq.wait_queue)):
            print("service_id: ", self.wq.wait_queue[i][0],"||", end=" ")
            print("start_time: ", self.wq.wait_queue[i][1].start_time,"|",
                  "current_time: ", self.wq.wait_queue[i][1].current_time,"|",
                  "service_time: ", self.wq.wait_queue[i][1].service_time,"|",
                  "wait_time: ", self.wq.wait_queue[i][1].wait_time,"|",
                  "indoor_temp: ", self.wq.wait_queue[i][1].indoor_temp,"|",
                  "temperature: ", self.wq.wait_queue[i][1].temperature,"|",
                  "fan_speed: ", self.wq.wait_queue[i][1].fan_speed)


    def check_room_state(self):
        dicts = []
        for room_service in self.lists:
            room_id = room_service[0]
            service_id, service = self.find_service(room_id)
            # state 只有开机的服务，关机的已被删除
            if self.sq.get_service(service_id) is not None:
                state = 2  # 表示正在服务
            else:
                state = 1  # 表示正在等待
            current_temp = service.indoor_temp
            target_temp = service.temperature
            fan_speed = service.fan_speed
            fee_rate = 1   #三种费率选哪个
            day_in = "2019-4-5"
            fee = self.GetRoomFee(room_id, day_in)    #day_in从哪获得？
            duration = service.service_time     #哪里获得duration
            dicts.append([room_id,state, current_temp, target_temp, fan_speed, fee_rate, fee, duration])


if __name__ == "__main__":
    conn = create_connection()
    db_init(conn)
    dis=Dispatcher()
    a=dis.create_service(1,17)
    dis.SetRdr(1,"2019-1-2",1,1.2,8)
    dis.SetRdr(2,"2019-1-2",1,1.2,4)
    dis.SetRdr(1,"2019-1-2",1,1.2,3)
    dis.SetRdr(2,"2019-1-2",1,1.2,2)
    dis.SetRdr(1,"2019-1-2",1,1.2,1)
    #dis.GetServiceFee(1,"2019-1-2")
    print(dis.GetRoomFee(2,"2019-1-2"))
    dis.change_fan_speed("1",3)
    #/for i in range (100):
     #   print(dis.change_temperature("1",i))
    #dis.change_fan_speed(123,1)
