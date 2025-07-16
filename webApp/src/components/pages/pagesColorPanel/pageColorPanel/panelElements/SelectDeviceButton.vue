<template>
    <div className="flex w-full flex-row items-center">
        <button @click="selectDevice" class="flex w-full flex-row justify-between p-1 rounded-4xl  bg-black/20  hover:bg-black/50 hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 transition duration-300 active:bg-black/100">
            <img v-if="data[0].imgIcon" :src="data[0].imgIcon" alt="icon" class="h-6 w-6 ml-2" />
            <span class="font-bold text-lg"> {{ title }}</span>
            <img v-if="data[1].imgIcon" :src="data[1].imgIcon" alt="icon" class="h-6 w-6 mr-2" />
        </button>
    </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';

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
        data: { id: number; imgIcon: string; title: string }[],
    } {
        return {
            data: [
                { id: 1, imgIcon: "/public/img/icons8-dep-50.png", title: '' },
                { id: 2, imgIcon: "/public/img/icons8-ped-50.png", title: '' }
            ]
        }

    },
    methods: {
        selectDevice() {
            this.$emit('selectDevice');
        },
    }
});
</script>