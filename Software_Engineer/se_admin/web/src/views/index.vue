<style scoped lang="less">
	.index{
		width: 100%;
		margin: auto;
		text-align: center;
		font-size:30px;
		h1{
			height: 150px;
			color: white;
			img{
				height: 100%;
			}
		}
		h2{
			color: #666;
			margin-bottom: 200px;
			p{
				margin: 0 0 50px;
			}
		}
		.ivu-row-flex{
			height: 100%;
		}
	}
</style>
<template>
	<div class="index">
		<Layout>
        <Header><h1>Air Conditioner Client</h1></Header>
        <Content>
        	<Input v-model="wsuri" placeholder="Enter something..."></Input>
        	<Row type="flex" justify="center" align="middle">
				<Col span="10">
					<Form :model="formItem" :label-width="80">
						<FormItem label="Switch">
							<Button @click="powerOn">开机</Button>
							<Button @click="powerOff">关机</Button>
							<Button @click="reInit">重新连接服务端</Button>
						</FormItem>
						<FormItem label="Room Id">
							<Input v-model="formItem.roomId" placeholder="Enter something..."></Input>
						</FormItem>
						<FormItem label="Target temperature">
							<Icon type="ios-thermometer" />
							<InputNumber :max="100" v-model="formItem.targetTemp"></InputNumber>℃
						</FormItem>
						<FormItem label="Mode">
							<RadioGroup v-model="formItem.mode" type="button">
								<Radio label="cool"></Radio>
								<Radio label="warm"></Radio>
							</RadioGroup>
						</FormItem>
						<FormItem label="Target wind speed">
							<RadioGroup v-model="formItem.wind_speed" type="button">
								<Radio label="high"></Radio>
								<Radio label="medium"></Radio>
								<Radio label="low"></Radio>
							</RadioGroup>
						</FormItem>
						<FormItem>
							<Button type="primary" @click="submitSetting">Submit</Button>
						</FormItem>
					</Form>
				</Col>
				<Col span="6">
					
				</Col>
				<Col span="8">
					<Table :columns="tbcolumns" :data="tbdata"></Table>
				</Col>
			</Row>
        </Content>
        <Footer>Powered by group D.</Footer>
    	</Layout>

		
	</div>
</template>
<script>
	export default {
		data() {
			return {
				wsuri : "ws://localhost:9999",
				timeUnit : 10*1000,
				highlimit_temp:35,
                lowlimit_temp:16,
                highfan_change_temp:1.5,
                lowfan_change_temp:1.0,
                medfan_change_temp:0.5,
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
				tbcolumns: [
					{
						title: 'Item',
						key: 'item'
					},
					{
						title: 'Data',
						key: 'data'
					}
				]
			}
		},
		created: function(){
			this.initWebSocket();
		},
		mounted: function(){
			this.timer = setInterval(this.tempSim, this.timeUnit);
		},
		destoryed: function(){
			this.websocketclose();
		},
		computed: {
			tbdata: function(){
				return [
				{
					item: 'state', 
					data: this.nowState.state
				},
				{
					item: 'temperature', 
					data: this.nowState.temperature + '℃'
				},
				{
					item: 'wind speed', 
					data: this.nowState.wind_speed
				},
				{
					item: 'energy', 
					data: this.nowState.energy
				},
				{
					item: 'fee', 
					data: this.nowState.fee
				}
				]
			}
		},
		methods: {
			tempSim(){
				let defaultTemp = 30;
				let delta = 0;
				this.nowState.wind_speed = this.formItem.wind_speed;
				switch(this.nowState.wind_speed){
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

				if(this.nowState.state == 'close' || this.nowState.state == 'busy'){
					// air conditioner is stopped, recover temperature to default Temp
					//this.nowState.temperature += (this.nowState.temperature < defaultTemp)*(0.5/(60/(this.timeUnit/1000)));
					this.nowState.temperature += (this.nowState.temperature < defaultTemp)*(0.5);
					this.sendTemp();
					return;
				}
				
				// air conditioner is running
				if(this.nowState.state == "ok"){
					if(this.formItem.mode == "cool"){
						this.nowState.temperature -= delta;
						console.log('temp -' + delta);
					}else{
						this.nowState.temperature += delta;
						console.log('temp +' + delta);
					}
				}
				console.log(this.nowState.state, delta);
				this.sendTemp();
				 
			},
			reInit(){
				this.initWebSocket();
			},
			powerOn(){
				let sendData = {
					"poweron": {
						"room_id": this.formItem.roomId,
						"cur_temp": this.nowState.temperature
					}
				};
				this.websocketsend(sendData);
			},
			powerOff(){
				let sendData = {
					"poweroff": {
						"room_id": this.formItem.roomId,
						"state": this.nowState.state
					} //开机or待机
				};
				this.websocketsend(sendData);
			},
			sendTemp(){

				let sendData = {
					"temp_update": {
						"room_id": this.formItem.roomId,
						"cur_temp": this.nowState.temperature
					}
				};
				this.websocketsend(sendData);
			},
			submitSetting(){
				// check para
				let targetTemp = this.formItem.targetTemp;
				let nowTemp = this.nowState.temperature;
				if(this.formItem.mode == 'cool' && nowTemp < targetTemp){
					this.$Message.info("now Temp is lower than target Temp, please resetting!")
					return;
				}else if(this.formItem.mode == 'warm' && nowTemp > targetTemp){
					this.$Message.info("now Temp is higher than target Temp, please resetting!")
					return;
				}
				if(this.formItem.targetTemp > this.highlimit_temp || this.formItem.targetTemp < this.lowlimit_temp){
					this.$Message.info("The target temperature not in range, please resetting!");
					return;
				}
				let sendData =  {"config": {
										"room_id": this.formItem.roomId,
										"fan": this.formItem.wind_speed == 'low'? 0:this.formItem.wind_speed == 'medium'? 1:2,
										"mode": this.formItem.mode == 'cool'? 0:1,
										"target_temp": this.formItem.targetTemp
								}};
				this.websocketsend(sendData);
			},
			initWebSocket(){ //初始化weosocket 
				let uri = this.wsuri;//ws地址
				this.websock = new WebSocket(uri); 
				this.websock.onopen = this.websocketonopen;
				this.websock.onerror = this.websocketonerror;
				this.websock.onmessage = this.websocketonmessage; 
				this.websock.onclose = this.websocketclose;
		   }, 
		　　websocketonopen() {
			console.log("WebSocket连接成功");
		　　},
		　　websocketonerror(e) { //错误
			console.log("WebSocket连接发生错误");
		　　},
		　　websocketonmessage(e){ //数据接收 
			　//注意：长连接我们是后台直接1秒推送一条数据， 
			  //但是点击某个列表时，会发送给后台一个标识，后台根据此标识返回相对应的数据，
		  //这个时候数据就只能从一个出口出，所以让后台加了一个键，例如键为1时，是每隔1秒推送的数据，为2时是发送标识后再推送的数据，以作区分
			let data = eval('(' + e.data + ')');
			let key = Object.keys(data)[0];
			console.log('receive:',data);
			switch(key){
				case 'poweron':
					this.nowState.state = data.poweron;
					break;
				case 'poweroff':
					if(data.poweroff == 'fail'){
						this.$Message.info('Power off fail, please retry.')
					}
					break;
				case 'config':
					if(data.config == 'ok'){
						this.$Message.info('Setting success!')
					}else{
						this.$Message.info('Setting fail, please retry.')
					}
					break;
	
				case 'setpara':
					data = data.setpara;
					let mode = data.mode;
					let mode2word = {0:'cool',1:'warm'};
					let fan2word = {0:'low',1:'medium',2:'high'};
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
					this.$Message.info('Achieve target temperature!')
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
		　　websocketsend(dataObj){//数据发送 
				var dataJoson = JSON.stringify(dataObj);
				console.log(dataJoson);
				this.websock.send(dataJoson); 
		　　},
		　 websocketclose(e){ //关闭 
			console.log("connection closed (" + e.code + ")"); 
		　　}
			
		}
	}
</script>
