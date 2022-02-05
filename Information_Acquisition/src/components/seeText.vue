<template>
  <div>
    <vue-canvas-nest :config="{color:'0,0,0', count: 120}" :el="'#area'">
    </vue-canvas-nest>
    <div id="area" class="main" >
      <el-row>
        <el-col :span="18" :offset="3">
          <div class="title">{{result.title}}</div>
          <el-divider></el-divider>
          <el-tabs type="border-card">
            <el-tab-pane label="新闻内容">
              <div class="date">{{result.date}}</div>
              <div class="text">{{result.content}}</div>
              <div class="url">
                原文链接:
                <el-link type="primary" class="url" :underline="false"
                :href=result.url target="_blank">
                  {{result.url}}
                </el-link>
              </div>
              <div class="text"></div>
            </el-tab-pane>
            <el-tab-pane label="信息提取">
              <el-tabs tab-position="left">
                <el-tab-pane label="正则提取结果">
                  <h2>正则提取结果</h2>
                  <el-divider></el-divider>
                  <el-row>
                    <el-col :span="5" :offset="1" style="padding-bottom: 10px" >
                      <el-card class="box-card" shadow="hover" body-style="height:150px">
                        <div slot="header" class="clearfix">
                          <span>新闻作者</span>
                        </div>
                        <div v-for="w in result.writer" :key="w" class="item">
                          {{w}}
                        </div>
                      </el-card>
                    </el-col>

                    <el-col :span="5" :offset="1" style="padding-bottom: 10px">
                      <el-card class="box-card" shadow="hover" body-style="height:150px">
                        <div slot="header" class="clearfix" >
                          <span>包含国家</span>
                        </div>
                        <div v-for="c in result.countries" :key="c" class="item">
                          {{c}}
                        </div>
                      </el-card>
                    </el-col>

                    <el-col :span="5" :offset="1" style="padding-bottom: 10px">
                      <el-card class="box-card" shadow="hover" body-style="height:150px" >
                        <div slot="header" class="clearfix">
                          <span>包含军队</span>
                        </div>
                        <div v-for="a in result.army" :key="a" class="item">
                          {{a}}
                        </div>
                      </el-card>
                    </el-col>

                    <el-col :span="5" :offset="1" style="padding-bottom: 10px">
                      <el-card class="box-card" shadow="hover" body-style="height:150px">
                        <div slot="header" class="clearfix">
                          <span>包含军事设备</span>
                        </div>
                        <div v-for="e in result.equipments" :key="e" class="item">
                          {{e}}
                        </div>
                      </el-card>
                    </el-col>
                  </el-row>
                </el-tab-pane>
                <el-tab-pane label="命名实体提取">
                  <h2>命名实体提取</h2>
                  <el-divider></el-divider>
                  <el-row>
                    <el-col v-for="label in labels" :key="label" :span="5" :offset="1" style="padding-bottom: 10px" >
                      <el-card class="box-card" shadow="hover" body-style="height:180px">
                        <div slot="header" class="clearfix">
                          <span>{{label.mean}}</span>
                        </div>
                        <div v-for="ent in label.entities" :key="ent" class="item">
                          {{ent}}
                        </div>
                      </el-card>
                    </el-col>
                  </el-row>
                </el-tab-pane>
                <el-tab-pane label="句法分析结果">
                  <h2>句法分析结果</h2>
                  <el-divider></el-divider>
                  <div class="myh3">
                    关键词
                  </div>
                  <div style="text-align: left; margin-left: 30px">
                    <el-tag v-for="keyword in result.keyword" :key="keyword" class="keywordtag">
                      {{keyword[0]}}
                    </el-tag>
                  </div>

                  <div class="myh3" style="margin-top: 20px">
                    关键词的句中关系分析
                  </div>

                  <el-table
                    :data="relationship"
                    style="width: 100%; margin-left: 30px">
                    <el-table-column
                      prop="word1"
                      label="关键词1"
                      width="180">
                    </el-table-column>
                    <el-table-column
                      prop="word2"
                      label="关键词2"
                      width="180">
                    </el-table-column>
                    <el-table-column
                      prop="type"
                      label="关系类型"
                      width="180">
                    </el-table-column>
                  </el-table>

                </el-tab-pane>
              </el-tabs>


            </el-tab-pane>
          </el-tabs>

        </el-col>
      </el-row>
    </div>
  </div>

</template>

<script>
import vueCanvasNest from 'vue-canvas-nest'

export default {
  name: "seeText",
  components:{vueCanvasNest},
  data(){
    return{
      index:'',
      result: '',
      resultList:'',
      entities: '',
      limit_entity:10,
      relationship: [],
      labels:[
        {
          tag:'DATE',
          mean:'日期',
          entities: [],
          length:0,
        },
        {
          tag:'TIME',
          mean:'时间',
          entities: [],
          length:0
        },
        {
          tag:'EVENT',
          mean:'事件',
          entities: [],
          length:0
        },
        {
          tag:'GPE',
          mean:'国家或地区',
          entities: [],
          length:0
        },
        {
          tag:'ORG',
          mean:'军队或组织',
          entities: [],
          length:0
        },
        {
          tag:'FAC',
          mean:'建筑或地点',
          entities: [],
          length:0
        },
        {
          tag:'PERSON',
          mean:'人物名',
          entities: [],
          length:0
        },
        {
          tag:'PRODUCT',
          mean:'武器或设备',
          entities: [],
          length:0
        },
        {
          tag:'LAW',
          mean:'条约',
          entities: [],
          length:0
        }
      ],
      rela_label:{
        "nmod": "依存关系",
        "obj":"形容关系",

        "nummod":"依存关系",

        "abbrev":"缩写",
        "acomp":"形容词的补充",
        "advcl":"状语从句修饰词",
        "advmod":"状语",
        "agent":"代理",
        "amod":"形容词",
        "appos":"同位词",
        "attr":"属性",
        "aux":"非主要动词和助词",
        "auxpass":"被动词",
        "cc":"并列关系",
        "ccomp":"从句补充",
        "complm":"引导从句的词好重聚中的主要动词",
        "conj":"连接两个并列的词",
        "cop":"系动词",
        "csubj":"从主关系",
        "csubjpass":"主从被动关系",
        "dep":"依赖关系",
        "det":"决定词,如冠词等",
        "dobj":"直接宾语",
        "expl":"附加语",
        "infmod":"动词不定式",
        "iobj":"非直接宾语",
        "mark":"标志",
        "mwe":"多个词的表示",
        "neg":"否定词",
        "nn":"名词组合形式",
        "npadvmod":"名词作状语",
        "nsubj":"名词主语",
        "nsubjpass":"被动的名词主语",
        "num":"数值修饰",
        "number":"组合数字",
        "parataxis":"并列关系",
        "partmod":"动词形式的修饰",
        "pcomp":"介词补充",
        "pobj":"介词的宾语",
        "poss":"所有形式",
        "possessive":"表示所有者和那个的关系",
        "preconj":"preconjunct",
        "predet":"前缀决定",
        "prep":"介词修饰符",
        "prepc":"介词修饰符",
        "prt":"动词短语",
        "purpcl":"目的从句",
        "quantmod":"数量短语",
        "rcmod":"相关关系",
        "ref":"指示物,指代",
        "rel":"相关联",
        "root":"最重要的词,从它开始,根节点",
        "tmod":"时间修饰语",
        "xcomp":"开放子句的补充",
        "xsubj":"掌控者",
      }
    }
  },
  created() {
    this.resultList=JSON.parse(localStorage.getItem('resultList'));
    this.getParams();
  },
  methods: {
    getParams() {
      // 取到路由带过来的参数
      const routerParams = this.$route.query.index;
      console.log("接受的文章的index: ", routerParams);
      this.index = routerParams
      this.result = this.resultList[this.index]

      console.log("获取到的文章内容: ",this.result)
      this.divideEntities()
    },
    divideEntities(){

      this.entities = []
      for (var i = 0; i < 9; i++){
        this.entities[i] = new Array()
      }
      for (i = 0; i < this.result.entity.length; i++ ){
        var ent = this.result.entity[i]
        for (var j = 0; j < 9; j++) if (ent[1] == this.labels[j]["tag"]){
          if (this.labels[j].entities.length < this.limit_entity){
            this.labels[j].entities.push(ent[0])
            this.labels[j].length ++
          }
          break
        }
      }
      for (i= 0; i < this.result.relationship.length ; i++){
        var rela = this.result.relationship[i]
        this.relationship.push({
          word1: rela[0],
          word2: rela[1],
          type: this.rela_label[rela[2]]
        })
      }
    }
  }
}
</script>

<style scoped>
  .main{
    width:99%;
    height: 99%;
    position: absolute;
  }

  .title{
    font-size: 28px;
    font-weight: bolder;
    margin-top: 60px;
  }

  .date{
    color: darkgray;
  }

  .text{
    margin-top: 20px;
    font-size: 18px;
    text-align: left;
    text-indent:2em;
    text-align:justify;
    line-height:36px;
    margin-bottom: 40px;
  }

  .item{
    font-size: 14px;
    text-align: left;
    line-height:19px;
  }

  .url{
    font-size: 18px;
    text-align: left;
    line-height:36px;
  }

  .infocard{
    height: 100px;
  }

  .keywordtag{
    margin-right: 10px;
    margin-bottom: 10px;
    font-size: 16px;
    font-weight: bolder;

  }

  .myh3{
    font-size: 20px;
    font-weight: bolder;
    margin-bottom: 20px;
    margin-left: 30px;
    text-align: left;
  }
</style>