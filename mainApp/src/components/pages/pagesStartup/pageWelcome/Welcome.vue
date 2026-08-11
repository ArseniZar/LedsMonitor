<template>


  <div class="fixed inset-0 w-full p-10 pt-0 h-full flex justify-center items-center">
    <div class="flex flex-col gap-5 w-full max-w-md mx-auto h-full items-center justify-center">
      <button
        class="text-center text-2xl text-shadow-2xs font-bold text-white/90 hover:text-white/100 hover:scale-110 duration-200">
        Lumvia 1.0
      </button>

      <button @click="handleAddToken"
        class="text-center font-bold w-full shadow-lg text-white/90 mx-auto p-2 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 transition block">
        Start
      </button>
    </div>
  </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue';


export default defineComponent({
  name: 'Welcome',
  emits: ['addToken'],
  methods: {
    handleAddToken() {
      this.$emit('addToken');
    }
  }
})
</script>
