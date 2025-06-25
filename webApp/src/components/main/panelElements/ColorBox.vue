<template>
  <label class="block h-6 max-w-[60px] w-full ">
    <input @change="onSelect" type="radio" name="color" :value="colorBox.numberBox" class="hidden peer" />
    <div
      class=" w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]"
      :style="{
        backgroundColor: colorBox.color,
      }">
    </div>
  </label>
</template>

<script lang="ts">
import type { PropType } from 'vue';
import type  { BoxColor } from '../../../interface';
export default {
  name: 'ColorMainBox',
  props: {
    colorBox: {
      type: Object as PropType<BoxColor>,
      required: true
    },
  },
  methods: {
    onSelect(event: Event) {
      const input = event.target as HTMLInputElement;
      this.$emit('onSelect', input.value);
    }
  }
};
</script>
