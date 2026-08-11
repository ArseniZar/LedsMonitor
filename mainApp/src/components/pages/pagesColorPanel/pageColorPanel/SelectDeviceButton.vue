<template>
    <div className="flex w-full flex-row items-center">
        <button @click="selectDevice"
            class="flex w-full flex-row justify-between p-1 rounded-4xl  bg-black/20  hover:bg-black/50 hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 transition duration-300 active:bg-black/100">
            <img :src="getImage('img/icons8-dep-50.png')" alt="icon" class="h-6 w-6 ml-2" />
            <span class="font-bold text-lg select-none"> {{ title }}</span>
            <img :src="getImage('img/icons8-dep-50.png')" alt="icon" class="h-6 w-6 mr-2" />
        </button>
    </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import { getPreloadedImage } from '../../../../basic/utils/imagePreloader';

export default defineComponent({
    name: 'SelectDeviceButton',
    emits: ['selectDevice'],
    props: {
        title: {
            type: String,
            required: true
        },
    },
    data(): {
    } {
        return {
        }

    },
    methods: {
        selectDevice() {
            this.$emit('selectDevice');
        },
        getImage(url: string): string | undefined {
            const img = getPreloadedImage(url);
            return img ? img.src : undefined;
        }
    }
});
</script>