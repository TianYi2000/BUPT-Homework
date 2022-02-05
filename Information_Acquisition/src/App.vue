<template>
  <div id="app">
    <router-view v-on:childChange="showResult" v-bind:parent-query="query"></router-view>
  </div>
</template>

<script>
export default {
  name: 'app',
  data () {
    return {
      query: ''
    }
  },
  methods: {
    showResult: function(data){
      // alert('hello')
      this.query=data
      console.log(this.query)
      this.$router.replace({path:'/result'})
    }
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 0px;
}
</style>
