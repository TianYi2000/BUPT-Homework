webpackJsonp([0],{

/***/ 18:
/***/ (function(module, exports, __webpack_require__) {


/* styles */
__webpack_require__(21)

var Component = __webpack_require__(9)(
  /* script */
  __webpack_require__(20),
  /* template */
  __webpack_require__(23),
  /* scopeId */
  "data-v-1bfd43ee",
  /* cssModules */
  null
)
Component.options.__file = "D:\\Projects\\se_admin\\Distributed-Temperature-Control-System-master\\Distributed-Temperature-Control-System-master\\web\\src\\views\\main.vue"
if (Component.esModule && Object.keys(Component.esModule).some(function (key) {return key !== "default" && key !== "__esModule"})) {console.error("named exports are not supported in *.vue files.")}
if (Component.options.functional) {console.error("[vue-loader] main.vue: functional components are not supported with templates, they should use render functions.")}

/* hot reload */
if (false) {(function () {
  var hotAPI = require("vue-loader/node_modules/vue-hot-reload-api")
  hotAPI.install(require("vue"), false)
  if (!hotAPI.compatible) return
  module.hot.accept()
  if (!module.hot.data) {
    hotAPI.createRecord("data-v-1bfd43ee", Component.options)
  } else {
    hotAPI.reload("data-v-1bfd43ee", Component.options)
  }
})()}

module.exports = Component.exports


/***/ }),

/***/ 20:
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
			formItem: {
				Mode: "cool",
				Temp_highLimit: 50,
				Temp_lowLimit: 10,
				default_TargetTemp: 25,
				FeeRate_H: 3,
				FeeRate_M: 2,
				FeeRate_L: 1
			},
			roomState: [{
				roomId: "123",
				state: "power on",
				Current_Temp: 21,
				Target_Temp: 25,
				Fan: 'low',
				FeeRate: 1,
				Fee: 30,
				Duration: 20
			}, {
				roomId: "456",
				state: "power on",
				Current_Temp: 21,
				Target_Temp: 25,
				Fan: 'low',
				FeeRate: 1,
				Fee: 30,
				Duration: 20
			}]

		};
	},

	created: function created() {
		this.initWebSocket();
	},
	mounted: function mounted() {
		var timeUnit = 1000;
		this.timer = setInterval(this.checkState, timeUnit);
	},
	destoryed: function destoryed() {
		this.websocketclose();
	},
	computed: {},
	methods: {
		powerOn: function powerOn() {
			var sendData = {
				"systemBoot": ""
			};
			this.websocketsend(sendData);
		},
		powerOff: function powerOff() {
			var sendData = {
				"startUp": ""
			};
			this.websocketsend(sendData);
		},
		checkState: function checkState() {
			var sendData = {
				"checkState": ""
			};
			this.websocketsend(sendData);
		},
		submitSetting: function submitSetting() {
			var sendData = { "config": this.formItem };
			this.websocketsend(sendData);
		},
		initWebSocket: function initWebSocket() {
			//初始化weosocket 
			var wsuri = "ws://206.189.215.142:3000"; //ws地址
			this.websock = new WebSocket(wsuri);
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
			switch (key) {
				case 'poweron':
					this.nowState.state = data.poweron;
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

/***/ 21:
/***/ (function(module, exports) {

// removed by extract-text-webpack-plugin

/***/ }),

/***/ 23:
/***/ (function(module, exports, __webpack_require__) {

module.exports={render:function (){var _vm=this;var _h=_vm.$createElement;var _c=_vm._self._c||_h;
  return _c('div', {
    staticClass: "index"
  }, [_c('Layout', [_c('Header', [_c('h1', [_vm._v("Main")])]), _vm._v(" "), _c('Content', [_c('Row', {
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
  }, [_vm._v("PowerOn")]), _vm._v(" "), _c('Button', {
    on: {
      "click": _vm.powerOff
    }
  }, [_vm._v("StartUp")])], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Temp_Limit"
    }
  }, [_vm._v("\n\t\t\t\t\t\t\thigh limit:"), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.Temp_highLimit),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "Temp_highLimit", $$v)
      },
      expression: "formItem.Temp_highLimit"
    }
  }), _vm._v("\n\t\t\t\t\t\t\tlow limit"), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.Temp_lowLimit),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "Temp_lowLimit", $$v)
      },
      expression: "formItem.Temp_lowLimit"
    }
  })], 1), _vm._v(" "), _c('FormItem', {
    attrs: {
      "label": "Default target temperature"
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
      value: (_vm.formItem.default_TargetTemp),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "default_TargetTemp", $$v)
      },
      expression: "formItem.default_TargetTemp"
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
      value: (_vm.formItem.Mode),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "Mode", $$v)
      },
      expression: "formItem.Mode"
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
      "label": "Fee rate"
    }
  }, [_vm._v("\n\t\t\t\t\t\t\thigh:"), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.FeeRate_H),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "FeeRate_H", $$v)
      },
      expression: "formItem.FeeRate_H"
    }
  }), _vm._v("\n\t\t\t\t\t\t\tmiddle:"), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.FeeRate_M),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "FeeRate_M", $$v)
      },
      expression: "formItem.FeeRate_M"
    }
  }), _vm._v("\n\t\t\t\t\t\t\tlow:"), _c('InputNumber', {
    attrs: {
      "max": 100
    },
    model: {
      value: (_vm.formItem.FeeRate_L),
      callback: function($$v) {
        _vm.$set(_vm.formItem, "FeeRate_L", $$v)
      },
      expression: "formItem.FeeRate_L"
    }
  })], 1), _vm._v(" "), _c('FormItem', [_c('Button', {
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
  }, _vm._l((_vm.roomState), function(item) {
    return _c('span', {
      key: item
    }, [_c('Card', [_c('p', {
      attrs: {
        "slot": "title"
      },
      slot: "title"
    }, [_vm._v("Room " + _vm._s(item.roomId))]), _vm._v(" "), _c('p', [_vm._v("state:" + _vm._s(item.state))]), _vm._v(" "), _c('p', [_vm._v("Current_Temp:" + _vm._s(item.Current_Temp))]), _vm._v(" "), _c('p', [_vm._v("Target_Temp:" + _vm._s(item.Target_Temp))]), _vm._v(" "), _c('p', [_vm._v("Fan:" + _vm._s(item.Fan))]), _vm._v(" "), _c('p', [_vm._v("FeeRate:" + _vm._s(item.FeeRate))]), _vm._v(" "), _c('p', [_vm._v("Fee:" + _vm._s(item.Fee))]), _vm._v(" "), _c('p', [_vm._v("Duration:" + _vm._s(item.Duration))])])], 1)
  }))], 1)], 1), _vm._v(" "), _c('Footer', [_vm._v("Powered by group D.")])], 1)], 1)
},staticRenderFns: []}
module.exports.render._withStripped = true
if (false) {
  module.hot.accept()
  if (module.hot.data) {
     require("vue-loader/node_modules/vue-hot-reload-api").rerender("data-v-1bfd43ee", module.exports)
  }
}

/***/ })

});
//# sourceMappingURL=0.chunk.js.map