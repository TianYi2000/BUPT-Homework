from Service import Service

class ServiceQueue:
    '''
        初始化，新建service_queue,用来保存service_id和service
        service_queue为记录了service_id和service的list
        '''

    def __init__(self):
        self.service_queue = []

    '''
    添加一个service_id和service到service_queue，返回是否成功
    '''

    def append_service(self, service_id, service):
        self.service_queue.append([service_id, service])

        return True

    '''
    从service_queue中移除service_id和对应的service,返回service_id和对应的service
    '''

    def move_service(self, service_id):

        for i in range(0,len(self.service_queue)):
            #print("********")
            #print(i)
            if self.service_queue[i][0]==service_id:
                service=self.service_queue[i][1]
                del self.service_queue[i]
                break


        return service_id, service

    '''
    返回服务队列中的数量
    '''
    def get_service_num(self):

        return len(self.service_queue)

    '''
    查找服务队列中服务时长最长的service
    返回service_id的list
    '''
    def get_longest_service(self):
        longest=-1
        longest_id=[]

        for i in range(0,len(self.service_queue)):
            if self.service_queue[i][1].service_time>=longest:
                longest=self.service_queue[i][1].service_time

        for i in range(0,len(self.service_queue)):
            if self.service_queue[i][1].service_time==longest:
                longest_id.append(self.service_queue[i][0])


        return longest_id

    '''
    查找服务队列中给定service_id集合中服务时长最长的service
    返回service_id的list
    '''

    def get_longest_service_in_list(self,list):
        longest = -1
        longest_id = []

        for i in range(0, len(self.service_queue)):
            if self.service_queue[i][1].service_time >= longest and self.service_queue[i][0] in list:
                longest = self.service_queue[i][1].service_time

        for i in range(0, len(self.service_queue)):
            if self.service_queue[i][1].service_time == longest and self.service_queue[i][0] in list:
                longest_id.append(self.service_queue[i][0])

        return longest_id

    '''
    查找服务队列中风速最短的service
    返回service_id的list,最低风速
    '''

    def get_lowest_speed_service(self):
        lowest = 99
        lowest_id = []

        for i in range(0, len(self.service_queue)):
            if self.service_queue[i][1].fan_speed <= lowest:
                lowest = self.service_queue[i][1].fan_speed

        for i in range(0,len(self.service_queue)):
            if self.service_queue[i][1].fan_speed==lowest:
                lowest_id.append(self.service_queue[i][0])


        return lowest_id,lowest

    '''
    根据service_id 查询service
    若没有则返回None
    '''

    def get_service(self, service_id):
        for service_map in self.service_queue:
            if service_map[0] == service_id:
                return service_map[1]

        return None




    '''
    返回服务队列
    '''
    def get_service_queue(self):

        return self.service_queue
