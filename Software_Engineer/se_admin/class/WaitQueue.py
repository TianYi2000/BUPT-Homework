from Service import Service

class WaitQueue:

    '''
    初始化，新建wait_queue,用来保存service_id和service
    wait_queue为记录了service_id和service的list
    '''
    def __init__(self):
        self.wait_queue=[]

    '''
    添加一个service_id和service到wait_queue，返回是否成功
    '''
    def append_service(self,service_id,service):

        self.wait_queue.append([service_id,service])
        return True;

    '''
    从wait_queue中移除service_id和对应的service,返回service_id和对应的service
    '''
    def move_service(self,service_id):

        for i in range(0,len(self.wait_queue)):

            if self.wait_queue[i][0]==service_id:
                service=self.wait_queue[i][1]
                del self.wait_queue[i]
                break

        return service_id,service


    '''
    选择等待时长最小的服务
    返回service_id的list
    '''
    def get_lowest_wait_service(self):
        lowest = 9999999
        lowest_id = []
        for i in range(0, len(self.wait_queue)):
            if self.wait_queue[i][1].wait_time <= lowest:
                lowest = self.wait_queue[i][1].wait_time
        for i in range(0, len(self.wait_queue)):
            if self.wait_queue[i][1].wait_time == lowest:
                lowest_id.append(self.wait_queue[i][0])

        return lowest_id

    '''
    返回等待时长小于0的服务
    返回service_id的list
    '''

    def get_finish_wait_service(self):
        finished_id = []

        for i in range(0, len(self.wait_queue)):
            if self.wait_queue[i][1].wait_time <= 0:
                finished_id.append(self.wait_queue[i][0])

        return finished_id


    '''
    根据service_id 查询service
    若没有则返回None
    '''
    def get_service(self, service_id):
        for service_map in self.wait_queue:
            if service_map[0] == service_id:
                return service_map[1]
        return None

    '''
    返回服务队列中的数量
    '''

    def get_wait_num(self):

        return len(self.wait_queue)

    '''
    返回等待队列
    '''
    def get_wait_queue(self):

        return self.wait_queue

