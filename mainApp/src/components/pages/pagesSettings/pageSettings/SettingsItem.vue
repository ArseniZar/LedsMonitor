<template>
    <button @click="handleClickItem" @mousedown="handleLongPressItemStart" @mouseup="handleLongPressItemEnd"
        @mouseleave="handleLongPressItemEnd" @touchstart="handleLongPressItemStart" @touchend="handleLongPressItemEnd"
        @touchcancel="handleLongPressItemEnd"
        class="flex  px-3 flex-row gap-4 w-full p-2.5 rounded-xl hover:scale-105 active:bg-black/80 active:scale-110 transition-transform duration-400">
        <img v-if="item.imgIcon" :src="getImage(item.imgIcon)" alt="Check" class="h-6" />
        <p class="text-white/90 text-left w-full break-words font-bold select-none">{{ item.title }} </p>
        <img v-if="visible" :src="getImage('img/icons8-dep-50.png')" alt="Check" class="h-6" />
    </button>

</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../basic/types/itemData';
import { LongPressHandler } from '../../../../basic/classes/LongPressHadler'
import { getPreloadedImage } from '../../../../basic/utils/imagePreloader';

export default defineComponent({
    name: 'SettingsItem',
    emits: ['clickItem', 'longPressItem'],
    props: {
        item: {
            type: Object as PropType<ItemData>,
            required: true
        },
        visible: {
            type: Boolean,
            required: false,
            default: true,
        },
    },
    data(): {
        longPressHandler: LongPressHandler | null;
    } {
        return {
            longPressHandler: null,
        }
    },
    mounted() {
        this.longPressHandler = new LongPressHandler(400, () => {
            this.$emit('longPressItem', this.item.id);
        });
    },

    methods: {
        handleClickItem() {
            this.$emit('clickItem', this.item.id);
        },

        handleLongPressItemStart() {
            this.longPressHandler?.handleLongPressStart();
        },

        handleLongPressItemEnd() {
            this.longPressHandler?.handleLongPressEnd();
        },

        getImage(url: string): string | undefined {
            const img = getPreloadedImage(url);
            return img ? img.src : undefined;
        }

    }
});
</script>