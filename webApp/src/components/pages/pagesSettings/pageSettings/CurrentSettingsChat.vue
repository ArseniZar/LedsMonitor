<template>
  <div class="w-full h-full flex items-center gap-1 flex-col rounded-xl">
    <img :src="item.imgIcon" alt="Check" class="h-20" />
    <span :class="[
      'font-bold text-center text-2xl transition-all duration-300 select-none',
      isUpdated ? 'text-sky-600 scale-105' : 'text-white'
    ]">
      {{ item.title }}
    </span>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import type { PropType } from 'vue';
import type { ItemData } from '../../../../basic/types/itemData';

export default defineComponent({
  name: 'CurrentSettingsChat',
  props: {
    item: {
      type: Object as PropType<ItemData>,
      required: true
    },
  },
  data() {
    return {
      isUpdated: false
    };
  },
  watch: {
    item: {
      deep: true,
      handler() {
        this.isUpdated = true;
        setTimeout(() => {
          this.isUpdated = false;
        }, 600);
      }
    }
  }
});
</script>
