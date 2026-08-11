<template>
  <div @click="onBackgroundClick" className="fixed inset-0 backdrop-blur-md  bg-black/40 min-h-screen">
    <div class="h-full w-10/12 p-4 mx-auto max-w-md">
      <div class="fixed left-1/2 transform -translate-x-1/2 max-w-md w-10/12 p-4">
        <HeaderBar v-if="headerStatus" :visibleMode="'back'" @goBack="handleGoBack" />
      </div>
      <div className="flex gap-2 flex-col items-center justify-center h-full">
        <h1 className="text-center text-2xl font-bold text-white/90">{{ text }}</h1>
        <input v-model="value" type="text" ref="myInput"
          className="p-2  px-3 text-xs  bg-black  shadow-md text-white/90 rounded-2xl focus:outline-none  focus:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] w-full"
          placeholder="Enter text here" />
        <div class="w-full rounded-2xl min-h-9">
          <p className="text-white/80 text-center p-1 w-full block select-none">{{ alertValue }}</p>
          <Spin v-if="spinStatus" />
        </div>

        <button @click="submit"
          className="text-center w-full  font-light shadow-md text-white/90  p-2  bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block active:scale-105 active:bg-black/95 active:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]">Next
        </button>
      </div>
    </div>

  </div>

</template>


<script lang="ts">
import { defineComponent } from 'vue';
import type { ResultData } from '../../telegram/telegramService';
import HeaderBar from './header/HeaderBar.vue';
import Spin from './Spin.vue';

export default defineComponent({
  name: 'Form',
  emits: ['goBack', 'submit'],

  components: {
    HeaderBar,
    Spin
  },

  props: {
    headerStatus: {
      type: Boolean,
      required: false,
      default: true
    },
    text: {
      type: String,
      required: true
    },
    validData: {
      type: Function,
      required: false,
    },


  },
  data(): {
    value: string,
    alertValue: String,
    spinStatus: boolean,
  } {
    return {
      value: '' as string,
      alertValue: '' as string,
      spinStatus: false,
    }
  },

  methods: {
    handleGoBack() {
      this.$emit('goBack');
    },

    onBackgroundClick(event: MouseEvent) {
      const input = this.$refs.myInput as HTMLInputElement | undefined;
      if (input && !input.contains(event.target as Node)) {
        input.blur();
      }
    },

    async submit() {
      this.alertValue = '';
      this.spinStatus = true;
      const trimmedValue = this.value.trim();
      if (this.validData) {
        const isValid: ResultData<typeof this.validData> = await this.validData(trimmedValue);
        if (!isValid.status) {
          this.alertValue = isValid.alertMessage || 'Invalid data';
          this.spinStatus = false;
          return;
        }
      }

      this.alertValue = '';
      this.spinStatus = false;
      this.$emit('submit', trimmedValue);
    }

  }
});
</script>