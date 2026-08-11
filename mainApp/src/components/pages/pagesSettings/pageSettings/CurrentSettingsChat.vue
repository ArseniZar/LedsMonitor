<template>
  <div class="w-full h-full flex items-center gap-1 flex-col rounded-xl">
    <img v-if="item.imgIcon" :src="getImage(item.imgIcon)" alt="Check" class="h-20" />
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
import { getPreloadedImage } from '../../../../basic/utils/imagePreloader';

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
  },
  methods: {
    getImage(url: string): string | undefined {
      const img = getPreloadedImage(url);
      return img ? img.src : undefined;
    }
  }
});
</script>
