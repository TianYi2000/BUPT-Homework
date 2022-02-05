import sqlite3
import datetime

def create_connection(db_file=".\sql.db"):
    try:
        conn = sqlite3.connect(db_file)
    except:
        print("connection fail")
    return conn


def close_connection(conn):
    conn.close()


def db_init(conn):
    try:
        cur = conn.cursor()
        cur.execute('''
        create table if not exists rdr
        (
            room_id int,
            operate_id int,
            day_in timestamp,
            request_time timestamp,
            request_duration varchar(255),
            fanspeed int,
            feerate float,
            fee float,
            temp float
        );''')
        cur.execute('''
        create table if not exists invoice
        (
            room_id varchar(255),
            date_in timestamp,
            date_out timestamp,
            total_fee float
        );''')
        cur.execute('''
        create table if not exists report
        (
            date timestamp,
            room_id int,
            times_of_onoff int,
            duration int,
            total_fee float,
            times_of_dispatch int,
            number_of_rdr int,
            times_of_changetemp int,
            times_of_changespeed int

        );''')
        print("database init success")
    except:
        print("init fail")


def db_select(conn, attrib, table, limit):
    try:
        cur = conn.cursor()
        str = ""
        if limit:
            cur.execute("select {} from {} where {};".format(attrib, table, limit))
        else:
            cur.execute("select {} from {};".format(attrib, table))
        return cur.fetchall()
    except:
        print("select fail")


def db_delete(conn, table, limit):
    try:
        cur = conn.cursor()
        cur.execute("delete from {} where {};".format(table, limit))
    except:
        print("delte fail")


def db_update(conn, table, oprt, limit):
    try:
        cur = conn.cursor()
        cur.exeute("update {} set {} where {};".format(table, oprt, limit))
    except:
        print("update fail")


def db_insert(conn, table, value):
    try:
        cur = conn.cursor()
        cur.execute("insert into {} values {};".format(table, value))
    except:
        print("insert fail")
def get_conn():
    conn = create_connection()
    return conn

def set_rdr(room_id, operate_id, day_in, fanspeed, feerate, fee, temp):
    conn = get_conn()
    cur = conn.cursor()
    request_time = datetime.datetime.now()
    day_in = datetime.datetime.strptime(day_in, "%Y-%m-%d")
    duration = str(request_time-day_in)
    cur.execute("insert into rdr values (?,?,?,?,?,?,?,?,?)",(room_id,operate_id,day_in,request_time,duration,fanspeed,feerate,fee,temp))
    conn.commit()
    close_connection(conn)
    print("set_rdr")
def get_rdr(roomid, day_in):
    conn = get_conn()
    cur = conn.cursor()
    day_in = datetime.datetime.strptime(day_in, "%Y-%m-%d")
    cur.execute("select * from rdr where room_id = ? and day_in = ?",(roomid, day_in))
    conn.commit()
    return_list = cur.fetchall()
    close_connection(conn)
    return return_list
def set_invoice(room_id, day_in,total_fee):
    conn = get_conn()
    cur = conn.cursor()
    day_in = datetime.datetime.strptime(day_in, "%Y-%m-%d")
    cur.execute("insert into invoice(room_id, date_in, total_fee) values(?,?,?)",(room_id,day_in,total_fee))
    conn.commit()
    close_connection(conn)
def get_invoice(room_id,day_in):
    conn = get_conn()
    cur = conn.cursor()
    day_in = datetime.datetime.strptime(day_in, "%Y-%m-%d")
    cur.execute("select * from invoice where room_id = ? and date_in = ?",(room_id,day_in))
    conn.commit()
    return_list = cur.fetchall()
    close_connection(conn)
    return return_list
def set_report(date, room_id, service_id, times_of_onoff, duration, total_fee, times_of_dispatch, number_of_rdr, times_of_changetemp, times_of_changespeed):
    date = datetime.datetime.strptime(date,"%Y-%m-%d")
    conn = get_conn()
    cur = conn.cursor()
    cur.execute("insert into report values(?,?,?,?,?,?,?,?,?,?)",(date,room_id, serivce_id,times_of_onoff,duration,total_fee,times_of_dispatch,number_of_rdr,times_of_changetemp,times_of_changespeed))
    conn.commit()
    close_connection(conn)

def get_report(list_room_id, type_report, date):
    conn = get_conn()
    cur = conn.cursor()
    report=[]
    if type_report==0:
        date = datetime.datetime.strptime(date,"%Y-%m-%d")
        for room_id in list_room_id:
            cur.execute("select * from report where room_id=? and date=?",(room_id,date))
            report.append(cur.fetchall())
    elif type_report==1:
        '''
        import datetime
    d = "2013-W26"
    r = datetime.datetime.strptime(d + '-1', "%Y-W%W-%w")
    print(r)
        '''
        d = date
        r1 = datetime.datetime.strptime(d + '-1',"%Y-W%W-%w")
        date1 = r1
        r2 += datetime.timedelta(days=6)
        date2 = r2
        for room_id in list_room_id:
            cur.execute("select * from report where room_id=? and date(date) >=date(?) and date(date) <=date(?)",(room_id,date1,date2))
            report.append(cur.fetchall())
        return report
    else:
        date1 = date+"-1-1"
        date1 = datetime.datetime.strptime(date1,"%Y-%m-%d")
        date2 = date+"-12-31"
        date2 = datetime.datetime.strptime(date2,"%Y-%m-%d")
        for room_id in list_room_id:
            cur.execute("select * from report where room_id=? and date(date) >= date(?) and date(date) <= date(?)",(room_id,date1,date2))
            report.append(cur.fetchall())
    return report
#conn = create_connection()
#db_init(conn)
#close_connection(conn)
#set_rdr(room_id, operate_id, day_in, fanspeed, feerate, fee)
#set_rdr(1, 1, "2019-1-2", 3, 0.5, 4.6)
#print(get_rdr(1,"2019-01-02 00:00:00"))
#set_invoice(room_id, day_in,total_fee)
#set_invoice(1,"2019-01-02",123.543)

#set_rdr(1,1,"2019-1-2",3,0.5,5.6)
#print(get_rdr(1,"2019-1-2"))
#set_invoice(1,"2019-1-2",123.544)
#print(get_invoice(1,"2019-1-2"))
#set_report(date, times_of_onoff, operate_id, duration, total_fee, times_of_dispatch, number_of_rdr, number_of_change_time, number_of_changespeed)
#set_report("2019-4-6",1,2,1,12,12.1,12,12,1,1)
#print(get_report([1],0,"2019-4-5"))
#print(get_report([1],2,"2019"))
