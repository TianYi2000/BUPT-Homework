<template>
<!--  结果列表，包含相关度、题目、主要匹配内容、URL、日期-->
  <div class="main">
    <vue-canvas-nest :config="{color:'0,0,0', count: 120}" :el="'#area'">
    </vue-canvas-nest>
    <div id="area">
      <el-row>
        <el-col :span="20" :offset="3" >
          <el-row>
            <el-col :span="20" >
              <h2>
                “{{this.query}}”的搜索结果
              </h2>
            </el-col>
          </el-row>
          <el-divider></el-divider>
        </el-col>

        <el-col :span="18" v-for="result in resultList" :key="result.rank" :offset="3" class="showresult">
          <el-card :body-style="{ padding: '0px' }" shadow="hover" @click="showText(result)">
            <div slot="header" class="clearfix">
              <span class="title">{{result.title}}</span>
              <span class="rank">#{{result.rank}}</span>
              <span class="relate">{{result.relate}}</span>

            </div>
            <div style="padding: 14px;">
              <el-row>
                <el-col :span="18" :offset="3">
                  <div>
                    {{result.content.substr(0,48)}}...
                  </div>
                </el-col>
              </el-row>

              <div class="bottom clearfix">
                <el-button type="text" class="button" @click="showText(result)">查看全文</el-button>
                <el-link type="primary" class="url" :underline="false" :href=result.url
                         target="_blank" :title=result.url>
                  跳转原址
                </el-link>
                <span class="date">{{result.date}}</span>
              </div>
            </div>
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>

</template>

<script>
import vueCanvasNest from 'vue-canvas-nest'
export default {
  name: 'searchResult',
  components:{vueCanvasNest},
  props: ['parentQuery'],
  data () {
    return {
      query : '',
      resultList: this.$root.resultList,
    }
  },
  created() {
    this.getParams();
    this.resultList=JSON.parse(localStorage.getItem('resultList'));
  },
  computed:{
  },
  methods: {
    getParams() {
      // 取到路由带过来的参数
      const routerParams = this.$route.query.content;
      this.query = routerParams
      console.log("获取到的搜索关键词: ",this.query)
    },
    showText(result){
      const {href} = this.$router.resolve({
        path: '/page',
        query: {
          index: result.rank-1
        }
      })
      window.open(href, '_self')
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

  .showresult{
    margin-bottom: 20px;
  }

  .title {
    font-size: 20px;
  }

  .bottom {
    margin-top: 13px;
    line-height: 12px;
  }

  .rank , .date {
    padding: 4px 3px;
    color: darkgray;
    float: left;
  }

  .relate , .button {
    font-size: 15px;
    padding: 4px 3px;
    color: #409eff;
    float: right;
  }

  .url{
    font-size: 15px;
    padding: 6px;
    float: right;
  }

  .clearfix:before,
  .clearfix:after {
      display: table;
      content: "";
  }

  .clearfix:after {
      clear: both
  }

</style>