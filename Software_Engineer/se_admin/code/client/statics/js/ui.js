//main control page
var STATE_ENUM = {
    'on': 0,
    'off': 1,
    'sleep': 2
}
var g_roomId = 0
var g_state = STATE_ENUM.on
var g_current_temp = 25
var ws = null
function WebSocketInit() {
    if ("WebSocket" in window) {
        console.log('import websocket success');
        ws = new WebSocket("ws://206.189.215.142:3000");
        ws.onopen = function() {
            ws.send("发送数据");
            console.log('connected')
        };
        ws.onmessage = function(evt) {
            var received_msg = evt.data;
            console.log('onmessage:' + received_msg)
        };
        ws.onclose = function() {
            console.log('disconnected')
        };
        ws.onerror = function(evt) {
            console.log('Error:' + evt)
        }
    } else {
        alert("您的浏览器不支持 WebSocket!");
    }
}

function sendMessage(message) {
	ws.send(JSON.stringify(message))
}

function sendSystemBoot() {
    message = {
        "systemBoot": ''
    }
    sendMessage(message)
}

function sendPowerOn() {
    message = {
        "poweron": {
            "room_id": g_roomId,
            "cur_temp": current_temp
        }
    }
    sendMessage(message)
}

function sendPowerOff() {
    message = {
        "poweroff": {
            "room_id": g_roomId,
            "state": g_state
        } //开机or待机
    }
    sendMessage(message)
}

function clickOn() {
	sendPowerOn()
}

function clickOff() {
	sendPowerOff()
}

function clickSleep() {

}

function getValue(tag_id) {
	return $('#' + tag_id).attr('value')
}

function setPara() {
	let para = {
		"Mode":"",
		"Temp_highLimit": getValue("Temp_highLimit"),
		"Temp_lowLimit": getValue("Temp_lowLimit"),
		"default_TargetTemp": getValue("default_TargetTemp"),
		"FeeRate_H": getValue("FeeRate_H"),
		"FeeRate_M": getValue("FeeRate_M"),
		"FeeRate_L": getValue("FeeRate_L")
	}
	let data = {
		'config' : para
	}
	sendMessage(data)
	alert("发送修改请求")
}

function setMonitor(data) {
	
}

function power_onclick() {}
$(document).ready(function() {
    WebSocketInit();
})