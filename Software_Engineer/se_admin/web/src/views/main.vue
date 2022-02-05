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
        <Header><h1>Main</h1></Header>
        <Content>
        	
        	<Row type="flex" justify="center" align="middle">
				<Col span="10">
					<Form :model="formItem" :label-width="80">
						<FormItem label="Switch">
							<Button @click="powerOn">PowerOn</Button>
							<Button @click="powerOff">StartUp</Button>
						</FormItem>
						<FormItem label="Temp_Limit">
							high limit:<InputNumber :max="100" v-model="formItem.Temp_highLimit"></InputNumber>
							low limit<InputNumber :max="100" v-model="formItem.Temp_lowLimit"></InputNumber>
						</FormItem>
						<FormItem label="Default target temperature">
							<Icon type="ios-thermometer" />
							<InputNumber :max="100" v-model="formItem.default_TargetTemp"></InputNumber>℃
						</FormItem>
						<FormItem label="Mode">
							<RadioGroup v-model="formItem.Mode" type="button">
								<Radio label="cool"></Radio>
								<Radio label="warm"></Radio>
							</RadioGroup>
						</FormItem>
						<FormItem label="Fee rate">
							high:<InputNumber :max="100" v-model="formItem.FeeRate_H"></InputNumber>
							middle:<InputNumber :max="100" v-model="formItem.FeeRate_M"></InputNumber>
							low:<InputNumber :max="100" v-model="formItem.FeeRate_L"></InputNumber>
						</FormItem>
						<FormItem>
							<Button type="primary" @click="submitSetting">Submit</Button>
						</FormItem>
					</Form>
				</Col>
				<Col span="6">
					
				</Col>
				<Col span="8">
					<span v-for="item in roomState" :key="item">
						<Card>
			                <p slot="title">Room {{item.roomId}}</p>
			                <p>state:{{item.state}}</p>
			                <p>Current_Temp:{{item.Current_Temp}}</p>
			                <p>Target_Temp:{{item.Target_Temp}}</p>
			                <p>Fan:{{item.Fan}}</p>
			                <p>FeeRate:{{item.FeeRate}}</p>
			                <p>Fee:{{item.Fee}}</p>
			                <p>Duration:{{item.Duration}}</p>
			            </Card>
		        	</span>
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
				formItem: {
					Mode: "cool",
					Temp_highLimit: 50, 
					Temp_lowLimit: 10,
					default_TargetTemp: 25,
					FeeRate_H: 3,
					FeeRate_M: 2,
					FeeRate_L: 1
				},
				roomState: [
					{
						roomId: "123",
						state: "power on",
						Current_Temp: 21,
						Target_Temp: 25,
						Fan: 'low',
						FeeRate: 1,
						Fee: 30,
						Duration: 20
					},
					{
						roomId: "456",
						state: "power on",
						Current_Temp: 21,
						Target_Temp: 25,
						Fan: 'low',
						FeeRate: 1,
						Fee: 30,
						Duration: 20
					}

				]
				
				
			}
		},
		created: function(){
			this.initWebSocket();
		},
		mounted: function(){
			var timeUnit = 1000;
			this.timer = setInterval(this.checkState, timeUnit);
		},
		destoryed: function(){
			this.websocketclose();
		},
		computed: {
			
		},
		methods: {
			powerOn(){
				let sendData = {
					"systemBoot": ""
				};
				this.websocketsend(sendData);
			},
			powerOff(){
				let sendData = {
					"startUp": ""
				};
				this.websocketsend(sendData);
			},
			checkState(){
				let sendData = {
					"checkState": ""
				};
				this.websocketsend(sendData);
			},
			submitSetting(){
				let sendData =  {"config": this.formItem};
				this.websocketsend(sendData);
			},
			initWebSocket(){ //初始化weosocket 
				const wsuri = "ws://206.189.215.142:3000";//ws地址
				this.websock = new WebSocket(wsuri); 
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
			switch(key){
				case 'poweron':
					this.nowState.state = data.poweron;
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
