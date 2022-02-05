<template>
  <div >
    <vue-canvas-nest :config="{color:'0,0,0', count: 120}" :el="'#area'">
    </vue-canvas-nest>
    <div id="area" class="main">
      <el-row style="margin-top: 100px">
        <el-col :span="18" :offset="3">
          <img src="../../public/logo.png" width="200px">
          <h1>军事信息检索系统</h1>
          <el-row>
            <el-col :span="18" :offset="2">
              <el-input v-model="query"></el-input>
            </el-col>
            <el-col :span="4">
              <el-button type="primary" v-on:click="submit">search</el-button>
            </el-col>
          </el-row>

        </el-col>
        <el-col :span="4" :offset="10">
          <h2 v-show="clickSearch" class="transition-box">加载中...</h2>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
import vueCanvasNest from 'vue-canvas-nest'
export default {
  name: 'searchEngine',
  components:{vueCanvasNest},
  data () {
    return {
      query: '',
      retData: '',
      resultList: [],
      limit_result: 10,
      clickSearch: false,
    }
  },
  methods: {
    async submit(){
      this.clickSearch=true
      console.log(this.$root.resultList);
      this.$root.resultList=[]
      console.log(this.query)

      const data = {
        str: this.query,
        limit: this.limit_result,
      }
      await this.$axios.post(
        'http://localhost:5000/search',
        this.$qs.stringify(data)
      )
      .then(response => {
        this.retData=response.data
        console.log("搜索结果：",response.data);
      })
      .catch(function (error) {
        console.log(error);
        return;
      });
      this.resultList =[]
      for (var i = 0; i < this.limit_result; i++){
        this.resultList.push({
          rank: i+1,
          relate: this.retData[i][0]["similarity"].toFixed(2),
          title: this.retData[i][2]["title"],
          url: this.retData[i][3]["url"],
          date: this.retData[i][4]["date"],
          content: this.retData[i][5]["content"],
          equipments: this.retData[i][6]["equipments"],
          countries: this.retData[i][7]["countries"],
          army: this.retData[i][8]["army"],
          writer: this.retData[i][9]["writer"],
          keyword: this.retData[i][10]["keyword"],
          relationship: this.retData[i][11]["relationship"],
          entity: this.retData[i][12]["entity"],
        })
      }
      console.log("存储结果",this.resultList)
      this.$root.resultList=[]
      this.$root.resultList=this.resultList
      localStorage.setItem('resultList',  JSON.stringify(this.resultList));
      this.clickSearch=false
      const {href} = this.$router.resolve({
        path: '/result',
        query: {
          content : this.query
        }
      })
      window.open(href, '_blank')
    }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

  .main{
    width:99%;
    height: 99%;
    position: absolute;
  }

</style>
