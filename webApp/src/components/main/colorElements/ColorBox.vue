<template>
  <label class="block h-6 max-w-[60px] w-full ">
    <input @change="onSelect" type="radio" name="color" :value="uniqueId" class="hidden peer" />
    <div
      class=" w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]"
      :style="{
        backgroundColor: color,
      }">
    </div>
  </label>
</template>

<script lang="ts">
export default {
  name: 'ColorMainBox',
  props: {
    color: {
      type: String,
      required: true
    },
    uniqueId: {
      type: String,
      required: true,
    },
  },
  methods: {
    onSelect(event: Event) {
      const input = event.target as HTMLInputElement;
      // console.log(input.value);
      this.$emit('onSelect', input.value);
    }
  }
};
</script>
