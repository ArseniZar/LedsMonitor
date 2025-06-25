<template>

  <div className=" w-8/12 p-10 mx-auto flex flex-col gap-5 items-center justify-center min-h-screen">

    <button
      className="text-center text-2xl  font-bold text-white/90  hover:text-white/100 hover:scale-110  duration-200">
      Web APP
    </button>


    <button @click="handleAddDevice"
      className="text-center font-bold w-full shadow-lg text-white/90 mx-auto p-2  bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block">
      Add new device</button>
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue';


export default defineComponent({
  name: 'Welcome',
  emits: ['addDevice'],
  methods: {
    handleAddDevice() {
      this.$emit('addDevice');
    }
  }
})
</script>
