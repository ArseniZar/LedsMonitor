<template>


  <div class="fixed inset-0  bg-black/30  backdrop-blur-md  min-h-screen">
    <div class="fixed top-10 left-5">
      <BackPage v-on:goBack="handleGoBack" />
    </div>
    <div className=" p-15  flex gap-3 flex-col items-center justify-center w-full h-full  ">
      <h1 className="text-center text-2xl font-bold text-white/90">Введи имя устройства</h1>
      <input v-model="name" type="text"
        className="p-2  px-5  bg-black  shadow-md text-white/90 rounded-2xl focus:outline-none  focus:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] w-full max-w-xl"
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