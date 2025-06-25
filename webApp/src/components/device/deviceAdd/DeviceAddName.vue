<template>


  <div className="fixed inset-0 backdrop-blur-md  bg-black/40 overflow-y-auto w-full  min-h-screen">
    <div class="fixed top-10 left-10">
      <BackPage v-on:goBack="handleGoBack"/>
    </div>
    <div className=" p-1 flex gap-3 flex-col items-center justify-center w-8/12 max-w-md  mx-auto h-full ">
      <h1 className="text-center text-2xl font-bold text-white/90">Введи имя</h1>
      <input v-model="name" type="text"
        className="p-2  px-3 text-xs  bg-black  shadow-md text-white/90 rounded-2xl focus:outline-none  focus:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] w-full max-w-xl"
        placeholder="Enter text here" />
      <p className="text-white/80  text-center p-1  block min-h-9">{{ alert }}</p>
      <div class="w-full px-10">
        <button @click="submitName"
          className="text-center  w-full   max-w-xl  font-light shadow-md text-white/90 mx-auto p-2  bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]">GO
        </button>
      </div>
    </div>
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import BackPage from '../BackPage.vue';
export default defineComponent({
  name: 'DeviceAddName',
  emits: ['goBack','submitName'],
  data() {
    return {
      name: '' as string,
      alert: '' as string
    };
  },
  components: {
    BackPage
  },
  methods: {
    submitName() {
      if (this.name.trim() === '') {
        this.alert = 'Please enter a valid name';
        return;
      }
      this.$emit('submitName', this.name.trim());
      this.name = '';
    },
    handleGoBack() {
      this.$emit('goBack');
    }
  }
});
</script>