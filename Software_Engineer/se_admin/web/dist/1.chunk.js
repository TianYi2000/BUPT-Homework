webpackJsonp([1],{

/***/ 17:
/***/ (function(module, exports, __webpack_require__) {


/* styles */
__webpack_require__(22)

var Component = __webpack_require__(9)(
  /* script */
  __webpack_require__(19),
  /* template */
  __webpack_require__(24),
  /* scopeId */
  "data-v-452015ed",
  /* cssModules */
  null
)
Component.options.__file = "D:\\Projects\\se_admin\\Distributed-Temperature-Control-System-master\\Distributed-Temperature-Control-System-master\\web\\src\\views\\index.vue"
if (Component.esModule && Object.keys(Component.esModule).some(function (key) {return key !== "default" && key !== "__esModule"})) {console.error("named exports are not supported in *.vue files.")}
if (Component.options.functional) {console.error("[vue-loader] index.vue: functional components are not supported with templates, they should use render functions.")}

/* hot reload */
if (false) {(function () {
  var hotAPI = require("vue-loader/node_modules/vue-hot-reload-api")
  hotAPI.install(require("vue"), false)
  if (!hotAPI.compatible) return
  module.hot.accept()
  if (!module.hot.data) {
    hotAPI.createRecord("data-v-452015ed", Component.options)
  } else {
    hotAPI.reload("data-v-452015ed", Component.options)
  }
})()}

module.exports = Component.exports


/***/ }),

/***/ 19:
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
Object.defineProperty(__webpack_exports__, "__esModule", { value: true });
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

/* harmony default export */ __webpack_exports__["default"] = ({
	data: function data() {
		return {
			wsuri: "ws://localhost:9999",
			timeUnit: 10 * 1000,
			highlimit_temp: 35,
			lowlimit_temp: 16,
			highfan_change_temp: 1.5,
			lowfan_change_temp: 1.0,
			medfan_change_temp: 0.5,
			formItem: {
				roomId: 4,
				switch: "open",
				targetTemp: 20,
				mode: "cool",
				wind_speed: "low"
			},
			nowState: {
				state: 'close',
				temperature: 30,
				wind_speed: 'low',
				energy: 0,
				fee: 0
			},
			tbcolumns: [{
				title: 'Item',
				key: 'item'
			}, {
				title: 'Data',
				key: 'data'
			}]
		};
	},

	created: function created() {
		this.initWebSocket();
	},
	mounted: function mounted() {
		this.timer = setInterval(this.tempSim, this.timeUnit);
	},
	destoryed: function destoryed() {
		this.websocketclose();
	},
	computed: {
		tbdata: function tbdata() {
			return [{
				item: 'state',
				data: this.nowState.state
			}, {
				item: 'temperature',
				data: this.nowState.temperature + '℃'
			}, {
				item: 'wind speed',
				data: this.nowState.wind_speed
			}, {
				item: 'energy',
				data: this.nowState.energy
			}, {
				item: 'fee',
				data: this.nowState.fee
			}];
		}
	},
	methods: {
		tempSim: function tempSim() {
			var defaultTemp = 30;
			var delta = 0;
			this.nowState.wind_speed = this.formItem.wind_speed;
			switch (this.nowState.wind_speed) {
				case 'low':
					delta = this.lowfan_change_temp;
					break;
				case 'medium':
					delta = this.medfan_change_temp;
					break;
				case 'high':
					delta = this.highfan_change_temp;
					break;
				default:
					delta = 0.0;
					break;
			}

			if (this.nowState.state == 'close' || this.nowState.state == 'busy') {
				// air conditioner is stopped, recover temperature to default Temp
				//this.nowState.temperature += (this.nowState.temperature < defaultTemp)*(0.5/(60/(this.timeUnit/1000)));
				this.nowState.temperature += (this.nowState.temperature < defaultTemp) * 0.5;
				this.sendTemp();
				return;
			}

			// air conditioner is running
			if (this.nowState.state == "ok") {
				if (this.formItem.mode == "cool") {
					this.nowState.temperature -= delta;
					console.log('temp -' + delta);
				} else {
					this.nowState.temperature += delta;
					console.log('temp +' + delta);
				}
			}
			console.log(this.nowState.state, delta);
			this.sendTemp();
		},
		reInit: function reInit() {
			this.initWebSocket();
		},
		powerOn: function powerOn() {
			var sendData = {
				"poweron": {
					"room_id": this.formItem.roomId,
					"cur_temp": this.nowState.temperature
				}
			};
			this.websocketsend(sendData);
		},
		powerOff: function powerOff() {
			var sendData = {
				"poweroff": {
					"room_id": this.formItem.roomId,
					"state": this.nowState.state //开机or待机
				} };
			this.websocketsend(sendData);
		},
		sendTemp: function sendTemp() {

			var sendData = {
				"temp_update": {
					"room_id": this.formItem.roomId,
					"cur_temp": this.nowState.temperature
				}
			};
			this.websocketsend(sendData);
		},
		submitSetting: function submitSetting() {
			// check para
			var targetTemp = this.formItem.targetTemp;
			var nowTemp = this.nowState.temperature;
			if (this.formItem.mode == 'cool' && nowTemp < targetTemp) {
				this.$Message.info("now Temp is lower than target Temp, please resetting!");
				return;
			} else if (this.formItem.mode == 'warm' && nowTemp > targetTemp) {
				this.$Message.info("now Temp is higher than target Temp, please resetting!");
				return;
			}
			if (this.formItem.targetTemp > this.highlimit_temp || this.formItem.targetTemp < this.lowlimit_temp) {
				this.$Message.info("The target temperature not in range, please resetting!");
				return;
			}
			var sendData = { "config": {
					"room_id": this.formItem.roomId,
					"fan": this.formItem.wind_speed == 'low' ? 0 : this.formItem.wind_speed == 'medium' ? 1 : 2,
					"mode": this.formItem.mode == 'cool' ? 0 : 1,
					"target_temp": this.formItem.targetTemp
				} };
			this.websocketsend(sendData);
		},
		initWebSocket: function initWebSocket() {
			//初始化weosocket 
			var uri = this.wsuri; //ws地址
			this.websock = new WebSocket(uri);
			this.websock.onopen = this.websocketonopen;
			this.websock.onerror = this.websocketonerror;
			this.websock.onmessage = this.websocketonmessage;
			this.websock.onclose = this.websocketclose;
		},
		websocketonopen: function websocketonopen() {
			console.log("WebSocket连接成功");
		},
		websocketonerror: function websocketonerror(e) {
			//错误
			console.log("WebSocket连接发生错误");
		},
		websocketonmessage: function websocketonmessage(e) {
			//数据接收 
			//注意：长连接我们是后台直接1秒推送一条数据， 
			//但是点击某个列表时，会发送给后台一个标识，后台根据此标识返回相对应的数据，
			//这个时候数据就只能从一个出口出，所以让后台加了一个键，例如键为1时，是每隔1秒推送的数据，为2时是发送标识后再推送的数据，以作区分
			var data = eval('(' + e.data + ')');
			var key = Object.keys(data)[0];
			console.log('receive:', data);
			switch (key) {
				case 'poweron':
					this.nowState.state = data.poweron;
					break;
				case 'poweroff':
					if (data.poweroff == 'fail') {
						this.$Message.info('Power off fail, please retry.');
					}
					break;
				case 'config':
					if (data.config == 'ok') {
						this.$Message.info('Setting success!');
					} else {
						this.$Message.info('Setting fail, please retry.');
					}
					break;

				case 'setpara':
					data = data.setpara;
					var mode = data.mode;
					var mode2word = { 0: 'cool', 1: 'warm' };
					var fan2word = { 0: 'low', 1: 'medium', 2: 'high' };
					this.formItem.mode = mode2word[mode];
					this.formItem.targetTemp = data.target_temp;
					this.highlimit_temp = data.highlimit_temp;
					this.lowlimit_temp = data.lowlimit_temp;
					this.highfan_change_temp = data.highfan_change_temp;
					this.lowfan_change_temp = data.lowfan_change_temp;
					this.medfan_change_temp = data.medfan_change_temp;
					this.formItem.wind_speed = fan2word[data.fan];

					console.log('asd');
					break;

				case 'finish':
					this.$Message.info('Achieve target temperature!');
					this.nowState.state = 'close';
					break;
				case 'cost':
					this.nowState.fee = data.cost;
					this.nowState.energy = data.cost;
					break;
				case 'energy':
					this.nowState.energy = data.energy;
					break;
			}
		},
		websocketsend: function websocketsend(dataObj) {
			//数据发送 
			var dataJoson = JSON.stringify(dataObj);
			console.log(dataJoson);
			this.websock.send(dataJoson);
		},
		websocketclose: function websocketclose(e) {
			//关闭 
			console.log("connection closed (" + e.code + ")");
		}
	}
});

/***/ }),

/***/ 22:
/***/ (function(module, exports) {

// removed by extract-text-webpack-plugin

/***/ }),

/***/ 24:
/***/ (function(module, exports, __webpack_require__) {

module.exports={render:function (){var _vm=this;var _h=_vm.$createElement;var _c=_vm._self._c||_h;
  return _c('div', {
    staticClass: "index"
  }, [_c('Layout', [_c('Header', [_c('h1', [_vm._v("Air Conditioner Client")])]), _vm._v(" "), _c('Content', [_c('Input', {
    attrs: {
      "placeholder": "Enter something..."
    },
    model: {
      value: (_vm.wsuri),
      callback: function($$v) {
        _vm.wsuri = $$v
      },
      expression: "wsuri"
    }
  }), _vm._v(" "), _c('Row', {
    attrs: {
      "type": "flex",
      "justify": "center",
      "align": "middle"
    }
  }, [_c('Col', {
    attrs: {
      "span": "10"
    }
  }, [_c('Form', {
    attrs: {
      "model": _vm.formItem,
      "label-width": 80
    }
  }, [_c('FormItem', {
    attrs: {
      "label": "Switch"
    }
  }, [_c('Button', {
    on: {
      "click": _vm.powerOn
    }
  }, [_vm._v("开机")]), _vm._v(" "), _c('Button', {
    on: {
      "click": _vm.powerOff
    }
  }, [_vm._v("关机")]), _vm._v(" "), _c('Button', {
    on: {
      "click": _vm.reInit
    }
  }, [_vm._v("重新连接服务端")])], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Room Id"
    }
  }, [_c('Input', {
    attrs: {
      "placeholder": "Enter something..."
    },
    model: {
      value: (_vm.formItem.roomId),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "roomId", $$v)
      },
      expression: "formItem.roomId"
    }
  })], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Target temperature"
    }
  }, [_c('Icon', {
    attrs: {
      "type": "ios-thermometer"
    }
  }), _vm._v(" "), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.targetTemp),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "targetTemp", $$v)
      },
      expression: "formItem.targetTemp"
    }
  }), _vm._v("℃\n\t\t\t\t\t\t")], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Mode"
    }
  }, [_c('RadioGroup', {
    attrs: {
      "type": "button"
    },
    model: {
      value: (_vm.formItem.mode),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "mode", $$v)
      },
      expression: "formItem.mode"
    }
  }, [_c('Radio', {
    attrs: {
      "label": "cool"
    }
  }), _vm._v(" "), _c('Radio', {
    attrs: {
      "label": "warm"
    }
  })], 1)], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Target wind speed"
    }
  }, [_c('RadioGroup', {
    attrs: {
      "type": "button"
    },
    model: {
      value: (_vm.formItem.wind_speed),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "wind_speed", $$v)
      },
      expression: "formItem.wind_speed"
    }
  }, [_c('Radio', {
    attrs: {
      "label": "high"
    }
  }), _vm._v(" "), _c('Radio', {
    attrs: {
      "label": "medium"
    }
  }), _vm._v(" "), _c('Radio', {
    attrs: {
      "label": "low"
    }
  })], 1)], 1), _vm._v(" "), _c('FormItem', [_c('Button', {
    attrs: {
      "type": "primary"
    },
    on: {
      "click": _vm.submitSetting
    }
  }, [_vm._v("Submit")])], 1)], 1)], 1), _vm._v(" "), _c('Col', {
    attrs: {
      "span": "6"
    }
  }), _vm._v(" "), _c('Col', {
    attrs: {
      "span": "8"
    }
  }, [_c('Table', {
    attrs: {
      "columns": _vm.tbcolumns,
      "data": _vm.tbdata
    }
  })], 1)], 1)], 1), _vm._v(" "), _c('Footer', [_vm._v("Powered by group D.")])], 1)], 1)
},staticRenderFns: []}
module.exports.render._withStripped = true
if (false) {
  module.hot.accept()
  if (module.hot.data) {
     require("vue-loader/node_modules/vue-hot-reload-api").rerender("data-v-452015ed", module.exports)
  }
}

/***/ })

});
//# sourceMappingURL=1.chunk.js.map